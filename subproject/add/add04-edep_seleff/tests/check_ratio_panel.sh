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

expect_grep 'TGraphSmooth' "${main_file}" "main.cpp missing TGraphSmooth usage"
expect_grep 'TGraphErrors' "${main_file}" "main.cpp missing TGraphErrors usage"
expect_grep 'TPad' "${main_file}" "main.cpp missing TPad usage"
expect_grep 'ISS / MC' "${main_file}" "ratio y-axis title missing"
expect_grep 'mc_y == 0\.0' "${main_file}" "missing MC zero guard"
expect_grep 'ratio_err = ratio' "${main_file}" "missing propagated ratio uncertainty"
expect_grep 'SmoothLowess' "${main_file}" "missing Lowess smoothing"
expect_grep '0\.30' "${main_file}" "missing default Lowess span"
expect_grep 'Write\(\"gratio\"' "${main_file}" "missing ratio graph write"
expect_grep 'Write\(\"gratio_smooth\"' "${main_file}" "missing smooth graph write"
expect_grep 'new TLine' "${main_file}" "missing ratio reference line"
expect_grep 'new TPad\(\"pad_top\"' "${main_file}" "missing top pad"
expect_grep 'new TPad\(\"pad_ratio\"' "${main_file}" "missing ratio pad"
if rg -q 'new TSpline3\("sratio"' "${main_file}"; then
  echo "FAIL: direct TSpline3 interpolation should be removed"
  exit 1
fi

echo "PASS: ratio panel wiring checks"
