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
# LISTDIR=/afs/cern.ch/user/c/chguan/public/250614.01-DAILYFLUX/00-LIST/02.3
# LISTDIR=/afs/cern.ch/work/c/chguan/public/01--251112/00.1-LIST/04.1
# LISTDIR=/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/00.1-LIST/04.1
LISTDIR=/eos/ams/user/c/chguan/public/260428.01--POSIFLUX/00.1-list/acc1
# ==========
LISTNAME=tskimMC.txt
# ==========
LIST=$LISTDIR/$LISTNAME
# ==========
# ----260309
NFPJ=20
NFPJ=10
NFPJ=5
NFPJ=1
# ==============================================
for JID in $(seq 1 1); do
# for JID in $(seq 21 21); do
    echo "===== TEST START ====="
    echo "Transferring JID=$JID ..."
    echo "----------------------"
    echo 
source ./scripts/CODE.sh $(pwd) $LIST $JID $NFPJ
done
# ==============================================





    echo "===== MOVE START ====="
source mv.sh
    echo "===== MOVE END   ====="
    echo "-----------------------"
