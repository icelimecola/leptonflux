# ============================================================ rm
rm -r dataiss 
# ============================================================ mkdir
mkdir -p ./dataiss/pdf
mkdir -p ./dataiss/tfit
# ============================================================ mv
mv ./*.pdf ./dataiss/pdf/
mv ./fitresult_*.root ./dataiss/tfit/