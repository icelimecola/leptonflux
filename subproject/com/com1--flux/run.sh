#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

mkdir -p dataout build

echo "IN RUN_ROOT ===== compile"
g++ src/main.cpp $(root-config --cflags --libs) -o build/flux_compare

echo "IN RUN_ROOT ===== run"
./build/flux_compare
