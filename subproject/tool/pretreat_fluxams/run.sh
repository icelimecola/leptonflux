#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

input_file="${1:-datain/hflux_ams.root}"
# output_file="${2:-dataout/hflux_ams_projx.root}"
output_file="${2:-dataout/hflux_ams.root}"

mkdir -p "$(dirname "${output_file}")"

ESC_CSTR(){
    local sval="$1"
    sval="${sval//\\/\\\\}"
    sval="${sval//\"/\\\"}"
    printf '%s' "${sval}"
}

input_esc="$(ESC_CSTR "${input_file}")"
output_esc="$(ESC_CSTR "${output_file}")"
root_cmd="src/proj_fluxams.cpp(3,(char*[]){(char*)\"PROJ_FLUXAMS\",(char*)\"${input_esc}\",(char*)\"${output_esc}\"})"

root_ver="$(root-config --version 2>/dev/null || echo "")"
if [[ "${root_ver}" == 5.* ]]; then
    mkdir -p bin
    g++ -std=c++17 src/proj_fluxams.cpp $(root-config --cflags --libs) -o bin/proj_fluxams.exe
    ./bin/proj_fluxams.exe "${input_file}" "${output_file}"
    exit $?
fi

root -l -b -q "${root_cmd}"
