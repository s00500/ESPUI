# ESPUI Build Tools

This directory contains tools for preparing ESPUI's static web assets (HTML, CSS, JavaScript) for embedding in the ESP firmware.

## prepare_static_ui_sources

This tool processes frontend files from the `data/` directory:

1. **Minifies** HTML, CSS, and JavaScript files to reduce size
2. **Compresses** the minified content using gzip
3. **Generates** C header files with `PROGMEM` constants for the `src/` directory

The generated headers contain both raw minified strings and gzipped byte arrays, allowing ESPUI to serve web content directly from flash memory.

### Prerequisites

**Go version:**
```bash
# Dependencies are managed via go.mod, installed automatically
go mod tidy
```

### Usage

Both versions support the same command-line interface:

```bash
# Auto mode - process all files in data/ and output to src/
# Go:
go run prepare_static_ui_sources.go -a

# Auto mode without writing intermediate .min.* files
# Go:
go run prepare_static_ui_sources.go -a -m

# Explicit source and target directories
# Go:
go run prepare_static_ui_sources.go -s ../data -t ../src

# Process a single file
# Go:
go run prepare_static_ui_sources.go -s ../data/js/controls.js -t ../src/dataControlsJS.h
```

### Command-Line Options

| Flag | Long Form | Description |
|------|-----------|-------------|
| `-a` | `--auto`, `--all` | Auto mode: find all source files in `data/` and write headers to `src/` |
| `-s` | `--source`, `--sources` | Source directory or file to process |
| `-t` | `--target` | Target directory or file for output |
| `-m` | `--nostoremini` | Skip writing intermediate `.min.*` files |

### Output Format

The tool generates C header files with this structure:

```c
const char JS_CONTROLS[] PROGMEM = R"=====(
// minified content here
)=====";

const uint8_t JS_CONTROLS_GZIP[4975] PROGMEM = { 31,139,8,0,... };
```

### File Naming Convention

Source files are mapped to header files as follows:

| Source File | Header File | Constants |
|-------------|-------------|-----------|
| `data/index.htm` | `src/dataIndexHTML.h` | `HTML_INDEX`, `HTML_INDEX_GZIP` |
| `data/css/style.css` | `src/dataStyleCSS.h` | `CSS_STYLE`, `CSS_STYLE_GZIP` |
| `data/js/controls.js` | `src/dataControlsJS.h` | `JS_CONTROLS`, `JS_CONTROLS_GZIP` |

### Building the Go Version

To compile the Go tool into a standalone binary:

```bash
cd tools
go build -o prepare_static_ui_sources prepare_static_ui_sources.go
./prepare_static_ui_sources -a
```

### Development Workflow

After modifying any file in `data/`:

1. Run the tool to regenerate headers:
   ```bash
   cd tools
   go run prepare_static_ui_sources.go -a
   ```

2. Commit both the modified source files and the regenerated headers in `src/`
