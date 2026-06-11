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
# LISTDIR=/afs/cern.ch/user/c/chguan/public/250614.01-DAILYFLUX/00-LIST/02.2
# LISTDIR=/afs/cern.ch/work/c/chguan/public/01--251112/00.1-LIST/02.2
# LISTDIR=/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/00.1-LIST/02.2
# LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-LIST/02.2
LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-list/num2
# ==========
# ----nele
# LISTNAME=tbuffer.txt
# LISTNAME=tbuffermc.txt
# ----npos
# LISTNAME=tbuffer-n.txt
# LISTNAME=tbuffermc.txt
# ----seleff
LISTNAME=tbuffer-seleff.txt
# LISTNAME=tbuffermc-seleff.txt
# ==========
LIST=$LISTDIR/$LISTNAME
# ==========
# ----251029
# NFPJ=538
# ----260419
# NFPJ=416
# ----260508
NFPJ=576



# ============================================== CONDOR
# ==========
ncpu=1
# memory=1000M
# ========== JobFlavour--1h/2h/8h/1d/3d
# jf="microcentury"
# jf="longlunch"
jf="workday"
# jf="tomorrow"
# jf="testmatch"


# ============================================== COMMON USE
# ----------
loop0=0
loopmax=1
n=0
i0=0
# ---------- 260211
loop0=0
loopmax=1
n=47
i0=0
# ---------- 260308
loop0=0
loopmax=1
n=1
i0=9
# ---------- 260322
loop0=0
loopmax=1
n=50
i0=0
# ---------- 260322
loop0=0
loopmax=1
n=8
i0=0
# ---------- 260329
loop0=0
loopmax=1
n=63
i0=0
# # ---------- 260329
# loop0=0
# loopmax=1
# n=3
# i0=5
# ---------- 260329
loop0=0
loopmax=1
n=54
i0=0
# ---------- 260530
loop0=0
loopmax=1
n=29
i0=0
# ---------- 260610
loop0=0
loopmax=1
n=42
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
    -a "list=$LIST" \
    -a "nfpj=$NFPJ"
done
# ==============================================




    echo "===== ASK START ====="
condor_q