#!/usr/bin/env bash
set -euo pipefail

# ============ init ============
# ====for main
FPATH="datain/dataiss/tfit/"
FNAME="fitresult_enebin03_1.33GeV.root"
FPATHNAME=""
TNAME="tfit"
XNAME="tmid"
YNAME="trkeff"
YERRNAME="trkeff_err"
YTITLE="Tracker Efficiency"
YMIN=""
YMAX=""
FOUTNAME="hene"
# ----keep time args for main.cpp CLI compatibility
NT=225
TMIN=1305417600
TMAX=1830297600
# ----xrange
XMIN=""
XMAX=""
# ====for run.sh
BATCH_DIR="datain/dataiss/tfit"

#====================================================
#---- cmdline override
#---- $1:
#     1) "ALL" -> 单次全能量绘图入口（可选 $2 指定目录/通配符）
#        可选 $3 指定变量配置文件(varcfg)
#     2) 其他字符串 -> 作为 ISS 输入目录/通配符/单文件
#        可选 $2 指定变量配置文件(varcfg)
ALL_INPUT=""
VARCFG=""
if [[ $# -ge 1 ]]; then
    if [[ "$1" == "ALL" ]]; then
        FPATHNAME="__RUN_ALL__"
        if [[ $# -ge 2 ]]; then
            ALL_INPUT="$2"
        fi
        if [[ $# -ge 3 ]]; then
            VARCFG="$3"
        fi
    else
        FPATHNAME="$1"
        if [[ $# -ge 2 ]]; then
            VARCFG="$2"
        fi
    fi
fi

#====================================================
THIS_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "${THIS_DIR}"

echo "IN RUN_ROOT ===== start"
echo "IN RUN_ROOT ===== FPATH=${FPATH}"
echo "IN RUN_ROOT ===== FNAME=${FNAME}"
echo "IN RUN_ROOT ===== FPATHNAME=${FPATHNAME}"
echo "IN RUN_ROOT ===== BATCH_DIR=${BATCH_DIR}"
echo "IN RUN_ROOT ===== YNAME=${YNAME}"
echo "IN RUN_ROOT ===== YERRNAME=${YERRNAME}"
echo "IN RUN_ROOT ===== YTITLE=${YTITLE}"
echo "IN RUN_ROOT ===== XMIN=${XMIN}"
echo "IN RUN_ROOT ===== XMAX=${XMAX}"
echo "IN RUN_ROOT ===== YMIN=${YMIN}"
echo "IN RUN_ROOT ===== YMAX=${YMAX}"
echo "IN RUN_ROOT ===== FOUTNAME=${FOUTNAME}"
echo "IN RUN_ROOT ===== TNAME=${TNAME}"
echo "IN RUN_ROOT ===== XNAME=${XNAME}"
echo "IN RUN_ROOT ===== NT=${NT} TMIN=${TMIN} TMAX=${TMAX}"
echo "IN RUN_ROOT ===== VARCFG=${VARCFG}"

#====================================================
ESC_CSTR(){
    local sval="$1"
    sval="${sval//\\/\\\\}"
    sval="${sval//\"/\\\"}"
    printf '%s' "${sval}"
}

TRIM(){
    local s="$1"
    s="${s#${s%%[![:space:]]*}}"
    s="${s%${s##*[![:space:]]}}"
    printf '%s' "${s}"
}

SAFE_TAG(){
    local s="$1"
    local tag=""
    tag="$(printf '%s' "${s}" | tr -c 'A-Za-z0-9_' '_')"
    tag="${tag#_}"
    tag="${tag%_}"
    if [[ -z "${tag}" ]]; then
        tag="var"
    fi
    printf '%s' "${tag}"
}

declare -a CFG_YNAME=()
declare -a CFG_YERRNAME=()
declare -a CFG_YTITLE=()
declare -a CFG_YMIN=()
declare -a CFG_YMAX=()
HAS_VARCFG=0

LOAD_VARCFG(){
    local cfg="$1"
    local line=""
    local yname=""
    local yerrname=""
    local ytitle=""
    local ymin=""
    local ymax=""

    if [[ -z "${cfg}" ]]; then
        return
    fi
    if [[ ! -f "${cfg}" ]]; then
        echo "ERR RUN_ROOT ===== varcfg not found: ${cfg}"
        exit 3
    fi

    while IFS= read -r line || [[ -n "${line}" ]]; do
        [[ -z "${line}" ]] && continue
        [[ "${line}" =~ ^[[:space:]]*# ]] && continue

        IFS='|' read -r yname yerrname ytitle ymin ymax <<< "${line}"
        yname="$(TRIM "${yname}")"
        yerrname="$(TRIM "${yerrname}")"
        ytitle="$(TRIM "${ytitle}")"
        ymin="$(TRIM "${ymin}")"
        ymax="$(TRIM "${ymax}")"
        if [[ -z "${yname}" || -z "${yerrname}" || -z "${ytitle}" ]]; then
            echo "ERR RUN_ROOT ===== bad varcfg line: ${line}"
            echo "ERR RUN_ROOT ===== expected format: yname|yerrname|ytitle|ymin|ymax"
            exit 4
        fi

        CFG_YNAME+=("${yname}")
        CFG_YERRNAME+=("${yerrname}")
        CFG_YTITLE+=("${ytitle}")
        CFG_YMIN+=("${ymin}")
        CFG_YMAX+=("${ymax}")
    done < "${cfg}"

    if [[ ${#CFG_YNAME[@]} -eq 0 ]]; then
        echo "ERR RUN_ROOT ===== varcfg has no valid items: ${cfg}"
        exit 5
    fi

    HAS_VARCFG=1
    echo "IN RUN_ROOT ===== varcfg loaded: ${cfg} n_var=${#CFG_YNAME[@]}"
}

NORMALIZE_FPATHNAME(){
    local p="$1"
    if [[ -z "${p}" ]]; then
        printf '%s' ""
        return
    fi
    if [[ "${p}" == *"*"* || "${p}" == *"?"* || "${p}" == *"["* ]]; then
        printf '%s' "${p}"
        return
    fi
    if [[ -d "${p}" ]]; then
        local d="${p%/}"
        printf '%s' "${d}/*.root"
        return
    fi
    if [[ "${p}" == *.root ]]; then
        printf '%s' "${p}"
        return
    fi
    if [[ -f "${p}.root" ]]; then
        printf '%s' "${p}.root"
        return
    fi
    printf '%s' "${p}*.root"
}

MATCH_MC_FPATHNAME(){
    local p_iss="$1"
    local p_mc=""

    if [[ "${p_iss}" != *"dataiss/tfit"* ]]; then
        return 1
    fi

    p_mc="${p_iss/dataiss\/tfit/datamc\/tfit}"
    printf '%s' "${p_mc}"
}

HAS_MATCHED_ROOT(){
    local pattern="$1"
    local matches=()
    shopt -s nullglob
    matches=(${pattern})
    shopt -u nullglob
    [[ ${#matches[@]} -gt 0 ]]
}

RUN_ONE(){
    local fpathname_now="$1"
    local fpathname_mc_now="$2"
    local foutname_now="$3"
    local fpathname_use=""
    local fpathname_mc_use=""
    local yname_esc=""
    local yerrname_esc=""
    local ytitle_esc=""
    local xmin_esc=""
    local xmax_esc=""
    local ymin_esc=""
    local ymax_esc=""
    local fpathname_esc=""
    local fpathname_mc_esc=""
    local foutname_esc=""

    fpathname_use="$(NORMALIZE_FPATHNAME "${fpathname_now}")"
    fpathname_mc_use="$(NORMALIZE_FPATHNAME "${fpathname_mc_now}")"
    yname_esc="$(ESC_CSTR "${YNAME}")"
    yerrname_esc="$(ESC_CSTR "${YERRNAME}")"
    ytitle_esc="$(ESC_CSTR "${YTITLE}")"
    xmin_esc="$(ESC_CSTR "${XMIN}")"
    xmax_esc="$(ESC_CSTR "${XMAX}")"
    ymin_esc="$(ESC_CSTR "${YMIN}")"
    ymax_esc="$(ESC_CSTR "${YMAX}")"
    fpathname_esc="$(ESC_CSTR "${fpathname_use}")"
    fpathname_mc_esc="$(ESC_CSTR "${fpathname_mc_use}")"
    foutname_esc="$(ESC_CSTR "${foutname_now}")"

    echo "IN RUN_ROOT ===== fpathname_now=${fpathname_now}"
    echo "IN RUN_ROOT ===== fpathname_use=${fpathname_use}"
    echo "IN RUN_ROOT ===== fpathname_mc_now=${fpathname_mc_now}"
    echo "IN RUN_ROOT ===== fpathname_mc_use=${fpathname_mc_use}"
    echo "IN RUN_ROOT ===== foutname_now=${foutname_now}"
    echo "IN RUN_ROOT ===== yname_now=${YNAME} yerr_now=${YERRNAME}"

    root_ver="$(root-config --version 2>/dev/null || echo "")"
    if [[ "${root_ver}" == 5.* ]]; then
        echo "IN RUN_ROOT ===== ROOT5 detected, use compiled binary"
        mkdir -p bin
        g++ -std=c++17 src/main.cpp $(root-config --cflags --libs) -o bin/main_edep_test.exe
        ./bin/main_edep_test.exe "${FPATH}" "${FNAME}" "${fpathname_use}" "${YNAME}" "${YERRNAME}" "${YTITLE}" "${foutname_now}" "${TNAME}" "${XNAME}" "${NT}" "${TMIN}" "${TMAX}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}" "${fpathname_mc_use}"
    else
        local root_cmd
        root_cmd="src/main.cpp(18,(char*[]){(char*)\"ADD_EDEP\",(char*)\"${FPATH}\",(char*)\"${FNAME}\",(char*)\"${fpathname_esc}\",(char*)\"${yname_esc}\",(char*)\"${yerrname_esc}\",(char*)\"${ytitle_esc}\",(char*)\"${foutname_esc}\",(char*)\"${TNAME}\",(char*)\"${XNAME}\",(char*)\"${NT}\",(char*)\"${TMIN}\",(char*)\"${TMAX}\",(char*)\"${xmin_esc}\",(char*)\"${xmax_esc}\",(char*)\"${ymin_esc}\",(char*)\"${ymax_esc}\",(char*)\"${fpathname_mc_esc}\"})"

        echo "IN RUN_ROOT ===== root -l -b -q"
        root -l -b -q "${root_cmd}"
    fi
}

RUN_BY_VARSET(){
    local fpathname_now="$1"
    local fpathname_mc_now="$2"
    local foutname_base="$3"
    local ytag=""
    local i=0

    if [[ ${HAS_VARCFG} -eq 0 ]]; then
        RUN_ONE "${fpathname_now}" "${fpathname_mc_now}" "${foutname_base}"
        return
    fi

    for ((i=0; i<${#CFG_YNAME[@]}; i++)); do
        YNAME="${CFG_YNAME[$i]}"
        YERRNAME="${CFG_YERRNAME[$i]}"
        YTITLE="${CFG_YTITLE[$i]}"
        YMIN="${CFG_YMIN[$i]}"
        YMAX="${CFG_YMAX[$i]}"
        ytag="$(SAFE_TAG "${YNAME}")"
        RUN_ONE "${fpathname_now}" "${fpathname_mc_now}" "${foutname_base}_${ytag}"
    done
}

LOAD_VARCFG "${VARCFG}"

if [[ "${FPATHNAME}" == "__RUN_ALL__" ]]; then
    if [[ -n "${ALL_INPUT}" ]]; then
        f_iss_pattern="$(NORMALIZE_FPATHNAME "${ALL_INPUT}")"
    else
        f_iss_pattern="$(NORMALIZE_FPATHNAME "${BATCH_DIR}")"
    fi
else
    if [[ -n "${FPATHNAME}" ]]; then
        f_iss_pattern="$(NORMALIZE_FPATHNAME "${FPATHNAME}")"
    else
        f_iss_pattern="$(NORMALIZE_FPATHNAME "${FPATH}")"
    fi
fi

f_mc_pattern="$(MATCH_MC_FPATHNAME "${f_iss_pattern}" || true)"
if [[ -z "${f_mc_pattern}" ]]; then
    echo "ERR RUN_ROOT ===== matched mc pattern not found: ${f_iss_pattern}"
    exit 6
fi
if ! HAS_MATCHED_ROOT "${f_iss_pattern}"; then
    echo "ERR RUN_ROOT ===== no iss root files matched: ${f_iss_pattern}"
    exit 7
fi
if ! HAS_MATCHED_ROOT "${f_mc_pattern}"; then
    echo "ERR RUN_ROOT ===== no mc root files matched: ${f_mc_pattern}"
    exit 8
fi

echo "IN RUN_ROOT ===== iss_pattern=${f_iss_pattern}"
echo "IN RUN_ROOT ===== mc_pattern=${f_mc_pattern}"
RUN_BY_VARSET "${f_iss_pattern}" "${f_mc_pattern}" "${FOUTNAME}"

echo "IN RUN_ROOT ===== done"
