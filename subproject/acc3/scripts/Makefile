# ======== CXX
CXX=$(shell root-config --cxx)
MARCH=$(shell root-config --arch)
MARCH=linuxx8664icx5.34_9
ROOTLIB=$(shell root-config --libs)

ifeq "$(CXX)" "icpx"
CFLIB=-lifcore
CF=ifort
else
CFLIB=-lgfortran
CF=gfortran
endif

# ======== INDEX
AMSSRC=/afs/cern.ch/ams/Offline/vdev/
NAGDIR=/afs/cern.ch/exp/ams/Offline/CERN/NagLib/
CERNDIRLIB=/afs/cern.ch/exp/ams/Offline/CERN/2005/lib/
# SRC= ./
SRC= ./src/
BIN= ./bin/
# ======== SOURCE NAME
# SRCS= $(SRC)TRK.C
# SRCS=$(SRC)t_add.C $(SRC)Analysis.C $(SRC)MyBDT_ChargePID.C $(SRC)ConfigParser.C
SRCS=$(SRC)t_add.cpp $(SRC)Analysis.cpp $(SRC)MyBDT_ChargePID.cpp $(SRC)ConfigParser.cpp
# SRCS= $(SRC)TRK.cpp
# ======== OBJECT NAME
# MYOBJS := $(SRCS:$(SRC)%.C=$(BIN)%.o)
MYOBJS := $(SRCS:$(SRC)%.cpp=$(BIN)%.o)

# ======== CXXFLAGS
CPPFLAGS= -D_PGTRACK_ -D__ROOTSHAREDLIBRARY__ 
CPPFLAGS+= -I$(SRC) -I$(ROOTSYS)/include -I$(AMSSRC)include #-I../TRDVertex/
#CXXFLAGS= -no-multibyte-chars -std=c++11 -g $(WFLAGS) $(CPPFLAGS) 
CXXFLAGS= $(CPPFLAGS) -O3 -qopenmp -std=c++11 -g -axSSE4.2,SSE4.1,SSSE3,AVX,CORE-AVX2,CORE-AVX512 -fp-model precise  -Wabi -D_GLIBCXX_USE_CXX11_ABI=0

# ======== LIBS
# CXXLIBS=-L/cvmfs/sft.cern.ch/lcg/contrib/gcc/9.2.0/x86_64-slc6-gcc9-opt/lib64/ -static-libstdc++
ROOTLIBS=$(ROOTLIB) -lMinuit -lNetx -lProof -lProofPlayer -lTMVA -lXMLIO  -lTreePlayer -lMLP -lRooFit -lRooFitCore -lFoam  -lGeom -lEG $(CFLIB)
AMSLIBS=-L$(AMSSRC)/lib/$(MARCH) -lntuple_slc6_PG
NAGLIBS=-L$(NAGDIR) -lnag64
CERNLIBS=-L$(CERNDIRLIB) -lmathlib -liomp5

LIBS= $(CXXLIBS) $(ROOTLIBS) $(AMSLIBS) $(NAGLIBS) $(CERNLIBS)

# # ======== MAKEFILE
# all: TRK

# # ===============./build/*.o -> ./main.exe
# TRK: $(MYOBJS)
# 	@echo IN MAKE#02 ========== Compiling $^ ...
# 	$(CXX) $(CXXFLAGS) $^ -o $(SRC)$@.exe $(LIBS)
# # 	$(CXX) $(CXXFLAGS) $^ -o $(SRC)$@ $(LIBS)
# 	@echo IN MAKE#02 ========== Compiling DONE ↑

# # ===============./main.cpp -> ./build/main.o
# $(BIN)%.o: $(SRC)%.C
# # $(BIN)%.o: $(SRC)%.cpp
# 	@echo IN MAKE#01 ========== Compiling $< ...
# 	$(CXX) -c $(CXXFLAGS) $< -o $@
# 	@echo IN MAKE#01 ========== Compiling DONE ↑


# all: Analysis
# all: readmcinfo
all: calcFoldedAcceptance

readmcinfo: $(SRC)readmcinfo.cpp
	$(CXX) -qopenmp -o readmcinfo.exe $(SRC)readmcinfo.cpp $(CXXFLAGS) $(LIBS)

# calcFoldedAcceptance: calcFoldedAcceptance.C
# 	$(CXX) -qopenmp -o calcFoldedAcceptance.exe calcFoldedAcceptance.C $(CXXFLAGS) $(LIBS)
calcFoldedAcceptance: $(SRC)calcFoldedAcceptance.cpp
	$(CXX) -qopenmp -o calcFoldedAcceptance.exe $(SRC)calcFoldedAcceptance.cpp $(CXXFLAGS) $(LIBS)

# $(BIN)%.o: $(SRC)%.C
$(BIN)%.o: $(SRC)%.cpp
	@echo Compiling  $< ...
	$(CXX) -qopenmp -c $(CXXFLAGS) $< -o $@

Analysis: $(MYOBJS)
	@echo Compiling  $^ ...
	$(CXX) -qopenmp $(CXXFLAGS)  $^ -o $@.exe  $(LIBS) 

AnalysisLepton: $(SRC)t_add.C $(SRC)AnalysisLepton.C $(SRC)Analysis.h $(SRC)t_add.h $(SRC)MyBDT_ChargePID.C $(SRC)ConfigParser.C
	$(CXX) -qopenmp -o AnalysisLepton.exe t_add.C AnalysisLepton.C MyBDT_ChargePID.C ConfigParser.C $(CXXFLAGS) $(LIBS)





