    echo "===== CLEAR START ====="
source /eos/ams/user/c/chguan/public/scripts/a02-Clean_datafile.sh
    echo "===== CLEAR END   ====="
    echo "-----------------------"
    echo 


    echo "===== ENV START ====="
source /eos/ams/user/c/chguan/public/scripts/SS00-setenv.sh
source /eos/ams/user/c/chguan/public/scripts/SS01-amsroot.sh
    echo "===== ENV END   ====="
    echo "---------------------"
    echo 


    echo "===== COMPILE START ====="
make -f ./scripts/Makefile
    echo "===== COMPILE END   ====="
    echo "-------------------------"
    echo 



# ==============================================
# ==========
STEP=1
# ==========
SAMPLE=el.B1236
# ==========
STREAM=el.pl1.0_254000
# ==============================================
source ./scripts/CODE.sh $(pwd) $STEP $SAMPLE $STREAM
# ==============================================





    echo "===== MOVE START ====="
source mv.sh
    echo "===== MOVE END   ====="
    echo "-----------------------"
