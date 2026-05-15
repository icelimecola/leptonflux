#include <iostream>
using namespace std;

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <glob.h>
#include <set>
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
#include "TLegend.h"

//====================================================
const int NENEBIN = 54;
const double ENERGY_BINS[NENEBIN + 1] = {
	0.80, 1, 1.16, 1.33, 1.51,
	1.71, 1.92, 2.15, 2.40, 2.67,
	2.97, 3.29, 3.64, 4.02, 4.43,
	4.88, 5.37, 5.90, 6.47, 7.09,
	7.76, 8.48, 9.26, 10.10, 11,
	12, 13, 14.10, 15.30, 16.60,
	18, 19.50, 21.10, 22.80, 24.70,
	26.70, 28.80, 31.10, 33.50, 36.10,
	38.90, 41.90, 45.10, 48.50, 52.20,
	56.10, 60.30, 64.80, 69.70, 74.90,
	80.50, 86.50, 93, 100, 108
};

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
	double elow;
	double eup;
	double y;
	double yerr;
	TString fpathname;
};

struct VarDataSeries{
	vector<VarDataEntry> vdata;
	int n_entry;
};


//==================================================== TOOL_
bool IS_WILDCARD_PATTERN(const TString &pattern){
	return pattern.Contains("*") || pattern.Contains("?") || pattern.Contains("[");
}

TString NORMALIZE_PATTERN(const TString &pattern){
	if(pattern.Length() == 0) return pattern;
	if(IS_WILDCARD_PATTERN(pattern)) return pattern;

	std::filesystem::path path(pattern.Data());
	if(std::filesystem::is_directory(path)){
		return Form("%s/*.root", pattern.Strip(TString::kTrailing, '/').Data());
	}
	if(pattern.EndsWith(".root")) return pattern;
	if(std::filesystem::exists(std::filesystem::path((pattern + ".root").Data()))){
		return pattern + ".root";
	}
	return pattern + "*.root";
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

bool MATCH_MC_FPATHNAME(const TString &fpathname_iss, TString &fpathname_mc){
	fpathname_mc = fpathname_iss;
	if(!fpathname_mc.Contains("dataiss/tfit")){
		cerr<<"ERR MATCH_MC_FPATHNAME ===== cannot map iss to mc: "<<fpathname_iss<<endl;
		return false;
	}
	fpathname_mc.ReplaceAll("dataiss/tfit", "datamc/tfit");
	return true;
}

bool CHECK_EMATCH(const VarDataEntry &iss, const VarDataEntry &mc){
	double eps = 1.0e-9;
	if(fabs(iss.elow - mc.elow) > eps || fabs(iss.eup - mc.eup) > eps){
		cerr<<"ERR CHECK_EMATCH ===== ISS/MC energy mismatch"
			<<" iss=("<<iss.elow<<","<<iss.eup<<")"
			<<" mc=("<<mc.elow<<","<<mc.eup<<")"
			<<" iss_file="<<iss.fpathname
			<<" mc_file="<<mc.fpathname
			<<endl;
		return false;
	}
	return true;
}

int FIND_ENEBIN_INDEX(double elow, double eup){
	double eps = 1.0e-9;
	for(int i_enebin=0; i_enebin<NENEBIN; i_enebin++){
		if(fabs(ENERGY_BINS[i_enebin] - elow) < eps && fabs(ENERGY_BINS[i_enebin + 1] - eup) < eps){
			return i_enebin;
		}
	}
	return -1;
}

double GET_HMAX(const TH1D *h){
	double hmax = 0.0;
	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		double value = h->GetBinContent(ibin) + h->GetBinError(ibin);
		if(value > hmax) hmax = value;
	}
	return hmax;
}


//==================================================== INIT_
void INIT(int argc, char *argv[], VarConf &var){
	//====init var
	var.fpath = "datain/dataiss/tfit/";
	var.fname = "fitresult_enebin03_1.33GeV.root";
	var.fpathname = "";
	var.fpathname_mc = "";
	var.tname = "tfit";
	var.xname = "tmid";
	var.yname = "nele";
	var.yerrname = "nele_err";
	var.ytitle = "Electron Number";
	var.has_xmin = false;
	var.has_xmax = false;
	var.xmin = 0.80;
	var.xmax = 108.0;
	var.has_ymin = false;
	var.has_ymax = false;
	var.ymin = 0.0;
	var.ymax = 0.0;
	var.foutname = "hene";
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
	var.fpathname = NORMALIZE_PATTERN(var.fpathname);
	if(var.fpathname_mc.Length() > 0) var.fpathname_mc = NORMALIZE_PATTERN(var.fpathname_mc);
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


//==================================================== READ_
bool READ_TFIT(const VarConf &var, const TString &fpathname, VarDataEntry &entry){
	//====init var
	double y = 0.0;
	double yerr = 0.0;
	double elow = -1.0;
	double eup = -1.0;
	//====init chain
	TChain chain(var.tname);
	int n_file = chain.Add(fpathname);
	if(n_file != 1){
		cerr<<"ERR READ_TFIT ===== expect exactly 1 input file, now "<<n_file
			<<" pattern="<<fpathname
			<<endl;
		return false;
	}
	//====setbranch
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
	//====read
	int n_entry = chain.GetEntries();
	if(n_entry != 1){
		cerr<<"ERR READ_TFIT ===== entry number must be 1, now "<<n_entry
			<<" file="<<fpathname
			<<endl;
		return false;
	}
	chain.GetEntry(0);
	entry.elow = elow;
	entry.eup = eup;
	entry.y = y;
	entry.yerr = yerr;
	entry.fpathname = fpathname;
	cout<<"IN READ_TFIT ===== file="<<fpathname
		<<" elow="<<entry.elow
		<<" eup="<<entry.eup
		<<" y="<<entry.y
		<<" yerr="<<entry.yerr
		<<endl;
	return true;
}

bool BUILD_SERIES(const VarConf &var, VarDataSeries &iss, VarDataSeries &mc, TH1D *hiss, TH1D *hmc){
	vector<TString> vfile_iss;
	vector<bool> is_filled_iss(NENEBIN, false);
	vector<bool> is_filled_mc(NENEBIN, false);

	if(!GET_FILE_LIST(var.fpathname, vfile_iss)) return false;

	iss.vdata.clear();
	mc.vdata.clear();
	iss.vdata.reserve(vfile_iss.size());
	mc.vdata.reserve(vfile_iss.size());

	for(size_t ifile=0; ifile<vfile_iss.size(); ifile++){
		const TString &fpathname_iss = vfile_iss.at(ifile);
		TString fpathname_mc;
		VarDataEntry entry_iss{};
		VarDataEntry entry_mc{};
		int i_enebin = -1;

		if(!MATCH_MC_FPATHNAME(fpathname_iss, fpathname_mc)) return false;
		if(!READ_TFIT(var, fpathname_iss, entry_iss)) return false;
		if(!READ_TFIT(var, fpathname_mc, entry_mc)) return false;
		if(!CHECK_EMATCH(entry_iss, entry_mc)) return false;

		i_enebin = FIND_ENEBIN_INDEX(entry_iss.elow, entry_iss.eup);
		if(i_enebin < 0){
			cerr<<"ERR BUILD_SERIES ===== cannot find energy bin"
				<<" elow="<<entry_iss.elow
				<<" eup="<<entry_iss.eup
				<<" file="<<fpathname_iss
				<<endl;
			return false;
		}
		if(is_filled_iss.at(i_enebin) || is_filled_mc.at(i_enebin)){
			cerr<<"ERR BUILD_SERIES ===== duplicate energy bin fill"
				<<" i_enebin="<<i_enebin
				<<" elow="<<entry_iss.elow
				<<" eup="<<entry_iss.eup
				<<" file="<<fpathname_iss
				<<endl;
			return false;
		}

		hiss->SetBinContent(i_enebin + 1, entry_iss.y);
		hiss->SetBinError(i_enebin + 1, entry_iss.yerr);
		hmc->SetBinContent(i_enebin + 1, entry_mc.y);
		hmc->SetBinError(i_enebin + 1, entry_mc.yerr);
		is_filled_iss.at(i_enebin) = true;
		is_filled_mc.at(i_enebin) = true;
		iss.vdata.push_back(entry_iss);
		mc.vdata.push_back(entry_mc);
	}

	for(int i_enebin=0; i_enebin<NENEBIN; i_enebin++){
		if(!is_filled_iss.at(i_enebin) || !is_filled_mc.at(i_enebin)){
			cerr<<"ERR BUILD_SERIES ===== missing energy bin"
				<<" i_enebin="<<i_enebin
				<<" elow="<<ENERGY_BINS[i_enebin]
				<<" eup="<<ENERGY_BINS[i_enebin + 1]
				<<endl;
			return false;
		}
	}

	iss.n_entry = iss.vdata.size();
	mc.n_entry = mc.vdata.size();
	cout<<"IN BUILD_SERIES ===== n_entry_iss="<<iss.n_entry
		<<" n_entry_mc="<<mc.n_entry
		<<endl;
	return true;
}


//==================================================== DRAW_
bool DRAW(const VarConf &var, const VarDataSeries &iss, const VarDataSeries &mc, TH1D *hiss, TH1D *hmc){
	//====init--output
	TFile *fout = TFile::Open(var.foutname_root, "RECREATE");
	if(fout == nullptr || fout->IsZombie()){
		cerr<<"ERR DRAW ===== failed to open output root: "<<var.foutname_root<<endl;
		return false;
	}
	hiss->Write();
	hmc->Write();
	//====init--canvas
	TCanvas *c = new TCanvas("c","c",1000,500);
	TAxis *xaxis = hiss->GetXaxis();
	TAxis *yaxis = hiss->GetYaxis();
	//====canvas
	hiss->SetStats(0);
	hmc->SetStats(0);
	hiss->SetNameTitle("", "");
	c->SetTopMargin(0.13);
	c->SetBottomMargin(0.15);
	c->SetLeftMargin(0.13);
	c->SetRightMargin(0.08);
	c->cd();
	gPad->SetGridx();
	gPad->SetGridy();
	//====x
	if(var.has_xmin || var.has_xmax){
		double xdrawmin = var.has_xmin ? var.xmin : ENERGY_BINS[0];
		double xdrawmax = var.has_xmax ? var.xmax : ENERGY_BINS[NENEBIN];
		xaxis->SetRangeUser(xdrawmin, xdrawmax);
	}
	xaxis->SetNameTitle("Energy [GeV]", "Energy [GeV]");
	xaxis->CenterTitle();
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(0.05);
	xaxis->SetTitleOffset(1.2);
	xaxis->SetLabelOffset(0.012);
	//====y
	yaxis->SetNameTitle("hene", var.ytitle);
	yaxis->CenterTitle();
	yaxis->SetTitleFont(62);
	yaxis->SetTitleSize(0.05);
	yaxis->SetTitleOffset(0.9);
	yaxis->SetLabelOffset(0.012);

	hiss->SetMarkerStyle(20);
	hiss->SetMarkerSize(0.9);
	hiss->SetMarkerColor(kBlue);
	hiss->SetLineColor(kBlue);
	hiss->SetLineWidth(2);

	hmc->SetMarkerStyle(24);
	hmc->SetMarkerSize(0.9);
	hmc->SetMarkerColor(kRed);
	hmc->SetLineColor(kRed);
	hmc->SetLineWidth(2);

	double hmax = std::max(GET_HMAX(hiss), GET_HMAX(hmc));
	if(var.has_ymin) hiss->SetMinimum(var.ymin);
	else hiss->SetMinimum(0.0);
	if(var.has_ymax) hiss->SetMaximum(var.ymax);
	else hiss->SetMaximum(hmax > 0.0 ? hmax * 1.25 : 1.0);

	//====draw
	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);
	hiss->Draw("E1P");
	hmc->Draw("E1P same");

	TLegend *leg = new TLegend(0.18, 0.20, 0.30, 0.34);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->AddEntry(hiss, "ISS", "ep");
	leg->AddEntry(hmc, "MC", "ep");
	leg->Draw();

	c->cd(0);
	TLatex latex;
	latex.SetNDC();
	latex.SetTextFont(62);
	latex.SetTextSize(0.033);
	latex.SetTextAlign(22);
	latex.DrawLatex(0.5, 0.95, Form("Energy Dependence, nbin=%d", iss.n_entry));

	//====write
	c->SaveAs(var.foutname_pdf);
	fout->cd();
	c->Write();
	leg->Write("leg");
	delete leg;
	delete c;
	fout->Write();
	fout->Close();
	delete fout;
	//====print
	cout<<"IN DRAW ===== output root: "<<var.foutname_root<<endl;
	cout<<"IN DRAW ===== output pdf : "<<var.foutname_pdf<<endl;
	cout<<"IN DRAW ===== n_entry_iss="<<iss.n_entry
		<<" n_entry_mc="<<mc.n_entry
		<<endl;
	return true;
}


//==================================================== FIT_RUN_
int ADD_EDEP(int argc, char *argv[]){
	VarConf var{};
	VarDataSeries iss{};
	VarDataSeries mc{};
	INIT(argc, argv, var);

	TH1D *hene_iss = new TH1D("hene_iss", "hene_iss", NENEBIN, ENERGY_BINS);
	TH1D *hene_mc = new TH1D("hene_mc", "hene_mc", NENEBIN, ENERGY_BINS);

	if(!BUILD_SERIES(var, iss, mc, hene_iss, hene_mc)){
		delete hene_iss;
		delete hene_mc;
		return 1;
	}
	if(!DRAW(var, iss, mc, hene_iss, hene_mc)){
		delete hene_iss;
		delete hene_mc;
		return 2;
	}

	delete hene_iss;
	delete hene_mc;
	return 0;
}


//==================================================== main
int main(int argc, char *argv[]){
	return ADD_EDEP(argc, argv);
}
