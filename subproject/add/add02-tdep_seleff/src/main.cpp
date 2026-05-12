#include <iostream>
using namespace std;

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLatex.h"

//====================================================
struct VarConf{
	TString fpath;
	TString fname;
	TString fpathname;
	TString tname;
	TString xname;
	TString yname;
	TString yerrname;
	TString ytitle;
	double ymin;
	double ymax;
	bool has_ymin;
	bool has_ymax;
	TString foutname;
	TString foutname_root;
	TString foutname_pdf;
	double elow;
	double eup;
	int nt;
	int tmin;
	int tmax;
};


//==================================================== INIT_
void INIT(int argc, char *argv[], VarConf &var){
	//---- 260421 default cfg for tfit
	var.fpath = "datain/26*/dataiss/tfit/";
	var.fname = "fitresult_enebin03_1.33GeV.root";
	var.fpathname = ""; // Initialize fpathname as an empty string
	var.tname = "tfit";
	var.xname = "tmid";
	var.yname = "nele";
	var.yerrname = "nele_err";
	var.ytitle = "Electron Number";
	var.ymin = 0.0;
	var.ymax = 0.0;
	var.has_ymin = false;
	var.has_ymax = false;
	var.foutname = "htime";
	var.elow = -1.0;
	var.eup = -1.0;

	// const double width_tbin = 60*60*24;  //---- 86400[s]
	// static const int nt=6000;
	// const double tmin = 1305417600;      //---- Sun May 15 2011 00:00:00 GMT+0000
	// const double tmax = 1823817600;
	var.nt = 6000;
	var.tmin = 1305417600;
	var.tmax = 1823817600;

	//---- argv override (保持简单)
	if(argc > 1) var.fpath = argv[1];
	if(argc > 2) var.fname = argv[2];
	if(argc > 3) var.fpathname = argv[3];
	if(argc > 4) var.yname = argv[4];
	if(argc > 5) var.yerrname = argv[5];
	if(argc > 6) var.ytitle = argv[6];
	if(argc > 7) var.foutname = argv[7];
	if(argc > 8) var.tname = argv[8];
	if(argc > 9) var.xname = argv[9];
	if(argc > 10) var.nt = atoi(argv[10]);
	if(argc > 11) var.tmin = atoi(argv[11]);
	if(argc > 12) var.tmax = atoi(argv[12]);
	if(argc > 13 && argv[13][0] != '\0'){
		var.ymin = atof(argv[13]);
		var.has_ymin = true;
	}
	if(argc > 14 && argv[14][0] != '\0'){
		var.ymax = atof(argv[14]);
		var.has_ymax = true;
	}

	//---- name + suffix
	if(var.fpathname.Length() == 0) var.fpathname = var.fpath + var.fname;
	var.foutname_root = var.foutname + ".root";
	var.foutname_pdf = var.foutname + ".pdf";

	cout<<"IN INIT_conf ===== fpath="<<var.fpath
		<<" fname="<<var.fname
		<<" fpathname="<<var.fpathname
		<<" tree="<<var.tname
		<<" x="<<var.xname
		<<" y="<<var.yname
		<<" yerr="<<var.yerrname
		<<" ytitle="<<var.ytitle
		<<" ymin="<<(var.has_ymin ? Form("%g", var.ymin) : TString("N/A"))
		<<" ymax="<<(var.has_ymax ? Form("%g", var.ymax) : TString("N/A"))
		<<" outbase="<<var.foutname
		<<" nt="<<var.nt
		<<" tmin="<<var.tmin
		<<" tmax="<<var.tmax
		<<endl;
}

//==================================================== FITHIST_
void HIST(VarConf &var, TH1D *h){
	int xvalue = 0;
	double yvalue = 0.0;
	double yerrvalue = 0.0;
	double elow = -1.0;
	double eup = -1.0;
	bool has_yerr = false;
	TChain chain(var.tname);
	chain.Add(var.fpathname);
	chain.SetBranchAddress(var.xname, &xvalue);
	chain.SetBranchAddress(var.yname, &yvalue);
	if(chain.GetBranch(var.yerrname)){
		chain.SetBranchAddress(var.yerrname, &yerrvalue);
		has_yerr = true;
	}

	//---- elow/eup branch: lowercase first, fallback to legacy uppercase
	if(chain.GetBranch("elow")) chain.SetBranchAddress("elow", &elow);
	else if(chain.GetBranch("Elow")) chain.SetBranchAddress("Elow", &elow);
	if(chain.GetBranch("eup")) chain.SetBranchAddress("eup", &eup);
	else if(chain.GetBranch("Eup")) chain.SetBranchAddress("Eup", &eup);

	int nx = chain.GetEntries();
	for(int ix=0; ix<nx; ix++){
		chain.GetEntry(ix);
		h->Fill((double)xvalue, yvalue);
		if(has_yerr){
			int ibin = h->FindBin((double)xvalue);
			h->SetBinError(ibin, yerrvalue);
		}
	}
	//---- write back to config (single-file path uses one energy bin)
	var.elow = elow;
	var.eup = eup;
	cout<<"IN HIST ===== pattern="<<var.fpathname
		<<" n_entry_total="<<nx
		<<" elow="<<var.elow
		<<" eup="<<var.eup
		<<" has_yerr="<<has_yerr
		<<" h_sum="<<h->Integral()
		<<endl;
}


//==================================================== FITDRAW_
void DRAW(const VarConf &var, TH1D *h){
	const int x_unix_low = 1305849600;
	// const int x_unix_up  = 1635811200;
	const int x_unix_up  = 1761955200;

	TFile *fout = TFile::Open(var.foutname_root, "RECREATE");
	h->Write();

	TCanvas *c = new TCanvas("c","c",1000,600);
	TAxis *xaxis = h->GetXaxis();
	TAxis *yaxis = h->GetYaxis();

	h->SetStats(0);
	h->SetNameTitle("", "");
	c->SetTopMargin(0.13);
	c->SetBottomMargin(0.15);
	c->SetLeftMargin(0.13);
	c->SetRightMargin(0.08);
	c->cd();
	gPad->SetGridx();
	gPad->SetGridy();

	xaxis->SetRangeUser(x_unix_low, x_unix_up);
	xaxis->SetNameTitle("Date", "Date");
	xaxis->CenterTitle();
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(0.05);
	xaxis->SetTitleOffset(1.2);
	xaxis->SetLabelOffset(0.025);
	gStyle->SetTimeOffset(0);
	xaxis->SetTimeDisplay(1);
	xaxis->SetTimeFormat("%b/%d/%Y");
	xaxis->SetNdivisions(-505);

	yaxis->SetNameTitle("expstime", var.ytitle);
	yaxis->CenterTitle();
	yaxis->SetTitleFont(62);
	yaxis->SetTitleSize(0.05);
	yaxis->SetTitleOffset(0.9);
	yaxis->SetLabelOffset(0.012);
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.9);
	h->SetMarkerColor(kBlue);
	h->SetLineColor(kBlue);
	if(var.has_ymin) h->SetMinimum(var.ymin);
	if(var.has_ymax) h->SetMaximum(var.ymax);

	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);
	h->Draw("E1X0P");

	//==================================================== add total title
	c->cd(0);
	TLatex latex;
	latex.SetNDC();
	latex.SetTextFont(62);
	latex.SetTextSize(0.033);
	latex.SetTextAlign(22);
	if(var.elow>0 && var.eup>0) latex.DrawLatex(0.5, 0.97, Form("Energy %g to %g GeV", var.elow, var.eup));
	else latex.DrawLatex(0.5, 0.97, "Energy N/A");

	c->SaveAs(var.foutname_pdf);
	fout->cd();
	c->Write();
	delete c;

	fout->Write();
	fout->Close();
	delete fout;

	cout<<"IN DRAW ===== output root: "<<var.foutname_root<<endl;
	cout<<"IN DRAW ===== output pdf : "<<var.foutname_pdf<<endl;
}



//==================================================== FIT_RUN_
int ADD_TIME(int argc, char *argv[]){
	VarConf var;
	INIT(argc, argv, var);
	TH1D *htime = new TH1D("htime","htime",var.nt, var.tmin, var.tmax);
	HIST(var, htime);
	DRAW(var, htime);
	delete htime;
	return 0;
}


//==================================================== main
int main(int argc, char *argv[]){
	return ADD_TIME(argc, argv);
}
