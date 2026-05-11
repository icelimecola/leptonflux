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
# LISTDIR=/afs/cern.ch/user/c/chguan/public/250614.01-DAILYFLUX/00-LIST/01.1
# LISTDIR=/afs/cern.ch/work/c/chguan/public/01--251112/00.1-LIST/01.1
# LISTDIR=/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/00.1-LIST/01.1
LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-LIST/01.1
# ==========
LISTNAME=tskim.txt
# ==========
LIST=$LISTDIR/$LISTNAME
# ==========
# NFPJ=1
NFPJ=20


# ============================================== CONDOR
# ==========
ncpu=1
# ========== JobFlavour--20m/1h/2h/8h/1d/3d
# jf="espresso"
# jf="microcentury"
# jf="longlunch"
jf="workday"
# jf="tomorrow"
# jf="testmatch"
# ==========
ilossfile=a07-dataiss/a07-missing.txt


# ============================================== COMMON USE
# ----------
loop0=0
loopmax=1
n=0
i0=0
# ---------- 260202
loop0=0
loopmax=1
n=1431
i0=0
# ---------- 260511
loop0=0
loopmax=1
n=1449
i0=0


# ==============================================
for ((i=loop0; i<loopmax; i++)); do
    echo "===== SUBMIT START ====="
    echo "Submitting $i/$loopmax ..."
    echo "----------------------"
    echo 
    #==== for stage01
    condor_submit ./scripts/CONDOR.sub \
    -a "ncpu=$ncpu" \
    -a "jf=$jf" \
    -a "n=$n" \
    -a "pwd=$(pwd)" \
    -a "i0=$i0" \
    -a "list=$LIST" \
    -a "nfpj=$NFPJ"
    #==== for stage02
    # condor_submit ./scripts/CONDOR.sub \
    # -a "ilossfile=$ilossfile" \
    # -a "ncpu=$ncpu" \
    # -a "jf=$jf" \
    # -a "n=$n" \
    # -a "pwd=$(pwd)" \
    # -a "i0=$i0" \
    # -a "list=$LIST" \
    # -a "nfpj=$NFPJ"
done
# ==============================================




echo "===== ASK START ====="
condor_q