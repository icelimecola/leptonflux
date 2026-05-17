#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT_DIR}"

main_file="src/main.cpp"

expect_grep(){
  local pattern="$1"
  local file="$2"
  local msg="$3"
  if ! rg -q --multiline "${pattern}" "${file}"; then
    echo "FAIL: ${msg}"
    exit 1
  fi
}

expect_grep 'splineFit3\.h' "${main_file}" "main.cpp missing splineFit3 include"
expect_grep 'TGraphErrors' "${main_file}" "main.cpp missing TGraphErrors usage"
expect_grep 'TPad' "${main_file}" "main.cpp missing TPad usage"
expect_grep 'ISS / MC' "${main_file}" "ratio y-axis title missing"
expect_grep 'mc_y == 0\.0' "${main_file}" "missing MC zero guard"
expect_grep 'ratio_err = ratio' "${main_file}" "missing propagated ratio uncertainty"
expect_grep 'SplineFit \*spfit_ratio' "${main_file}" "missing ratio spline fit object"
expect_grep 'CalConfInt\(0\.68\)' "${main_file}" "missing 0.68 confidence interval"
expect_grep 'GetConfIntBand' "${main_file}" "missing confidence band graph build"
expect_grep 'Write\(\"gratio\"' "${main_file}" "missing ratio graph write"
expect_grep 'Write\(\"spfit_ratio\"' "${main_file}" "missing spline fit write"
expect_grep 'Write\(\"gratio_conf_band\"' "${main_file}" "missing confidence band write"
expect_grep 'new TLine' "${main_file}" "missing ratio reference line"
expect_grep 'new TPad\(\"pad_top\"' "${main_file}" "missing top pad"
expect_grep 'new TPad\(\"pad_ratio\"' "${main_file}" "missing ratio pad"
if rg -q 'SmoothLowess|TGraphSmooth|gratio_smooth' "${main_file}"; then
  echo "FAIL: old Lowess smoothing should be removed"
  exit 1
fi

echo "PASS: ratio panel wiring checks"
