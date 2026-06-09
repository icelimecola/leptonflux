_Pragma("once")

#include <iostream>
using namespace std;
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TDirectory.h"
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
		//---- exps-TvE
		TH2D *h2exp_st_TvE[nsf];
		TH2D *h2exp_igrf_TvE[nsf];
		TH2D *h2exp_ts05_TvE[nsf];
		TH1D *h1exp_st_E[nfov][nsf];
		TH1D *h1exp_igrf_E[nfov][nsf];
		// TH1D *h1exp_st_Efine[nfov][nsf];
		// TH1D *h1exp_igrf_Efine[nfov][nsf];
		TH1D *h1exp_st_T[nfov][nsf][nenebin];
		TH1D *h1exp_igrf_T[nfov][nsf][nenebin];
		//---- func
		void HIST_FillExps_TvE();
		void HIST_WriteHist_TvE();
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


void ExpsHist::HIST_FillExps_TvE(){
	//============ init ============
	//====init array
	double fov[nfov] = { 25, 30, 35, 40 };
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	//====set length
	int nfov,nsf;
	nfov=1;
	// nsf=1;
	nsf=5;
	//============ init--time ============
    //---- event time
	double event_unix = info_utime;
	//==== 1day
	//----prl130posi--260419
	const double wt = 60*60*24;  //---- 86400[s]
	static const int nt=6000;
	const double tmin = 1305417600;
	const double tmax = tmin + nt*wt;	//----1823817600
	//==== 27days
	//----prl130posi--260421
	// const double wt = 60*60*24*27;
	// static const int nt=225;
	// const double tmin = 1305417600;
	// const double tmax = tmin + nt*wt;	//----1830297600
	//============ init--hist ============
	static int first_call = 1;
	if( first_call ){
		//====h2--t vs ene
		for(int isf=0;isf<nsf;isf++){
			h2exp_st_TvE[isf] = new TH2D( Form("h2exp_st_TvE_sf%g", factor[isf]), Form("h2exp_st_TvE_sf%g", factor[isf]), 
				nt, tmin, tmin+nt*wt,  nenebin, energy_bins);
			h2exp_igrf_TvE[isf] = new TH2D( Form("h2exp_igrf_TvE_sf%g", factor[isf]), Form("h2exp_igrf_TvE_sf%g", factor[isf]), 
				nt, tmin, tmin+nt*wt,  nenebin, energy_bins);
			h2exp_ts05_TvE[isf] = new TH2D( Form("h2exp_ts05_TvE_sf%g", factor[isf]), Form("h2exp_ts05_TvE_sf%g", factor[isf]), 
				nt, tmin, tmin+nt*wt,  nenebin, energy_bins);
		}
		//====h1ene
		for(int ifov=0;ifov<nfov;ifov++){
		for(int isf=0;isf<nsf;isf++){
			h1exp_st_E[ifov][isf] = new TH1D( Form("h1exp_st_E_fov%g_sf%g", fov[ifov], factor[isf]), Form("h1exp_st_E_fov%g_sf%g", fov[ifov], factor[isf]), 
				nenebin, energy_bins);
			h1exp_igrf_E[ifov][isf] = new TH1D( Form("h1exp_igrf_E_fov%g_sf%g", fov[ifov], factor[isf]), Form("h1exp_igrf_E_fov%g_sf%g", fov[ifov], factor[isf]), 
				nenebin, energy_bins);
			// h1exp_st_Efine[ifov][isf] = new TH1D( Form("h1exp_st_Efine_fov%g_sf%g", fov[ifov], factor[isf]), Form("h1exp_st_Efine_fov%g_sf%g", fov[ifov], factor[isf]), 
			// 	nenebin_fine, energy_bins_fine);
			// h1exp_igrf_Efine[ifov][isf] = new TH1D( Form("h1exp_igrf_Efine_fov%g_sf%g", fov[ifov], factor[isf]), Form("h1exp_igrf_Efine_fov%g_sf%g", fov[ifov], factor[isf]), 
			// 	nenebin_fine, energy_bins_fine);
		}
		}
		//====h1t
		for(int ifov=0;ifov<nfov;ifov++){
		for(int isf=0;isf<nsf;isf++){
		for(int iene=0;iene<nenebin;iene++){
			h1exp_st_T[ifov][isf][iene] = new TH1D( Form("h1exp_st_T_fov%g_sf%g_ene%gto%gGeV", fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1]), Form("h1exp_st_T_fov%g_sf%g_ene%gto%gGeV", fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1]), 
				nt, tmin, tmin+nt*wt);
			h1exp_igrf_T[ifov][isf][iene] = new TH1D( Form("h1exp_igrf_T_fov%g_sf%g_ene%gto%gGeV", fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1]), Form("h1exp_igrf_T_fov%g_sf%g_ene%gto%gGeV", fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1]), 
				nt, tmin, tmin+nt*wt);
		}
		}
		}
		//====reset
		first_call = 0;
	}
	//============ fill--h2 ============
	for(int isf=0;isf<nsf;isf++){
	for(int iene=0;iene<nenebin;iene++){
		double elow = energy_bins[iene];
		double eup = energy_bins[iene+1];
		double ecenter = 0.5*(elow + eup);
		if( elow >= factor[isf]*info_maxcf[0] ) h2exp_st_TvE[isf]->Fill( info_utime, ecenter, info_lfc );
		if( elow >= factor[isf]*info_maxcfi[0] ) h2exp_igrf_TvE[isf]->Fill( info_utime, ecenter, info_lfc );
		if( elow >= factor[isf]*maxcf_ts05[0] ) h2exp_ts05_TvE[isf]->Fill( info_utime, ecenter, info_lfc );
	}
	}
	//============ fill--h1ene ============
	for(int ifov=0; ifov<nfov; ifov++){
	for(int isf=0; isf<nsf; isf++){
	for(int iene=0; iene<nenebin; iene++){
		double elow = energy_bins[iene];
		double eup = energy_bins[iene+1];
		double ecenter = 0.5 * (elow + eup);
		if( elow >= factor[isf]*info_maxcf[ifov] ) h1exp_st_E[ifov][isf]->Fill( ecenter, info_lfc );
		if( elow >= factor[isf]*info_maxcfi[ifov] ) h1exp_igrf_E[ifov][isf]->Fill( ecenter, info_lfc );
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
	for(int ifov=0; ifov<nfov; ifov++){
	for(int isf=0; isf<nsf; isf++){
	for(int iene=0; iene<nenebin; iene++){
		if( energy_bins[iene] >= factor[isf]*info_maxcf[ifov] ) h1exp_st_T[ifov][isf][iene]->Fill( info_utime, info_lfc );
		// if( energy_bins[iene] >= factor[isf]*info_poscfi[ifov] ) h1exp_igrf_T[ifov][isf][iene]->Fill( info_utime, info_lfc );
		// if( energy_bins[iene] >= factor[isf]*info_negcfi[ifov] ) h1exp_igrf_T[ifov][isf][iene]->Fill( info_utime, info_lfc );
		if( energy_bins[iene] >= factor[isf]*info_maxcfi[ifov] ) h1exp_igrf_T[ifov][isf][iene]->Fill( info_utime, info_lfc );
	}
	}
	}
}


void ExpsHist::HIST_WriteHist_TvE(){
	//============ init
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	int nfov,nsf;
	nfov=1;
	// nsf=1;
	nsf=5;
	//============ save
	//====save--h2
		TDirectory *dir_top = gDirectory;
		TDirectory *dir_h2 = dir_top->GetDirectory("h2");
		if( !dir_h2 ) dir_h2 = dir_top->mkdir("h2");
		dir_h2->cd();
	for(int isf=0;isf<nsf;isf++){
		if( h2exp_st_TvE[isf] ) h2exp_st_TvE[isf]->Write();
		if( h2exp_igrf_TvE[isf] ) h2exp_igrf_TvE[isf]->Write();
		if( h2exp_ts05_TvE[isf] ) h2exp_ts05_TvE[isf]->Write();
	}
	//====save--h1ene
		dir_top->cd();
		TDirectory *dir_h1ene = dir_top->GetDirectory("h1ene");
		if( !dir_h1ene ) dir_h1ene = dir_top->mkdir("h1ene");
	for(int ifov=0;ifov<nfov;ifov++){
	for(int isf=0;isf<nsf;isf++){
			TDirectory *dir_sf = dir_h1ene->GetDirectory(Form("sf%g", factor[isf]));
			if( !dir_sf ) dir_sf = dir_h1ene->mkdir(Form("sf%g", factor[isf]));
			dir_sf->cd();
		if(h1exp_st_E[ifov][isf]) h1exp_st_E[ifov][isf]->Write();
		if(h1exp_igrf_E[ifov][isf]) h1exp_igrf_E[ifov][isf]->Write();
	}
	}
	//====save--h1t
		dir_top->cd();
		TDirectory *dir_h1t = dir_top->GetDirectory("h1t");
		if( !dir_h1t ) dir_h1t = dir_top->mkdir("h1t");
	for(int ifov=0;ifov<nfov;ifov++){
	for(int isf=0;isf<nsf;isf++){
			TDirectory *dir_sf = dir_h1t->GetDirectory(Form("sf%g", factor[isf]));
			if( !dir_sf ) dir_sf = dir_h1t->mkdir(Form("sf%g", factor[isf]));
			dir_sf->cd();
	for(int iene=0;iene<nenebin;iene++){
		if(h1exp_st_T[ifov][isf][iene]) h1exp_st_T[ifov][isf][iene]->Write();
		if(h1exp_igrf_T[ifov][isf][iene]) h1exp_igrf_T[ifov][isf][iene]->Write();
	}
	}
	}
		dir_top->cd();
}
