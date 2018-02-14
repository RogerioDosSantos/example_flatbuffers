#!/bin/bash

# Setup - Go to the directory where the bash file is
echo "* $(basename "$0")"
call_dir=$(pwd)
cd "$(dirname "$0")"
bash_dir=$(pwd)
echo "- Called from ${call_dir}"
echo "- Running from ${bash_dir}"
cd "${bash_dir}"

# Exit on any non-zero status.
trap 'exit' ERR
set -E

# Configuration
echo "- Configuration:"
if [ -z "$1" ]; then config_schema_path="${bash_dir}/../include/schema.fbs"; else config_schema_path="${call_dir}/$1"; fi 
echo "1- config_schema_path: ${config_schema_path}"
if [ -z "$2" ]; then config_output_dir="${bash_dir}/../src"; else config_output_dir="${call_dir}/$2"; fi 
echo "2- config_output_dir: ${config_output_dir}"
if [ -z "$3" ]; then config_flatbuffer_compiler_path="${bash_dir}/../../../../third-party/flatbuffers/stage/Linux-4.4.0-43-Microsoft/x86_64/bin/flatc"; else config_flatbuffer_compiler_path="${call_dir}/$3"; fi 
echo "3- config_flatbuffer_compiler_path: ${config_flatbuffer_compiler_path}"

# Instructions
echo "Compile Schema using Flatbuffer compiler"
${config_flatbuffer_compiler_path} -c -o ${config_output_dir} ${config_schema_path}

# Setup - Return to the called directory
cd "${call_dir}"

