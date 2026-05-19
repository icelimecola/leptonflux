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
	bool has_xmin;
	bool has_xmax;
	double xmin;
	double xmax;
	bool has_ymin;
	bool has_ymax;
	double ymin;
	double ymax;
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
	//====init var
	var.fpath = "datain/26*/dataiss/tfit/";
	var.fname = "fitresult_enebin03_1.33GeV.root";
	var.fpathname = ""; // Initialize fpathname as an empty string
	var.tname = "tfit";
	var.xname = "tmid";
	var.yname = "nele";
	var.yerrname = "nele_err";
	var.ytitle = "Electron Number";
	var.has_xmin = false;
	var.has_xmax = false;
	var.xmin = 1305849600;
	var.xmax = 1761955200;
	var.has_ymin = false;
	var.has_ymax = false;
	var.ymin = 0.0;
	var.ymax = 0.0;
	var.foutname = "htime";
	var.elow = -1.0;
	var.eup = -1.0;
	var.nt = 6000;
	var.tmin = 1305417600;
	var.tmax = 1823817600;
	//====get
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
		var.xmin = atof(argv[13]);
		var.has_xmin = true;
	}
	if(argc > 14 && argv[14][0] != '\0'){
		var.xmax = atof(argv[14]);
		var.has_xmax = true;
	}
	if(argc > 15 && argv[15][0] != '\0'){
		var.ymin = atof(argv[15]);
		var.has_ymin = true;
	}
	if(argc > 16 && argv[16][0] != '\0'){
		var.ymax = atof(argv[16]);
		var.has_ymax = true;
	}
	//====process
	if(var.fpathname.Length() == 0) var.fpathname = var.fpath + var.fname;
	var.foutname_root = var.foutname + ".root";
	var.foutname_pdf = var.foutname + ".pdf";
	//====print
	cout<<"IN INIT_conf ===== fpath="<<var.fpath
		<<" fname="<<var.fname
		<<" fpathname="<<var.fpathname
		<<" tree="<<var.tname
		<<" x="<<var.xname
		<<" y="<<var.yname
		<<" yerr="<<var.yerrname
		<<" ytitle="<<var.ytitle
		<<" xmin="<<(var.has_xmin ? Form("%g", var.xmin) : TString("N/A"))
		<<" xmax="<<(var.has_xmax ? Form("%g", var.xmax) : TString("N/A"))
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
	//====init var
	int x = 0;
	double y = 0.0;
	double yerr = 0.0;
	double elow = -1.0;
	double eup = -1.0;
	//====init chain
	TChain chain(var.tname);
	chain.Add(var.fpathname);
	//====setbranch
	chain.SetBranchAddress(var.xname, &x);
	chain.SetBranchAddress(var.yname, &y);
	chain.SetBranchAddress(var.yerrname, &yerr);
	chain.SetBranchAddress("Elow", &elow);
	chain.SetBranchAddress("Eup", &eup);
	//====fill hist
	int nx = chain.GetEntries();
	for(int ix=0; ix<nx; ix++){
		chain.GetEntry(ix);
		h->Fill((double)x, y);
		h->SetBinError(h->FindBin((double)x), yerr);
	}
	var.elow = elow;
	var.eup = eup;
	//====print
	cout<<"IN HIST ===== pattern="<<var.fpathname
		<<" n_entry_total="<<nx
		<<" elow="<<var.elow
		<<" eup="<<var.eup
		<<" h_sum="<<h->Integral()
		<<endl;
}


//==================================================== FITDRAW_
void DRAW(const VarConf &var, TH1D *h){
	//====init--output
	TFile *fout = TFile::Open(var.foutname_root, "RECREATE");
	h->Write();
	//====init--canvas
	TCanvas *c = new TCanvas("c","c",1000,400);
	// TCanvas *c = new TCanvas("c","c",1000,600);
	TAxis *xaxis = h->GetXaxis();
	TAxis *yaxis = h->GetYaxis();
	//====canvas
	h->SetStats(0);
	h->SetNameTitle("", "");
	c->SetTopMargin(0.13);
	c->SetBottomMargin(0.15);
	c->SetLeftMargin(0.13);
	c->SetRightMargin(0.08);
	c->cd();
	gPad->SetGridx();
	gPad->SetGridy();
	//====x
	if(var.has_xmin && var.has_xmax) xaxis->SetRangeUser(var.xmin, var.xmax);
	if(!var.has_xmin && !var.has_xmax) xaxis->SetRangeUser(var.xmin, var.xmax);
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
	//====y
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
	//====draw
	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);
	h->Draw("E1X0P");
	//====draw title
	c->cd(0);
	TLatex latex;
	latex.SetNDC();
	latex.SetTextFont(62);
	latex.SetTextSize(0.033);
	latex.SetTextAlign(22);
	if(var.elow>0 && var.eup>0) latex.DrawLatex(0.5, 0.97, Form("Energy %g to %g GeV", var.elow, var.eup));
	else latex.DrawLatex(0.5, 0.97, "Energy N/A");
	//====write
	c->SaveAs(var.foutname_pdf);
	fout->cd();
	c->Write();
	delete c;
	fout->Write();
	fout->Close();
	delete fout;
	//====print
	cout<<"IN DRAW ===== output root: "<<var.foutname_root<<endl;
	cout<<"IN DRAW ===== output pdf : "<<var.foutname_pdf<<endl;
}



//==================================================== FIT_RUN_
int ADD_TIME(int argc, char *argv[]){
	VarConf var{};
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
