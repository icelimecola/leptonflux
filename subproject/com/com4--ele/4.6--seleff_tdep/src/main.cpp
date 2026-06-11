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
#include "TH2.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"

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

TString BUILD_GCX_PATH(const TString &eff_tag, int itag){
	TString fpath = Form("datain/seleff_gcx/%s/htime_tfit%02d_%sGeV_%s.root",
			eff_tag.Data(), itag, FORMAT_ENE(energy_bins[itag]).Data(), eff_tag.Data());
	return fpath;
}

bool HAS_NONZERO_HIST(TH1 *h){
	if(h == nullptr) return false;
	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		if(h->GetBinContent(ibin) != 0.0) return true;
		if(h->GetBinError(ibin) != 0.0) return true;
	}
	return false;
}

int FIND_LAST_NONZERO_TSU_ENEBIN(TH2 *h2_tsu){
	if(h2_tsu == nullptr) return 0;

	int imax = min(nenebin, h2_tsu->GetNbinsY());
	for(int itag=imax; itag>=1; itag--){
		for(int ibin=1; ibin<=h2_tsu->GetNbinsX(); ibin++){
			if(h2_tsu->GetBinContent(ibin, itag) != 0.0) return itag;
			if(h2_tsu->GetBinError(ibin, itag) != 0.0) return itag;
		}
	}

	return 0;
}

int FIND_LAST_NONZERO_GCX_ENEBIN(const TString &eff_tag){
	for(int itag=nenebin; itag>=1; itag--){
		TString fpath = BUILD_GCX_PATH(eff_tag, itag);
		if(gSystem->AccessPathName(fpath)) continue;

		TFile *f_gcx = new TFile(fpath, "read");
		if(f_gcx == nullptr || f_gcx->IsZombie()){
			if(f_gcx != nullptr){
				f_gcx->Close();
				delete f_gcx;
			}
			continue;
		}

		TH1D *h_gcx = dynamic_cast<TH1D*>(f_gcx->Get("hratio"));
		bool is_nonzero = HAS_NONZERO_HIST(h_gcx);
		f_gcx->Close();
		delete f_gcx;

		if(is_nonzero) return itag;
	}

	return 0;
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
		hratio->SetBinContent(rbin, y_gcx / y_tsu);
		hratio->SetBinError(rbin, 0.0);
	}

	return hratio;
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

TH1D *BUILD_TSU_PROJECTION(TH2 *h2_tsu, int itag, const TString &eff_tag, const TString &eff_label){
	TH1D *h_tsu = dynamic_cast<TH1D*>(h2_tsu->ProjectionX(
				Form("htsu_%s_raw_ene%02d", eff_tag.Data(), itag), itag, itag));
	h_tsu->SetDirectory(nullptr);
	h_tsu->SetStats(0);
	h_tsu->SetTitle(Form("%s efficiency, %g to %g GeV",
				eff_label.Data(), energy_bins[itag], energy_bins[itag + 1]));
	return h_tsu;
}

TH1D *BUILD_ON_REF_TIME_AXIS(TH1 *h_ref, TH1 *h_src, const TString &hname){
	int nbin = h_ref->GetNbinsX();
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(h_ref->GetXaxis()->GetBinLowEdge(ibin));
	}

	TH1D *h_new = new TH1D(hname, h_src->GetTitle(), nbin, &xbins[0]);
	h_new->SetDirectory(nullptr);
	h_new->SetStats(0);

	int nmatch = 0;
	for(int ibin=1; ibin<=nbin; ibin++){
		double x = h_ref->GetBinCenter(ibin);
		int jbin = h_src->GetXaxis()->FindFixBin(x);
		if(jbin < 1 || jbin > h_src->GetNbinsX()) continue;
		h_new->SetBinContent(ibin, h_src->GetBinContent(jbin));
		h_new->SetBinError(ibin, h_src->GetBinError(jbin));
		nmatch++;

		// Previous overlap weighted-average version kept for comparison:
		// double xlow = h_ref->GetXaxis()->GetBinLowEdge(ibin);
		// double xup = h_ref->GetXaxis()->GetBinUpEdge(ibin);
		// int jbin_min = h_src->GetXaxis()->FindFixBin(xlow);
		// int jbin_max = h_src->GetXaxis()->FindFixBin(xup);
		// if(jbin_min < 1) jbin_min = 1;
		// if(jbin_max > h_src->GetNbinsX()) jbin_max = h_src->GetNbinsX();
		//
		// double y = 0.0;
		// double err2 = 0.0;
		// double sum_frac = 0.0;
		// for(int jbin=jbin_min; jbin<=jbin_max; jbin++){
		// 	double tlow = h_src->GetXaxis()->GetBinLowEdge(jbin);
		// 	double tup = h_src->GetXaxis()->GetBinUpEdge(jbin);
		// 	double overlap = min(xup, tup) - max(xlow, tlow);
		// 	if(overlap <= 0.0) continue;
		//
		// 	double frac = overlap / (tup - tlow);
		// 	y += h_src->GetBinContent(jbin) * frac;
		// 	err2 += pow(h_src->GetBinError(jbin) * frac, 2);
		// 	sum_frac += frac;
		// }
		//
		// if(sum_frac <= 0.0) continue;
		// h_new->SetBinContent(ibin, y / sum_frac);
		// h_new->SetBinError(ibin, sqrt(err2) / sum_frac);
	}

	cout<<"======== align time axis ======== "<<endl
		<<"hname="<<hname<<endl
		<<"method=same-day center match"<<endl
		<<"src_xmin="<<Form("%.0f", h_src->GetXaxis()->GetXmin())<<endl
		<<"ref_xmin="<<Form("%.0f", h_ref->GetXaxis()->GetXmin())<<endl
		<<"nmatch="<<nmatch<<endl
		<<endl;
	return h_new;
}

bool FIND_COMMON_RANGE(TH1 *h_gcx, TH1 *h_tsu, double &xmin, double &xmax){
	xmin = max(h_gcx->GetXaxis()->GetXmin(), h_tsu->GetXaxis()->GetXmin());
	xmax = min(h_gcx->GetXaxis()->GetXmax(), h_tsu->GetXaxis()->GetXmax());

	bool is_find_xmin = false;
	bool is_find_xmax = false;
	for(int ibin=1; ibin<=h_gcx->GetNbinsX(); ibin++){
		double x = h_gcx->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		if(h_gcx->GetBinContent(ibin) == 0.0) continue;
		if(h_tsu->GetBinContent(ibin) == 0.0) continue;
		xmin = x;
		is_find_xmin = true;
		break;
	}
	for(int ibin=h_gcx->GetNbinsX(); ibin>=1; ibin--){
		double x = h_gcx->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		if(h_gcx->GetBinContent(ibin) == 0.0) continue;
		if(h_tsu->GetBinContent(ibin) == 0.0) continue;
		xmax = x;
		is_find_xmax = true;
		break;
	}

	return is_find_xmin && is_find_xmax && xmin < xmax;
}

bool DRAW_PAIR(TFile *fout, TH1 *h_gcx_in, TH1 *h_tsu_in,
		int itag, const TString &eff_tag, const TString &eff_label, const TString &outdir){
	TH1 *h_gcx = dynamic_cast<TH1*>(h_gcx_in->Clone(Form("hgcx_%s_ene%02d", eff_tag.Data(), itag)));
	TH1 *h_tsu = dynamic_cast<TH1*>(h_tsu_in->Clone(Form("htsu_%s_draw_ene%02d", eff_tag.Data(), itag)));
	h_gcx->SetDirectory(nullptr);
	h_tsu->SetDirectory(nullptr);
	h_gcx->SetTitle(Form("%g to %g GeV",
				energy_bins[itag], energy_bins[itag + 1]));

	double xmin = 0.0;
	double xmax = 0.0;
	if(!FIND_COMMON_RANGE(h_gcx, h_tsu, xmin, xmax)){
		cerr<<"ERR DRAW_PAIR ===== no common nonzero x range for ene"<<Form("%02d", itag)<<endl;
		delete h_gcx;
		delete h_tsu;
		return false;
	}

	TH1D *hratio = BUILD_RATIO(h_gcx, h_tsu, Form("hratio_%s_ene%02d", eff_tag.Data(), itag), xmin, xmax);

	TCanvas *c = new TCanvas(Form("c_%s_ene%02d", eff_tag.Data(), itag),
			Form("c_%s_ene%02d", eff_tag.Data(), itag), 1100, 720);
	TPad *pad_top = new TPad(Form("pad_top_%s_ene%02d", eff_tag.Data(), itag), "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad(Form("pad_ratio_%s_ene%02d", eff_tag.Data(), itag), "", 0.0, 0.0, 1.0, 0.32);

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
	h_tsu->GetYaxis()->SetTitle(eff_label + " efficiency");
	h_tsu->GetYaxis()->CenterTitle();
	h_tsu->GetYaxis()->SetTitleFont(62);
	h_tsu->GetYaxis()->SetTitleSize(0.052);
	h_tsu->GetYaxis()->SetTitleOffset(0.82);
	h_tsu->GetYaxis()->SetLabelOffset(0.012);

	double hmax = max(GET_HMAX(h_gcx, xmin, xmax), GET_HMAX(h_tsu, xmin, xmax));
	h_tsu->SetMinimum(0.8);
	h_tsu->SetMaximum(hmax > 0.0 ? hmax * 1.18 : 1.0);

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
	hratio->SetMinimum(0.95);
	hratio->SetMaximum(1.05);
	STYLE_HIST(hratio, kBlack, 20);

	TBox *box5 = new TBox(xmin, 0.97, xmax, 1.03);
	TBox *box3 = new TBox(xmin, 0.99, xmax, 1.01);
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
	TString foutpdf = Form("%s/%s_compare_ene%02d.pdf", outdir.Data(), eff_tag.Data(), itag);
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

	cout<<"IN DRAW_PAIR ===== eff="<<eff_tag
		<<" ene"<<Form("%02d", itag)
		<<" x=["<<xmin<<","<<xmax<<"]"
		<<" out="<<foutpdf
		<<endl;
	return true;
}

int main(){
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	gStyle->SetTimeOffset(0);
	TGaxis::SetMaxDigits(3);

	vector<TString> eff_tags = {"trdeff", "ntrkeff", "ecaleff", "matcheff"};
	vector<TString> eff_labels = {"TRD", "NTRK", "ECAL", "MATCH"};
	// vector<TString> tsu_files = {"mtrd.root", "ntrk.root", "ecal.root", "match.root"};
	vector<TString> tsu_files = {"trd.root", "ntrk.root", "ecal.root", "match.root"};
	// vector<TString> tsu_hnames = {"h2d_corr_mTRD", "h2d_corr_NTRK", "h2d_corr_ECAL", "h2d_corr_MATCH"};
	vector<TString> tsu_hnames = {"h2d_corr_TRD", "h2d_corr_NTRK", "h2d_corr_ECAL", "h2d_corr_MATCH"};

	for(size_t ieff=0; ieff<eff_tags.size(); ieff++){
		TString eff_tag = eff_tags.at(ieff);
		TString eff_label = eff_labels.at(ieff);
		TString fpath_tsu = "datain/seleff_tsu/" + tsu_files.at(ieff);
		TString outdir = "dataout/" + eff_tag;

		TFile *f_tsu = new TFile(fpath_tsu, "read");
		TFile *fout = new TFile(outdir + "/" + eff_tag + "_compare.root", "recreate");

		TH2 *h2_tsu = dynamic_cast<TH2*>(f_tsu->Get(tsu_hnames.at(ieff)));
		if(h2_tsu == nullptr){
			cerr<<"ERR MAIN ===== missing tsu hist"
				<<" eff="<<eff_tag
				<<" hname="<<tsu_hnames.at(ieff)
				<<endl;
			return 1;
		}

		cout<<"IN MAIN ===== eff="<<eff_tag<<endl
			<<"IN MAIN ===== input tsu: "<<fpath_tsu<<endl
			<<"IN MAIN ===== tsu hist: "<<h2_tsu->GetName()
			<<" class="<<h2_tsu->ClassName()
			<<" nx="<<h2_tsu->GetNbinsX()
			<<" ny="<<h2_tsu->GetNbinsY()<<endl
			<<endl;

		int last_tsu_ene = FIND_LAST_NONZERO_TSU_ENEBIN(h2_tsu);
		int last_gcx_ene = FIND_LAST_NONZERO_GCX_ENEBIN(eff_tag);
		int last_ene = min(nenebin, min(last_tsu_ene, last_gcx_ene));
		if(last_ene <= 0){
			cerr<<"WARN MAIN ===== skip eff without common nonzero ene bin"
				<<" eff="<<eff_tag
				<<" last_tsu_ene="<<last_tsu_ene
				<<" last_gcx_ene="<<last_gcx_ene
				<<endl;
			fout->Close();
			f_tsu->Close();
			continue;
		}

		cout<<"IN MAIN ===== ene loop"
			<<" eff="<<eff_tag
			<<" last_tsu_ene="<<last_tsu_ene
			<<" last_gcx_ene="<<last_gcx_ene
			<<" last_ene="<<last_ene
			<<endl
			<<endl;

		for(int itag=1; itag<=last_ene; itag++){
			TString fpath_gcx = BUILD_GCX_PATH(eff_tag, itag);
			TFile *f_gcx = new TFile(fpath_gcx, "read");
			TH1D *h_gcx = dynamic_cast<TH1D*>(f_gcx->Get("hratio"));
			if(h_gcx == nullptr){
				cerr<<"ERR MAIN ===== missing gcx hratio"
					<<" eff="<<eff_tag
					<<" itag="<<itag
					<<" fpath="<<fpath_gcx
					<<endl;
				return 2;
			}

			TH1D *h_tsu_raw = BUILD_TSU_PROJECTION(h2_tsu, itag, eff_tag, eff_label);
			TH1D *h_tsu = BUILD_ON_REF_TIME_AXIS(h_gcx, h_tsu_raw,
					Form("htsu_%s_aligned_ene%02d", eff_tag.Data(), itag));

			cout<<"IN MAIN ===== eff="<<eff_tag
				<<" ene"<<Form("%02d", itag)<<endl
				<<"IN MAIN ===== input gcx: "<<fpath_gcx
				<<" hist=hratio"
				<<" nb="<<h_gcx->GetNbinsX()
				<<" x=["<<Form("%.0f", h_gcx->GetXaxis()->GetXmin())
				<<","<<Form("%.0f", h_gcx->GetXaxis()->GetXmax())<<"]"<<endl
				<<endl;

			if(!DRAW_PAIR(fout, h_gcx, h_tsu, itag, eff_tag, eff_label, outdir)){
				delete h_tsu_raw;
				delete h_tsu;
				f_gcx->Close();
				return 3;
			}

			delete h_tsu_raw;
			delete h_tsu;
			f_gcx->Close();
		}

		fout->Write();
		fout->Close();
		f_tsu->Close();

		cout<<"IN MAIN ===== output root: "<<outdir + "/" + eff_tag + "_compare.root"<<endl;
	}
	return 0;
}
