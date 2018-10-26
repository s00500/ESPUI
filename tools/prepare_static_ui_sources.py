#!/usr/bin/env python3

from jsmin import jsmin as jsminify
from htmlmin import minify as htmlminify
from csscompressor import compress as cssminify
import gzip
import sys
import os.path
import argparse
import re
from glob import glob

TARGET_TEMPLATE = '''const char {constant}[] PROGMEM = R"=====(
{minidata}
)=====";

const uint8_t {constant}_GZIP[{gziplen}] PROGMEM = {{ {gzipdata} }};
'''

def parse_arguments(args=None):
    parser = argparse.ArgumentParser(
        description="Prepares ESPUI header files by minifying and gzipping JS and CSS source files.")
    parser.add_argument("--sources", "-s", dest="sources", default="../examples/gui/data",
                        help="Sources directory containing CSS or JS files")
    parser.add_argument("--target", "-t", dest="target", default="../src",
                        help="Target directory containing header files")
    parser.add_argument("--nostoremini", "-m", action="store_false", dest="storemini",
                        help="Store intermediate minified files")
    args = parser.parse_args(args)
    return args

def get_context(infile, outfile):
    infile	= os.path.realpath(infile)
    dir, name, type 	= (os.path.basename(os.path.dirname(infile)), os.path.basename(infile).split(os.path.extsep)[0], os.path.basename(infile).split(os.path.extsep)[-1] )
    type = type.strip(".")
    if dir == type:
        dir = os.path.basename(os.path.dirname(os.path.dirname(infile)))
    if type == "htm":
        type = 'html'
    name = os.path.basename(name)
    indir	= os.path.dirname(infile)
    if os.path.isdir(outfile):
        outdir 	= os.path.realpath(outfile)
        outfilename	= "%s%s%s.h" % (dir, name.capitalize(), type.upper())
        outfile	= os.path.realpath(os.path.sep.join([outdir, outfilename]))
    else:
        outfile	= os.path.realpath(outfile)
        outdir	= os.path.dirname(outfile)
        outfilename	= os.path.basename(outfile)
    minifile	= re.sub('\.([^.]+)$', '.min.\\1', infile) if not ".min." in infile else infile
    constant	= '%s_%s' % (type.upper(), name.upper())
    return locals()
    
def perform_gzip(c):
    compressed = gzip.compress(bytes(c['minidata'], 'utf-8'))
    c['gzipdata'] = ','.join([ str(b) for b in compressed ])
    c['gziplen'] = len(compressed)
    print("  GZIP data length: %s" % c['gziplen'])
    return c
    
def perform_minify(c):
    with open(c['infile']) as infile:
        minifier = cssminify if c['type'] == 'css' else jsminify if c['type'] == 'js' else htmlminify
        print("  Using %s minifier" % c['type'])
        c['minidata'] = minifier(infile.read())
    return perform_gzip(c)

def process_file(infile, outdir, storemini=True):
    print("Processing file %s" % infile)
    c = get_context(infile, outdir)
    c = perform_minify(c)
    if storemini:
        if c['infile'] == c['minifile']:
            print("  Original file is already minified, refusing to overwrite it")
        else:
            print("  Writing minified file %s" % c['minifile'])
            with open(c['minifile'], 'w+') as minifile:
                minifile.write(c['minidata'])
    with open(c['outfile'], 'w+') as outfile:
        print("  Using C constant names %s and %s_GZIP" % (c['constant'], c['constant']))
        print("  Writing C header file %s" % c['outfile'])
        outfile.write(TARGET_TEMPLATE.format(**c))
        
def filenamefilter(pattern, strings):
    return filter(re.compile(pattern).search, strings)        
        
def process_dir(sourcedir, outdir, recursive=True, storemini=True):
    pattern = r'/*\.(css|js|htm|html)$'
    files = glob(sourcedir + "/**/*", recursive=True)+glob(sourcedir + "/*") if recursive else glob(sourcedir + "/*")
    files = filenamefilter(pattern, files)
    for f in set(files):
        if not '.min.' in f:
            process_file(f, outdir, storemini)
        elif not os.path.isfile(f.replace(".min.", ".")):
            process_file(f, outdir, storemini)

def main(args):
    if not args.sources is None:
        if os.path.isfile(args.sources):
            print("Source %s is a file, will process one file only." % args.sources)
            process_file(args.sources, args.target, storemini = args.storemini)
        elif os.path.isdir(args.sources):
            print("Source %s is a directory, searching for files recursively..." % args.sources)
            process_dir(args.sources, args.target, recursive = True, storemini = args.storemini)

if __name__ == "__main__" and "get_ipython" not in dir():
    main(parse_arguments())
