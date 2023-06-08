#!/bin/bash

# First the following commands before using this script
#bear make clean_all
#bear make [...]

# This script will replace the path of the source files in the compile_commands.json
# Use sed to replace /root/coffer with current directory

# Get the current directory
current_dir=$(pwd)

# Replace the path of the source files in the compile_commands.json
sed -i "s/\/root\/coffer/${current_dir//\//\\/}/g" compile_commands.json