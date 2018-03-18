
"""
    Script for geting all dlls from conan cache folders and bringing them to a specified folder, 
    for convenience and build automation. 

    Usage: py getdlls.py -d <path_where_the_dlls_go>

    dlls are broken up by environment, which is determined by conaninfo.txt files. 
"""

import sys
import getopt
from pathlib import Path
import os
import re
import glob
from shutil import copy2
import logging


dllsdir_default = 'dlls'

build_type_text = 'build_type=(.*)?\s'
build_type_re = re.compile(build_type_text)


def handle_package(build_dir, dllsdir):
    conaninfo = os.path.join(build_dir, 'conaninfo.txt')
    conanlink = os.path.join(build_dir, '.conan_link')
    
    if os.path.exists(conaninfo):
        conaninfo_fh = open(conaninfo, "r")
        conaninfo_txt = conaninfo_fh.read()
        conaninfo_re_res = build_type_re.search(conaninfo_txt)

        if conaninfo_re_res is None:
            return

        env = conaninfo_re_res.group(1)

        dllsdir_build = os.path.abspath(os.path.join(dllsdir, env))

        if not os.path.exists(dllsdir_build):
            os.makedirs(dllsdir_build)

        glob_str = os.path.join(build_dir, '**', '*.dll')
        for filename in glob.iglob(glob_str, recursive=True):
            copy2(filename, dllsdir_build)

    elif os.path.exists(conanlink):
        conanlink_fh = open(conanlink, "r")
        conanlink_txt = conanlink_fh.read()
        handle_package(conanlink_txt, dllsdir)

    else:
        logging.warning(f'No conaninfo.txt or .conan_link in {build_dir}')


def find_all_build_dirs():
    home = str(Path.home())
    conan_data_path = os.path.join(home, '.conan', 'data')

    lib_dir_re_text = os.path.join(
        conan_data_path, '.*', 'package', '[a-z|0-9]*$').replace('\\', '\\\\')
    lib_dir_re = re.compile(lib_dir_re_text)

    build_dirs = set()
    for root, dirs, files in os.walk(conan_data_path, topdown=False):
        if lib_dir_re.match(root):
            build_dirs.add(root)
    return build_dirs


def main(argv):
    dllsdir = ''

    try:
        opts, args = getopt.getopt(argv, "hdb", ["dllsdir", "buildconfigs"])

    except getopt.GetoptError:
        print('getdlls.py -d <dllsdir>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('getdlls.py -d <dllsdir>')
            sys.exit()
        elif opt in ("-d", "--dllsdir"):
            dllsdir = arg
    
    dllsdir = dllsdir or dllsdir_default

    build_dirs = find_all_build_dirs()

    for build_dir in build_dirs:                 
        handle_package(build_dir, dllsdir)
           

if __name__ == "__main__":
    main(sys.argv[1:])
