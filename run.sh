#!/bin/bash

# exit the script on command errors or unset variables
# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail
IFS=$'\n\t'

# https://stackoverflow.com/a/246128/295807
readonly script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "${script_dir}"

mkdir -p "${script_dir}/build"

do_in_build="make valgrind"
# NOTE: the imgcat_tree function probably obsoletes this one
do_in_build="make && ./binary_trees && dot -Tpng tmp.dot | imgcat"
do_in_build="make && ./binary_trees"
git ls-files | entr -c -s "cd ${script_dir}/build && ${do_in_build}"

