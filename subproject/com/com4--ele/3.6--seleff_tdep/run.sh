#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

mkdir -p dataout/trdeff dataout/ntrkeff dataout/ecaleff dataout/matcheff

echo "IN RUN_ROOT ===== root"
root -l -b -q src/main.cpp
