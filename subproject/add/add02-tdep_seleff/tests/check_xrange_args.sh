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
expect_grep 'TString fpathname_mc;' "${main_file}" "VarConf missing MC pathname"

expect_grep 'var\.xmin = 1305849600;' "${main_file}" "INIT missing default xmin"
expect_grep 'var\.xmax = 1761955200;' "${main_file}" "INIT missing default xmax"
expect_grep 'if\(argc > 13 && argv\[13\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing xmin argv parser"
expect_grep 'if\(argc > 14 && argv\[14\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing xmax argv parser"
expect_grep 'if\(argc > 15 && argv\[15\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing ymin argv parser"
expect_grep 'if\(argc > 16 && argv\[16\]\[0\] != '\''\\0'\''\)' "${main_file}" "INIT missing ymax argv parser"
expect_grep 'if\(argc > 17\) var\.fpathname_mc = argv\[17\];' "${main_file}" "INIT missing MC pathname argv parser"
expect_grep 'xmin=' "${main_file}" "INIT print missing xmin"
expect_grep 'xmax=' "${main_file}" "INIT print missing xmax"
expect_grep 'ymin=' "${main_file}" "INIT print missing ymin"
expect_grep 'ymax=' "${main_file}" "INIT print missing ymax"
expect_grep 'fpathname_mc=' "${main_file}" "INIT print missing MC pathname"
expect_grep 'xaxis->SetRangeUser\(var\.xmin, var\.xmax\);' "${main_file}" "DRAW missing configurable x range"
expect_grep 'if\(var\.has_xmin && var\.has_xmax\)' "${main_file}" "DRAW missing x range guard"
expect_grep 'TH1D \*htime = new TH1D\("htime","htime",var\.nt, var\.tmin, var\.tmax\);' "${main_file}" "Histogram bin range changed unexpectedly"
expect_grep 'if\(mc\.n_entry != 1\)' "${main_file}" "MC single-entry guard missing"
expect_grep 'TLine \*line_mc' "${main_file}" "MC horizontal line not drawn"
expect_grep 'TLegend \*leg' "${main_file}" "ISS/MC legend missing"

expect_grep '^XMIN=1305849600$' "${run_file}" "run.sh missing XMIN"
expect_grep '^XMAX=1761955200$' "${run_file}" "run.sh missing XMAX"
expect_grep 'XMIN=\$\{XMIN\}' "${run_file}" "run.sh log missing XMIN"
expect_grep 'XMAX=\$\{XMAX\}' "${run_file}" "run.sh log missing XMAX"
expect_grep 'xmin_now=\$\{XMIN\} xmax_now=\$\{XMAX\}' "${run_file}" "run.sh per-run log missing XMIN/XMAX"
expect_grep 'MATCH_MC_FPATHNAME' "${run_file}" "run.sh missing ISS->MC matcher"
expect_grep 'dataiss/tfit' "${run_file}" "run.sh missing dataiss path mapping"
expect_grep 'datamc\\/tfit' "${run_file}" "run.sh missing datamc path mapping"
expect_grep 'ERR RUN_ROOT ===== matched mc file not found' "${run_file}" "run.sh missing single-file MC error"
expect_grep 'WARN RUN_ROOT ===== matched mc file not found' "${run_file}" "run.sh missing ALL-mode MC warning"
expect_grep 'fpathname_mc_now=' "${run_file}" "run.sh per-run log missing MC pathname"
expect_grep '"\$\{TMIN\}" "\$\{TMAX\}" "\$\{XMIN\}" "\$\{XMAX\}" "\$\{YMIN\}" "\$\{YMAX\}" "\$\{fpathname_mc_use\}"' "${run_file}" "ROOT5 path missing MC arg"
expect_grep '\$\{xmin_esc\}.*\$\{xmax_esc\}.*\$\{ymin_esc\}.*\$\{ymax_esc\}.*\$\{fpathname_mc_esc\}' "${run_file}" "ROOT6 path missing MC arg"

echo "PASS: ISS+MC overlay wiring checks"
