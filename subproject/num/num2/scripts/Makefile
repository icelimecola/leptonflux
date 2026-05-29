# ======== CONTROL VAR
CODE=main
#  CODE=main_s1_server
#  CODE=main_server_s2
#  CODE=main_server_s3
CODE:=$(CODE).cpp
AACGMnAMSSRC=0
$(info IN MAKE#00 ========== CODE $(CODE) ===== AACGMnAMSSRC $(AACGMnAMSSRC))



# ======== ENV VAR 
# =============== CXX
CXX=$(shell root-config --cxx)
# CXX=g++
# CXX=clang++
# CXX=root
$(info IN MAKE#00 ========== CXX $(CXX))
# =============== ROOTINC
# ROOTINC=$(shell root-config --incdir)
# ROOTINC=/Applications/root_v6.22.00/include
# ROOTINC=/usr/local/Cellar/root/6.30.02/include
# =============== ROOTLIB
ROOTLIB=$(shell root-config --libs)
# ROOTLIB=-L/Applications/root_v6.22.00/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic
$(info IN MAKE#00 ========== ROOTLIB $(ROOTLIB))

# ======== AMS Software
# =============== AMSDIR
AMSDIR=/afs/cern.ch/ams/Offline/vdev/
# =============== AMSMARCH
AMSMARCH=linuxx8664icx5.34_9
# ======== AACGM
AACGMDIR=./c_aacgm_v2.6/
AACGMSRC=$(AACGMDIR)aacgmlib_v2.c $(AACGMDIR)astalglib.c $(AACGMDIR)genmag.c $(AACGMDIR)igrflib.c $(AACGMDIR)mlt_v2.c $(AACGMDIR)rtime.c
# ======== OTHER
NAGDIR=/afs/cern.ch/exp/ams/Offline/CERN/NagLib/
CERNDIR=/afs/cern.ch/exp/ams/Offline/CERN/2005/lib/
TRDDIR=/afs/cern.ch/user/t/tsu/public/TRDVertex/

# ======== SOURCE
# SRCDIR= ./
SRCDIR= ./src/
SRC=$(SRCDIR)$(CODE)
# ======== OBJECT
BIN= ./bin/
MYOBJS = $(SRC:$(SRCDIR)%.cpp=$(BIN)%.o)
ifeq ($(AACGMnAMSSRC),1)
MYOBJS+= $(AACGMSRC:$(AACGMDIR)%.c=$(BIN)%.o)
endif




# ======== CPPFLAGS (for libary)
# =============== CPPFLAGS_D
CPPFLAGS= -D_PGTRACK_ -D__ROOTSHAREDLIBRARY__
# =============== CPPFLAGS_I
CPPFLAGS+= -I$(SRCDIR) -I$(ROOTSYS)/include
ifeq ($(AACGMnAMSSRC),1)
CPPFLAGS+= -I$(AMSDIR)/include
endif
# ======== CXXFLAGS (for general)
CXXFLAGS= $(CPPFLAGS)
# =============== SUB_FLAGS
CXXFLAGS+= -O3 -qopenmp -std=c++11 -g -axSSE4.2,SSE4.1,SSSE3,AVX,CORE-AVX2,CORE-AVX512 -fp-model precise  -Wabi
CXXFLAGS+= -D_GLIBCXX_USE_CXX11_ABI=0



# ======== LIBS 
LIBS= 
ifeq ($(AACGMnAMSSRC),1)
LIBS+= $(AMSLIBS)
endif
LIBS+= $(ROOTLIBS) $(CXXLIBS) $(NAGLIBS) $(CERNLIBS) $(TRDLIBS) 

# =============== CXXLIBS
CXXLIBS= -L/cvmfs/sft.cern.ch/lcg/contrib/gcc/12.1.0.multilib/x86_64-el9/lib64/ -static-libstdc++ $(CFLIBS)
# ======================= CFLIBS
# CFLIBSDIR=/usr/lib64
ifeq "$(CXX)" "icpx"
	CFLIBS=-lifcore
	CF=ifort
else
	CFLIBS=-lgfortran
	CF=gfortran
endif
# =============== OTHER LIBS
ROOTLIBS= $(ROOTLIB) -lMinuit -lNetx -lProof -lProofPlayer -lTMVA -lXMLIO  -lTreePlayer -lMLP -lRooFit -lRooFitCore -lFoam -lGeom -lEG
AMSLIBS= -L$(AMSDIR)lib/$(AMSMARCH) -lntuple_slc6_PG
NAGLIBS= -L$(NAGDIR) -lnag64
CERNLIBS= -L$(CERNDIR) -lmathlib -liomp5
TRDLIBS= -L$(TRDDIR) -lTRDVertex




# ======== MAKEFILE
all: main

# ===============./build/*.o -> ./main.exe
main: $(MYOBJS)
	@echo IN MAKE#02 ========== Compiling $^ ...
# 	$(CXX) $(CXXFLAGS) $^ -o $(SRCDIR)$@.exe $(LIBS)
# 	$(CXX) $(CXXFLAGS) $^ -o $(BIN)$@.exe $(LIBS)
	$(CXX) $(CXXFLAGS) $^ -o $@.exe $(LIBS)
	@echo IN MAKE#02 ========== Compiling DONE ↑

# ===============./main.cpp -> ./build/main.o
$(BIN)%.o: $(SRCDIR)%.cpp
	@echo IN MAKE#01 ========== Compiling $< ...
	$(CXX) -c $(CXXFLAGS) $< -o $@
	@echo IN MAKE#01 ========== Compiling DONE ↑

# =============== (aacgm) ./c_aacgm/*.c -> ./build/*.o
ifeq ($(AACGMnAMSSRC),1)
$(BIN)%.o: $(AACGMDIR)%.c
	@echo IN MAKE#01.5 ========== Compiling $< ...
	$(CXX) -c $(CXXFLAGS) $^ -o $@
	@echo IN MAKE#01.5 ========== Compiling DONE ↑
endif
