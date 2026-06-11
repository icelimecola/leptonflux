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
# LISTDIR=/afs/cern.ch/user/c/chguan/public/250614.01-DAILYFLUX/00-LIST/02.3
# LISTDIR=/afs/cern.ch/work/c/chguan/public/01--251112/00.1-LIST/02.3
# LISTDIR=/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/00.1-LIST/02.3
# LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-LIST/02.3
LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-list/num3
# ==========
# ----nele
# LISTNAME=tbin.txt
# LISTNAME=tbinmc-nele.txt
# ----npos
# LISTNAME=tbin-n.txt
# LISTNAME=tbinmc-n.txt
# ----seleff
LISTNAME=tbin-seleff.txt
# LISTNAME=tbinmc-seleff.txt
# ==========
LIST=$LISTDIR/$LISTNAME


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


# ============================================== COMMON USE
# ----------
loop0=0
loopmax=1
n=0
i0=0
# ----------
loop0=0
loopmax=1
n=1
i0=3
# ----------260304
loop0=0
loopmax=1
n=47
i0=0
# ----------260322
# loop0=0
# loopmax=1
# n=50
# i0=0
# ----------260329
# loop0=0
# loopmax=1
# n=63
# i0=0
# ----------260329
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
    -a "list=$LIST"
done
# ==============================================




    echo "===== ASK START ====="
condor_q