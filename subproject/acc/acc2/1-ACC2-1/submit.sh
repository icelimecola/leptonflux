    echo "===== CLEAR START ====="
source /eos/ams/user/c/chguan/public/scripts/a01-Clean_condor.sh
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



# ============================================== CODE
# ==========
STEP=0
# ==========
# LISTDIR=/afs/cern.ch/user/c/chguan/public/250614.01-DAILYFLUX/00-LIST/02.3
# LISTDIR=/afs/cern.ch/work/c/chguan/public/01--251112/00.1-LIST/04.1
LISTDIR=/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/00.1-LIST/04.1
# ==========
LISTNAME=tskimMC.txt
# ==========
LIST=$LISTDIR/$LISTNAME
# ==========
# ----260309
NFPJ=5
NFPJ=20



# ============================================== CONDOR
# ==========
ncpu=1
# ========== JobFlavour--1h/2h/8h/1d/3d
jf="microcentury"
# jf="longlunch"
# jf="workday"
# jf="tomorrow"
# jf="testmatch"


# ============================================== COMMON USE
# ----------
loop0=0
loopmax=1
n=0
i0=0
# ---------- 260309
loop0=0
loopmax=1
n=1
i0=9
# ---------- 251027
loop0=0
loopmax=1
n=18
i0=0



# ==============================================
for ((i=loop0; i<loopmax; i++)); do
    echo "===== SUBMIT START ====="
    echo "Submitting $i/$loopmax ..."
    echo "----------------------"
    echo 
    condor_submit ./scripts/CONDOR.sub \
    -a "ncpu=$ncpu" \
    -a "jf=$jf" \
    -a "n=$n" \
    -a "pwd=$(pwd)" \
    -a "i0=$i0" \
    -a "step=$STEP" \
    -a "list=$LIST" \
    -a "nfpj=$NFPJ"
done
# ==============================================




    echo "===== ASK START ====="
condor_q