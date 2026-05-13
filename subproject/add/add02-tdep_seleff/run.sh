#!/usr/bin/env bash
set -euo pipefail

# ============ init ============
# ====for main
FPATH="datain/26*/dataiss/tfit/"
FNAME="fitresult_enebin03_1.33GeV.root"
FPATHNAME=""
TNAME="tfit"
XNAME="tmid"
YNAME="trkeff"
YERRNAME="trkeff_err"
YTITLE="Tracker Efficiency"
YMIN=""
YMAX=""
FOUTNAME="htime"
# ----tbin
NT=6000
TMIN=1305417600
TMAX=1823817600
# NT=225
# TMIN=1305417600
# TMAX=1830297600
# ----xrange
XMIN=1305417600
XMAX=1761955200
# ====for run.sh
BATCH_DIR="datain/260421.01--tfit/dataiss/tfit"

#====================================================
#---- cmdline override
#---- $1:
#     1) "ALL" -> 批处理（可选 $2 指定目录/通配符）
#        可选 $3 指定变量配置文件(varcfg)
#     2) 其他字符串 -> 作为 fpathname (优先于 FPATH+FNAME)
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
#---- 进入项目根目录，保证相对路径稳定
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
#---- 转义字符串，避免路径中的特殊字符破坏 ROOT 命令
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

FPATH_ESC="$(ESC_CSTR "${FPATH}")"
FNAME_ESC="$(ESC_CSTR "${FNAME}")"
TNAME_ESC="$(ESC_CSTR "${TNAME}")"
XNAME_ESC="$(ESC_CSTR "${XNAME}")"

declare -a CFG_YNAME=()
declare -a CFG_YERRNAME=()
declare -a CFG_YTITLE=()
declare -a CFG_YMIN=()
declare -a CFG_YMAX=()
HAS_VARCFG=0


# ============ pass var ============
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

RUN_ONE(){
    local fpathname_now="$1"
    local foutname_now="$2"
    local fpathname_use=""
    local yname_esc=""
    local yerrname_esc=""
    local ytitle_esc=""
    local xmin_esc=""
    local xmax_esc=""
    local ymin_esc=""
    local ymax_esc=""
    fpathname_use="$(NORMALIZE_FPATHNAME "${fpathname_now}")"

    yname_esc="$(ESC_CSTR "${YNAME}")"
    yerrname_esc="$(ESC_CSTR "${YERRNAME}")"
    ytitle_esc="$(ESC_CSTR "${YTITLE}")"
    xmin_esc="$(ESC_CSTR "${XMIN}")"
    xmax_esc="$(ESC_CSTR "${XMAX}")"
    ymin_esc="$(ESC_CSTR "${YMIN}")"
    ymax_esc="$(ESC_CSTR "${YMAX}")"

    echo "IN RUN_ROOT ===== fpathname_now=${fpathname_now}"
    if [[ "${fpathname_use}" != "${fpathname_now}" ]]; then
        echo "IN RUN_ROOT ===== fpathname_norm=${fpathname_use}"
    fi
    echo "IN RUN_ROOT ===== foutname_now=${foutname_now}"
    echo "IN RUN_ROOT ===== yname_now=${YNAME} yerr_now=${YERRNAME}"
    echo "IN RUN_ROOT ===== xmin_now=${XMIN} xmax_now=${XMAX}"
    echo "IN RUN_ROOT ===== ymin_now=${YMIN} ymax_now=${YMAX}"

    root_ver="$(root-config --version 2>/dev/null || echo "")"
    if [[ "${root_ver}" == 5.* ]]; then
        echo "IN RUN_ROOT ===== ROOT5 detected, use compiled binary"
        mkdir -p bin
        g++ -std=c++17 src/main.cpp $(root-config --cflags --libs) -o bin/main_utime_test.exe
        ./bin/main_utime_test.exe "${FPATH}" "${FNAME}" "${fpathname_use}" "${YNAME}" "${YERRNAME}" "${YTITLE}" "${foutname_now}" "${TNAME}" "${XNAME}" "${NT}" "${TMIN}" "${TMAX}" "${XMIN}" "${XMAX}" "${YMIN}" "${YMAX}"
    else
        local fpathname_esc
        local foutname_esc
        fpathname_esc="$(ESC_CSTR "${fpathname_use}")"
        foutname_esc="$(ESC_CSTR "${foutname_now}")"

        local root_cmd
        root_cmd="src/main.cpp(17,(char*[]){(char*)\"ADD_TIME\",(char*)\"${FPATH_ESC}\",(char*)\"${FNAME_ESC}\",(char*)\"${fpathname_esc}\",(char*)\"${yname_esc}\",(char*)\"${yerrname_esc}\",(char*)\"${ytitle_esc}\",(char*)\"${foutname_esc}\",(char*)\"${TNAME_ESC}\",(char*)\"${XNAME_ESC}\",(char*)\"${NT}\",(char*)\"${TMIN}\",(char*)\"${TMAX}\",(char*)\"${xmin_esc}\",(char*)\"${xmax_esc}\",(char*)\"${ymin_esc}\",(char*)\"${ymax_esc}\"})"

        echo "IN RUN_ROOT ===== root -l -b -q"
        root -l -b -q "${root_cmd}"
    fi
}

RUN_BY_VARSET(){
    local fpathname_now="$1"
    local foutname_base="$2"
    local ytag=""
    local i=0

    if [[ ${HAS_VARCFG} -eq 0 ]]; then
        RUN_ONE "${fpathname_now}" "${foutname_base}"
        return
    fi

    for ((i=0; i<${#CFG_YNAME[@]}; i++)); do
        YNAME="${CFG_YNAME[$i]}"
        YERRNAME="${CFG_YERRNAME[$i]}"
        YTITLE="${CFG_YTITLE[$i]}"
        YMIN="${CFG_YMIN[$i]}"
        YMAX="${CFG_YMAX[$i]}"
        ytag="$(SAFE_TAG "${YNAME}")"
        RUN_ONE "${fpathname_now}" "${foutname_base}_${ytag}"
    done
}

#====================================================
#---- normalize single-input pathname:
#     ""                      -> keep empty (fallback to FPATH+FNAME in C++)
#     "/path/dir"             -> "/path/dir/*.root"
#     "/path/prefix"          -> "/path/prefix*.root"
#     "/path/file.root"       -> keep
#     contains wildcard       -> keep
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

if [[ "${FPATHNAME}" == "__RUN_ALL__" ]]; then
    LOAD_VARCFG "${VARCFG}"

    batch_pattern=""
    if [[ -n "${ALL_INPUT}" ]]; then
        if [[ "${ALL_INPUT}" == *"*"* || "${ALL_INPUT}" == *"?"* || "${ALL_INPUT}" == *"["* ]]; then
            batch_pattern="${ALL_INPUT}"
        else
            batch_pattern="${ALL_INPUT%/}"/*.root
        fi
    else
        batch_pattern="${BATCH_DIR}"/*.root
    fi

    shopt -s nullglob
    files=(${batch_pattern})
    shopt -u nullglob

    if [[ ${#files[@]} -eq 0 ]]; then
        echo "ERR RUN_ROOT ===== no root files matched: ${batch_pattern}"
        exit 2
    fi

    echo "IN RUN_ROOT ===== ALL mode, pattern=${batch_pattern} n_file=${#files[@]}"
    n_ok=0
    n_fail=0
    for f in "${files[@]}"; do
        b="$(basename "${f}")"
        b_noext="${b%.root}"
        tag="${b_noext}"
        if [[ "${b_noext}" =~ ^fitresult_enebin([0-9]+)_(.+GeV)$ ]]; then
            tag="ene${BASH_REMATCH[1]}_${BASH_REMATCH[2]}"
        fi
        foutname_now="${FOUTNAME}_${tag}"

        if [[ ${HAS_VARCFG} -eq 0 ]]; then
            if RUN_ONE "${f}" "${foutname_now}"; then
                n_ok=$((n_ok+1))
            else
                echo "WARN RUN_ROOT ===== failed: ${f}"
                n_fail=$((n_fail+1))
            fi
        else
            for ((i_var=0; i_var<${#CFG_YNAME[@]}; i_var++)); do
                YNAME="${CFG_YNAME[$i_var]}"
                YERRNAME="${CFG_YERRNAME[$i_var]}"
                YTITLE="${CFG_YTITLE[$i_var]}"
                YMIN="${CFG_YMIN[$i_var]}"
                YMAX="${CFG_YMAX[$i_var]}"
                ytag="$(SAFE_TAG "${YNAME}")"

                if RUN_ONE "${f}" "${foutname_now}_${ytag}"; then
                    n_ok=$((n_ok+1))
                else
                    echo "WARN RUN_ROOT ===== failed: ${f} y=${YNAME}"
                    n_fail=$((n_fail+1))
                fi
            done
        fi
    done

    echo "IN RUN_ROOT ===== ALL done, n_ok=${n_ok} n_fail=${n_fail}"
    if [[ ${n_fail} -gt 0 ]]; then
        exit 1
    fi
else
    LOAD_VARCFG "${VARCFG}"
    RUN_BY_VARSET "${FPATHNAME}" "${FOUTNAME}"
fi

echo "IN RUN_ROOT ===== done"
