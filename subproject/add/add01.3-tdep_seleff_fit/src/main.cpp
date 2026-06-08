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
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPaveText.h"

#include "../third_party/splinefit3/splineFit3.h"

//====================================================
struct VarConf{
	TString fpath;
	TString fname;
	TString fpathname;
	TString fpathname_mc;
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

struct VarDataEntry{
	double x;
	double y;
	double yerr;
};

struct VarDataSeries{
	vector<VarDataEntry> vdata;
	double elow;
	double eup;
	int n_entry;
};


//==================================================== INIT_
void INIT(int argc, char *argv[], VarConf &var){
	//====init var
	var.fpath = "datain/26*/dataiss/tfit/";
	var.fname = "fitresult_enebin03_1.33GeV.root";
	var.fpathname = ""; // Initialize fpathname as an empty string
	var.fpathname_mc = "";
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
	if(argc > 17) var.fpathname_mc = argv[17];
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
		<<" fpathname_mc="<<var.fpathname_mc
		<<" outbase="<<var.foutname
		<<" nt="<<var.nt
		<<" tmin="<<var.tmin
		<<" tmax="<<var.tmax
		<<endl;
}

//==================================================== FITHIST_
bool READ_TFIT(const VarConf &var, const TString &fpathname, VarDataSeries &series){
	//====init var
	UInt_t x = 0;
	double y = 0.0;
	double yerr = 0.0;
	double elow = -1.0;
	double eup = -1.0;
	//====init chain
	TChain chain(var.tname);
	int n_file = chain.Add(fpathname);
	if(n_file <= 0){
		cerr<<"ERR READ_TFIT ===== no input file matched: "<<fpathname<<endl;
		return false;
	}
	//====setbranch
	if(chain.SetBranchAddress(var.xname, &x) < 0){
		cerr<<"ERR READ_TFIT ===== missing x branch: "<<var.xname<<" in "<<fpathname<<endl;
		return false;
	}
	if(chain.SetBranchAddress(var.yname, &y) < 0){
		cerr<<"ERR READ_TFIT ===== missing y branch: "<<var.yname<<" in "<<fpathname<<endl;
		return false;
	}
	if(chain.SetBranchAddress(var.yerrname, &yerr) < 0){
		cerr<<"ERR READ_TFIT ===== missing yerr branch: "<<var.yerrname<<" in "<<fpathname<<endl;
		return false;
	}
	if(chain.SetBranchAddress("Elow", &elow) < 0){
		cerr<<"ERR READ_TFIT ===== missing branch: Elow in "<<fpathname<<endl;
		return false;
	}
	if(chain.SetBranchAddress("Eup", &eup) < 0){
		cerr<<"ERR READ_TFIT ===== missing branch: Eup in "<<fpathname<<endl;
		return false;
	}
	//====fill hist
	int nx = chain.GetEntries();
	series.vdata.clear();
	series.vdata.reserve(nx);
	for(int ix=0; ix<nx; ix++){
		chain.GetEntry(ix);
		VarDataEntry entry{};
		entry.x = (double)x;
		entry.y = y;
		entry.yerr = yerr;
		series.vdata.push_back(entry);
	}
	series.elow = elow;
	series.eup = eup;
	series.n_entry = nx;
	//====print
	cout<<"IN READ_TFIT ===== pattern="<<fpathname
		<<" n_entry_total="<<nx
		<<" elow="<<series.elow
		<<" eup="<<series.eup
		<<endl;
	return true;
}

bool HIST(const VarDataSeries &iss, TH1D *h){
	for(int ix=0; ix<iss.n_entry; ix++){
		const VarDataEntry &entry = iss.vdata.at(ix);
		h->Fill(entry.x, entry.y);
		h->SetBinError(h->FindBin(entry.x), entry.yerr);
	}
	cout<<"IN HIST ===== n_entry_iss="<<iss.n_entry
		<<" h_sum="<<h->Integral()
		<<endl;
	return true;
}

bool CHECK_EMATCH(const VarDataSeries &iss, const VarDataSeries &mc){
	double eps = 1.0e-9;
	if(fabs(iss.elow-mc.elow) > eps || fabs(iss.eup-mc.eup) > eps){
		cerr<<"ERR CHECK_EMATCH ===== ISS/MC energy mismatch"
			<<" iss=("<<iss.elow<<","<<iss.eup<<")"
			<<" mc=("<<mc.elow<<","<<mc.eup<<")"
			<<endl;
		return false;
	}
	return true;
}

bool GET_NEAREST_ISS_YNODE(const VarDataSeries &iss, const double *xnode, double *ynode, const int nnode){
	for(int inode=0; inode<nnode; inode++){
		double dx_min = 0.0;
		bool is_first = true;
		for(int ix=0; ix<iss.n_entry; ix++){
			const VarDataEntry &entry = iss.vdata.at(ix);
			double dx = fabs(entry.x - xnode[inode]);
			if(is_first || dx < dx_min){
				dx_min = dx;
				ynode[inode] = entry.y;
				is_first = false;
			}
		}
		if(is_first){
			cerr<<"ERR GET_NEAREST_ISS_YNODE ===== no ISS entry for inode="<<inode<<endl;
			return false;
		}
		cout<<"IN GET_NEAREST_ISS_YNODE ===== inode="<<inode
			<<" xnode="<<xnode[inode]
			<<" ynode="<<ynode[inode]
			<<" dx_min="<<dx_min
			<<endl;
	}
	return true;
}

bool BUILD_RATIO_HIST(const SplineFit *spfit_iss, const TGraphErrors *giss_conf,
		const VarDataSeries &mc, const double xdrawmin, const double xdrawmax,
		TH1D *hratio){
	if(spfit_iss == nullptr || spfit_iss->f1SplineFit == nullptr){
		cerr<<"ERR BUILD_RATIO_HIST ===== ISS spline fit is null"<<endl;
		return false;
	}
	if(giss_conf == nullptr){
		cerr<<"ERR BUILD_RATIO_HIST ===== ISS confidence graph is null"<<endl;
		return false;
	}
	if(mc.n_entry <= 0){
		cerr<<"ERR BUILD_RATIO_HIST ===== MC entry is empty"<<endl;
		return false;
	}
	double mc_y = mc.vdata.at(0).y;
	double mc_err = mc.vdata.at(0).yerr;
	if(mc_y == 0.0){
		cerr<<"ERR BUILD_RATIO_HIST ===== MC value is zero"<<endl;
		return false;
	}
	hratio->Reset("ICES");
	hratio->SetNameTitle("hratio", "hratio");
	hratio->SetStats(0);
	for(int ibin=1; ibin<=hratio->GetNbinsX(); ibin++){
		double xcenter = hratio->GetBinCenter(ibin);
		double xlow = hratio->GetBinLowEdge(ibin);
		double xup = hratio->GetBinLowEdge(ibin + 1);
		if(xcenter < xdrawmin || xcenter > xdrawmax) continue;
		if(xup <= xlow) continue;

		double iss_y = spfit_iss->f1SplineFit->Integral(xlow, xup) / (xup - xlow);
		if(!std::isfinite(iss_y) || iss_y == 0.0) continue;
		double iss_err = 0.0;
		double dx_min = 0.0;
		bool is_first = true;
		for(int ip=0; ip<giss_conf->GetN(); ip++){
			double gx = 0.0;
			double gy = 0.0;
			giss_conf->GetPoint(ip, gx, gy);
			double dx = fabs(gx - xcenter);
			if(is_first || dx < dx_min){
				dx_min = dx;
				iss_err = giss_conf->GetErrorY(ip);
				is_first = false;
			}
		}
		double ratio = iss_y / mc_y;
		double ratio_err = fabs(ratio) * sqrt(pow(iss_err / iss_y, 2) + pow(mc_err / mc_y, 2));
		hratio->SetBinContent(ibin, ratio);
		hratio->SetBinError(ibin, ratio_err);
	}
	cout<<"IN BUILD_RATIO_HIST ===== hratio built with mc_y="<<mc_y
		<<" mc_err="<<mc_err
		<<endl;
	return true;
}

bool BUILD_RATIO_HIST_ISS(const TH1D *hiss, const VarDataSeries &mc,
		const double xdrawmin, const double xdrawmax, TH1D *hratio){
	if(hiss == nullptr || hratio == nullptr){
		cerr<<"ERR BUILD_RATIO_HIST_ISS ===== ISS/ratio hist is null"<<endl;
		return false;
	}
	if(mc.n_entry <= 0){
		cerr<<"ERR BUILD_RATIO_HIST_ISS ===== MC entry is empty"<<endl;
		return false;
	}
	double mc_y = mc.vdata.at(0).y;
	double mc_err = mc.vdata.at(0).yerr;
	if(mc_y == 0.0){
		cerr<<"ERR BUILD_RATIO_HIST_ISS ===== MC value is zero"<<endl;
		return false;
	}
	hratio->Reset("ICES");
	hratio->SetNameTitle("hratio", "hratio");
	hratio->SetStats(0);
	for(int ibin=1; ibin<=hratio->GetNbinsX(); ibin++){
		double xcenter = hratio->GetBinCenter(ibin);
		if(xcenter < xdrawmin || xcenter > xdrawmax) continue;

		double iss_y = hiss->GetBinContent(ibin);
		double iss_err = hiss->GetBinError(ibin);
		if(!std::isfinite(iss_y) || iss_y == 0.0) continue;
		double ratio = iss_y / mc_y;
		double ratio_err = fabs(ratio) * sqrt(pow(iss_err / iss_y, 2) + pow(mc_err / mc_y, 2));
		hratio->SetBinContent(ibin, ratio);
		hratio->SetBinError(ibin, ratio_err);
	}
	cout<<"IN BUILD_RATIO_HIST_ISS ===== hratio built with mc_y="<<mc_y
		<<" mc_err="<<mc_err
		<<endl;
	return true;
}

//==================================================== FITDRAW_
bool DRAW(const VarConf &var, const VarDataSeries &iss, const VarDataSeries &mc, TH1D *h){
	//====init--output
	TFile *fout = TFile::Open(var.foutname_root, "RECREATE");
	if(fout == nullptr || fout->IsZombie()){
		cerr<<"ERR DRAW ===== failed to open output root: "<<var.foutname_root<<endl;
		return false;
	}
	h->Write();
	//====init--spline fit iss
	vector<double> vx_iss;
	vector<double> vy_iss;
	vector<double> vex_iss;
	vector<double> vey_iss;
	TGraphErrors *giss = nullptr;
	SplineFit *spfit_iss = nullptr;
	TGraphErrors *giss_conf = nullptr;
	TGraph *giss_conf_band = nullptr;
	TH1D *hratio = nullptr;
	TH1D *hratio27 = nullptr;
	TH1D *hratio1 = nullptr;
	//====init--canvas
	TCanvas *c = new TCanvas("c","c",1000,400);
	TAxis *xaxis = h->GetXaxis();
	TAxis *yaxis = h->GetYaxis();
	double xdrawmin = var.xmin;
	double xdrawmax = var.xmax;
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
	if(var.has_xmin && !var.has_xmax){
		xdrawmin = var.xmin;
		xdrawmax = xaxis->GetXmax();
		xaxis->SetRangeUser(xdrawmin, xdrawmax);
	}
	if(!var.has_xmin && var.has_xmax){
		xdrawmin = xaxis->GetXmin();
		xdrawmax = var.xmax;
		xaxis->SetRangeUser(xdrawmin, xdrawmax);
	}
	//====spline fit iss
	vx_iss.clear();
	vy_iss.clear();
	vex_iss.clear();
	vey_iss.clear();
	for(int ix=0; ix<iss.n_entry; ix++){
		const VarDataEntry &entry = iss.vdata.at(ix);
		if(entry.x < xdrawmin || entry.x > xdrawmax) continue;
		vx_iss.push_back(entry.x);
		vy_iss.push_back(entry.y);
		vex_iss.push_back(0.0);
		vey_iss.push_back(entry.yerr);
	}
	if(vx_iss.size() < 4){
		cerr<<"ERR DRAW ===== not enough ISS points for spline fit"
			<<" n_point="<<vx_iss.size()
			<<endl;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
	giss = new TGraphErrors(vx_iss.size(), &vx_iss[0], &vy_iss[0], &vex_iss[0], &vey_iss[0]);
	giss->SetName("giss");
	//----trd
	// // spfit_iss = new SplineFit(4, "b1e1",
	// spfit_iss = new SplineFit(7, "b2e1",
	// 		SplineFit::LinearXY | SplineFit::ExtrapolateLB | SplineFit::ExtrapolateLE);
	// spfit_iss->SetRange(xdrawmin, xdrawmax);
	// spfit_iss->BuildTF1("spfit_iss");
	// spfit_iss->SetGraph(giss);
	// // double xnode_iss[8] = {1308916800, 1318248000, 1325246400, 1332244800, 1336910400, 1343908800, 1514203200, 1759147200};
	// // double xnode_iss[7] = {1308916800, 1318248000, 1325246400, 1332244800, 1343908800, 1514203200, 1759147200};
	// double xnode_iss[7] = {1308916800, 1318248000, 1325246400, 1334577600, 1367236800, 1514203200, 1759147200};
	// double ynode_iss[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	// if(!GET_NEAREST_ISS_YNODE(iss, xnode_iss, ynode_iss, 7)){
	// 	delete spfit_iss;
	// 	delete giss;
	// 	delete c;
	// 	fout->Close();
	// 	delete fout;
	// 	return false;
	// }
	//----ntrk
	spfit_iss = new SplineFit(4, "b1e1",
	// spfit_iss = new SplineFit(7, "b2e1",
			SplineFit::LinearXY | SplineFit::ExtrapolateLB | SplineFit::ExtrapolateLE);
	spfit_iss->SetRange(xdrawmin, xdrawmax);
	spfit_iss->BuildTF1("spfit_iss");
	spfit_iss->SetGraph(giss);
	// double xnode_iss[7] = {1308916800, 1318248000, 1325246400, 1334577600, 1367236800, 1514203200, 1759147200};
	// double ynode_iss[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double xnode_iss[4] = {1308916800, 1396400000, 1618000000, 1759147200};
	double ynode_iss[4] = {0.0, 0.0, 0.0, 0.0};
	if(!GET_NEAREST_ISS_YNODE(iss, xnode_iss, ynode_iss, 4)){
		delete spfit_iss;
		delete giss;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
	spfit_iss->SetNodesByX(xnode_iss);
	spfit_iss->SetYnode(ynode_iss);
	spfit_iss->SetXnodeLimits(0);
	//---- fix only valley y-node; keep other y-nodes free as in reference spline workflow
	int inode_valley = 1;
	int ipar_valley = 1 + 5 + inode_valley;
	// spfit_iss->f1SplineFit->SetParLimits(ipar_valley, ynode_iss[inode_valley], ynode_iss[inode_valley]);
	spfit_iss->doFit(xdrawmin, xdrawmax, "FQ");
	//====ratio histograms
	hratio = (TH1D*)h->Clone("hratio");
	hratio->SetDirectory(nullptr);
	hratio27 = (TH1D*)h->Clone("hratio27");
	hratio27->SetDirectory(nullptr);
	//---- 1day
	//----prl130posi--260419
	const double wt = 60*60*24;  //---- 86400[s]
	static const int nt_ratio1 = 6000;
	const double tmin_ratio1 = 1305417600;
	const double tmax_ratio1 = tmin_ratio1 + nt_ratio1 * wt;  //----1823817600
	hratio1 = new TH1D("hratio1", "hratio1", nt_ratio1, tmin_ratio1, tmax_ratio1);
	hratio1->SetDirectory(nullptr);
	vector<double> vx_conf;
	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		double xcenter = h->GetBinCenter(ibin);
		if(xcenter <= xdrawmin || xcenter >= xdrawmax) continue;
		vx_conf.push_back(xcenter);
	}
	for(int ibin=1; ibin<=hratio1->GetNbinsX(); ibin++){
		double xcenter = hratio1->GetBinCenter(ibin);
		if(xcenter <= xdrawmin || xcenter >= xdrawmax) continue;
		vx_conf.push_back(xcenter);
	}
	sort(vx_conf.begin(), vx_conf.end());
	vx_conf.erase(unique(vx_conf.begin(), vx_conf.end()), vx_conf.end());
	giss_conf = spfit_iss->CalConfIntVD(0.68, vx_conf);
	giss_conf_band = spfit_iss->GetConfIntBand(3001, kBlue - 9);
	if(giss_conf == nullptr || giss_conf_band == nullptr){
		cerr<<"ERR DRAW ===== failed to build ISS spline confidence interval"<<endl;
		delete hratio1;
		delete hratio27;
		delete hratio;
		delete spfit_iss;
		delete giss;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
	if(!BUILD_RATIO_HIST_ISS(h, mc, xdrawmin, xdrawmax, hratio)){
		delete hratio1;
		delete hratio27;
		delete hratio;
		delete spfit_iss;
		delete giss;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
	if(!BUILD_RATIO_HIST(spfit_iss, giss_conf, mc, xdrawmin, xdrawmax, hratio27)){
		delete hratio1;
		delete hratio27;
		delete hratio;
		delete spfit_iss;
		delete giss;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
	if(!BUILD_RATIO_HIST(spfit_iss, giss_conf, mc, xdrawmin, xdrawmax, hratio1)){
		delete hratio1;
		delete hratio27;
		delete hratio;
		delete spfit_iss;
		delete giss;
		delete c;
		fout->Close();
		delete fout;
		return false;
	}
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
	// if(var.has_ymin) h->SetMinimum(var.ymin);
	// if(var.has_ymax) h->SetMaximum(var.ymax);
	//---- auto y-range: use ISS points in draw range and MC reference
	bool has_yrange = false;
	double y_min_auto = 0.0;
	double y_max_auto = 0.0;
	for(int ix=0; ix<iss.n_entry; ix++){
		const VarDataEntry &entry = iss.vdata.at(ix);
		if(entry.x < xdrawmin || entry.x > xdrawmax) continue;
		if(!std::isfinite(entry.y)) continue;
		if(entry.y <= 0.0) continue;
		if(!has_yrange){
			y_min_auto = entry.y;
			y_max_auto = entry.y;
			has_yrange = true;
		}
		else{
			y_min_auto = min(y_min_auto, entry.y);
			y_max_auto = max(y_max_auto, entry.y);
		}
	}
	for(int ix=0; ix<mc.n_entry; ix++){
		const VarDataEntry &entry = mc.vdata.at(ix);
		if(!std::isfinite(entry.y)) continue;
		if(entry.y <= 0.0) continue;
		if(!has_yrange){
			y_min_auto = entry.y;
			y_max_auto = entry.y;
			has_yrange = true;
		}
		else{
			y_min_auto = min(y_min_auto, entry.y);
			y_max_auto = max(y_max_auto, entry.y);
		}
	}
	if(has_yrange){
		double y_width_auto = y_max_auto - y_min_auto;
		if(y_width_auto <= 0.0) y_width_auto = max(fabs(y_max_auto) * 0.05, 1.0e-6);
		h->SetMinimum(y_min_auto - 0.40 * y_width_auto);
		h->SetMaximum(y_max_auto + 0.20 * y_width_auto);
		cout<<"IN DRAW ===== auto y-range ymin="<<y_min_auto - 0.40 * y_width_auto
			<<" ymax="<<y_max_auto + 0.20 * y_width_auto
			<<" data_min="<<y_min_auto
			<<" data_max="<<y_max_auto
			<<endl;
	}
	//====draw
	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);
	h->Draw("E1X0P");
	giss_conf_band->SetName("giss_conf_band");
	giss_conf_band->SetFillColor(kBlue - 9);
	giss_conf_band->SetFillStyle(3002);
	giss_conf_band->SetLineColor(kBlue + 1);
	giss_conf_band->SetLineWidth(2);
	spfit_iss->f1SplineFit->SetLineColor(kBlue + 1);
	spfit_iss->f1SplineFit->SetLineWidth(2);
	spfit_iss->f1SplineFit->SetLineStyle(1);
	spfit_iss->f1SplineFit->Draw("L same");
	h->Draw("E1X0P same");
	//---- stat box position in NDC, same coordinate convention as TLegend
	double stat_x1 = 0.62;
	double stat_y1 = 0.68;
	double stat_x2 = 0.88;
	double stat_y2 = 0.86;
	TPaveText *st_iss = new TPaveText(stat_x1, stat_y1, stat_x2, stat_y2, "NDC");
	st_iss->SetName("st_iss");
	st_iss->SetFillColor(0);
	st_iss->SetLineColor(kBlue + 1);
	st_iss->SetTextColor(kBlue + 1);
	st_iss->SetTextAlign(12);
	st_iss->SetTextFont(42);
	st_iss->AddText(Form("#chi^{2} / ndf = %.1f / %d",
				spfit_iss->f1SplineFit->GetChisquare(),
				spfit_iss->f1SplineFit->GetNDF()));
	// st_iss->Draw("same");
	TLine *line_mc = new TLine(xdrawmin, mc.vdata.at(0).y, xdrawmax, mc.vdata.at(0).y);
	line_mc->SetLineColor(kRed);
	line_mc->SetLineWidth(2);
	line_mc->SetLineStyle(2);
	line_mc->Draw("same");
	TLegend *leg = new TLegend(0.18, 0.16, 0.28, 0.30);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->AddEntry(h, "ISS", "ep");
	leg->AddEntry(spfit_iss->f1SplineFit, "ISS fit", "l");
	leg->AddEntry(line_mc, "MC", "l");
	leg->Draw();
	//====draw title
	c->cd(0);
	TLatex latex;
	latex.SetNDC();
	latex.SetTextFont(62);
	latex.SetTextSize(0.033);
	latex.SetTextAlign(22);
	if(iss.elow>0 && iss.eup>0) latex.DrawLatex(0.5, 0.95, Form("Energy %g to %g GeV", iss.elow, iss.eup));
	else latex.DrawLatex(0.5, 0.97, "Energy N/A");
	//====write
	c->SaveAs(var.foutname_pdf);
	fout->cd();
	hratio->Write("hratio");
	hratio27->Write("hratio27");
	hratio1->Write("hratio1");
	giss->Write("giss");
	giss_conf->Write("giss_conf");
	giss_conf_band->Write("giss_conf_band");
	spfit_iss->f1SplineFit->Write("spfit_iss");
	line_mc->Write("line_mc");
	st_iss->Write("st_iss");
	c->Write();
	leg->Write("leg");
	delete leg;
	delete st_iss;
	delete line_mc;
	delete hratio1;
	delete hratio27;
	delete hratio;
	delete spfit_iss;
	delete giss;
	delete c;
	fout->Write();
	fout->Close();
	delete fout;
	//====print
	cout<<"IN DRAW ===== output root: "<<var.foutname_root<<endl;
	cout<<"IN DRAW ===== output pdf : "<<var.foutname_pdf<<endl;
	return true;
}



//==================================================== FIT_RUN_
int ADD_TIME(int argc, char *argv[]){
	VarConf var{};
	VarDataSeries iss{};
	VarDataSeries mc{};
	INIT(argc, argv, var);
	if(var.fpathname_mc.Length() == 0){
		cerr<<"ERR ADD_TIME ===== mc pathname is empty"<<endl;
		return 1;
	}
	TH1D *htime = new TH1D("htime","htime",var.nt, var.tmin, var.tmax);
	if(!READ_TFIT(var, var.fpathname, iss)){
		delete htime;
		return 2;
	}
	if(!READ_TFIT(var, var.fpathname_mc, mc)){
		delete htime;
		return 3;
	}
	if(!CHECK_EMATCH(iss, mc)){
		delete htime;
		return 4;
	}
	if(mc.n_entry != 1){
		cerr<<"ERR ADD_TIME ===== mc entry number must be 1, now "<<mc.n_entry<<endl;
		delete htime;
		return 5;
	}
	if(!HIST(iss, htime)){
		delete htime;
		return 6;
	}
	if(!DRAW(var, iss, mc, htime)){
		delete htime;
		return 7;
	}
	delete htime;
	return 0;
}


//==================================================== main
int main(int argc, char *argv[]){
	return ADD_TIME(argc, argv);
}
