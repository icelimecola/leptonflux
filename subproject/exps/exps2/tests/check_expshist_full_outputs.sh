#!/bin/bash
set -euo pipefail

file="src/ExpsHist.hpp"

require_pattern() {
    local pattern="$1"
    local message="$2"
    if ! rg -n "$pattern" "$file" >/dev/null; then
        echo "FAIL: $message"
        exit 1
    fi
}

require_pattern '^[[:space:]]*if\( h2exp_st_TvE\[j\] \) h2exp_st_TvE\[j\]->Add' "missing h2 stormer Add()"
require_pattern '^[[:space:]]*if\( h2exp_igrf_TvE\[j\] \) h2exp_igrf_TvE\[j\]->Add' "missing h2 igrf Add()"
require_pattern '^[[:space:]]*if\( h2exp_ts05_TvE\[j\] \) h2exp_ts05_TvE\[j\]->Add' "missing h2 ts05 Add()"
require_pattern '^[[:space:]]*h1exp_st_T\[ifov\]\[jfactor\]\[kene\]->Add' "missing T stormer Add()"
require_pattern '^[[:space:]]*h1exp_igrf_T\[ifov\]\[jfactor\]\[kene\]->Add' "missing T igrf Add()"
if rg -n 'energy_bins\[kene\][[:space:]]*>=[[:space:]]*info_maxcf\[ifov\]\*factor\[jfactor\]' "$file" >/dev/null; then
    echo "FAIL: merge stage still gates T histograms by info_maxcf"
    exit 1
fi
require_pattern '^[[:space:]]*if\( h2exp_st_TvE\[i\] \) h2exp_st_TvE\[i\]->Write' "missing h2 stormer Write()"
require_pattern '^[[:space:]]*if\( h2exp_igrf_TvE\[i\] \) h2exp_igrf_TvE\[i\]->Write' "missing h2 igrf Write()"
require_pattern '^[[:space:]]*if\( h2exp_ts05_TvE\[i\] \) h2exp_ts05_TvE\[i\]->Write' "missing h2 ts05 Write()"
require_pattern '^[[:space:]]*if\(h1exp_st_T\[i\]\[j\]\[k\]\)' "missing T stormer Write() guard"
require_pattern '^[[:space:]]*h1exp_st_T\[i\]\[j\]\[k\]->Write' "missing T stormer Write()"
require_pattern '^[[:space:]]*h1exp_igrf_T\[i\]\[j\]\[k\]->Write' "missing T igrf Write()"

echo "PASS: ExpsHist saves and merges h2/E/T outputs"
