_Pragma("once")

#include <iostream>
using namespace std;
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "VarMinitree.hpp"


class ExpsHist:virtual public VarMinitree{
    public:
		ExpsHist():VarMinitree(){cout<<"Constructor_expsfillexps"<<endl;}
        ~ExpsHist(){};
	public:
        bool HIST_IsGoodDAQ();
		//-- exps经纬度分布
		TH2D *h2d_exposure_mLongVSmLat;
		TH1D *h1d_exposure_mLat;
		TH1D *h1d_exposure_mLong;
		TH2D *h2d_exposure_gLongVSgLat;
		TH1D *h1d_exposure_gLat;
		TH1D *h1d_exposure_gLong;
		TH1D* h1d_exposure_mLat2;
		//-- func
		void HIST_FillExps();
		void HIST_WriteHist();
		void HIST_DrawHist();
	public:
		//-- exps day-e 分布
		TH2D *h2exp_st_dayVSene[nsf];
		TH2D *h2exp_igrf_dayVSene[nsf];
		TH2D *h2exp_ts05_dayVSene[nsf];
		TH1D *h1exp_st_ene[nfov][nsf];
		TH1D *h1exp_igrf_ene[nfov][nsf];
		TH1D *h1exp_st_enefine[nfov][nsf];
		TH1D *h1exp_igrf_enefine[nfov][nsf];
		TH1D *h1exp_st_day[nfov][nsf][nenebin];
		TH1D *h1exp_igrf_day[nfov][nsf][nenebin];
		//-- func
		void HIST_FillExps_dayVSene();
		void HIST_WriteHist_dayVSene();
};



bool ExpsHist::HIST_IsGoodDAQ(){
	return !(info_lf < 0.05 || info_saa || info_isinshadow || info_badsec);
}


void ExpsHist::HIST_FillExps(){
	// const int n_tbin = 5000;
	// const double t0 = 1305417600;
	static int first_call = 1;
	if( first_call ){
		// else fio->ins_fileout->cd();
		//==== GTOD
		h2d_exposure_mLongVSmLat = new TH2D("h2d_exposure_mLongVSmLat","Collection Time Dis in Long and Lat (AACGM)",400,-200,200,200,-100,100);
		h1d_exposure_mLong = new TH1D("h1d_exposure_mLong","Collection Time Dis in Long (AACGM)",400,-200,200);
		h1d_exposure_mLat = new TH1D("h1d_exposure_mLat","Collection Time Dis in Lat (AACGM)",200,-100,100);
		//==== AACGM
		h2d_exposure_gLongVSgLat = new TH2D("h2d_exposure_gLongVSgLat","Collection Time Dis in Long and Lat (AACGM)",400,-200,200,200,-100,100);
		h1d_exposure_gLong = new TH1D("h1d_exposure_gLong","Collection Time Dis in Long (AACGM)",400,-200,200);
		h1d_exposure_gLat = new TH1D("h1d_exposure_gLat","Collection Time Dis in Lat (AACGM)",200,-100,100);
		//==== MLAT2
		h1d_exposure_mLat2 = new TH1D("h1d_exposure_mLat2","Collection Time Dis in Lat (AACGM)",36,-90,90);
		first_call = 0;
	}
    h2d_exposure_mLongVSmLat->Fill( info_mlon,info_mlat, info_lfc );
    h1d_exposure_mLong->Fill( info_mlon, info_lfc );
    h1d_exposure_mLat->Fill( info_mlat, info_lfc );
    h2d_exposure_gLongVSgLat->Fill( info_phi, info_theta, info_lfc );
    h1d_exposure_gLong->Fill( info_phi, info_lfc );
    h1d_exposure_gLat->Fill( info_theta, info_lfc );
    //===
	h1d_exposure_mLat2->Fill( info_mlat, info_lfc );
}




void ExpsHist::HIST_WriteHist(){
	cout << "h2d_exposure_mLongVSmLat = " << h2d_exposure_mLongVSmLat << endl;
	h2d_exposure_mLongVSmLat->Write();
	h1d_exposure_mLong->Write();
	h1d_exposure_mLat->Write();
	h2d_exposure_gLongVSgLat->Write();
	h1d_exposure_gLong->Write();
	h1d_exposure_gLat->Write();
	h1d_exposure_mLat2->Write();
}




void ExpsHist::HIST_DrawHist(){
	TCanvas ins_can("c1","c1_title",1000,1000);
	ins_can.Divide(2,2);
	ins_can.cd(1);
	h2d_exposure_gLongVSgLat->Draw();
	ins_can.cd(2);
	h2d_exposure_mLongVSmLat->Draw();
	ins_can.cd(3);
	h1d_exposure_gLat->Draw("h");
	ins_can.cd(4);
	h1d_exposure_mLat->Draw("h");
	ins_can.Write();
}


void ExpsHist::HIST_FillExps_dayVSene(){
	//============ init ============
	double fov[nfov] = { 25, 30, 35, 40 };
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	int nfov=1;
	// int nsf=1;
	int nsf=5;
	//============ init--time ============
	//---- 1day--251101
    const double width_tbin = 60*60*24;  //---- 86400[s]
	// const int n_tbin = 27*120;
    // static const int n_tbin=290;
    static const int n_tbin=5000;                   //---- (uesd by lj)
    // const double t0_unix = 1304179200;	    //---- 2011-04-30 16:00:00 GMT+0000 (uesd by lj)
    // const double t0_unix = 1304121600;	    //---- 2011-04-30 00:00:00 GMT+0000 
    const double t0_unix = 1305417600;	    //---- 2011-05-15 00:00:00 GMT+0000 
    //---- 27days
    // const double width_tbin = 60*60*24*27;
    // // static const int n_tbin=120;
    // static const int n_tbin=185;
    // const double t0_unix = 1305417600;	//-- 2011-05-15 00:00:00 GMT+0000 
    //---- event time
	double event_unix = info_utime;
	//============ init--hist ============
	static int first_call = 1;
	if( first_call ){
		//====h2--t vs ene
		for(int j=0;j<nsf;j++){
			h2exp_st_dayVSene[j] = new TH2D( Form("h2_exp_dayVSene_stormer_sf%g", factor[j]), Form("FOV 25, safety factor %g;Date;Energy [GeV];Collection Time[s]", factor[j]), 
				n_tbin, t0_unix, t0_unix+n_tbin*width_tbin,  nenebin, energy_bins);
			h2exp_igrf_dayVSene[j] = new TH2D( Form("h2_exp_dayVSene_igrf_sf%g", factor[j]), Form("FOV 25, safety factor %g;Date;Energy [GeV];Collection Time[s]", factor[j]), 
				n_tbin, t0_unix, t0_unix+n_tbin*width_tbin,  nenebin, energy_bins);
			h2exp_ts05_dayVSene[j] = new TH2D( Form("h2_exp_dayVSene_ts05_sf%g", factor[j]), Form("FOV 25, safety factor %g;Date;Energy [GeV];Collection Time[s]", factor[j]), 
				n_tbin, t0_unix, t0_unix+n_tbin*width_tbin,  nenebin, energy_bins);
		}
		//====h1ene
		for(int i=0;i<nfov;i++){
		for(int j=0;j<nsf;j++){
			h1exp_st_ene[i][j] = new TH1D( Form("h1exp_st_ene_fov%g_sf%g", fov[i], factor[j]), Form("FOV %g degree, safety factor %g", fov[i], factor[j]), 
				nenebin, energy_bins);
			h1exp_igrf_ene[i][j] = new TH1D( Form("h1exp_igrf_ene_fov%g_sf%g", fov[i], factor[j]), Form("FOV %g degree, safety factor %g", fov[i], factor[j]), 
				nenebin, energy_bins);
			h1exp_st_enefine[i][j] = new TH1D( Form("h1exp_st_enefine_fov%g_sf%g", fov[i], factor[j]), Form("FOV %g degree, safety factor %g", fov[i], factor[j]), 
				nenebin_fine, energy_bins_fine);
			h1exp_igrf_enefine[i][j] = new TH1D( Form("h1exp_igrf_enefine_fov%g_sf%g", fov[i], factor[j]), Form("FOV %g degree, safety factor %g", fov[i], factor[j]), 
				nenebin_fine, energy_bins_fine);
		}
		}
		//====h1t
		for(int i=0;i<nfov;i++){
		for(int j=0;j<nsf;j++){
		for(int k=0;k<nenebin;k++){
			h1exp_st_day[i][j][k] = new TH1D( Form("h1_exp_day_stormer_fov%g_sf%g_ene%gto%gGeV", fov[i], factor[j],energy_bins[k],energy_bins[k+1]), Form("FOV %g degree, safety factor %g, energy %g to %g GeV", fov[i], factor[j],energy_bins[k],energy_bins[k+1]), 
				n_tbin, t0_unix, t0_unix+n_tbin*width_tbin);
			h1exp_igrf_day[i][j][k] = new TH1D( Form("h1_exp_day_igrf_fov%g_sf%g_ene%gto%gGeV", fov[i], factor[j],energy_bins[k],energy_bins[k+1]), Form("FOV %g degree, safety factor %g, energy %g to %g GeV", fov[i], factor[j],energy_bins[k],energy_bins[k+1]), 
				n_tbin, t0_unix, t0_unix+n_tbin*width_tbin);
		}
		}
		}
		//====reset
		first_call = 0;
	}
	//============ fill--h2 ============
	// for(int isf=0;isf<nsf;isf++){
	// for(int jene=0;jene<h2exp_st_dayVSene[isf]->GetNbinsY();jene++){
	// 	if( h2exp_st_dayVSene[isf]->GetYaxis()->GetBinLowEdge(jene+1) >= factor[isf]*info_maxcf[0] ){
	// 		h2exp_st_dayVSene[isf]->
	// 			// Fill( info_utime, h2_exp_dayVSene_stormer[j]->GetYaxis()->GetBinCenter(iene+1), info_lfc );
	// 			Fill( event_unix, h2exp_st_dayVSene[isf]->GetYaxis()->GetBinCenter(jene+1), info_lfc );
	// 	}
	// 	if( h2exp_igrf_dayVSene[isf]->GetYaxis()->GetBinLowEdge(jene+1) >= factor[isf]*info_maxcfi[0] ){
	// 		h2exp_igrf_dayVSene[isf]->
	// 			Fill( event_unix, h2exp_igrf_dayVSene[isf]->GetYaxis()->GetBinCenter(jene+1), info_lfc );
	// 	}
	// 	if( h2exp_ts05_dayVSene[isf]->GetYaxis()->GetBinLowEdge(jene+1) >= factor[isf]*maxcf_ts05[0] ){
	// 		h2exp_ts05_dayVSene[isf]->
	// 			Fill( event_unix, h2exp_ts05_dayVSene[isf]->GetYaxis()->GetBinCenter(jene+1), info_lfc );
	// 	}
	// }
	// }
	//============ fill--h1ene ============
	for(int ifov=0; ifov<nfov; ifov++){
	for(int jsf=0; jsf<nsf; jsf++){
	for(int kene=0; kene<h1exp_st_ene[ifov][jsf]->GetNbinsX(); kene++){
		double elow = h1exp_st_ene[ifov][jsf]->GetBinLowEdge(kene+1);
		double eup = h1exp_st_ene[ifov][jsf]->GetBinLowEdge(kene+2);
		double ecenter = 0.5 * (elow + eup);
		if( elow >= factor[jsf]*info_maxcf[ifov] ){
			h1exp_st_ene[ifov][jsf]->Fill( ecenter, info_lfc );
		}
		if( elow >= factor[jsf]*info_maxcfi[ifov] ){
			h1exp_igrf_ene[ifov][jsf]->Fill( ecenter, info_lfc );
		}
	}
	}            
	}
	//============ fill--h1enefine ============
	// for(int ifov=0; ifov<nfov; ifov++){
	// for(int jsf=0; jsf<nsf; jsf++){
	// for(int kene=0; kene<h1exp_st_enefine[ifov][jsf]->GetNbinsX(); kene++){
	// 	if( h1exp_st_enefine[ifov][jsf]->GetBinLowEdge(kene+1) >= info_maxcf[ifov]*factor[jsf] ){
	// 		h1exp_st_enefine[ifov][jsf]->
	// 			Fill( h1exp_st_enefine[ifov][jsf]->GetBinCenter(kene+1), info_lfc );
	// 	}        
	// 	if( h1exp_igrf_enefine[ifov][jsf]->GetBinLowEdge(kene+1) >= info_maxcfi[ifov]*factor[jsf] ){
	// 		h1exp_igrf_enefine[ifov][jsf]->
	// 			Fill( h1exp_igrf_enefine[ifov][jsf]->GetBinCenter(kene+1), info_lfc );
	// 	}        
	// }
	// }            
	// }
	//============ fill--h1t ============
	// // for(int ifov=0; ifov<nfov; ifov++){
	// // for(int jsf=0; jsf<nsf; jsf++){
	// // for(int kene=0; kene<nenebin; kene++){
	// // 	if( energy_bins[kene] >= factor[jsf]*info_maxcf[ifov] ){
	// // 		h1_exp_day_stormer[ifov][jsf][kene]->
	// // 			Fill( event_unix, info_lfc );
	// // 		h1_exp_day_igrf[ifov][jsf][kene]->
	// // 			Fill( event_unix, info_lfc );
	// // 	}
	// // }
	// // }
	// // }
	// //---- 251205
	// for(int ifov=0; ifov<nfov; ifov++){
	// for(int jsf=0; jsf<nsf; jsf++){
	// for(int kene=0; kene<nenebin; kene++){
	// 	// if( h1_exp_day_stormer[ifov][jsf][kene]->GetBinLowEdge(kene+1) >= info_maxcf[ifov]*factor[jsf] ){
	// 	if( energy_bins[kene] >= info_maxcf[ifov]*factor[jsf] ){
	// 		h1exp_st_day[ifov][jsf][kene]->
	// 			Fill( event_unix, info_lfc );
	// 	}
	// 	// if( h1_exp_day_igrf[ifov][jsf][kene]->GetBinLowEdge(kene+1) >= info_maxcfi[ifov]*factor[jsf] ){
	// 	if( energy_bins[kene] >= info_maxcfi[ifov]*factor[jsf] ){
	// 	// if( energy_bins[kene] >= info_poscfi[ifov]*factor[jsf] ){
	// 	// if( energy_bins[kene] >= info_negcfi[ifov]*factor[jsf] ){
	// 		h1exp_igrf_day[ifov][jsf][kene]->
	// 			Fill( event_unix, info_lfc );
	// 	}
	// }
	// }
	// }
}


void ExpsHist::HIST_WriteHist_dayVSene(){
	//============ init
	int nfov=1;
	// int nsf=1;
	int nsf=5;
	//============ save
	//====save--h2
	// for(int i=0;i<nsf;i++){
	// 	if( h2_exp_dayVSene_stormer[i] ) h2_exp_dayVSene_stormer[i]->Write();
	// 	if( h2_exp_dayVSene_igrf[i] ) h2_exp_dayVSene_igrf[i]->Write();
	// 	if( h2_exp_dayVSene_ts05[i] ) h2_exp_dayVSene_ts05[i]->Write();
	// }
	//====save--h1ene
	for(int i=0;i<nfov;i++){
	for(int j=0;j<nsf;j++){
		if(h1exp_st_ene[i][j]){
			h1exp_st_ene[i][j]->Write();
			h1exp_igrf_ene[i][j]->Write();
		}
	}
	}
	//====save--h1t
	// for(int i=0;i<nfov;i++){
	// for(int j=0;j<nsf;j++){
	// for(int k=0;k<nenebin;k++){
	// 	if(h1_exp_day_stormer[i][j][k]){
	// 		h1_exp_day_stormer[i][j][k]->Write();
	// 		h1_exp_day_igrf[i][j][k]->Write();
	// 	}
	// }
	// }
	// }
}