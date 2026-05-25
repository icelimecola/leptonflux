#     echo "===== CLEAR START ====="
# source /eos/ams/user/c/chguan/public/scripts/a01-Clean_condor.sh
# source /eos/ams/user/c/chguan/public/scripts/a02-Clean_datafile.sh
#     echo "===== CLEAR END   ====="
#     echo "-----------------------"
#     echo 


#     echo "===== ENV START ====="
# source /eos/ams/user/c/chguan/public/scripts/SS00-setenv.sh
# source /eos/ams/user/c/chguan/public/scripts/SS01-amsroot.sh
#     echo "===== ENV END   ====="
#     echo "---------------------"
#     echo 


#     echo "===== COMPILE START ====="
# make -f ./scripts/Makefile
#     echo "===== COMPILE END   ====="
#     echo "-------------------------"
#     echo 



# # ============================================== CODE
# # ==========




# # ============================================== CONDOR
# # ==========
# ncpu=1
# # ========== JobFlavour--1h/2h/8h/1d/3d
# jf="microcentury"
# # jf="longlunch"
# # jf="workday"
# # jf="tomorrow"
# # jf="testmatch"



# # ============================================== COMMON USE
# # ----------
# loop0=0
# loopmax=1
# n=0
# i0=0
# # ---------- 260310
# loop0=0
# loopmax=1
# n=1
# i0=0




# # ==============================================
# for ((i=loop0; i<loopmax; i++)); do
#     echo "===== SUBMIT START ====="
#     echo "Submitting $i/$loopmax ..."
#     echo "----------------------"
#     echo 
#     condor_submit ./scripts/CONDOR.sub \
#     -a "ncpu=$ncpu" \
#     -a "jf=$jf" \
#     -a "n=$n" \
#     -a "pwd=$(pwd)" \
#     -a "i0=$i0"
# done
# # ==============================================




#     echo "===== ASK START ====="
# condor_q