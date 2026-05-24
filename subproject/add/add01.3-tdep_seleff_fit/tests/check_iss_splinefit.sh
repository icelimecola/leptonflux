#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
MAIN_FILE="${THIS_DIR}/../src/main.cpp"

expect_grep(){
    local pattern="$1"
    local target="$2"
    local msg="$3"
    if ! rg -q "${pattern}" "${target}"; then
        echo "FAIL: ${msg}"
        exit 1
    fi
}

expect_grep 'splineFit3\.h' "${MAIN_FILE}" "main.cpp missing splineFit3 include"
expect_grep 'SplineFit \*spfit_iss' "${MAIN_FILE}" "missing ISS spline fit object"
expect_grep 'SplineFit\(5, "b2e1"' "${MAIN_FILE}" "ISS spline should follow current five-node script setting"
expect_grep 'double xnode_iss\[5\]' "${MAIN_FILE}" "missing current five ISS spline nodes"
expect_grep 'GET_NEAREST_ISS_YNODE' "${MAIN_FILE}" "ISS spline nodes should use nearest real data-point y values"
expect_grep 'SetYnode\(ynode_iss\)' "${MAIN_FILE}" "ISS spline y-nodes should be set from selected data points"
expect_grep 'SetXnodeLimits\(0\)' "${MAIN_FILE}" "ISS spline x-nodes should be fixed"
expect_grep 'inode_valley = 1' "${MAIN_FILE}" "missing selected valley node index"
if ! rg -Fq 'SetParLimits(ipar_valley, ynode_iss[inode_valley], ynode_iss[inode_valley])' "${MAIN_FILE}"; then
    echo "FAIL: missing single valley y-node constraint"
    exit 1
fi
expect_grep '1308916800' "${MAIN_FILE}" "missing current node 0"
expect_grep '1325246400' "${MAIN_FILE}" "missing current node 1"
expect_grep '1339243200' "${MAIN_FILE}" "missing current node 2"
expect_grep '1514203200' "${MAIN_FILE}" "missing current node 3"
expect_grep '1759147200' "${MAIN_FILE}" "missing current node 4"
if rg -q 'SetYnodeLimits\(0\)' "${MAIN_FILE}"; then
    echo "FAIL: ISS spline y-nodes should stay free during fit"
    exit 1
fi
expect_grep 'CalConfInt\(0\.68\)' "${MAIN_FILE}" "missing ISS confidence interval"
expect_grep 'Write\(\"spfit_iss\"' "${MAIN_FILE}" "missing ISS spline fit write"
expect_grep 'Write\(\"giss_conf_band\"' "${MAIN_FILE}" "missing ISS confidence band write"

echo "PASS: ISS splinefit structure checks"
