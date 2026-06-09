_Pragma("once")

#include <iostream>
using namespace std;
#include "TString.h"
// #include "TChain.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TTimeStamp.h"
#include "TColor.h"
#include "TGaxis.h"
#include "VarMinitree.hpp"

class ExpsHist : virtual public VarMinitree{
    public:
		ExpsHist(){cout<<"Constructor_AddExpsHist"<<endl;}
        ~ExpsHist(){};
	public:
        TFile *ins_filein;
        TCanvas *ins_can;
	public:
		//-- exps经纬度分布
		TH2D *h2d_exposure_mLongVSmLat;
		TH1D *h1d_exposure_mLat;
		TH1D *h1d_exposure_mLong;
		TH2D *h2d_exposure_gLongVSgLat;
		TH1D *h1d_exposure_gLat;
		TH1D *h1d_exposure_gLong;
		TH1D *h1d_exposure_mLat2;
		//-- func
		void EXPSHIST_AddHist(vector<TString>, int, int);
		void EXPSHIST_WriteHist();
		void EXPSHIST_DrawHist();
	public:
		//---- exps-TvE
		TH2D *h2exp_st_TvE[nsf];
		TH2D *h2exp_igrf_TvE[nsf];
		TH2D *h2exp_ts05_TvE[nsf];
		TH1D *h1exp_st_E[nfov][nsf];
		TH1D *h1exp_igrf_E[nfov][nsf];
		TH1D *h1exp_st_Efine[nfov][nsf];
		TH1D *h1exp_igrf_Efine[nfov][nsf];
		TH1D *h1exp_st_T[nfov][nsf][nenebin];
		TH1D *h1exp_igrf_T[nfov][nsf][nenebin];
		//---- func
		void EXPSHIST_AddHist_dayVSene(vector<TString>, int, int);
		void EXPSHIST_WriteHist_dayVSene();
		void EXPSHIST_DrawHist_dayVSene();
};

//===============================================================================
void ExpsHist::EXPSHIST_AddHist(vector<TString> fullfilename_vector,int jobid,int nfilesperjob){
    //====================================================
    //-- 初始化hist
    static int first_call = 1;
	if( first_call ){
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
    //====================================================
    //-- 循环参数初始化
    if (fullfilename_vector.size()==1){
        jobid=1;
        nfilesperjob=1;
    }
    //-- add hist
    for(int i=nfilesperjob*(jobid-1);i<nfilesperjob*(jobid);i++){
        //-- 异常情况
        if(i<0)break;
        if(i>=fullfilename_vector.size())break;
        //-- 读入第i个expshist
        cout<<"Adding File : "<<i<<" "<<fullfilename_vector.at(i)<<endl;
        ins_filein=new TFile(fullfilename_vector.at(i));
        //-- add hist
        h2d_exposure_mLongVSmLat->Add((TH2D*)ins_filein->Get("h2d_exposure_mLongVSmLat"));
        h1d_exposure_mLat->Add((TH1D*)ins_filein->Get("h1d_exposure_mLat"));
        h1d_exposure_mLong->Add((TH1D*)ins_filein->Get("h1d_exposure_mLong"));
        h2d_exposure_gLongVSgLat->Add((TH2D*)ins_filein->Get("h2d_exposure_gLongVSgLat"));
        h1d_exposure_gLat->Add((TH1D*)ins_filein->Get("h1d_exposure_gLat"));
        h1d_exposure_gLong->Add((TH1D*)ins_filein->Get("h1d_exposure_gLong"));
        h1d_exposure_mLat2->Add((TH1D*)ins_filein->Get("h1d_exposure_mLat2"));
    }
}
void ExpsHist::EXPSHIST_WriteHist(){
	cout << "h2d_exposure_mLongVSmLat = " << h2d_exposure_mLongVSmLat << endl;
	h2d_exposure_mLongVSmLat->Write();
	h1d_exposure_mLong->Write();
	h1d_exposure_mLat->Write();
	h2d_exposure_gLongVSgLat->Write();
	h1d_exposure_gLong->Write();
	h1d_exposure_gLat->Write();
	h1d_exposure_mLat2->Write();
}
void ExpsHist::EXPSHIST_DrawHist(){
    TCanvas ins_can("c1","c1_title",1000,400);
	ins_can.Divide(2,2);
    TH2D *h2d_temp;
    TH1D *h1d_temp;
    TAxis *xaxis,*yaxis,*zaxis;
    ///======
	ins_can.cd(1);
    h2d_temp=h2d_exposure_gLongVSgLat;
    ///===
    h2d_temp->SetNameTitle("","");
    xaxis=h2d_temp->GetXaxis();
    yaxis=h2d_temp->GetYaxis();
    zaxis=h2d_temp->GetZaxis();
    xaxis->SetNameTitle("longitude","Longitude (GTOD)");
    xaxis->CenterTitle();
    xaxis->SetTitleOffset(1.2);
    xaxis->SetTitleFont(62);
    xaxis->SetTitleSize(0.05);
    xaxis->SetLabelOffset(0.02);
    yaxis->SetNameTitle("latitude","Latitude (GTOD)");
    yaxis->CenterTitle();
    yaxis->SetTitleOffset(1);
    yaxis->SetTitleFont(62);
    yaxis->SetTitleSize(0.05);
    // zaxis->SetNameTitle("expstime","ExposureTime");
    // zaxis->CenterTitle();
    // zaxis->SetTitleOffset(0.8);
    h2d_temp->SetStats(0);
    gPad->SetLogz();
	h2d_temp->Draw("colz");
    ///======
	ins_can.cd(2);
    h2d_temp=h2d_exposure_mLongVSmLat;
    ///===
    h2d_temp->SetNameTitle("","");
    xaxis=h2d_temp->GetXaxis();
    yaxis=h2d_temp->GetYaxis();
    zaxis=h2d_temp->GetZaxis();
    xaxis->SetNameTitle("longitude","Longitude (AACGM)");
    xaxis->CenterTitle();
    xaxis->SetTitleOffset(1.2);
    xaxis->SetTitleFont(62);
    xaxis->SetTitleSize(0.05);
    xaxis->SetLabelOffset(0.02);
    yaxis->SetNameTitle("latitude","Latitude (AACGM)");
    yaxis->CenterTitle();
    yaxis->SetTitleOffset(1);
    yaxis->SetTitleFont(62);
    yaxis->SetTitleSize(0.05);
    // zaxis->SetNameTitle("expstime","ExposureTime");
    // zaxis->CenterTitle();
    // zaxis->SetTitleOffset(0.8);
    h2d_temp->SetStats(0);
    gPad->SetLogz();
	h2d_temp->Draw("colz");
    ///======
	ins_can.cd(3);
    h1d_temp=h1d_exposure_gLat;
    ///===
    h1d_temp->SetNameTitle("","");
    xaxis=h1d_temp->GetXaxis();
    yaxis=h1d_temp->GetYaxis();
    zaxis=h1d_temp->GetZaxis();
    xaxis->SetNameTitle("Latitude","Latitude (GTOD)");
    xaxis->CenterTitle();
    xaxis->SetTitleOffset(1.2);
    xaxis->SetTitleFont(62);
    xaxis->SetTitleSize(0.05);
    xaxis->SetLabelOffset(0.02);
    yaxis->SetNameTitle("expstime","ExposureTime (s)");
    yaxis->CenterTitle();
    yaxis->SetTitleOffset(1);
    yaxis->SetTitleFont(62);
    yaxis->SetTitleSize(0.05);
    h1d_temp->SetStats(0);
	h1d_temp->Draw("h");
    ///======
	ins_can.cd(4);
    h1d_temp=h1d_exposure_mLat;
    ///===
    h1d_temp->SetNameTitle("","");
    xaxis=h1d_temp->GetXaxis();
    yaxis=h1d_temp->GetYaxis();
    zaxis=h1d_temp->GetZaxis();
    xaxis->SetNameTitle("Latitude","Latitude (AACGM)");
    xaxis->CenterTitle();
    xaxis->SetTitleOffset(1.2);
    xaxis->SetTitleFont(62);
    xaxis->SetTitleSize(0.05);
    xaxis->SetLabelOffset(0.02);
    yaxis->SetNameTitle("expstime","ExposureTime (s)");
    yaxis->CenterTitle();
    yaxis->SetTitleOffset(1);
    yaxis->SetTitleFont(62);
    yaxis->SetTitleSize(0.05);
    h1d_temp->SetStats(0);
	h1d_temp->Draw("h");
    ///======
	ins_can.Write();
    ///======
    // ins_can.SaveAs("expshist_draw.png");
    // ins_can.SaveAs("expshist_draw.svg");
    ins_can.SaveAs("expshist_draw.pdf");
}

//===============================================================================
void ExpsHist::EXPSHIST_AddHist_dayVSene(vector<TString> fullfilename_vector,int jobid,int nfilesperjob){
	//============ init ============
	//====init array
	double fov[nfov] = { 25, 30, 35, 40 };
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	//====set length
	int nfov,nsf;
	nfov=1;
	// int nsf=1;
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
            //     nenebin_fine, energy_bins_fine);
            // h1exp_igrf_Efine[ifov][isf] = new TH1D( Form("h1exp_igrf_Efine_fov%g_sf%g", fov[ifov], factor[isf]), Form("h1exp_igrf_Efine_fov%g_sf%g", fov[ifov], factor[isf]), 
                // nenebin_fine, energy_bins_fine);
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
	//============ add hist ============
	if( fullfilename_vector.size() == 1 ){
		jobid = 1;
		nfilesperjob = 1;
	}
	for(int i=nfilesperjob*(jobid-1); i<nfilesperjob*jobid; i++){
		if( i < 0 ) break;
		if( i >= fullfilename_vector.size() ) break;
		cout<<"Adding File : "<<i<<" "<<fullfilename_vector.at(i)<<endl;
		ins_filein = new TFile(fullfilename_vector.at(i));
		//====add--h2
		for(int isf=0;isf<nsf;isf++){
            if( h2exp_st_TvE[isf] ) h2exp_st_TvE[isf]->Add((TH2D*)ins_filein->Get(Form("h2/h2exp_st_TvE_sf%g", factor[isf])));
			if( h2exp_igrf_TvE[isf] ) h2exp_igrf_TvE[isf]->Add((TH2D*)ins_filein->Get(Form("h2/h2exp_igrf_TvE_sf%g", factor[isf])));
			if( h2exp_ts05_TvE[isf] ) h2exp_ts05_TvE[isf]->Add((TH2D*)ins_filein->Get(Form("h2/h2exp_ts05_TvE_sf%g", factor[isf])));
		}
		//====add--h1ene
		for(int ifov=0;ifov<nfov;ifov++){
        for(int isf=0;isf<nsf;isf++){
            if( h1exp_st_E[ifov][isf] ){
                h1exp_st_E[ifov][isf]->Add((TH1D*)ins_filein->Get(Form("h1ene/sf%g/h1exp_st_E_fov%g_sf%g", factor[isf], fov[ifov], factor[isf])));
                h1exp_igrf_E[ifov][isf]->Add((TH1D*)ins_filein->Get(Form("h1ene/sf%g/h1exp_igrf_E_fov%g_sf%g", factor[isf], fov[ifov], factor[isf])));
            }
            // if(h1exp_st_Efine[ifov][isf]){
            //     h1exp_st_Efine[ifov][isf]->Add((TH1D*)ins_filein->Get(Form("h1exp_st_Efine_fov%g_sf%g", fov[ifov], factor[isf])));
            //     h1exp_igrf_Efine[ifov][isf]->Add((TH1D*)ins_filein->Get(Form("h1exp_igrf_Efine_fov%g_sf%g", fov[ifov], factor[isf])));
            // }
        }
        }
		//====add--h1t
        for(int ifov=0;ifov<nfov;ifov++){
        for(int isf=0;isf<nsf;isf++){
        for(int iene=0;iene<nenebin;iene++){
            h1exp_st_T[ifov][isf][iene]->Add((TH1D*)ins_filein->Get(Form("h1t/st/sf%g/h1exp_st_T_fov%g_sf%g_ene%gto%gGeV", factor[isf], fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1])));
            h1exp_igrf_T[ifov][isf][iene]->Add((TH1D*)ins_filein->Get(Form("h1t/igrf/sf%g/h1exp_igrf_T_fov%g_sf%g_ene%gto%gGeV", factor[isf], fov[ifov], factor[isf],energy_bins[iene],energy_bins[iene+1])));
        }
        }
        }
    }
}



void ExpsHist::EXPSHIST_WriteHist_dayVSene(){
	//============ init ============
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	int nfov,nsf;
	nfov=1;
	// int nsf=1;
	nsf=5;
	//============ save ============
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
	TDirectory *dir_h1t_st = dir_h1t->GetDirectory("st");
	if( !dir_h1t_st ) dir_h1t_st = dir_h1t->mkdir("st");
	TDirectory *dir_h1t_igrf = dir_h1t->GetDirectory("igrf");
	if( !dir_h1t_igrf ) dir_h1t_igrf = dir_h1t->mkdir("igrf");
	for(int ifov=0;ifov<nfov;ifov++){
    for(int isf=0;isf<nsf;isf++){
		TDirectory *dir_st_sf = dir_h1t_st->GetDirectory(Form("sf%g", factor[isf]));
		if( !dir_st_sf ) dir_st_sf = dir_h1t_st->mkdir(Form("sf%g", factor[isf]));
		TDirectory *dir_igrf_sf = dir_h1t_igrf->GetDirectory(Form("sf%g", factor[isf]));
		if( !dir_igrf_sf ) dir_igrf_sf = dir_h1t_igrf->mkdir(Form("sf%g", factor[isf]));
    for(int iene=0;iene<nenebin;iene++){
        if(h1exp_st_T[ifov][isf][iene]){
			dir_st_sf->cd();
            h1exp_st_T[ifov][isf][iene]->Write();
		}
		if(h1exp_igrf_T[ifov][isf][iene]){
			dir_igrf_sf->cd();
            h1exp_igrf_T[ifov][isf][iene]->Write();
        }
    }
    }
	}
        dir_top->cd();
}



void ExpsHist::EXPSHIST_DrawHist_dayVSene(){
    //============ init ============
	//====init array
	double fov[nfov] = { 25, 30, 35, 40 };
	double factor[nsf] = { 1, 1.1, 1.2, 1.3, 1.4 };
	//====set length
	int nfov,nsf;
	nfov=1;
	// int nsf=1;
	nsf=5;
	int target_isf = 0;   //---- sf = 1.1
	// int target_isf = 1;   //---- sf = 1.1
	const double xmin = 1305849600;
	const double xmax = 1761955200;
    //==================================================== temp var
    TH2D *h2d_temp;
    TH1D *h1d_temp;
    TAxis *xaxis,*yaxis,*zaxis;
    //==================================================== H2
	// for(int isf=0;isf<nsf;isf++){
    for(int isf=target_isf;isf<=target_isf;isf++){
        //============================ init
        TCanvas ins_can(Form("ch2_igrf_sf%g", factor[isf]), Form("can_h2_igrf_sf%g", factor[isf]), 1000, 400);
        h2d_temp = h2exp_igrf_TvE[isf];
        xaxis=h2d_temp->GetXaxis();
        yaxis=h2d_temp->GetYaxis();
        zaxis=h2d_temp->GetZaxis();
        //============================ hist
        h2d_temp->SetNameTitle("","");
        h2d_temp->SetStats(0);
        //============================ canvas
        ins_can.SetBottomMargin(0.12);
        ins_can.SetLeftMargin(0.12);
        ins_can.SetRightMargin(0.15);
        //============================ x axis
        xaxis->SetNameTitle("Date","Date");
        xaxis->CenterTitle();               //-- 居中标题
        xaxis->SetTitleOffset(1.2);         //-- 标题偏离
        xaxis->SetTitleFont(62);            //-- 标题字体
        xaxis->SetTitleSize(0.045);         //-- 标题大小
        xaxis->SetLabelOffset(0.02);        //-- 刻度偏离
        //-- 时间参量,主要包含初始值和总长度,单位长度为60*60*24=86400[s]
        const int n_tbin = 27*120;
        const double width_tbin = 60*60*24;
        //-- t0
        // double x1 = xaxis->GetBinLowEdge(1);
        // double x2 = xaxis->GetBinLowEdge(2369);
        // xaxis->SetRangeUser(x1,x2);
        gStyle->SetTimeOffset(0);
        xaxis->SetTimeDisplay(1);
        // xaxis->SetTimeFormat("%m/%d/%Y");
        xaxis->SetTimeFormat("%b/%d/%Y");
        xaxis->SetRangeUser(xmin, xmax);
        //============================ y axis
        yaxis->SetNameTitle("Energy","Energy [GeV]");
        yaxis->CenterTitle();               //-- 居中标题
        yaxis->SetTitleOffset(1);           //-- 标题偏离
        yaxis->SetTitleFont(62);            //-- 标题字体
        yaxis->SetTitleSize(0.045);          //-- 标题大小
        yaxis->SetLabelOffset(0.01);        //-- 刻度偏离
        gPad->SetLogy();
        //============================ z axis
        zaxis->SetNameTitle("expstime","Exposure Time [s]");
        zaxis->CenterTitle();               //-- 居中标题
        zaxis->SetTitleFont(62);            //-- 标题字体
        zaxis->SetTitleSize(0.045);          //-- 标题大小
        // zaxis->SetTitleOffset(0.8);
        gPad->SetLogz();
        // gStyle->SetPalette(kRainBow);
        //============================ draw & save
        h2d_temp->Draw("colz");
	    ins_can.Write();
        ins_can.SaveAs(Form("h2exps_igrf_sf%g.pdf", factor[isf]));
    }
    //==================================================== H1--ENE
    for(int ifov=0;ifov<nfov;ifov++){
        // for(int isf=0;isf<nsf;isf++){
        for(int isf=target_isf;isf<=target_isf;isf++){
            //============================ init
            TCanvas ins_can(Form("ch1e_igrf_fov%g_sf%g", 25., factor[isf]), Form("ch1e_igrf_fov%g_sf%g", 25., factor[isf]), 1000, 400);
            // h1d_temp = h1exp_st_E[ifov][isf];
            h1d_temp = h1exp_igrf_E[ifov][isf];
            xaxis=h1d_temp->GetXaxis();
            yaxis=h1d_temp->GetYaxis();
            zaxis=h1d_temp->GetZaxis();
            //============================ hist
            h1d_temp->SetNameTitle("","");
            h1d_temp->SetStats(0);
            //============================ canvas
            ins_can.SetBottomMargin(0.15);
            ins_can.SetLeftMargin(0.12);
            // ins_can.SetRightMargin(0.15);
            gPad->SetGridx();  // 打开 X 方向主刻度线的网格
            gPad->SetGridy();  // 打开 Y 方向主刻度线的网格
            //============================ x axis
            xaxis->SetNameTitle("Energy","Energy [GeV]");
            xaxis->CenterTitle();
            xaxis->SetTitleOffset(1.3);
            xaxis->SetTitleFont(62);
            xaxis->SetTitleSize(0.045);
            xaxis->SetLabelOffset(0.013);
            gPad->SetLogx();
            //============================ y axis
            yaxis->SetNameTitle("expstime","Exposure Time [s]");
            yaxis->CenterTitle();
            yaxis->SetTitleOffset(1);
            yaxis->SetTitleFont(62);
            yaxis->SetTitleSize(0.045);
            yaxis->SetLabelOffset(0.012);
            //============================ draw & save
            h1d_temp->Draw("h");
            ins_can.Write();
            ins_can.SaveAs(Form("h1eexps_igrf_fov%g_sf%g.pdf", 25., factor[isf]));
        }
    }
    //==================================================== H1--DAY
    for(int ifov=0;ifov<nfov;ifov++){
        // for(int isf=0;isf<nsf;isf++){
        for(int isf=target_isf;isf<=target_isf;isf++){
			for(int iene=0;iene<nenebin;iene++){
                //============================ init
                TCanvas ins_can(Form("ch1t_igrf_sf%g_ene%g", factor[isf], energy_bins[iene]), Form("ch1t_igrf_sf%g_ene%g", factor[isf], energy_bins[iene]), 1000, 400);
                h1d_temp = h1exp_igrf_T[ifov][isf][iene];
                xaxis=h1d_temp->GetXaxis();
                yaxis=h1d_temp->GetYaxis();
                zaxis=h1d_temp->GetZaxis();
                //============================ hist
                h1exp_igrf_T[ifov][isf][iene]->SetTitleFont(62,"t");
                h1d_temp->SetNameTitle("",Form("Energy %g to %g GeV",energy_bins[iene],energy_bins[iene+1]));
                // gStyle->SetTitleFontSize();
                gStyle->SetTitleFont(62,"T");
                gStyle->SetTitleSize(0.05,"t");
                gStyle->SetTitleAlign(22);       // 设置全局标题对齐方式为居中靠上
                gStyle->SetTitleX(0.5);          // 设置标题在X方向上的位置（归一化坐标，0.5居中）
                gStyle->SetTitleY(0.94);         // 设置标题在Y方向上的位置（归一化坐标，0.98靠近顶部）
                // gStyle->SetTitleOffset(5,"t");     // 设置全局标题偏移量
                // h1d_temp->SetTitleSize(0.056);
                // h1d_temp->SetTitleOffset(0.025);
                h1d_temp->SetStats(0);
                //============================ canvas
                ins_can.SetTopMargin(0.13);
                ins_can.SetBottomMargin(0.15);
                ins_can.SetLeftMargin(0.13);
                ins_can.SetRightMargin(0.08);
                gPad->SetGridx();
                gPad->SetGridy();
                //============================ x axis
                xaxis->SetNameTitle("Date","Date");
                xaxis->CenterTitle();               //-- 居中标题
                xaxis->SetTitleFont(62);            //-- 标题字体
                // xaxis->SetTitleSize(0.045);         //-- 标题大小
                xaxis->SetTitleSize(0.05);         //-- 标题大小
                // xaxis->SetTitleOffset(1.2);         //-- 标题偏离
                xaxis->SetTitleOffset(1.2);         //-- 标题偏离
                xaxis->SetLabelOffset(0.025);        //-- 刻度偏离
                // double x1 = xaxis->GetBinLowEdge(1);
                // double x2 = xaxis->GetBinLowEdge(2369);
                // double x2 = xaxis->GetBinLowEdge(1700); //---- 2025-10-19
                // double x2 = xaxis->GetBinLowEdge(2295); //---- 2025-10-22
                // double x2 = xaxis->GetBinLowEdge(4949); //---- 2025-10-26
                // double x1 = xaxis->GetBinLowEdge(21);       //---- 2025-10-30
                // double x2 = xaxis->GetBinLowEdge(4582); //---- 2025-10-30
                // double x1 = xaxis->GetBinLowEdge(6);       //---- 251109
                // double x2 = xaxis->GetBinLowEdge(4567); //---- 251109
                // xaxis->SetRangeUser(x1,x2);
                gStyle->SetTimeOffset(0);
                xaxis->SetTimeDisplay(1);
                // xaxis->SetTimeFormat("%m/%d/%Y");
                xaxis->SetTimeFormat("%b/%d/%Y");
                xaxis->SetNdivisions(-505);
                xaxis->SetRangeUser(xmin, xmax);
                //============================ y axis
                yaxis->SetNameTitle("expstime","Exposure Time [s]");
                yaxis->CenterTitle();
                yaxis->SetTitleFont(62);
                // yaxis->SetTitleSize(0.045);
                yaxis->SetTitleSize(0.05);
                yaxis->SetTitleOffset(0.9);
                yaxis->SetLabelOffset(0.012);
                // yaxis->SetMaxDigits(3);
                TGaxis::SetMaxDigits(3);
                //============================ draw & save
                h1d_temp->Draw("HIST");
                ins_can.Write();
                ins_can.SaveAs(Form("h1texps_igrf_sf%g_ene%g.pdf", factor[isf], energy_bins[iene]));
            } 
        }
    }
}
