#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
MAIN_FILE="${THIS_DIR}/../src/main.cpp"
RUN_FILE="${THIS_DIR}/../run.sh"

expect_grep(){
    local pattern="$1"
    local target="$2"
    local msg="$3"
    if ! rg -q "${pattern}" "${target}"; then
        echo "FAIL: ${msg}"
        exit 1
    fi
}

expect_grep 'READ_ALL_EFF' "${MAIN_FILE}" "missing centralized read function"
expect_grep 'DRAW_COMBINE' "${MAIN_FILE}" "missing separate draw function"
expect_grep 'hratio' "${MAIN_FILE}" "missing hratio read/draw"
expect_grep 'gratio_conf_band' "${MAIN_FILE}" "missing confidence band read/draw"
expect_grep 'spfit_ratio' "${MAIN_FILE}" "missing spline fit read/draw"
expect_grep 'totaleff' "${MAIN_FILE}" "missing totaleff handling"
expect_grep 'datain/hene_\*eff\.root' "${RUN_FILE}" "missing add05 default input pattern"

echo "PASS: combine plot structure checks"
