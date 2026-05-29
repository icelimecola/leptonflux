#!/bin/bash
# ========== PART I ========== 
# ========== START_TIME
START_TIME=$(date "+%Y-%m-%d %H:%M:%S %Z") # 获取格式化的当前时间，包含时区信息
START_TS=$(date +%s)
echo "===== CODE START ====="
echo "Start Time: $START_TIME" 
echo "Hostname: $(hostname)" 
echo "---------------------"
echo




# ========== PART II ==========
# ========== 2.1--PARAMETERS_1
# ==========
WORKDIR=$1
# ==========
CODENAME=main
# ========== 2.2--PARAMETERS_2
# ==========
file=$2
# ==========
jobid=1
# ==========
nfilesperjob=$3
# ========== 
let enebin=0+$4



# ========== 2.3.1--ENV
    echo "===== ENV START ====="
source /eos/ams/user/c/chguan/public/scripts/SS00-setenv.sh
source /eos/ams/user/c/chguan/public/scripts/SS01-amsroot.sh
    echo "===== ENV END   ====="
    echo "---------------------"
    echo 
# ========== 2.3.2--OUTPUT
echo "===== CODE Running ====="
echo "$WORKDIR/$CODENAME.exe    $file $jobid $nfilesperjob $enebin"
echo "-----------------------"
# ========== 2.3.3--RUN
$WORKDIR/$CODENAME.exe    $file $jobid $nfilesperjob $enebin





# ========== PART III ==========
# ========== END_TIME
PROGRAM_EXIT_CODE=$?
END_TIME=$(date "+%Y-%m-%d %H:%M:%S %Z")
END_TS=$(date +%s)
DURATION=$((END_TS - START_TS))
printf -v DURATION_STR '%02d:%02d:%02d' $((DURATION/3600)) $((DURATION%3600/60)) $((DURATION%60))
# ========== OUTPUT
echo
echo "===== CODE START ====="
echo "Start Time: $START_TIME" 
echo "Hostname: $(hostname)" 
echo "---------------------"
echo "End Time: $END_TIME" 
echo "Exit Code: $PROGRAM_EXIT_CODE"
echo "Total Duration: ${DURATION_STR}"
echo "===== CODE END   ====="
echo "---------------------"
echo
