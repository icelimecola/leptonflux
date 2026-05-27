_Pragma("once")

#include <iostream>
using namespace std;
#include "TTimeStamp.h"
#include "root_RVSP.h"
#include "../third_party/c_aacgm_v2.6/aacgmlib_v2.h"
#include "../third_party/c_aacgm_v2.6/mlt_v2.h"
#include "VarMinitree.hpp"

class ExpsAMSRTI:virtual public VarMinitree{
    public:
		ExpsAMSRTI():VarMinitree(),rti(){cout<<"exposureAMSRootDocument-cont"<<endl;}
        ~ExpsAMSRTI(){};
	public:
		AMSSetupR::RTI rti;
    public:
		//--main
		void AMSRTI_RunRTI();
		//--sub
		void AMSRTI_noRTI();
		void AMSRTI_CalcPosition();
		void AMSRTI_CalcCutoff();
		void AMSRTI_CalcBadrun();
        bool AMSRTI_CalcGoodsec();
		void AMSRTI_CalcOther();
		void AACGM_CalcGeomagPosition();
};

void ExpsAMSRTI::AMSRTI_RunRTI(){
	// cout<<"info_utime in amsrti: "<<info_utime<<endl;
	// AMSSetupR::RTI::UseLatest(7);
	AMSSetupR::RTI::UseLatest(8);
	int ret = AMSEventR::GetRTI( rti, info_utime );
	if( ret > 1 ){
		cout << "no official RTI found, ret = " << ret << endl;
		AMSRTI_noRTI();
	}else{
		AMSRTI_CalcPosition();
		AMSRTI_CalcCutoff();
		AMSRTI_CalcBadrun();
		AMSRTI_CalcOther();
		AACGM_CalcGeomagPosition();
	}
}

void ExpsAMSRTI::AMSRTI_noRTI(){
	info_lf = 0.;
	for(int i=0;i<4;i++){
		info_maxcf[0]=0;
		info_maxcfi[0]=0;
		maxcf_ts05[i]=0;
	}
	isbadrun=1; // will not be used
}

void ExpsAMSRTI::AMSRTI_CalcPosition(){
	info_zenith = rti.zenith;
	info_theta = rti.theta*180.0/3.14159;
	info_phi = rti.phi*180.0/3.14159;
	if(info_phi>180.0) {info_phi=info_phi-360.0;}
	info_r =rti.r;
	info_glat = rti.glat;
	info_glong = rti.glong;
}

//============ AMSRTI_CalcCutoff
//utime->rti_cutoff->var_cutoff
void ExpsAMSRTI::AMSRTI_CalcCutoff(){
	//==== rti_cf->var_cf
	for(int i=0;i<4;i++){
		info_maxcf[i] = TMath::Max( fabs(rti.cf[i][0]), fabs(rti.cf[i][1]) );
		info_poscf[i] =  fabs(rti.cf[i][1]);
		info_negcf[i] =  fabs(rti.cf[i][0]);
		info_maxcfi[i] = TMath::Max( fabs(rti.cfi[i][0]), fabs(rti.cfi[i][1]) );
		info_poscfi[i] =  fabs(rti.cfi[i][1]);
		info_negcfi[i] =  fabs(rti.cfi[i][0]);
	}
	//==== maxcf_ts05
	float cfn_ts[4][2];
	rti.gettsy05cf()->getcutoff(cfn_ts, 0, info_utime, rti.theta, rti.phi);
	for(int i=0;i<4;i++){
		maxcf_ts05[i] = TMath::Max( fabs(cfn_ts[i][0]), fabs(cfn_ts[i][1]) );
	}
}

//============ AMSRTI_CalcBadrun
//utime->rti_badrun->var_badrun
//待验证
void ExpsAMSRTI::AMSRTI_CalcBadrun(){
	string badrun_reason;
	//====== isbadrun
	isbadrun = rti.IsBadRun(badrun_reason);
	//====== isbad
	for(int i=0;i<nbadruntype;i++){
		std::size_t found = badrun_reason.find( badruntyps[i] );
		if( found!=std::string::npos ){
			isbad[i] = 1;
		}
	}
	// ====== isbadrun_xxx
	// int* isbad=isbad;
	//==== _sub detector
	if( isbad[16] || isbad[17] || isbad[18] ) isbadrun_trd = 1;
	if( isbad[1] || isbad[2] || isbad[3] || isbad[4] || isbad[5] ) isbadrun_ecal = 1;
	if( isbad[15] ) isbadrun_trk = 1;
	//==== _other
	for(int i=7; i<=14; i++) if( isbad[i] ) isbadrun_general = 1;
	if( isbad[0] ) isbadrun_daq = 1;
	if( isbad[6] ) isbadrun_fov = 1;
}

//============ AMSRTI_CalcGoodsec
//==== GoodSecond
//-- zenith >= 40
//zenith--ams zenith angle (degrees)
//-- n_TriggerEvents / n_ExistEvent >= 0.98
//ntrig--events with trigger
//nev--exist events  nev+nerr=sumev
//-- n_ErrorEvent >= 0
//nerr--absent events
//-- n_ErrorEvent / n_ExistEvent <= 0.1
//-- n_PartDetectorEvent > 0
//npart--events with tof+trd+tracker+ecal
//-- rti.nev <= 1800
bool ExpsAMSRTI::AMSRTI_CalcGoodsec(){
	if( rti.zenith > 40 ) return false;
	if( rti.ntrig / rti.nev < 0.98 ) return false;
	if( rti.nerr < 0 || rti.nerr / rti.nev > 0.1 ) return false;
	// if( rti.npart / rti.ntrig < 0.07/1600*rti.ntrig || rti.npart / rti.ntrig > 0.25 ) return false;
	if( rti.npart <= 0 || rti.nev > 1800 ) return false;
	return true;
}

void ExpsAMSRTI::AMSRTI_CalcOther(){
	//==== DAQ_condition
	info_saa = rti.IsInSAA();
	info_badsec = not AMSRTI_CalcGoodsec();
	info_badalign = rti.dl1l9[0][1]>35 || rti.dl1l9[1][1]>45;
	//==== events number
	info_ntrig = rti.ntrig;
	info_nev = rti.nev;
	info_nerr = rti.nerr;
	info_npart = rti.npart;
	//==== lifetime
	info_lf = rti.lf;
	info_lfc = rti.lf*rti.nev/(rti.nerr + rti.nev);
}

void ExpsAMSRTI::AACGM_CalcGeomagPosition(){
    //==== GTOD-POSITION
	double LAT=info_theta;
	double LON=info_phi;
	double GR=info_r;
	double ALT=(GR/100.0-6371393)/1000.00;
	// cout<<"GTOD_POSITION= "<<LAT<<","<<LON<<","<<ALT<<endl;
	//==== TIME
	TTimeStamp TT(info_utime);
	int date=TT.GetDate();
	int year=date/10000;
	int mon=TT.GetMonth();
	int day=date%100;
	int time=TT.GetTime();
	int hr=time/10000;
	int min=(time%10000)/100;
	int sec=time%100; 
	// cout<<"TIME= "<<date<<","<<year<<","<<mon<<","<<day<<","<<hr<<","<<min<<","<<sec<<endl;
	//==== AACGM
	double mlat,mlon,rr;
	AACGM_v2_SetDateTime(year,mon,day,hr,min,sec);
	AACGM_v2_Convert(LAT,LON,ALT,&mlat,&mlon,&rr,G2A);
	// cout<<"GTOD_POSITION"<<LAT<<","<<LON<<","<<ALT<<endl;
	// cout<<"AACGM_POSITION"<<mlat<<","<<mlon<<","<<rr<<endl;
	//==== RESULT
	info_mlat=mlat;
	info_mlon=mlon;
}