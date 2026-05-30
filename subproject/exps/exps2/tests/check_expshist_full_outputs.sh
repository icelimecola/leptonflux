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

require_pattern '^[[:space:]]*if\( h2exp_st_TvE\[isf\] \) h2exp_st_TvE\[isf\]->Add\(\(TH2D\*\)ins_filein->Get\(Form\("h2/h2exp_st_TvE_sf%g"' "missing h2 stormer Add()"
require_pattern '^[[:space:]]*if\( h2exp_igrf_TvE\[isf\] \) h2exp_igrf_TvE\[isf\]->Add\(\(TH2D\*\)ins_filein->Get\(Form\("h2/h2exp_igrf_TvE_sf%g"' "missing h2 igrf Add()"
require_pattern '^[[:space:]]*if\( h2exp_ts05_TvE\[isf\] \) h2exp_ts05_TvE\[isf\]->Add\(\(TH2D\*\)ins_filein->Get\(Form\("h2/h2exp_ts05_TvE_sf%g"' "missing h2 ts05 Add()"
require_pattern '^[[:space:]]*h1exp_st_T\[ifov\]\[isf\]\[iene\]->Add\(\(TH1D\*\)ins_filein->Get\(Form\("h1t/sf%g/h1exp_st_T' "missing T stormer Add()"
require_pattern '^[[:space:]]*h1exp_igrf_T\[ifov\]\[isf\]\[iene\]->Add\(\(TH1D\*\)ins_filein->Get\(Form\("h1t/sf%g/h1exp_igrf_T' "missing T igrf Add()"
require_pattern '^[[:space:]]*h1exp_st_E\[ifov\]\[isf\]->Add\(\(TH1D\*\)ins_filein->Get\(Form\("h1ene/sf%g/h1exp_st_E' "missing E stormer Add()"
require_pattern '^[[:space:]]*h1exp_igrf_E\[ifov\]\[isf\]->Add\(\(TH1D\*\)ins_filein->Get\(Form\("h1ene/sf%g/h1exp_igrf_E' "missing E igrf Add()"
if rg -n 'energy_bins\[kene\][[:space:]]*>=[[:space:]]*info_maxcf\[ifov\]\*factor\[jfactor\]' "$file" >/dev/null; then
    echo "FAIL: merge stage still gates T histograms by info_maxcf"
    exit 1
fi
require_pattern '^[[:space:]]*TDirectory \*dir_h2 = dir_top->GetDirectory\("h2"\)' "missing h2 output directory"
require_pattern '^[[:space:]]*TDirectory \*dir_h1ene = dir_top->GetDirectory\("h1ene"\)' "missing h1ene output directory"
require_pattern '^[[:space:]]*TDirectory \*dir_h1t = dir_top->GetDirectory\("h1t"\)' "missing h1t output directory"
require_pattern '^[[:space:]]*if\( h2exp_st_TvE\[isf\] \) h2exp_st_TvE\[isf\]->Write' "missing h2 stormer Write()"
require_pattern '^[[:space:]]*if\( h2exp_igrf_TvE\[isf\] \) h2exp_igrf_TvE\[isf\]->Write' "missing h2 igrf Write()"
require_pattern '^[[:space:]]*if\( h2exp_ts05_TvE\[isf\] \) h2exp_ts05_TvE\[isf\]->Write' "missing h2 ts05 Write()"
require_pattern '^[[:space:]]*if\(h1exp_st_T\[ifov\]\[isf\]\[iene\]\)' "missing T stormer Write() guard"
require_pattern '^[[:space:]]*h1exp_st_T\[ifov\]\[isf\]\[iene\]->Write' "missing T stormer Write()"
require_pattern '^[[:space:]]*h1exp_igrf_T\[ifov\]\[isf\]\[iene\]->Write' "missing T igrf Write()"

echo "PASS: ExpsHist saves and merges h2/E/T outputs in directories"
