#include <iostream>
using namespace std;

#include <algorithm>
#include <cmath>
#include <vector>

#include "TBox.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"

//----positron
// static const int nenebin = 29;
// static const double energy_bins[nenebin + 1] = {
// 	0.80, 1.00, 1.16, 1.33, 1.51,
// 	1.71, 1.92, 2.15, 2.40, 2.67,
// 	2.97, 3.29, 3.64, 4.02, 4.43,
// 	4.88, 5.37, 5.90, 6.47, 7.09,
// 	7.76, 8.48, 9.26, 10.10, 11.0,
// 	13.0, 16.6, 22.8, 41.9, 45.10
// };
// ----electron
static const int nenebin = 42;
static const double energy_bins[nenebin + 1] = {
	0.80, 1, 1.16, 1.33, 1.51,
	1.71, 1.92, 2.15, 2.40, 2.67,
	2.97, 3.29, 3.64, 4.02, 4.43,
	4.88, 5.37, 5.90, 6.47, 7.09,
	7.76, 8.48, 9.26, 10.10, 11,
	12, 13, 14.10, 15.30, 16.60,
	18, 19.50, 21.10, 22.80, 24.70,
	26.70, 28.80, 31.10, 33.50, 36.10,
	38.90, 41.90, 45.10
};

TString FORMAT_ENE(double x){
	TString s = Form("%g", x);
	return s;
}

double GET_HMAX(TH1 *h, double xmin, double xmax){
	double hmax = 0.0;

	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		double x = h->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		double y = h->GetBinContent(ibin) + h->GetBinError(ibin);
		if(y > hmax) hmax = y;
	}

	return hmax;
}

TH1D *BUILD_RATIO(TH1 *h_gcx, TH1 *h_tsu, const TString &hname, double xmin, double xmax){
	TAxis *xaxis = h_gcx->GetXaxis();
	int ibin_min = xaxis->FindFixBin(xmin);
	int ibin_max = xaxis->FindFixBin(xmax);
	if(ibin_min < 1) ibin_min = 1;
	if(ibin_max > h_gcx->GetNbinsX()) ibin_max = h_gcx->GetNbinsX();

	vector<double> xbins;
	for(int ibin=ibin_min; ibin<=ibin_max + 1; ibin++){
		xbins.push_back(xaxis->GetBinLowEdge(ibin));
	}

	TH1D *hratio = new TH1D(hname, hname, xbins.size() - 1, &xbins[0]);
	hratio->SetDirectory(nullptr);
	hratio->SetStats(0);
	hratio->SetTitle("");

	for(int ibin=ibin_min; ibin<=ibin_max; ibin++){
		double y_gcx = h_gcx->GetBinContent(ibin);
		double y_tsu = h_tsu->GetBinContent(ibin);
		if(y_gcx == 0.0 || y_tsu == 0.0) continue;

		int rbin = ibin - ibin_min + 1;
		double ratio = y_gcx / y_tsu;
		hratio->SetBinContent(rbin, ratio);
		hratio->SetBinError(rbin, 0);

		// Previous version for unaligned TSU histograms:
		// double x = h_gcx->GetBinCenter(ibin);
		// int jbin = h_tsu->GetXaxis()->FindFixBin(x);
		// if(jbin < 1 || jbin > h_tsu->GetNbinsX()) continue;
		// double y_tsu = h_tsu->GetBinContent(jbin);
	}

	return hratio;
}

void BUILD_PAIR_MOVING_AVERAGE_EQUAL(TH1 *h_gcx, TH1 *h_tsu,
		TH1D *&h_gcx_ma, TH1D *&h_tsu_ma,
		const TString &hname_gcx, const TString &hname_tsu,
		double xmin, double xmax){
	int nbin = h_gcx->GetNbinsX();
	int nside = 7 * 27;
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(h_gcx->GetXaxis()->GetBinLowEdge(ibin));
	}

	h_gcx_ma = new TH1D(hname_gcx, h_gcx->GetTitle(), nbin, &xbins[0]);
	h_tsu_ma = new TH1D(hname_tsu, h_tsu->GetTitle(), nbin, &xbins[0]);
	h_gcx_ma->SetDirectory(nullptr);
	h_tsu_ma->SetDirectory(nullptr);
	h_gcx_ma->SetStats(0);
	h_tsu_ma->SetStats(0);

	for(int ibin=1; ibin<=nbin; ibin++){
		double x = h_gcx->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;

		int jmin = ibin - nside;
		int jmax = ibin + nside;
		if(jmin < 1) jmin = 1;
		if(jmax > nbin) jmax = nbin;

		double sum_gcx = 0.0;
		double sum_tsu = 0.0;
		int nsum = 0;
		for(int jbin=jmin; jbin<=jmax; jbin++){
			double xj = h_gcx->GetBinCenter(jbin);
			if(xj < xmin || xj > xmax) continue;
			double y_gcx = h_gcx->GetBinContent(jbin);
			double y_tsu = h_tsu->GetBinContent(jbin);
			if(y_gcx == 0.0 || y_tsu == 0.0) continue;
			sum_gcx += y_gcx;
			sum_tsu += y_tsu;
			nsum++;
		}
		if(nsum <= 0) continue;

		h_gcx_ma->SetBinContent(ibin, sum_gcx / nsum);
		h_tsu_ma->SetBinContent(ibin, sum_tsu / nsum);
		h_gcx_ma->SetBinError(ibin, 0.0);
		h_tsu_ma->SetBinError(ibin, 0.0);
	}

	cout<<"======== clac pair ma equal ======== "<<endl
		<<"hname_gcx="<<hname_gcx<<endl
		<<"hname_tsu="<<hname_tsu<<endl
		<<"nside="<<nside<<endl
		<<"nbin="<<nbin<<endl
		<<"xmin="<<xmin<<endl
		<<"xmax="<<xmax<<endl
		<<endl;
}

void STYLE_HIST(TH1 *h, int color, int mstyle){
	h->SetStats(0);
	h->SetMarkerStyle(mstyle);
	h->SetMarkerSize(0.55);
	h->SetMarkerColor(color);
	h->SetLineColor(color);
	h->SetLineWidth(1);
}

void STYLE_TIME_AXIS(TAxis *xaxis, double title_size, double label_size, double title_offset){
	xaxis->SetNameTitle("Date", "Date");
	xaxis->CenterTitle();
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(title_size);
	xaxis->SetTitleOffset(title_offset);
	xaxis->SetLabelSize(label_size);
	xaxis->SetLabelOffset(0.025);
	xaxis->SetTimeDisplay(1);
	xaxis->SetTimeFormat("%b/%d/%Y");
	xaxis->SetNdivisions(-505);
}

TH1D *BUILD_TSU_PROJECTION(TH2 *h2_tsu, int itag, const TString &species){
	TH1D *h_tsu = dynamic_cast<TH1D*>(h2_tsu->ProjectionX(Form("htsu_%s_ene%02d", species.Data(), itag), itag, itag));
	h_tsu->SetDirectory(nullptr);
	h_tsu->SetStats(0);
	h_tsu->SetTitle(Form("raw count, %g to %g GeV",
				energy_bins[itag], energy_bins[itag + 1]));
	for(int ibin=1; ibin<=h_tsu->GetNbinsX(); ibin++){
		double y = h_tsu->GetBinContent(ibin);
		h_tsu->SetBinError(ibin, y > 0.0 ? sqrt(y) : 0.0);
	}
	return h_tsu;
}

TH1D *BUILD_TSU_ON_GCX_TIME_AXIS(TH1 *h_gcx, TH1 *h_tsu, const TString &hname){
	int nbin = h_gcx->GetNbinsX();
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(h_gcx->GetXaxis()->GetBinLowEdge(ibin));
	}

	TH1D *h_aligned = new TH1D(hname, h_tsu->GetTitle(), nbin, &xbins[0]);
	h_aligned->SetDirectory(nullptr);
	h_aligned->SetStats(0);

	int nmatch = 0;
	for(int ibin=1; ibin<=nbin; ibin++){
		double x = h_gcx->GetBinCenter(ibin);
		int jbin = h_tsu->GetXaxis()->FindFixBin(x);
		if(jbin < 1 || jbin > h_tsu->GetNbinsX()) continue;
		h_aligned->SetBinContent(ibin, h_tsu->GetBinContent(jbin));
		h_aligned->SetBinError(ibin, h_tsu->GetBinError(jbin));
		nmatch++;

		// Previous overlap-rebin version kept for comparison:
		// double xlow = h_gcx->GetXaxis()->GetBinLowEdge(ibin);
		// double xup = h_gcx->GetXaxis()->GetBinUpEdge(ibin);
		// int jbin_min = h_tsu->GetXaxis()->FindFixBin(xlow);
		// int jbin_max = h_tsu->GetXaxis()->FindFixBin(xup);
		// if(jbin_min < 1) jbin_min = 1;
		// if(jbin_max > h_tsu->GetNbinsX()) jbin_max = h_tsu->GetNbinsX();
		//
		// double y = 0.0;
		// double err2 = 0.0;
		// double sum_frac = 0.0;
		// for(int jbin=jbin_min; jbin<=jbin_max; jbin++){
		// 	double tlow = h_tsu->GetXaxis()->GetBinLowEdge(jbin);
		// 	double tup = h_tsu->GetXaxis()->GetBinUpEdge(jbin);
		// 	double overlap = min(xup, tup) - max(xlow, tlow);
		// 	if(overlap <= 0.0) continue;
		//
		// 	double frac = overlap / (tup - tlow);
		// 	y += h_tsu->GetBinContent(jbin) * frac;
		// 	err2 += pow(h_tsu->GetBinError(jbin) * frac, 2);
		// 	sum_frac += frac;
		// }
		//
		// if(sum_frac <= 0.0) continue;
		// h_aligned->SetBinContent(ibin, y);
		// h_aligned->SetBinError(ibin, sqrt(err2));
	}

	cout<<"======== align tsu time axis ======== "<<endl
		<<"hname="<<hname<<endl
		<<"method=same-day center match"<<endl
		<<"tsu_xmin="<<Form("%.0f", h_tsu->GetXaxis()->GetXmin())<<endl
		<<"gcx_xmin="<<Form("%.0f", h_gcx->GetXaxis()->GetXmin())<<endl
		<<"nmatch="<<nmatch<<endl
		<<endl;
	return h_aligned;
}

bool FIND_COMMON_RANGE(TH1 *h_gcx, TH1 *h_tsu, double &xmin, double &xmax){
	xmin = max(h_gcx->GetXaxis()->GetXmin(), h_tsu->GetXaxis()->GetXmin());
	xmax = min(h_gcx->GetXaxis()->GetXmax(), h_tsu->GetXaxis()->GetXmax());

	bool is_find_xmin = false;
	bool is_find_xmax = false;
	for(int ibin=1; ibin<=h_gcx->GetNbinsX(); ibin++){
		double x = h_gcx->GetBinCenter(ibin);
		int jbin = h_tsu->GetXaxis()->FindFixBin(x);
		if(x < xmin || x > xmax) continue;
		if(jbin < 1 || jbin > h_tsu->GetNbinsX()) continue;
		if(h_gcx->GetBinContent(ibin) == 0.0) continue;
		if(h_tsu->GetBinContent(jbin) == 0.0) continue;
		xmin = x;
		is_find_xmin = true;
		break;
	}
	for(int ibin=h_gcx->GetNbinsX(); ibin>=1; ibin--){
		double x = h_gcx->GetBinCenter(ibin);
		int jbin = h_tsu->GetXaxis()->FindFixBin(x);
		if(x < xmin || x > xmax) continue;
		if(jbin < 1 || jbin > h_tsu->GetNbinsX()) continue;
		if(h_gcx->GetBinContent(ibin) == 0.0) continue;
		if(h_tsu->GetBinContent(jbin) == 0.0) continue;
		xmax = x;
		is_find_xmax = true;
		break;
	}

	return is_find_xmin && is_find_xmax && xmin < xmax;
}

bool DRAW_PAIR(TFile *fout, TH1 *h_gcx_in, TH1 *h_tsu_in, int itag, const TString &species, const TString &outdir){
	double xmin = 0.0;
	double xmax = 0.0;
	if(!FIND_COMMON_RANGE(h_gcx_in, h_tsu_in, xmin, xmax)){
		cerr<<"ERR DRAW_PAIR ===== no common nonzero x range"
			<<" species="<<species
			<<" ene"<<Form("%02d", itag)
			<<endl;
		return false;
	}

	// Previous raw comparison:
	// TH1 *h_gcx = dynamic_cast<TH1*>(h_gcx_in->Clone(Form("hgcx_%s_ene%02d", species.Data(), itag)));
	// TH1 *h_tsu = dynamic_cast<TH1*>(h_tsu_in->Clone(Form("htsu_draw_%s_ene%02d", species.Data(), itag)));
	TH1D *h_gcx = nullptr;
	TH1D *h_tsu = nullptr;
	BUILD_PAIR_MOVING_AVERAGE_EQUAL(h_gcx_in, h_tsu_in, h_gcx, h_tsu,
			Form("hgcx_ma_%s_ene%02d", species.Data(), itag),
			Form("htsu_ma_%s_ene%02d", species.Data(), itag),
			xmin, xmax);
	h_gcx->SetTitle(Form("%g to %g GeV",
				energy_bins[itag], energy_bins[itag + 1]));
	TH1D *hratio = BUILD_RATIO(h_gcx, h_tsu, Form("hratio_%s_ene%02d", species.Data(), itag), xmin, xmax);

	TCanvas *c = new TCanvas(Form("c_%s_ene%02d", species.Data(), itag), Form("c_%s_ene%02d", species.Data(), itag), 1100, 720);
	TPad *pad_top = new TPad(Form("pad_top_%s_ene%02d", species.Data(), itag), "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad(Form("pad_ratio_%s_ene%02d", species.Data(), itag), "", 0.0, 0.0, 1.0, 0.32);

	pad_top->SetTopMargin(0.11);
	pad_top->SetBottomMargin(0.03);
	pad_top->SetLeftMargin(0.12);
	pad_top->SetRightMargin(0.08);
	pad_ratio->SetTopMargin(0.03);
	pad_ratio->SetBottomMargin(0.32);
	pad_ratio->SetLeftMargin(0.12);
	pad_ratio->SetRightMargin(0.08);
	pad_top->Draw();
	pad_ratio->Draw();

	pad_top->cd();
	gPad->SetGridx();
	gPad->SetGridy();
	STYLE_HIST(h_gcx, kRed + 1, 20);
	STYLE_HIST(h_tsu, kBlue + 1, 24);

	h_tsu->GetXaxis()->SetRangeUser(xmin, xmax);
	h_gcx->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(h_tsu->GetXaxis(), 0.0, 0.0, 1.2);
	h_tsu->GetYaxis()->SetTitle("Raw count");
	h_tsu->GetYaxis()->CenterTitle();
	h_tsu->GetYaxis()->SetTitleFont(62);
	h_tsu->GetYaxis()->SetTitleSize(0.052);
	h_tsu->GetYaxis()->SetTitleOffset(0.82);
	h_tsu->GetYaxis()->SetLabelOffset(0.012);

	double hmax = max(GET_HMAX(h_gcx, xmin, xmax), GET_HMAX(h_tsu, xmin, xmax));
	h_tsu->SetMinimum(0.0);
	h_tsu->SetMaximum(hmax > 0.0 ? hmax * 1.25 : 1.0);

	h_tsu->Draw("E1X0P");
	h_gcx->Draw("E1X0P same");

	TLegend *leg = new TLegend(0.15, 0.74, 0.31, 0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->SetTextSize(0.04);
	leg->AddEntry(h_gcx, "GCX", "ep");
	leg->AddEntry(h_tsu, "TSU", "ep");
	leg->Draw();

	pad_ratio->cd();
	gPad->SetGridx();
	gPad->SetGridy();

	hratio->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(hratio->GetXaxis(), 0.11, 0.10, 1.1);

	hratio->GetYaxis()->SetTitle("GCX / TSU");
	hratio->GetYaxis()->CenterTitle();
	hratio->GetYaxis()->SetTitleFont(62);
	hratio->GetYaxis()->SetTitleSize(0.10);
	hratio->GetYaxis()->SetTitleOffset(0.48);
	hratio->GetYaxis()->SetLabelSize(0.085);
	hratio->GetYaxis()->SetLabelOffset(0.012);
	hratio->GetYaxis()->SetNdivisions(505);
	hratio->SetMinimum(0.9);
	hratio->SetMaximum(1.1);
	STYLE_HIST(hratio, kBlack, 20);

	TBox *box5 = new TBox(xmin, 0.95, xmax, 1.05);
	TBox *box3 = new TBox(xmin, 0.97, xmax, 1.03);
	box5->SetFillColorAlpha(kOrange - 3, 0.35);
	box5->SetLineColor(kOrange - 3);
	box3->SetFillColorAlpha(kGreen + 1, 0.35);
	box3->SetLineColor(kGreen + 1);

	TLine *line1 = new TLine(xmin, 1.0, xmax, 1.0);
	line1->SetLineColor(kGray + 2);
	line1->SetLineStyle(2);
	line1->SetLineWidth(2);

	hratio->Draw("E1X0P");
	box5->Draw("same");
	box3->Draw("same");
	line1->Draw("same");
	hratio->Draw("E1X0P same");

	c->cd();
	TString foutpdf = Form("%s/man_compare_ene%02d.pdf", outdir.Data(), itag);
	c->SaveAs(foutpdf);

	fout->cd();
	h_gcx->Write();
	h_tsu->Write();
	hratio->Write();
	c->Write();

	delete line1;
	delete box3;
	delete box5;
	delete leg;
	delete hratio;
	delete c;
	delete h_gcx;
	delete h_tsu;

	cout<<"IN DRAW_PAIR ===== species="<<species
		<<" ene"<<Form("%02d", itag)
		<<" x=["<<xmin<<","<<xmax<<"]"
		<<" out="<<foutpdf
		<<endl;
	return true;
}

TH1D *READ_GCX_HIST(int itag, const TString &species){
	TString fpath = Form("datain/n_gcx/%s/htime_tfit%02d_%sGeV_%s.root",
			species.Data(), itag, FORMAT_ENE(energy_bins[itag]).Data(), species.Data());
	TFile *f_gcx = new TFile(fpath, "read");
	TH1D *h_in = dynamic_cast<TH1D*>(f_gcx->Get("htime"));
	if(h_in == nullptr){
		cerr<<"ERR READ_GCX_HIST ===== missing htime"
			<<" fpath="<<fpath
			<<endl;
		f_gcx->Close();
		delete f_gcx;
		return nullptr;
	}
	TH1D *h = dynamic_cast<TH1D*>(h_in->Clone(Form("hgcx_src_%s_ene%02d", species.Data(), itag)));
	h->SetDirectory(nullptr);
	f_gcx->Close();
	delete f_gcx;
	return h;
}

bool DRAW_SPECIES(TFile *fout, TH2D *h2_tsu, const TString &species, const TString &outdir){
	for(int itag=1; itag<=nenebin - 1; itag++){
		TH1D *h_gcx = READ_GCX_HIST(itag, species);
		if(h_gcx == nullptr) return false;

		TH1D *h_tsu_raw = BUILD_TSU_PROJECTION(h2_tsu, itag, species);
		TH1D *h_tsu = BUILD_TSU_ON_GCX_TIME_AXIS(h_gcx, h_tsu_raw, Form("htsu_aligned_%s_ene%02d", species.Data(), itag));
		if(!DRAW_PAIR(fout, h_gcx, h_tsu, itag, species, outdir)){
			delete h_gcx;
			delete h_tsu_raw;
			delete h_tsu;
			return false;
		}
		delete h_gcx;
		delete h_tsu_raw;
		delete h_tsu;
	}

	return true;
}

int main(){
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	gStyle->SetTimeOffset(0);
	TGaxis::SetMaxDigits(3);

	TString fpath_tsu = "datain/n_tsu.root";
	TFile *f_tsu = new TFile(fpath_tsu, "read");

	TH2D *h2_tsu_ele = dynamic_cast<TH2D*>(f_tsu->Get("h2d_event_ele_raw_daily"));
	if(h2_tsu_ele == nullptr){
		cerr<<"ERR MAIN ===== missing tsu ele raw daily hist"<<endl;
		return 1;
	}

	cout<<"IN MAIN ===== input tsu: "<<fpath_tsu<<endl
		<<"IN MAIN ===== tsu ele hist: "<<h2_tsu_ele->GetName()
		<<" class="<<h2_tsu_ele->ClassName()
		<<" nx="<<h2_tsu_ele->GetNbinsX()
		<<" ny="<<h2_tsu_ele->GetNbinsY()<<endl
		<<endl;

	TFile *fout_ele = new TFile("dataout/nele/man_compare.root", "recreate");
	if(!DRAW_SPECIES(fout_ele, h2_tsu_ele, "nele", "dataout/nele")){
		fout_ele->Close();
		f_tsu->Close();
		return 2;
	}
	fout_ele->Write();
	fout_ele->Close();

	f_tsu->Close();

	cout<<"IN MAIN ===== output root: dataout/nele/man_compare.root"<<endl;
	return 0;
}
