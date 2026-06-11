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
LISTNAME=tbuffer-n.txt
# LISTNAME=tbuffermc.txt
# ----seleff
# LISTNAME=tbuffer-seleff.txt
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


# ==============================================
# ----positron
# for ENEBIN in $(seq 28 28); do    
# ----electron
for ENEBIN in $(seq 40 40); do
    echo "===== TEST START ====="
    echo "Transferring ENEBIN=$ENEBIN ..."
    echo "----------------------"
    echo 
source ./scripts/CODE.sh $(pwd) $LIST $NFPJ $ENEBIN
done
# ==============================================





    echo "===== MOVE START ====="
source mv.sh
    echo "===== MOVE END   ====="
    echo "-----------------------"
