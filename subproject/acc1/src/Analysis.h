//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 20 09:32:55 2014 by ROOT version 5.34/09
// from TTree t/skimTree
// found on file: /tmp/wxu/ntuple_1476594709_1.root
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TDirectory.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TMath.h>
#include <TRandom.h>

#include <iostream>
using namespace std;

#include "t_add.h"
#include "binning.h"
#include "MyBDT_ChargePID.h"

double std_ecalbdtcut[78] = {
	-0.2, -0.2,
	-0.20, -0.20, -0.20, -0.20, -0.20,  -0.20, -0.20, -0.20, -0.20, -0.20, 
	-0.20, -0.20, -0.20, -0.20, -0.20,  -0.20, -0.20, -0.20, -0.20, -0.20, 
	-0.20, -0.20, -0.20, -0.25, -0.25,  -0.25, -0.25, -0.25, -0.25, -0.25,
	-0.3, -0.3, -0.3, -0.3, -0.3,  -0.3, -0.3, -0.3, -0.3, -0.3,
	-0.3, -0.3, -0.3, -0.3, -0.3,  -0.3, -0.3, -0.3, -0.3, -0.3,
	-0.3, -0.3, -0.3, -0.3, -0.3,  -0.3, -0.3, -0.3, -0.3, -0.3,
	-0.4, -0.4, -0.4, -0.4, -0.4,  -0.4, -0.4, -0.4, -0.4, -0.4,
	-0.5, -0.5, -0.5, -0.5, -0.5,  -0.5 
};

double std_Lkhd0cut[83] = {
	4.0, 4.0,
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 
	4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 3.8, 3.5, 3.3, 3.0, 
	3.0, 3.0, 2.8, 2.75, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7
};

class Analysis : public t_add{
	public:
		Analysis(){ IsFineBinning=0; }
		~Analysis() { }
		int selectDataFrom(TString fname, int id, int NFilesPerJob, TString treename, TString jobname);
		void LoopChain();
		void EvalChargeBDT();
		// user functions
		void SmearEnergy(double factor=1.0);
		void DefineEcalFiducialVolume();
		void TrackerEcalMatch();
		void TrdEcalMatch();
		void DefineEcalFiducialVolumeOld();
		void TrackerEcalMatchOld();
		void BookHistograms();
		void loadMCweight();
		void BuildMCWeight();
		void BuildMCWeightFromFluxModel(TString fname, TH1D *hweight);
		TH1D *h1d_mcweight_ele;
		TH1D *h1d_mcweight_pos;

		unsigned short int RestorePhysBPat(unsigned short int JMembPatt) {
			unsigned short int ft_pattern=JMembPatt;
			unsigned short int PhysBPatt=0;
			if ( (ft_pattern & 0x0001) == 0x0001 ) PhysBPatt |= 0x0001; // unbiased 3/4 TOF-CP
			if ( (ft_pattern & 0x0090) == 0x0090 ) PhysBPatt |= 0x0002; // 4/4 TOF-CT & ACC0
			if ( (ft_pattern & 0x0300) == 0x0300 ) PhysBPatt |= 0x0004; // 4/4 TOF-BZ & ACC1
			if ( (ft_pattern & 0x0020) == 0x0020 ) PhysBPatt |= 0x0008; // 4/4 FTZ
			if ( (ft_pattern & 0x0410) == 0x0410 ) PhysBPatt |= 0x0010; // 4/4 TOF-CT & ECALF&
			if ( (ft_pattern & 0x1000) == 0x1000 ) PhysBPatt |= 0x0020; // ECALA&
			if ( (ft_pattern & 0x0800) == 0x0800 ) PhysBPatt |= 0x0040; // unbiased ECALA||
			return PhysBPatt;
		}

		static const int nCut = 17;
		// static const int nCut = 18;
		int IsFineBinning;

		TFile *fout;
		TH1F *h1Ene3D[nCut];
		TH1F *h1EneC[nCut];
		TH1F *h1Ene17[nCut];
		TH1F *h1MCEne[nCut];
		TH2F *h2Ene3D_MCEne[nCut];
		TH2F *h2EneC_MCEne[nCut];
		TH2F *h2Ene17_MCEne[nCut];
		TH2F *h2Ene3DMCEneRatio_MCEne[nCut];
		TH2F *h2EneCMCEneRatio_MCEne[nCut];
		TH2F *h2Ene17MCEneRatio_MCEne[nCut];
		TH2F *h2EneCEne3DRatio_MCEne[nCut];
		TH2F *h2Ene17Ene3DRatio_MCEne[nCut];
		TH2F *h2EneCEne3DRatio_Ene3D[nCut];
		TH2F *h2Ene17Ene3DRatio_Ene3D[nCut];


		int inecal, trkecalmatch, trdecalmatch;
		MyBDT_ChargePID mychargepidbdt;
		float bdt_chargepid;
};

#endif
