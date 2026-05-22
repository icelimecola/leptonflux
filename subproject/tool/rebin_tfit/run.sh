#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 2 ]]; then
    echo "Usage: ./run.sh begin_index end_index [input_dir] [output_file]"
    exit 1
fi

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

ESC_CSTR(){
    local sval="$1"
    sval="${sval//\\/\\\\}"
    sval="${sval//\"/\\\"}"
    printf '%s' "${sval}"
}

argv_items='(char*)"MERGE_TFIT"'
argc=1
for arg in "$@"; do
    arg_esc="$(ESC_CSTR "${arg}")"
    argv_items="${argv_items},(char*)\"${arg_esc}\""
    argc=$((argc+1))
done

root_cmd="src/merge_tfit.cpp(${argc},(char*[]){${argv_items}})"
root_ver="$(root-config --version 2>/dev/null || echo "")"
if [[ "${root_ver}" == 5.* ]]; then
    mkdir -p bin
    echo "IN RUN_ROOT ===== ROOT5 detected, use compiled binary"
    g++ -std=c++17 src/merge_tfit.cpp $(root-config --cflags --libs) -o bin/merge_tfit.exe
    ./bin/merge_tfit.exe "$@"
    exit $?
fi

echo "IN RUN_ROOT ===== root -l -b -q"
echo "IN RUN_ROOT ===== root_ver=${root_ver}"
echo "IN RUN_ROOT ===== cmd=${root_cmd}"
root -l -b -q "${root_cmd}"
