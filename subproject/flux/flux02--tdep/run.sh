#!/usr/bin/env bash
set -euo pipefail

# ============ init ============
# ==== for main.cpp
SPECIES="npos"
DATADIR="datain"
FOUTNAME="hflux_t_igrf.root"
EXPS_MODE="igrf"
EXPS_SF="1"
XMIN="1305849600"
XMAX="1761955200"
YMIN="0"
YMAX="15"

# ==== for run.sh
THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
SRCDIR="${THIS_DIR}/src"
SRCFILE="${SRCDIR}/main.cpp"
EXEFILE="${SRCDIR}/flux_tdep.exe"

#====================================================
#---- cmdline override
#---- $1: foutname
#---- $2: datadir
#---- $3:
#     1) npos
#     2) nele
#     3) ALL -> run npos + nele
#---- $4: exps_mode (igrf/st)
#---- $5: exps_sf (1/1.1/1.2/...)
#---- $6: xmin
#---- $7: xmax
#---- $8: ymin
#---- $9: ymax
RUN_ALL=0
if [[ $# -ge 1 ]]; then
    FOUTNAME="$1"
fi
if [[ $# -ge 2 ]]; then
    DATADIR="$2"
fi
if [[ $# -ge 3 ]]; then
    if [[ "$3" == "ALL" ]]; then
        RUN_ALL=1
    else
        SPECIES="$3"
    fi
fi
if [[ $# -ge 4 ]]; then
    EXPS_MODE="$4"
fi
if [[ $# -ge 5 ]]; then
    EXPS_SF="$5"
fi
if [[ $# -ge 6 ]]; then
    XMIN="$6"
fi
if [[ $# -ge 7 ]]; then
    XMAX="$7"
fi
if [[ $# -ge 8 ]]; then
    YMIN="$8"
fi
if [[ $# -ge 9 ]]; then
    YMAX="$9"
fi

#====================================================
BUILD_EXE(){
    echo "IN RUN_ROOT ===== build ${EXEFILE}"
    g++ -std=c++17 -O2 "${SRCFILE}" -o "${EXEFILE}" $(root-config --cflags --libs)
}

RUN_ONE(){
    local foutname_now="$1"
    local datadir_now="$2"
    local species_now="$3"
    local exps_mode_now="$4"
    local exps_sf_now="$5"
    local xmin_now="$6"
    local xmax_now="$7"
    local ymin_now="$8"
    local ymax_now="$9"

    echo "IN RUN_ROOT ===== species=${species_now}"
    echo "IN RUN_ROOT ===== datadir=${datadir_now}"
    echo "IN RUN_ROOT ===== foutname=${foutname_now}"
    echo "IN RUN_ROOT ===== exps_mode=${exps_mode_now}"
    echo "IN RUN_ROOT ===== exps_sf=${exps_sf_now}"
    echo "IN RUN_ROOT ===== xmin=${xmin_now}"
    echo "IN RUN_ROOT ===== xmax=${xmax_now}"
    echo "IN RUN_ROOT ===== ymin=${ymin_now}"
    echo "IN RUN_ROOT ===== ymax=${ymax_now}"
    echo "IN RUN_ROOT ===== time-bin default in source = 1day"
    echo "IN RUN_ROOT ===== 27day option kept commented in src/main.cpp"

    "${EXEFILE}" "${foutname_now}" "${datadir_now}" "${species_now}" "${exps_mode_now}" "${exps_sf_now}" "${xmin_now}" "${xmax_now}" "${ymin_now}" "${ymax_now}"
}

#====================================================
cd "${THIS_DIR}"
echo "IN RUN_ROOT ===== start"
echo "IN RUN_ROOT ===== THIS_DIR=${THIS_DIR}"
echo "IN RUN_ROOT ===== SRCFILE=${SRCFILE}"
echo "IN RUN_ROOT ===== EXEFILE=${EXEFILE}"

BUILD_EXE

if [[ "${RUN_ALL}" -eq 1 ]]; then
    RUN_ONE "hflux_t_${EXPS_MODE}_sf${EXPS_SF}.root" "${DATADIR}" "npos" "${EXPS_MODE}" "${EXPS_SF}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}"
    RUN_ONE "hflux_t_${EXPS_MODE}_sf${EXPS_SF}_nele.root" "${DATADIR}" "nele" "${EXPS_MODE}" "${EXPS_SF}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}"
else
    RUN_ONE "${FOUTNAME}" "${DATADIR}" "${SPECIES}" "${EXPS_MODE}" "${EXPS_SF}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}"
fi

echo "IN RUN_ROOT ===== done"
