#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

mkdir -p dataout/nele

echo "IN RUN_ROOT ===== root"
root -l -b -q src/main.cpp
