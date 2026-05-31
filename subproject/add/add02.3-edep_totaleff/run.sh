#!/usr/bin/env bash
set -euo pipefail

FPATH="datain/hene_*eff.root"
FOUTNAME="hene_combine"
XMIN=""
XMAX=""
YMIN=0.83
YMAX=1.02

if [[ $# -ge 1 ]]; then
    FPATH="$1"
fi
if [[ $# -ge 2 ]]; then
    FOUTNAME="$2"
fi
if [[ $# -ge 3 ]]; then
    XMIN="$3"
fi
if [[ $# -ge 4 ]]; then
    XMAX="$4"
fi
if [[ $# -ge 5 ]]; then
    YMIN="$5"
fi
if [[ $# -ge 6 ]]; then
    YMAX="$6"
fi

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

echo "IN RUN_ROOT ===== start"
echo "IN RUN_ROOT ===== FPATH=${FPATH}"
echo "IN RUN_ROOT ===== FOUTNAME=${FOUTNAME}"
echo "IN RUN_ROOT ===== XMIN=${XMIN}"
echo "IN RUN_ROOT ===== XMAX=${XMAX}"
echo "IN RUN_ROOT ===== YMIN=${YMIN}"
echo "IN RUN_ROOT ===== YMAX=${YMAX}"

root_ver="$(root-config --version 2>/dev/null || echo "")"
if [[ "${root_ver}" == 5.* ]]; then
    echo "IN RUN_ROOT ===== ROOT5 detected, use compiled binary"
    mkdir -p bin
    g++ -std=c++17 src/main.cpp $(root-config --cflags --libs) -o bin/main_totaleff.exe
    ./bin/main_totaleff.exe "${FPATH}" "${FOUTNAME}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}"
else
    root -l -b -q "src/main.cpp(7,(char*[]){(char*)\"ADD_EDEP\",(char*)\"${FPATH}\",(char*)\"${FOUTNAME}\",(char*)\"${XMIN}\",(char*)\"${XMAX}\",(char*)\"${YMIN}\",(char*)\"${YMAX}\"})"
fi

echo "IN RUN_ROOT ===== done"
