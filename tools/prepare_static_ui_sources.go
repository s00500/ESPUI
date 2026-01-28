// Package main provides a tool to prepare ESPUI header files by minifying
// and gzipping HTML, JS, and CSS source files.
//
// Usage:
//
//	go run prepare_static_ui_sources.go -a
//	go run prepare_static_ui_sources.go -s ../data -t ../src
//
// Or build and run:
//
//	go build -o prepare_static_ui_sources prepare_static_ui_sources.go
//	./prepare_static_ui_sources -a
package main

import (
	"bytes"
	"compress/gzip"
	"flag"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/tdewolff/minify/v2"
	"github.com/tdewolff/minify/v2/css"
	"github.com/tdewolff/minify/v2/html"
	"github.com/tdewolff/minify/v2/js"
)

const targetTemplate = `const char %s[] PROGMEM = R"=====(
%s
)=====";

const uint8_t %s_GZIP[%d] PROGMEM = { %s };
`

type context struct {
	infile      string
	outfile     string
	outdir      string
	outfilename string
	minifile    string
	constant    string
	fileType    string
	name        string
	dir         string
	minidata    string
	gzipdata    string
	gziplen     int
}

func main() {
	auto := flag.Bool("a", false, "Automatically find all source files in data/ and write C header files to src/")
	flag.BoolVar(auto, "auto", false, "Automatically find all source files in data/ and write C header files to src/")
	flag.BoolVar(auto, "all", false, "Automatically find all source files in data/ and write C header files to src/")

	sources := flag.String("s", "", "Sources directory containing CSS or JS files OR one specific file to minify")
	flag.StringVar(sources, "source", "", "Sources directory containing CSS or JS files OR one specific file to minify")
	flag.StringVar(sources, "sources", "", "Sources directory containing CSS or JS files OR one specific file to minify")

	target := flag.String("t", "", "Target directory containing C header files OR one C header file")
	flag.StringVar(target, "target", "", "Target directory containing C header files OR one C header file")

	storeMini := flag.Bool("storemini", true, "Store intermediate minified files next to the originals")
	noStoreMini := flag.Bool("nostoremini", false, "Do not store intermediate minified files")
	flag.BoolVar(noStoreMini, "m", false, "Do not store intermediate minified files")

	flag.Parse()

	// Handle nostoremini flag
	if *noStoreMini {
		*storeMini = false
	}

	if !*auto && (*sources == "" || *target == "") {
		fmt.Println("ERROR: You need to specify either --auto/-a or both --source/-s and --target/-t")
		fmt.Println()
		flag.Usage()
		os.Exit(1)
	}

	// Get the directory where this script is located
	execPath, err := os.Executable()
	if err != nil {
		// Fallback to current working directory
		execPath, _ = os.Getwd()
		execPath = filepath.Join(execPath, "tools", "dummy")
	}
	scriptDir := filepath.Dir(execPath)

	// For "go run", use the current working directory approach
	if strings.Contains(execPath, "go-build") {
		cwd, _ := os.Getwd()
		scriptDir = cwd
	}

	// Set default paths if auto mode
	if *sources == "" {
		*sources = filepath.Join(scriptDir, "..", "data")
	}
	if *target == "" {
		*target = filepath.Join(scriptDir, "..", "src")
	}

	// Resolve to absolute paths
	*sources, _ = filepath.Abs(*sources)
	*target, _ = filepath.Abs(*target)

	if err := checkArgs(*sources, *target); err != nil {
		fmt.Printf("ERROR: %v\n", err)
		fmt.Println("Aborting.")
		os.Exit(1)
	}

	info, err := os.Stat(*sources)
	if err != nil {
		fmt.Printf("ERROR: Cannot stat source: %v\n", err)
		os.Exit(1)
	}

	if info.IsDir() {
		fmt.Printf("Source %s is a directory, searching for files recursively...\n", *sources)
		if err := processDir(*sources, *target, *storeMini); err != nil {
			fmt.Printf("ERROR: %v\n", err)
			os.Exit(1)
		}
	} else {
		fmt.Printf("Source %s is a file, will process one file only.\n", *sources)
		if err := processFile(*sources, *target, *storeMini); err != nil {
			fmt.Printf("ERROR: %v\n", err)
			os.Exit(1)
		}
	}
}

func checkArgs(sources, target string) error {
	if _, err := os.Stat(sources); os.IsNotExist(err) {
		return fmt.Errorf("source %s does not exist", sources)
	}

	targetParent := filepath.Dir(target)
	if info, err := os.Stat(targetParent); err != nil || !info.IsDir() {
		return fmt.Errorf("parent directory of target %s does not exist", target)
	}

	sourceInfo, _ := os.Stat(sources)
	targetInfo, targetErr := os.Stat(target)

	if sourceInfo.IsDir() && (targetErr != nil || !targetInfo.IsDir()) {
		return fmt.Errorf("source %s is a directory, target %s is not", sources, target)
	}

	return nil
}

func getContext(infile, outfile string) (*context, error) {
	infile, err := filepath.Abs(infile)
	if err != nil {
		return nil, err
	}

	// Extract directory, name, and type
	dir := filepath.Base(filepath.Dir(infile))
	base := filepath.Base(infile)
	ext := filepath.Ext(base)
	name := strings.TrimSuffix(base, ext)

	// Remove any .min suffix from name for constant generation
	name = strings.TrimSuffix(name, ".min")

	fileType := strings.TrimPrefix(strings.ToLower(ext), ".")

	// If directory name matches the file type, go up one level
	if strings.ToLower(dir) == fileType {
		dir = filepath.Base(filepath.Dir(filepath.Dir(infile)))
	}

	// Normalize htm to html
	if fileType == "htm" {
		fileType = "html"
	}

	indir := filepath.Dir(infile)

	c := &context{
		infile:   infile,
		fileType: fileType,
		name:     name,
		dir:      dir,
	}

	// Determine output file
	info, err := os.Stat(outfile)
	if err == nil && info.IsDir() {
		c.outdir, _ = filepath.Abs(outfile)
		c.outfilename = fmt.Sprintf("%s%s%s.h", dir, capitalize(name), strings.ToUpper(fileType))
		c.outfile = filepath.Join(c.outdir, c.outfilename)
	} else {
		c.outfile, _ = filepath.Abs(outfile)
		c.outdir = filepath.Dir(c.outfile)
		c.outfilename = filepath.Base(c.outfile)
	}

	// Determine minified file path
	if strings.Contains(infile, ".min.") {
		c.minifile = infile
	} else {
		// Replace .ext with .min.ext
		c.minifile = filepath.Join(indir, strings.TrimSuffix(base, ext)+".min"+ext)
	}

	// Generate constant name
	c.constant = fmt.Sprintf("%s_%s", strings.ToUpper(fileType), strings.ToUpper(name))

	return c, nil
}

func capitalize(s string) string {
	if len(s) == 0 {
		return s
	}
	return strings.ToUpper(s[:1]) + s[1:]
}

func performGzip(c *context) error {
	var buf bytes.Buffer
	gz := gzip.NewWriter(&buf)

	if _, err := gz.Write([]byte(c.minidata)); err != nil {
		return err
	}
	if err := gz.Close(); err != nil {
		return err
	}

	compressed := buf.Bytes()
	c.gziplen = len(compressed)

	// Convert bytes to comma-separated string
	parts := make([]string, len(compressed))
	for i, b := range compressed {
		parts[i] = fmt.Sprintf("%d", b)
	}
	c.gzipdata = strings.Join(parts, ",")

	fmt.Printf("  GZIP data length: %d\n", c.gziplen)
	return nil
}

func performMinify(c *context) error {
	data, err := os.ReadFile(c.infile)
	if err != nil {
		return err
	}

	m := minify.New()
	m.AddFunc("text/css", css.Minify)
	m.AddFunc("text/html", html.Minify)
	m.AddFunc("application/javascript", js.Minify)
	m.AddFunc("text/javascript", js.Minify)

	var mediaType string
	switch c.fileType {
	case "css":
		mediaType = "text/css"
	case "js":
		mediaType = "application/javascript"
	case "html", "htm":
		mediaType = "text/html"
	default:
		mediaType = "text/html"
	}

	fmt.Printf("  Using %s minifier\n", c.fileType)

	minified, err := m.String(mediaType, string(data))
	if err != nil {
		return fmt.Errorf("minification failed: %w", err)
	}

	c.minidata = minified
	return performGzip(c)
}

func processFile(infile, outdir string, storeMini bool) error {
	fmt.Printf("Processing file %s\n", infile)

	c, err := getContext(infile, outdir)
	if err != nil {
		return err
	}

	if err := performMinify(c); err != nil {
		return err
	}

	if storeMini {
		if c.infile == c.minifile {
			fmt.Println("  Original file is already minified, refusing to overwrite it")
		} else {
			fmt.Printf("  Writing minified file %s\n", c.minifile)
			if err := os.WriteFile(c.minifile, []byte(c.minidata), 0644); err != nil {
				return err
			}
		}
	}

	fmt.Printf("  Using C constant names %s and %s_GZIP\n", c.constant, c.constant)
	fmt.Printf("  Writing C header file %s\n", c.outfile)

	output := fmt.Sprintf(targetTemplate, c.constant, c.minidata, c.constant, c.gziplen, c.gzipdata)
	return os.WriteFile(c.outfile, []byte(output), 0644)
}

func processDir(sourceDir, outDir string, storeMini bool) error {
	pattern := regexp.MustCompile(`(?i)\.(css|js|htm|html)$`)
	processed := make(map[string]bool)

	err := filepath.WalkDir(sourceDir, func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		if d.IsDir() {
			return nil
		}

		if !pattern.MatchString(path) {
			return nil
		}

		// Skip if already processed
		if processed[path] {
			return nil
		}

		// Check if this is a .min. file
		if strings.Contains(filepath.Base(path), ".min.") {
			// Only process .min. files if the non-minified version doesn't exist
			nonMinPath := strings.Replace(path, ".min.", ".", 1)
			if _, err := os.Stat(nonMinPath); err == nil {
				// Non-minified version exists, skip this .min. file
				return nil
			}
		} else {
			// Mark corresponding .min. file as processed to avoid duplicate processing
			ext := filepath.Ext(path)
			minPath := strings.TrimSuffix(path, ext) + ".min" + ext
			processed[minPath] = true
		}

		processed[path] = true
		return processFile(path, outDir, storeMini)
	})

	return err
}
