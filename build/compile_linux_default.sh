#!/bin/bash

# Build configuration
config_file_path="./linux_default.cmake"

# Exit on any non-zero status.
trap 'exit' ERR
set -E

echo " "
echo "*** Build Configuration:" 
echo "1 - Config File Path: ${config_file_path}"

# Get variables based on the configuration
cd "$(dirname "$0")"
bash_dir=$(pwd)

# Get Config filename
config_file_name="${config_file_path##*/}"
config_name="${config_file_name%.*}"
echo "A - Config Name: ${config_name}"

build_dir="${bash_dir}/${config_name}"
mkdir -p ${build_dir}
cd ${build_dir}
echo "B - Build Directory: ${build_dir}"

echo " "
echo "*** Staging:" 
# cmake ../../../.. 
cmake ../.. -DCMAKE_INSTALL_PREFIX=${bash_dir}/../stage/ -Dconfig_file_path=${bash_dir}/${config_file_path}

echo " "
echo "*** Compiling:" 
# make VERBOSE=1
# make
# cmake --build . --config RelWithDebInfo --clean-first
cmake --build . --config RelWithDebInfo --clean-first --target install
