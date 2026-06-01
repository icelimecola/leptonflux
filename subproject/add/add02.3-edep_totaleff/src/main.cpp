#include <iostream>
using namespace std;

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <glob.h>
#include <map>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"

//====================================================
// const int NENEBIN = 54;
// const double ENERGY_BINS[NENEBIN + 1] = {
// 	0.80, 1, 1.16, 1.33, 1.51,
// 	1.71, 1.92, 2.15, 2.40, 2.67,
// 	2.97, 3.29, 3.64, 4.02, 4.43,
// 	4.88, 5.37, 5.90, 6.47, 7.09,
// 	7.76, 8.48, 9.26, 10.10, 11,
// 	12, 13, 14.10, 15.30, 16.60,
// 	18, 19.50, 21.10, 22.80, 24.70,
// 	26.70, 28.80, 31.10, 33.50, 36.10,
// 	38.90, 41.90, 45.10, 48.50, 52.20,
// 	56.10, 60.30, 64.80, 69.70, 74.90,
// 	80.50, 86.50, 93, 100, 108
// };
const int nenebin = 29;
const double *energy_bins = new double[nenebin+1]{
	0.80, 1.00, 1.16, 1.33, 1.51,
	1.71, 1.92, 2.15, 2.40, 2.67,
	2.97, 3.29, 3.64, 4.02, 4.43,
	4.88, 5.37, 5.90, 6.47, 7.09,
	7.76, 8.48, 9.26, 10.10, 11.0,
	13.0, 16.6, 22.8, 41.9, 45.10
};

struct VarConf{
	TString fpath;
	TString foutname;
	TString foutname_root;
	TString foutname_pdf;
	bool has_xmin;
	bool has_xmax;
	double xmin;
	double xmax;
	bool has_ymin;
	bool has_ymax;
	double ymin;
	double ymax;
};

struct VarEffItem{
	TString effname;
	TString efflabel;
	TString fpathname;
	int color;
	TH1D *hratio;
	TGraph *gratio_conf_band;
	TF1 *spfit_ratio;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
};


//==================================================== TOOL_
bool IS_WILDCARD_PATTERN(const TString &pattern){
	return pattern.Contains("*") || pattern.Contains("?") || pattern.Contains("[");
}

TString NORMALIZE_PATTERN(const TString &pattern){
	if(pattern.Length() == 0) return pattern;
	if(IS_WILDCARD_PATTERN(pattern)) return pattern;

	struct stat stbuf;
	if(stat(pattern.Data(), &stbuf) == 0 && S_ISDIR(stbuf.st_mode)){
		return Form("%s/hene_*eff.root", pattern.Strip(TString::kTrailing, '/').Data());
	}
	if(pattern.EndsWith(".root")) return pattern;
	if(access((pattern + ".root").Data(), F_OK) == 0){
		return pattern + ".root";
	}
	return pattern + "/hene_*eff.root";
}

bool GET_FILE_LIST(const TString &pattern_in, vector<TString> &vfile){
	TString pattern = NORMALIZE_PATTERN(pattern_in);
	glob_t glob_result;
	vfile.clear();
	int status = glob(pattern.Data(), 0, nullptr, &glob_result);
	if(status == GLOB_NOMATCH){
		cerr<<"ERR GET_FILE_LIST ===== no input file matched: "<<pattern<<endl;
		globfree(&glob_result);
		return false;
	}
	if(status != 0){
		cerr<<"ERR GET_FILE_LIST ===== glob failed: "<<pattern<<" status="<<status<<endl;
		globfree(&glob_result);
		return false;
	}
	for(size_t ifile=0; ifile<glob_result.gl_pathc; ifile++){
		vfile.push_back(glob_result.gl_pathv[ifile]);
	}
	globfree(&glob_result);
	sort(vfile.begin(), vfile.end());
	cout<<"IN GET_FILE_LIST ===== pattern="<<pattern
		<<" n_file="<<vfile.size()
		<<endl;
	return true;
}

TString BASENAME_NOEXT(const TString &fpathname){
	TString name = gSystem->BaseName(fpathname);
	if(name.EndsWith(".root")) name.Remove(name.Length() - 5);
	return name;
}

TString GET_EFFNAME_FROM_FPATHNAME(const TString &fpathname){
	TString name = BASENAME_NOEXT(fpathname);
	if(name.BeginsWith("hene_")) name.Remove(0, 5);
	return name;
}

TString GET_EFFLABEL(const TString &effname){
	static map<string, string> label_map = {
		{"chi2eff", "Chi2 Eff."},
		{"ecaleff", "ECAL Eff."},
		{"matcheff", "Match Eff."},
		{"ntrkeff", "Ntrk Eff."},
		{"pateff", "Pattern Eff."},
		{"qineff", "InnerQ Eff."},
		{"tofeff", "TOF Eff."},
		{"totaleff", "Total Eff."},
		{"trdeff", "TRD Eff."},
		{"trkeff", "Tracker Eff."}
	};
	string key = effname.Data();
	if(label_map.count(key) > 0) return label_map[key].c_str();
	return effname;
}

int GET_EFFCOLOR(const TString &effname){
	static map<string, int> color_map = {
		{"chi2eff", kGray + 2},
		{"ecaleff", kOrange + 7},
		{"matcheff", kMagenta + 1},
		{"ntrkeff", kViolet + 1},
		{"pateff", kBlue + 1},
		{"qineff", kCyan + 2},
		{"tofeff", kGreen + 2},
		{"totaleff", kRed + 1},
		{"trdeff", kSpring + 5},
		{"trkeff", kAzure + 2}
	};
	string key = effname.Data();
	if(color_map.count(key) > 0) return color_map[key];
	return kBlack;
}

int GET_EFFORDER(const TString &effname){
	static vector<string> order = {
		"chi2eff",
		"ecaleff",
		"matcheff",
		"ntrkeff",
		"pateff",
		"qineff",
		"tofeff",
		"trdeff",
		"trkeff",
		"totaleff"
	};
	string key = effname.Data();
	for(size_t i=0; i<order.size(); i++){
		if(order[i] == key) return (int)i;
	}
	return 999;
}

bool UPDATE_RANGE_BY_GRAPH(const TGraph *gr, double &xmin, double &xmax, double &ymin, double &ymax, bool &is_first){
	if(gr == nullptr) return false;
	int n_point = gr->GetN();
	if(n_point <= 0) return false;
	double x = 0.0;
	double y = 0.0;
	for(int ip=0; ip<n_point; ip++){
		gr->GetPoint(ip, x, y);
		if(is_first){
			xmin = x;
			xmax = x;
			ymin = y;
			ymax = y;
			is_first = false;
		}
		else{
			xmin = std::min(xmin, x);
			xmax = std::max(xmax, x);
			ymin = std::min(ymin, y);
			ymax = std::max(ymax, y);
		}
	}
	return true;
}

bool UPDATE_RANGE_BY_GRAPHERRORS(const TGraphErrors *gr, double &xmin, double &xmax, double &ymin, double &ymax, bool &is_first){
	if(gr == nullptr) return false;
	int n_point = gr->GetN();
	if(n_point <= 0) return false;
	double x = 0.0;
	double y = 0.0;
	for(int ip=0; ip<n_point; ip++){
		gr->GetPoint(ip, x, y);
		double ex = gr->GetErrorX(ip);
		double ey = gr->GetErrorY(ip);
		double xlow = x - ex;
		double xhigh = x + ex;
		double ylow = y - ey;
		double yhigh = y + ey;
		if(is_first){
			xmin = xlow;
			xmax = xhigh;
			ymin = ylow;
			ymax = yhigh;
			is_first = false;
		}
		else{
			xmin = std::min(xmin, xlow);
			xmax = std::max(xmax, xhigh);
			ymin = std::min(ymin, ylow);
			ymax = std::max(ymax, yhigh);
		}
	}
	return true;
}

bool UPDATE_RANGE_BY_HIST(const TH1D *h, double &xmin, double &xmax, double &ymin, double &ymax, bool &is_first){
	if(h == nullptr) return false;
	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		double y = h->GetBinContent(ibin);
		double ey = h->GetBinError(ibin);
		double x = h->GetBinCenter(ibin);
		double ex = h->GetBinWidth(ibin) * 0.5;
		double xlow = x - ex;
		double xhigh = x + ex;
		double ylow = y - ey;
		double yhigh = y + ey;
		if(is_first){
			xmin = xlow;
			xmax = xhigh;
			ymin = ylow;
			ymax = yhigh;
			is_first = false;
		}
		else{
			xmin = std::min(xmin, xlow);
			xmax = std::max(xmax, xhigh);
			ymin = std::min(ymin, ylow);
			ymax = std::max(ymax, yhigh);
		}
	}
	return true;
}

void DELETE_ALL_EFF(vector<VarEffItem> &veff){
	for(size_t i=0; i<veff.size(); i++){
		delete veff[i].hratio;
		delete veff[i].gratio_conf_band;
		delete veff[i].spfit_ratio;
	}
	veff.clear();
}


//==================================================== INIT_
void INIT(int argc, char *argv[], VarConf &var){
	var.fpath = "datain/hene_*eff.root";
	var.foutname = "hene_combine";
	var.has_xmin = false;
	var.has_xmax = false;
	var.xmin = energy_bins[0];
	var.xmax = energy_bins[nenebin];
	var.has_ymin = false;
	var.has_ymax = false;
	var.ymin = 0.0;
	var.ymax = 0.0;

	if(argc > 1) var.fpath = argv[1];
	if(argc > 2) var.foutname = argv[2];
	if(argc > 3 && argv[3][0] != '\0'){
		var.xmin = atof(argv[3]);
		var.has_xmin = true;
	}
	if(argc > 4 && argv[4][0] != '\0'){
		var.xmax = atof(argv[4]);
		var.has_xmax = true;
	}
	if(argc > 5 && argv[5][0] != '\0'){
		var.ymin = atof(argv[5]);
		var.has_ymin = true;
	}
	if(argc > 6 && argv[6][0] != '\0'){
		var.ymax = atof(argv[6]);
		var.has_ymax = true;
	}

	var.foutname_root = var.foutname + ".root";
	var.foutname_pdf = var.foutname + ".pdf";

	cout<<"IN INIT_conf ===== fpath="<<var.fpath
		<<" foutname="<<var.foutname
		<<" xmin="<<(var.has_xmin ? Form("%g", var.xmin) : TString("AUTO"))
		<<" xmax="<<(var.has_xmax ? Form("%g", var.xmax) : TString("AUTO"))
		<<" ymin="<<(var.has_ymin ? Form("%g", var.ymin) : TString("AUTO"))
		<<" ymax="<<(var.has_ymax ? Form("%g", var.ymax) : TString("AUTO"))
		<<endl;
}


//==================================================== READ_
bool READ_ALL_EFF(const VarConf &var, vector<VarEffItem> &veff,
		double &xall_min, double &xall_max, double &yall_min, double &yall_max){
	vector<TString> vfile;
	bool is_first = true;

	veff.clear();
	if(!GET_FILE_LIST(var.fpath, vfile)){
		return false;
	}

	for(size_t ifile=0; ifile<vfile.size(); ifile++){
		TString fpathname = vfile.at(ifile);
		TFile *fin = TFile::Open(fpathname, "READ");
		if(fin == nullptr || fin->IsZombie()){
			cerr<<"ERR READ_ALL_EFF ===== failed to open: "<<fpathname<<endl;
			delete fin;
			DELETE_ALL_EFF(veff);
			return false;
		}

		TString effname = GET_EFFNAME_FROM_FPATHNAME(fpathname);
		// if(effname == "chi2eff") continue;
		// if(effname == "ecaleff") continue;
		// if(effname == "matcheff") continue;
		// if(effname == "ntrkeff") continue;
		// if(effname == "pateff") continue;
		// if(effname == "qineff") continue;
		// if(effname == "tofeff") continue;
		// if(effname == "totaleff") continue;
		// if(effname == "trdeff") continue;
		if(effname == "trkeff") continue;
		TH1D *hratio_in = dynamic_cast<TH1D *>(fin->Get("hratio"));
		TGraph *gratio_conf_band_in = dynamic_cast<TGraph *>(fin->Get("gratio_conf_band"));
		TF1 *spfit_ratio_in = dynamic_cast<TF1 *>(fin->Get("spfit_ratio"));
		if(hratio_in == nullptr || gratio_conf_band_in == nullptr || spfit_ratio_in == nullptr){
			cerr<<"ERR READ_ALL_EFF ===== missing object in "<<fpathname
				<<" hratio="<<(hratio_in != nullptr)
				<<" conf_band="<<(gratio_conf_band_in != nullptr)
				<<" spfit_ratio="<<(spfit_ratio_in != nullptr)
				<<endl;
			fin->Close();
			delete fin;
			DELETE_ALL_EFF(veff);
			return false;
		}

		VarEffItem item{};
		item.effname = effname;
		item.efflabel = GET_EFFLABEL(effname);
		item.fpathname = fpathname;
		item.color = GET_EFFCOLOR(effname);
		item.hratio = dynamic_cast<TH1D *>(hratio_in->Clone(Form("hratio_%s", effname.Data())));
		item.hratio->SetDirectory(nullptr);
		item.gratio_conf_band = dynamic_cast<TGraph *>(gratio_conf_band_in->Clone(Form("gratio_conf_band_%s", effname.Data())));
		item.spfit_ratio = dynamic_cast<TF1 *>(spfit_ratio_in->Clone(Form("spfit_ratio_%s", effname.Data())));
		item.xmin = 0.0;
		item.xmax = 0.0;
		item.ymin = 0.0;
		item.ymax = 0.0;

		bool item_first = true;
		UPDATE_RANGE_BY_HIST(item.hratio, item.xmin, item.xmax, item.ymin, item.ymax, item_first);
		UPDATE_RANGE_BY_GRAPH(item.gratio_conf_band, item.xmin, item.xmax, item.ymin, item.ymax, item_first);
		if(item_first){
			cerr<<"ERR READ_ALL_EFF ===== empty graph in "<<fpathname<<endl;
			fin->Close();
			delete fin;
			delete item.hratio;
			delete item.gratio_conf_band;
			delete item.spfit_ratio;
			DELETE_ALL_EFF(veff);
			return false;
		}

		if(is_first){
			xall_min = item.xmin;
			xall_max = item.xmax;
			yall_min = item.ymin;
			yall_max = item.ymax;
			is_first = false;
		}
		else{
			xall_min = std::min(xall_min, item.xmin);
			xall_max = std::max(xall_max, item.xmax);
			yall_min = std::min(yall_min, item.ymin);
			yall_max = std::max(yall_max, item.ymax);
		}

		veff.push_back(item);
		cout<<"IN READ_ALL_EFF ===== eff="<<item.effname
			<<" label="<<item.efflabel
			<<" fpathname="<<item.fpathname
			<<" n_bin="<<item.hratio->GetNbinsX()
			<<endl;

		fin->Close();
		delete fin;
	}

	sort(veff.begin(), veff.end(),
			[](const VarEffItem &a, const VarEffItem &b){
				int oa = GET_EFFORDER(a.effname);
				int ob = GET_EFFORDER(b.effname);
				if(oa != ob) return oa < ob;
				return string(a.effname.Data()) < string(b.effname.Data());
			});

	if(is_first){
		cerr<<"ERR READ_ALL_EFF ===== no valid efficiency item"<<endl;
		return false;
	}
	return true;
}


//==================================================== DRAW_
bool DRAW_COMBINE(const VarConf &var, const vector<VarEffItem> &veff,
		double xall_min, double xall_max, double yall_min, double yall_max){
	TFile *fout = TFile::Open(var.foutname_root, "RECREATE");
	if(fout == nullptr || fout->IsZombie()){
		cerr<<"ERR DRAW_COMBINE ===== failed to open output root: "<<var.foutname_root<<endl;
		delete fout;
		return false;
	}

	double xdrawmin = var.has_xmin ? var.xmin : xall_min;
	double xdrawmax = var.has_xmax ? var.xmax : xall_max;
	double ydrawmin = var.has_ymin ? var.ymin : yall_min;
	double ydrawmax = var.has_ymax ? var.ymax : yall_max;
	double yspan = ydrawmax - ydrawmin;
	if(yspan <= 0.0) yspan = std::max(0.2, fabs(ydrawmax) * 0.2);
	if(!var.has_ymin) ydrawmin -= yspan * 0.15;
	if(!var.has_ymax) ydrawmax += yspan * 0.18;
	ydrawmin = std::min(ydrawmin, 1.0 - 0.02);
	ydrawmax = std::max(ydrawmax, 1.0 + 0.02);

	TCanvas *c = new TCanvas("c", "c", 1200, 700);
	c->SetTopMargin(0.12);
	c->SetBottomMargin(0.14);
	c->SetLeftMargin(0.11);
	c->SetRightMargin(0.05);
	c->cd();
	gPad->SetLogx();
	gPad->SetGridx();
	gPad->SetGridy();

	TH1D *hframe = new TH1D("hframe", "hframe", nenebin, energy_bins);
	hframe->SetStats(0);
	hframe->SetNameTitle("", "");
	hframe->GetXaxis()->SetRangeUser(xdrawmin, xdrawmax);
	hframe->GetYaxis()->SetRangeUser(ydrawmin, ydrawmax);
	hframe->GetXaxis()->SetTitle("Energy [GeV]");
	hframe->GetYaxis()->SetTitle("ISS / MC Efficiency Ratio");
	hframe->GetXaxis()->CenterTitle();
	hframe->GetYaxis()->CenterTitle();
	hframe->GetXaxis()->SetTitleFont(62);
	hframe->GetYaxis()->SetTitleFont(62);
	hframe->GetXaxis()->SetTitleSize(0.05);
	hframe->GetYaxis()->SetTitleSize(0.05);
	hframe->GetXaxis()->SetTitleOffset(1.1);
	hframe->GetYaxis()->SetTitleOffset(0.95);
	hframe->GetXaxis()->SetLabelOffset(0.012);
	hframe->GetYaxis()->SetLabelOffset(0.010);
	hframe->Draw("AXIS");
	hframe->Draw("AXIG SAME");

	TLine *line_ratio_unity = new TLine(xdrawmin, 1.0, xdrawmax, 1.0);
	line_ratio_unity->SetLineColor(kGray + 2);
	line_ratio_unity->SetLineStyle(2);
	line_ratio_unity->SetLineWidth(2);
	line_ratio_unity->Draw("same");

	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);
	for(size_t i=0; i<veff.size(); i++){
		TGraph *band = veff[i].gratio_conf_band;
		band->SetName(Form("gratio_conf_band_%s", veff[i].effname.Data()));
		band->SetFillColor(veff[i].color);
		band->SetFillStyle(3002);
		band->SetLineColor(veff[i].color);
		band->SetLineWidth(2);
		band->Draw("F same");
	}

	for(size_t i=0; i<veff.size(); i++){
		TF1 *spfit = veff[i].spfit_ratio;
		spfit->SetName(Form("spfit_ratio_%s", veff[i].effname.Data()));
		spfit->SetLineColor(veff[i].color);
		spfit->SetMarkerColor(veff[i].color);
		// spfit->SetLineWidth(3);
		spfit->SetLineWidth(1);
		spfit->SetLineStyle(1);
		spfit->Draw("L same");
	}

	for(size_t i=0; i<veff.size(); i++){
		TH1D *hratio = veff[i].hratio;
		hratio->SetName(Form("hratio_%s", veff[i].effname.Data()));
		hratio->SetMarkerStyle(20);
		hratio->SetMarkerSize(0.9);
		hratio->SetMarkerColor(veff[i].color);
		hratio->SetLineColor(veff[i].color);
		hratio->SetFillColor(veff[i].color);
		hratio->SetLineWidth(2);
		hratio->SetStats(0);
		hratio->Draw("E1X0P same");
	}

	TLegend *leg = new TLegend(0.26, 0.18, 0.74, 0.32);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->SetNColumns(2);
	for(size_t i=0; i<veff.size(); i++){
		leg->AddEntry(veff[i].hratio, veff[i].efflabel, "lp");
	}
	leg->Draw();

	// TLatex latex;
	// latex.SetNDC();
	// latex.SetTextFont(62);
	// latex.SetTextSize(0.035);
	// latex.DrawLatex(0.50, 0.95, "Combined Efficiency Ratio with Spline Fit");

	c->SaveAs(var.foutname_pdf);

	fout->cd();
	hframe->Write("hframe");
	line_ratio_unity->Write("line_ratio_unity");
	leg->Write("leg");
	for(size_t i=0; i<veff.size(); i++){
		veff[i].hratio->Write(Form("hratio_%s", veff[i].effname.Data()));
		veff[i].gratio_conf_band->Write(Form("gratio_conf_band_%s", veff[i].effname.Data()));
		veff[i].spfit_ratio->Write(Form("spfit_ratio_%s", veff[i].effname.Data()));
	}
	c->Write("c");
	fout->Write();
	fout->Close();
	delete fout;

	cout<<"IN DRAW_COMBINE ===== output root: "<<var.foutname_root<<endl;
	cout<<"IN DRAW_COMBINE ===== output pdf : "<<var.foutname_pdf<<endl;
	cout<<"IN DRAW_COMBINE ===== n_eff="<<veff.size()<<endl;
	return true;
}


//==================================================== FIT_RUN_
int ADD_EDEP(int argc, char *argv[]){
	VarConf var{};
	vector<VarEffItem> veff;
	double xall_min = 0.0;
	double xall_max = 0.0;
	double yall_min = 0.0;
	double yall_max = 0.0;

	INIT(argc, argv, var);
	if(!READ_ALL_EFF(var, veff, xall_min, xall_max, yall_min, yall_max)){
		DELETE_ALL_EFF(veff);
		return 1;
	}
	if(!DRAW_COMBINE(var, veff, xall_min, xall_max, yall_min, yall_max)){
		DELETE_ALL_EFF(veff);
		return 2;
	}

	DELETE_ALL_EFF(veff);
	return 0;
}

int main(int argc, char *argv[]){
	return ADD_EDEP(argc, argv);
}
