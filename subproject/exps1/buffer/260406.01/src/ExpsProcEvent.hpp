_Pragma("once")

#include <iostream>
using namespace std;
#include "TString.h"
#include "root_RVSP.h"
#include "VarMinitree.hpp"

class ExpsProcEvent:virtual public VarMinitree{
    public:
		ExpsProcEvent():VarMinitree(){cout<<"exposureprocessevent-cont"<<endl;}
		ExpsProcEvent(TString fn):VarMinitree(),filename(fn){cout<<"exposureprocessevent-cont"<<endl;}
        ~ExpsProcEvent(){}
    public:
		TString filename;
    public:
		void PROCESS();
		bool PROCESS_IsISS();
		bool PROCESS_IsMC();
		void PROCESS_MCdata();
		void PROCESS_AllData();
		void v01_PROCESS_MC_Setlvl1Patt();
		void v01_PROCESS_MC_CorrProtonEne(TString);
		void v01_PROCESS_MC_CorrLTOFq();
		void v02_PROCESS_SetEcalPosition();
        void v02_PROCESS_CalcInecal();
		void v02_PROCESS_CalcTrkecalmatch();
		void v02_PROCESS_SetRigidity();
};

void ExpsProcEvent::PROCESS(){
	if(PROCESS_IsMC()){
		v01_PROCESS_MC_Setlvl1Patt();
		v01_PROCESS_MC_CorrProtonEne(filename);
		v01_PROCESS_MC_CorrLTOFq();
	}
	//-- ECAL
	v02_PROCESS_SetEcalPosition();
	v02_PROCESS_CalcInecal();
	v02_PROCESS_CalcTrkecalmatch();
	//-- TRK
	v02_PROCESS_SetRigidity();
}

bool ExpsProcEvent::PROCESS_IsISS(){
    return dataType==_ISS_Data;
}

bool ExpsProcEvent::PROCESS_IsMC(){
    return dataType == _MC_Data;
}

void ExpsProcEvent::PROCESS_MCdata(){
	v01_PROCESS_MC_Setlvl1Patt();
	v01_PROCESS_MC_CorrProtonEne(filename);
	v01_PROCESS_MC_CorrLTOFq();
}

void ExpsProcEvent::PROCESS_AllData(){
	//-- ECAL
	v02_PROCESS_SetEcalPosition();
	v02_PROCESS_CalcInecal();
	v02_PROCESS_CalcTrkecalmatch();
	//-- TRK
	v02_PROCESS_SetRigidity();
}

void ExpsProcEvent::v01_PROCESS_MC_Setlvl1Patt(){
	lvl1_PhysBPatt = lvl1_PhysBPattMC;
	lvl1_JMembPatt = lvl1_JMembPattMC;
}

//============ v01_PROCESS_MC_CorrProtonEne
//-- MC-ECAL-ENE should be rescale by index = 1.3 (why?)
void ExpsProcEvent::v01_PROCESS_MC_CorrProtonEne(TString filename){
	if( filename.Contains("proton") || filename.Contains("Pr") ){
		Ene *= 1.3;
	}
}

void ExpsProcEvent::v01_PROCESS_MC_CorrLTOFq(){
	/*
	// inner tracker charge
	const double mean_qin_data = 1.042, sigma_qin_data = 0.055,
			       mean_qin_mc = 1.108, sigma_qin_mc = 0.055;
	trk_qin = (trk_qin - mean_qin_mc)/sigma_qin_mc*sigma_qin_data + mean_qin_data;
	// layer L1 charge 
	const double mean_q1_data = 1.042, sigma_q1_data = 0.117,
			       mean_q1_mc = 1.076, sigma_q1_mc = 0.100;
	trk_qlay[0] = (trk_qlay[0] - mean_q1_mc)/sigma_q1_mc*sigma_q1_data + mean_q1_data;
	// layer L9 charge
	const double mean_q9_data = 1.038, sigma_q9_data = 0.121,
			       mean_q9_mc = 1.096, sigma_q9_mc = 0.100;
	trk_qlay[8] = (trk_qlay[8] - mean_q9_mc)/sigma_q9_mc*sigma_q9_data + mean_q9_data;
	*/
	// lower TOF charge
	const double mean_tofqlow_data = 1.034, sigma_tofqlow_data = 0.051;
	const double mean_tofqlow_mc = 1.015, sigma_tofqlow_mc = 0.051;
	tof_qlow = (tof_qlow - mean_tofqlow_mc)/sigma_tofqlow_mc*sigma_tofqlow_data + mean_tofqlow_data;
}

//============ v02_PROCESS_SetEcalPosition
//-- xecal & yecal
void ExpsProcEvent::v02_PROCESS_SetEcalPosition(){
	xecal[0] = ShwrPos[0][0];
	xecal[1] = ShwrPos[1][0];
	yecal[0] = ShwrPos[0][1];
	yecal[1] = ShwrPos[1][1];
}

//============ v02_PROCESS_SetInEcal
//-- inecal
void ExpsProcEvent::v02_PROCESS_CalcInecal(){
	//-- ecalentry & ecalexit
    static const double ecal_entry_z0 = -143.2;
	static const double ecal_exit_z0 = -158.9;
	double  ecalentry[3][2],ecalexit[3][2];
	memset(ecalentry, 0, sizeof(ecalentry));
	memset(ecalexit, 0, sizeof(ecalexit));
    // int N_Shwr=N_Shwr;
	for(int ish=0; ish<N_Shwr; ish++){
        if( ShwrDir[ish][2]!=0 ){
			ecalentry[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalentry[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
			ecalexit[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_exit_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalexit[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_exit_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
		}else continue;
	}
	//-- inecal
    // int& inecal=inecal;
	inecal=0;
	if( N_Shwr>0 && TMath::Abs( ecalentry[0][0] ) < 31.5 && TMath::Abs( ecalentry[0][1] ) < 31.5 ) inecal |= 1;
	if( N_Shwr>0 && TMath::Abs( ecalexit[0][0] )  < 31.5 && TMath::Abs( ecalexit[0][1] )  < 31.5 ) inecal |= 2;
	if( N_Shwr>1 && TMath::Abs( ecalentry[1][0] ) < 31.5 && TMath::Abs( ecalentry[1][1] ) < 31.5 ) inecal |= 4;
	if( N_Shwr>1 && TMath::Abs( ecalexit[1][0] )  < 31.5 && TMath::Abs( ecalexit[1][1] )  < 31.5 ) inecal |= 8;
	if( N_Shwr>2 && TMath::Abs( ecalentry[2][0] ) < 31.5 && TMath::Abs( ecalentry[2][1] ) < 31.5 ) inecal |= 16;
	if( N_Shwr>2 && TMath::Abs( ecalexit[2][0] )  < 31.5 && TMath::Abs( ecalexit[2][1] )  < 31.5 ) inecal |= 32;
}

//============ v02_PROCESS_CalcTrkecalmatch
void ExpsProcEvent::v02_PROCESS_CalcTrkecalmatch(){
	//---- trkecalmatch
	trkecalmatch=0;
	//---- trkecalmatch--1&2
	if( TMath::Abs(trk_ecalentry[0]) < 32.4 ) trkecalmatch |= 1;
	if( TMath::Abs(trk_ecalentry[1]) < 32.4 ) trkecalmatch |= 2;
	//---- cut1
	if( N_Shwr<1 ) return;
	//---- find best match shower
	Float_t ecalentry[3][2];
	memset(ecalentry, 0, sizeof(ecalentry));
	static const double ecal_entry_z0 = -143.2;
	int index_shwr = -1;
	double dx,dy;
	double dist=0,min_dist = 1e100;
	for(int ish=0; ish<N_Shwr; ish++){
		if( ShwrDir[ish][2]!=0 ){
			ecalentry[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalentry[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
		}
		else continue;
		dx = trk_ecalentry[0] - ecalentry[ish][0];
		dy = trk_ecalentry[1] - ecalentry[ish][1];
		dist = dx*dx + dy*dy;
		if( min_dist > dist ){
			min_dist = dist;
			index_shwr = ish;
		}
	}
	if( index_shwr<0 ) return;
	//---- trkecalmatch--4&8
	trkecaldx = trk_ecalentry[0] - ecalentry[index_shwr][0];
	trkecaldy = trk_ecalentry[1] - ecalentry[index_shwr][1];
	if( TMath::Abs(trk_ecalentry[0] - ecalentry[index_shwr][0]) < 3  ) trkecalmatch |= 4;
	if( TMath::Abs(trk_ecalentry[1] - ecalentry[index_shwr][1]) < 10 ) trkecalmatch |= 8;
}

void ExpsProcEvent::v02_PROCESS_SetRigidity(){
	rigidity = trk_rig[1];
	//-- Y-only L9 hit should not be used in rigidity calculation
	if( !(trk_patx&256) && (trk_pat&256) ){ 
		trk_pat &= ~256;
		//-- InnerL1 
		if( trk_pat&1 ){
			rigidity = trk_rig[2];
		//-- Inner
		}else{
			rigidity = trk_rig[0]; 
		}
	}
	//-- time dependent rigidity scale correction
	if( PROCESS_IsISS() ){ 
		rigidity = AMSEventR::GetCorrectedRigidity( rigidity, info_utime, 0, 3 ); // 7-year rigidity scale
	}
}
