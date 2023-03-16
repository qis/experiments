#!/bin/sh
script_file=`readlink -f "$0"`
script_path=`dirname "${script_file}"`
source_path=`dirname "${script_path}"`
cd "${source_path}" || exit 1

date

cmake --preset debug-clang
cmake --preset release-clang

cmake --build build/debug-clang
cmake --build build/release-clang

date
