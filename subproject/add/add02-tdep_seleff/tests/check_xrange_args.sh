#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "${ROOT_DIR}"

main_file="src/main.cpp"
run_file="run.sh"

expect_grep(){
  local pattern="$1"
  local file="$2"
  local msg="$3"
  if ! rg -q --multiline "${pattern}" "${file}"; then
    echo "FAIL: ${msg}"
    exit 1
  fi
}

expect_grep 'double xmin;' "${main_file}" "VarConf missing xmin"
expect_grep 'double xmax;' "${main_file}" "VarConf missing xmax"
expect_grep 'bool has_xmin;' "${main_file}" "VarConf missing has_xmin"
expect_grep 'bool has_xmax;' "${main_file}" "VarConf missing has_xmax"

expect_grep 'var\.xmin = 1305849600;' "${main_file}" "INIT missing default xmin"
expect_grep 'var\.xmax = 1761955200;' "${main_file}" "INIT missing default xmax"
expect_grep 'if\(argc > 13 && argv\[13\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing xmin argv parser"
expect_grep 'if\(argc > 14 && argv\[14\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing xmax argv parser"
expect_grep 'if\(argc > 15 && argv\[15\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing ymin argv parser"
expect_grep 'if\(argc > 16 && argv\[16\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing ymax argv parser"
expect_grep 'xmin=' "${main_file}" "INIT print missing xmin"
expect_grep 'xmax=' "${main_file}" "INIT print missing xmax"
expect_grep 'ymin=' "${main_file}" "INIT print missing ymin"
expect_grep 'ymax=' "${main_file}" "INIT print missing ymax"
expect_grep 'xaxis->SetRangeUser\(var\.xmin, var\.xmax\);' "${main_file}" "DRAW missing configurable x range"
expect_grep 'if\(var\.has_xmin && var\.has_xmax\)' "${main_file}" "DRAW missing x range guard"
expect_grep 'TH1D \*htime = new TH1D\("htime","htime",var\.nt, var\.tmin, var\.tmax\);' "${main_file}" "Histogram bin range changed unexpectedly"

expect_grep '^XMIN=""$' "${run_file}" "run.sh missing XMIN"
expect_grep '^XMAX=""$' "${run_file}" "run.sh missing XMAX"
expect_grep 'XMIN=\$\{XMIN\}' "${run_file}" "run.sh log missing XMIN"
expect_grep 'XMAX=\$\{XMAX\}' "${run_file}" "run.sh log missing XMAX"
expect_grep 'xmin_now=\$\{XMIN\} xmax_now=\$\{XMAX\}' "${run_file}" "run.sh per-run log missing XMIN/XMAX"
expect_grep '"\$\{TMIN\}" "\$\{TMAX\}" "\$\{XMIN\}" "\$\{XMAX\}" "\$\{YMIN\}" "\$\{YMAX\}"' "${run_file}" "ROOT5 path missing reordered XMIN/XMAX args"
expect_grep '\$\{xmin_esc\}.*\$\{xmax_esc\}.*\$\{ymin_esc\}.*\$\{ymax_esc\}' "${run_file}" "ROOT6 path missing reordered XMIN/XMAX args"

echo "PASS: x-range argument wiring checks"
