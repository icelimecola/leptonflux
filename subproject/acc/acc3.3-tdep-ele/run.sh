#!/usr/bin/env bash
set -euo pipefail

THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
SRCFILE="${THIS_DIR}/src/calcFoldedAcceptance.cpp"
EXEFILE="${THIS_DIR}/src/calcFoldedAcceptance.exe"
MODE="${1:-root}"

cd "${THIS_DIR}"
echo "IN RUN_ROOT ===== start"
echo "IN RUN_ROOT ===== THIS_DIR=${THIS_DIR}"
echo "IN RUN_ROOT ===== SRCFILE=${SRCFILE}"
echo "IN RUN_ROOT ===== MODE=${MODE}"

if [[ "${MODE}" == "compile" ]]; then
	echo "IN RUN_ROOT ===== build ${EXEFILE}"
	clang++ $(root-config --cflags) -I"${THIS_DIR}/src" "${SRCFILE}" \
		-o "${EXEFILE}" \
		$(root-config --libs) -lMinuit -lRooFit -lRooFitCore -lFoam \
		-lTreePlayer -lMLP -lTMVA -lXMLIO -lGeom -lEG
	echo "IN RUN_ROOT ===== run ${EXEFILE}"
	"${EXEFILE}"
else
	echo "IN RUN_ROOT ===== run by ROOT cling"
	root -l -b -q -e ".L ${SRCFILE}" -e "main();"
fi

echo "IN RUN_ROOT ===== done"
