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

bool HAS_NONZERO_HIST(TH1 *h){
	if(h == nullptr) return false;
	for(int ibin=1; ibin<=h->GetNbinsX(); ibin++){
		if(h->GetBinContent(ibin) != 0.0) return true;
		if(h->GetBinError(ibin) != 0.0) return true;
	}
	return false;
}

int FIND_LAST_NONZERO_TH2_ENEBIN(TH2 *h2){
	if(h2 == nullptr) return 0;

	int imax = min(nenebin, h2->GetNbinsY());
	for(int itag=imax; itag>=1; itag--){
		for(int ibin=1; ibin<=h2->GetNbinsX(); ibin++){
			if(h2->GetBinContent(ibin, itag) != 0.0) return itag;
			if(h2->GetBinError(ibin, itag) != 0.0) return itag;
		}
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

TH1D *BUILD_RATIO(TH1 *h_tsu, TH1 *h_prl, const TString &hname, double xmin, double xmax){
	TAxis *xaxis = h_tsu->GetXaxis();
	int ibin_min = xaxis->FindFixBin(xmin);
	int ibin_max = xaxis->FindFixBin(xmax);
	if(ibin_min < 1) ibin_min = 1;
	if(ibin_max > h_tsu->GetNbinsX()) ibin_max = h_tsu->GetNbinsX();

	vector<double> xbins;
	for(int ibin=ibin_min; ibin<=ibin_max + 1; ibin++){
		xbins.push_back(xaxis->GetBinLowEdge(ibin));
	}

	TH1D *hratio = new TH1D(hname, hname, xbins.size() - 1, &xbins[0]);
	hratio->SetDirectory(nullptr);
	hratio->SetStats(0);
	hratio->SetTitle("");

	for(int ibin=ibin_min; ibin<=ibin_max; ibin++){
		double y_tsu = h_tsu->GetBinContent(ibin);
		double y_prl = h_prl->GetBinContent(ibin);
		if(y_tsu == 0.0 || y_prl == 0.0) continue;

		int rbin = ibin - ibin_min + 1;
		hratio->SetBinContent(rbin, y_tsu / y_prl);
		hratio->SetBinError(rbin, 0.0);
	}

	return hratio;
}

void BUILD_PAIR_MOVING_AVERAGE_EXPS(TH1 *h_tsu, TH1 *h_prl,
		TH1 *h_tsu_exps, TH1 *h_prl_exps,
		TH1D *&h_tsu_ma, TH1D *&h_prl_ma,
		const TString &hname_tsu, const TString &hname_prl,
		double xmin, double xmax){
	int nbin = h_tsu->GetNbinsX();
	int nside = 7 * 27;
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(h_tsu->GetXaxis()->GetBinLowEdge(ibin));
	}

	h_tsu_ma = new TH1D(hname_tsu, h_tsu->GetTitle(), nbin, &xbins[0]);
	h_prl_ma = new TH1D(hname_prl, h_prl->GetTitle(), nbin, &xbins[0]);
	h_tsu_ma->SetDirectory(nullptr);
	h_prl_ma->SetDirectory(nullptr);
	h_tsu_ma->SetStats(0);
	h_prl_ma->SetStats(0);

	for(int ibin=1; ibin<=nbin; ibin++){
		double x = h_tsu->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;

		int jmin = ibin - nside;
		int jmax = ibin + nside;
		if(jmin < 1) jmin = 1;
		if(jmax > nbin) jmax = nbin;

		double sum_tsu_yw = 0.0;
		double sum_prl_yw = 0.0;
		double sum_tsu_w = 0.0;
		double sum_prl_w = 0.0;
		int nsum = 0;
		for(int jbin=jmin; jbin<=jmax; jbin++){
			double xj = h_tsu->GetBinCenter(jbin);
			if(xj < xmin || xj > xmax) continue;
			double y_tsu = h_tsu->GetBinContent(jbin);
			double y_prl = h_prl->GetBinContent(jbin);
			double w_tsu = h_tsu_exps->GetBinContent(jbin);
			double w_prl = h_prl_exps->GetBinContent(jbin);
			if(y_tsu == 0.0 || y_prl == 0.0) continue;
			if(w_tsu <= 0.0 || w_prl <= 0.0) continue;
			sum_tsu_yw += y_tsu * w_tsu;
			sum_prl_yw += y_prl * w_prl;
			sum_tsu_w += w_tsu;
			sum_prl_w += w_prl;
			nsum++;

			// Previous equal-weight version kept for comparison:
			// sum_tsu += y_tsu;
			// sum_prl += y_prl;
			// nsum++;
		}
		if(nsum <= 0) continue;
		if(sum_tsu_w <= 0.0 || sum_prl_w <= 0.0) continue;

		h_tsu_ma->SetBinContent(ibin, sum_tsu_yw / sum_tsu_w);
		h_prl_ma->SetBinContent(ibin, sum_prl_yw / sum_prl_w);
		h_tsu_ma->SetBinError(ibin, 0.0);
		h_prl_ma->SetBinError(ibin, 0.0);

		// Previous equal-weight output kept for comparison:
		// h_tsu_ma->SetBinContent(ibin, sum_tsu / nsum);
		// h_prl_ma->SetBinContent(ibin, sum_prl / nsum);
	}

	cout<<"======== clac pair ma exps ======== "<<endl
		<<"hname_tsu="<<hname_tsu<<endl
		<<"hname_prl="<<hname_prl<<endl
		<<"weight=exps"<<endl
		<<"nside="<<nside<<endl
		<<"nbin="<<nbin<<endl
		<<"xmin="<<xmin<<endl
		<<"xmax="<<xmax<<endl
		<<endl;
}

TH1D *BUILD_EWIDTH_AVERAGE(vector<TH1D*> vh, vector<int> vitag, const TString &hname, const TString &htitle){
	int nbin = vh.at(0)->GetNbinsX();
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(vh.at(0)->GetXaxis()->GetBinLowEdge(ibin));
	}

	TH1D *hout = new TH1D(hname, htitle, nbin, &xbins[0]);
	hout->SetDirectory(nullptr);
	hout->SetStats(0);

	double width_total = 0.0;
	for(size_t ih=0; ih<vh.size(); ih++){
		width_total += energy_bins[vitag.at(ih) + 1] - energy_bins[vitag.at(ih)];
	}

	for(int ibin=1; ibin<=nbin; ibin++){
		double sumwy = 0.0;
		for(size_t ih=0; ih<vh.size(); ih++){
			double width = energy_bins[vitag.at(ih) + 1] - energy_bins[vitag.at(ih)];
			double y = vh.at(ih)->GetBinContent(ibin);
			sumwy += width * y;
		}
		if(width_total <= 0.0) continue;
		hout->SetBinContent(ibin, sumwy / width_total);
		hout->SetBinError(ibin, 0.0);
	}

	cout<<"======== clac ewidth avg ======== "<<endl
		<<"hname="<<hname<<endl
		<<"n_hist="<<vh.size()<<endl
		<<"width_total="<<width_total<<endl
		<<endl;
	return hout;
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

TH1D *BUILD_PROJECTION(TH2 *h2, int itag, const TString &prefix){
	TH1D *h = dynamic_cast<TH1D*>(h2->ProjectionX(Form("%s_raw_ene%02d", prefix.Data(), itag), itag, itag));
	h->SetDirectory(nullptr);
	h->SetStats(0);
	h->SetTitle(Form("flux time, %g to %g GeV",
				energy_bins[itag], energy_bins[itag + 1]));
	return h;
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

bool FIND_COMMON_RANGE(TH1 *h_tsu, TH1 *h_prl, double &xmin, double &xmax){
	xmin = max(h_tsu->GetXaxis()->GetXmin(), h_prl->GetXaxis()->GetXmin());
	xmax = min(h_tsu->GetXaxis()->GetXmax(), h_prl->GetXaxis()->GetXmax());

	bool is_find_xmin = false;
	bool is_find_xmax = false;
	for(int ibin=1; ibin<=h_tsu->GetNbinsX(); ibin++){
		double x = h_tsu->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		if(h_tsu->GetBinContent(ibin) == 0.0) continue;
		if(h_prl->GetBinContent(ibin) == 0.0) continue;
		xmin = x;
		is_find_xmin = true;
		break;
	}
	for(int ibin=h_tsu->GetNbinsX(); ibin>=1; ibin--){
		double x = h_tsu->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		if(h_tsu->GetBinContent(ibin) == 0.0) continue;
		if(h_prl->GetBinContent(ibin) == 0.0) continue;
		xmax = x;
		is_find_xmax = true;
		break;
	}

	return is_find_xmin && is_find_xmax && xmin < xmax;
}

bool DRAW_MERGE_PAIR(TFile *fout, TH1D *h_tsu, TH1D *h_prl,
		const TString &etag, const TString &outdir, double xmin, double xmax){
	TH1D *hratio = BUILD_RATIO(h_tsu, h_prl, Form("hratio_ma_%s", etag.Data()), xmin, xmax);

	TCanvas *c = new TCanvas(Form("c_ma_%s", etag.Data()), Form("c_ma_%s", etag.Data()), 1100, 720);
	TPad *pad_top = new TPad(Form("pad_top_ma_%s", etag.Data()), "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad(Form("pad_ratio_ma_%s", etag.Data()), "", 0.0, 0.0, 1.0, 0.32);

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
	STYLE_HIST(h_tsu, kRed + 1, 20);
	STYLE_HIST(h_prl, kBlue + 1, 24);

	h_tsu->GetXaxis()->SetRangeUser(xmin, xmax);
	h_prl->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(h_tsu->GetXaxis(), 0.0, 0.0, 1.2);
	h_tsu->GetYaxis()->SetTitle("Flux");
	h_tsu->GetYaxis()->CenterTitle();
	h_tsu->GetYaxis()->SetTitleFont(62);
	h_tsu->GetYaxis()->SetTitleSize(0.052);
	h_tsu->GetYaxis()->SetTitleOffset(0.82);
	h_tsu->GetYaxis()->SetLabelOffset(0.012);

	double hmax = max(GET_HMAX(h_tsu, xmin, xmax), GET_HMAX(h_prl, xmin, xmax));
	h_tsu->SetMinimum(0.0);
	h_tsu->SetMaximum(hmax > 0.0 ? hmax * 1.25 : 1.0);

	h_tsu->Draw("E1X0P");
	h_prl->Draw("E1X0P same");

	TLegend *leg = new TLegend(0.15, 0.74, 0.31, 0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->SetTextSize(0.04);
	leg->AddEntry(h_tsu, "TSU", "ep");
	leg->AddEntry(h_prl, "PRL", "ep");
	leg->Draw();

	pad_ratio->cd();
	gPad->SetGridx();
	gPad->SetGridy();

	hratio->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(hratio->GetXaxis(), 0.11, 0.10, 1.1);

	hratio->GetYaxis()->SetTitle("TSU / PRL");
	hratio->GetYaxis()->CenterTitle();
	hratio->GetYaxis()->SetTitleFont(62);
	hratio->GetYaxis()->SetTitleSize(0.10);
	hratio->GetYaxis()->SetTitleOffset(0.48);
	hratio->GetYaxis()->SetLabelSize(0.085);
	hratio->GetYaxis()->SetLabelOffset(0.012);
	hratio->GetYaxis()->SetNdivisions(505);
	// hratio->SetMinimum(0.95);
	// hratio->SetMaximum(1.05);
	hratio->SetMinimum(0.8);
	hratio->SetMaximum(1.2);
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
	TString foutpdf = Form("%s/maflux_tsu_prl_compare_%s.pdf", outdir.Data(), etag.Data());
	c->SaveAs(foutpdf);

	fout->cd();
	h_tsu->Write();
	h_prl->Write();
	hratio->Write();
	c->Write();

	delete line1;
	delete box3;
	delete box5;
	delete leg;
	delete hratio;
	delete c;

	cout<<"IN DRAW_MERGE_PAIR ===== tag="<<etag
		<<" x=["<<xmin<<","<<xmax<<"]"
		<<" out="<<foutpdf
		<<endl;
	return true;
}

bool DRAW_PAIR(TFile *fout, TH1 *h_tsu_in, TH1 *h_prl_in,
		TH1 *h_tsu_exps_in, TH1 *h_prl_exps_in,
		int itag, const TString &outdir, bool is_ma){
	double xmin = 0.0;
	double xmax = 0.0;
	if(!FIND_COMMON_RANGE(h_tsu_in, h_prl_in, xmin, xmax)){
		cerr<<"ERR DRAW_PAIR ===== no common nonzero x range for ene"<<Form("%02d", itag)<<endl;
		return false;
	}

	TH1 *h_tsu = nullptr;
	TH1 *h_prl = nullptr;
	TString tag = is_ma ? "ma" : "raw";
	if(is_ma){
		TH1D *h_tsu_ma = nullptr;
		TH1D *h_prl_ma = nullptr;
		BUILD_PAIR_MOVING_AVERAGE_EXPS(h_tsu_in, h_prl_in,
				h_tsu_exps_in, h_prl_exps_in, h_tsu_ma, h_prl_ma,
				Form("htsu_ma_ene%02d", itag), Form("hprl_ma_ene%02d", itag),
				xmin, xmax);
		h_tsu = h_tsu_ma;
		h_prl = h_prl_ma;
	}
	else{
		h_tsu = dynamic_cast<TH1*>(h_tsu_in->Clone(Form("htsu_raw_draw_ene%02d", itag)));
		h_prl = dynamic_cast<TH1*>(h_prl_in->Clone(Form("hprl_raw_draw_ene%02d", itag)));
		h_tsu->SetDirectory(nullptr);
		h_prl->SetDirectory(nullptr);
	}

	h_tsu->SetTitle(Form("%g to %g GeV",
				energy_bins[itag], energy_bins[itag + 1]));
	TH1D *hratio = BUILD_RATIO(h_tsu, h_prl, Form("hratio_%s_ene%02d", tag.Data(), itag), xmin, xmax);

	TCanvas *c = new TCanvas(Form("c_%s_ene%02d", tag.Data(), itag), Form("c_%s_ene%02d", tag.Data(), itag), 1100, 720);
	TPad *pad_top = new TPad(Form("pad_top_%s_ene%02d", tag.Data(), itag), "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad(Form("pad_ratio_%s_ene%02d", tag.Data(), itag), "", 0.0, 0.0, 1.0, 0.32);

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
	STYLE_HIST(h_tsu, kRed + 1, 20);
	STYLE_HIST(h_prl, kBlue + 1, 24);

	h_tsu->GetXaxis()->SetRangeUser(xmin, xmax);
	h_prl->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(h_tsu->GetXaxis(), 0.0, 0.0, 1.2);
	h_tsu->GetYaxis()->SetTitle("Flux");
	h_tsu->GetYaxis()->CenterTitle();
	h_tsu->GetYaxis()->SetTitleFont(62);
	h_tsu->GetYaxis()->SetTitleSize(0.052);
	h_tsu->GetYaxis()->SetTitleOffset(0.82);
	h_tsu->GetYaxis()->SetLabelOffset(0.012);

	double hmax = max(GET_HMAX(h_tsu, xmin, xmax), GET_HMAX(h_prl, xmin, xmax));
	h_tsu->SetMinimum(0.0);
	h_tsu->SetMaximum(hmax > 0.0 ? hmax * 1.25 : 1.0);

	h_tsu->Draw("E1X0P");
	h_prl->Draw("E1X0P same");

	TLegend *leg = new TLegend(0.15, 0.74, 0.31, 0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->SetTextSize(0.04);
	leg->AddEntry(h_tsu, "TSU", "ep");
	leg->AddEntry(h_prl, "PRL", "ep");
	leg->Draw();

	pad_ratio->cd();
	gPad->SetGridx();
	gPad->SetGridy();

	hratio->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(hratio->GetXaxis(), 0.11, 0.10, 1.1);

	hratio->GetYaxis()->SetTitle("TSU / PRL");
	hratio->GetYaxis()->CenterTitle();
	hratio->GetYaxis()->SetTitleFont(62);
	hratio->GetYaxis()->SetTitleSize(0.10);
	hratio->GetYaxis()->SetTitleOffset(0.48);
	hratio->GetYaxis()->SetLabelSize(0.085);
	hratio->GetYaxis()->SetLabelOffset(0.012);
	hratio->GetYaxis()->SetNdivisions(505);
	// hratio->SetMinimum(is_ma ? 0.95 : 0.0);
	// hratio->SetMaximum(is_ma ? 1.05 : 2.0);
	hratio->SetMinimum(is_ma ? 0.8 : 0.0);
	hratio->SetMaximum(is_ma ? 1.2 : 2.0);
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
	TString foutpdf = Form("%s/%sflux_tsu_prl_compare_ene%02d.pdf",
			outdir.Data(), is_ma ? "ma" : "raw", itag);
	c->SaveAs(foutpdf);

	fout->cd();
	h_tsu->Write();
	h_prl->Write();
	hratio->Write();
	c->Write();

	delete line1;
	delete box3;
	delete box5;
	delete leg;
	delete hratio;
	delete c;
	delete h_tsu;
	delete h_prl;

	cout<<"IN DRAW_PAIR ===== mode="<<tag
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

	TString fpath_tsu = "datain/hflux_tsu.root";
	TString fpath_prl = "datain/hflux_prl.root";
	TString outdir = "dataout";

	TFile *f_tsu = new TFile(fpath_tsu, "read");
	TFile *f_prl = new TFile(fpath_prl, "read");
	TFile *fout = new TFile(outdir + "/flux_tsu_prl_compare.root", "recreate");

	TH2D *h2_tsu = dynamic_cast<TH2D*>(f_tsu->Get("h2d_Flux_daily"));
	// TH2D *h2_prl = dynamic_cast<TH2D*>(f_prl->Get("hPosFluxDay"));
	TH2D *h2_prl = dynamic_cast<TH2D*>(f_prl->Get("hflux"));
	TH2D *h2_tsu_exps = dynamic_cast<TH2D*>(f_tsu->Get("h2d_exposure_daily"));
	// TH2D *h2_prl_exps = dynamic_cast<TH2D*>(f_prl->Get("hExpDay"));
	TH2D *h2_prl_exps = dynamic_cast<TH2D*>(f_prl->Get("hexposure"));
	if(h2_tsu == nullptr || h2_prl == nullptr || h2_tsu_exps == nullptr || h2_prl_exps == nullptr){
		cerr<<"ERR MAIN ===== missing flux/exps daily hist"<<endl;
		return 1;
	}

	cout<<"IN MAIN ===== input tsu: "<<fpath_tsu<<endl
		<<"IN MAIN ===== input prl: "<<fpath_prl<<endl
		<<"IN MAIN ===== tsu hist: "<<h2_tsu->GetName()
		<<" class="<<h2_tsu->ClassName()
		<<" nx="<<h2_tsu->GetNbinsX()
		<<" ny="<<h2_tsu->GetNbinsY()<<endl
		<<"IN MAIN ===== prl hist: "<<h2_prl->GetName()
		<<" class="<<h2_prl->ClassName()
		<<" nx="<<h2_prl->GetNbinsX()
		<<" ny="<<h2_prl->GetNbinsY()<<endl
		<<"IN MAIN ===== tsu exps hist: "<<h2_tsu_exps->GetName()
		<<" class="<<h2_tsu_exps->ClassName()
		<<" nx="<<h2_tsu_exps->GetNbinsX()
		<<" ny="<<h2_tsu_exps->GetNbinsY()<<endl
		<<"IN MAIN ===== prl exps hist: "<<h2_prl_exps->GetName()
		<<" class="<<h2_prl_exps->ClassName()
		<<" nx="<<h2_prl_exps->GetNbinsX()
		<<" ny="<<h2_prl_exps->GetNbinsY()<<endl
		<<endl;

	int last_tsu_ene = FIND_LAST_NONZERO_TH2_ENEBIN(h2_tsu);
	int last_prl_ene = FIND_LAST_NONZERO_TH2_ENEBIN(h2_prl);
	int last_tsu_exps_ene = FIND_LAST_NONZERO_TH2_ENEBIN(h2_tsu_exps);
	int last_prl_exps_ene = FIND_LAST_NONZERO_TH2_ENEBIN(h2_prl_exps);
	int last_ene = min(nenebin, min(min(last_tsu_ene, last_prl_ene), min(last_tsu_exps_ene, last_prl_exps_ene)));
	if(last_ene <= 0){
		cerr<<"ERR MAIN ===== no common nonzero ene bin"
			<<" last_tsu_ene="<<last_tsu_ene
			<<" last_prl_ene="<<last_prl_ene
			<<" last_tsu_exps_ene="<<last_tsu_exps_ene
			<<" last_prl_exps_ene="<<last_prl_exps_ene
			<<endl;
		return 2;
	}
	cout<<"IN MAIN ===== ene loop"
		<<" last_tsu_ene="<<last_tsu_ene
		<<" last_prl_ene="<<last_prl_ene
		<<" last_tsu_exps_ene="<<last_tsu_exps_ene
		<<" last_prl_exps_ene="<<last_prl_exps_ene
		<<" last_ene="<<last_ene
		<<endl
		<<endl;

	// for(int itag=1; itag<=27; itag++){
	for(int itag=1; itag<=last_ene; itag++){
		TH1D *h_tsu = BUILD_PROJECTION(h2_tsu, itag, "htsu");
		TH1D *h_prl_raw = BUILD_PROJECTION(h2_prl, itag, "hprl");
		TH1D *h_prl = BUILD_ON_REF_TIME_AXIS(h_tsu, h_prl_raw, Form("hprl_aligned_ene%02d", itag));
		TH1D *h_tsu_exps = BUILD_PROJECTION(h2_tsu_exps, itag, "htsu_exps");
		TH1D *h_prl_exps_raw = BUILD_PROJECTION(h2_prl_exps, itag, "hprl_exps");
		TH1D *h_prl_exps = BUILD_ON_REF_TIME_AXIS(h_tsu, h_prl_exps_raw, Form("hprl_exps_aligned_ene%02d", itag));

		if(!DRAW_PAIR(fout, h_tsu, h_prl, h_tsu_exps, h_prl_exps, itag, outdir, false)){
			delete h_tsu;
			delete h_prl_raw;
			delete h_prl;
			delete h_tsu_exps;
			delete h_prl_exps_raw;
			delete h_prl_exps;
			return 3;
		}
		if(!DRAW_PAIR(fout, h_tsu, h_prl, h_tsu_exps, h_prl_exps, itag, outdir, true)){
			delete h_tsu;
			delete h_prl_raw;
			delete h_prl;
			delete h_tsu_exps;
			delete h_prl_exps_raw;
			delete h_prl_exps;
			return 4;
		}

		delete h_tsu;
		delete h_prl_raw;
		delete h_prl;
		delete h_tsu_exps;
		delete h_prl_exps_raw;
		delete h_prl_exps;
	}

	//======== enebin merge ========
	vector<vector<int>> vmerge_itag = {
		{1, 2},
		{1, 2, 3, 4}
	};
	vector<TString> vmerge_tag = {
		"ene01_02",
		"ene01_04"
	};

	for(size_t imerge=0; imerge<vmerge_itag.size(); imerge++){
		vector<int> vitag = vmerge_itag.at(imerge);
		if(vitag.back() > last_ene){
			cerr<<"WARN MAIN ===== skip merge above common nonzero ene bin"
				<<" tag="<<vmerge_tag.at(imerge)
				<<" merge_last_ene="<<vitag.back()
				<<" last_ene="<<last_ene
				<<endl;
			continue;
		}
		vector<TH1D*> vh_tsu_ma;
		vector<TH1D*> vh_prl_ma;
		vector<TH1D*> vh_tmp;
		double xmin = 0.0;
		double xmax = 0.0;
		bool is_first = true;

		for(size_t i=0; i<vitag.size(); i++){
			int itag = vitag.at(i);
			TH1D *h_tsu = BUILD_PROJECTION(h2_tsu, itag, Form("htsu_merge_%s", vmerge_tag.at(imerge).Data()));
			TH1D *h_prl_raw = BUILD_PROJECTION(h2_prl, itag, Form("hprl_merge_%s", vmerge_tag.at(imerge).Data()));
			TH1D *h_prl = BUILD_ON_REF_TIME_AXIS(h_tsu, h_prl_raw,
					Form("hprl_merge_aligned_%s_ene%02d", vmerge_tag.at(imerge).Data(), itag));
			TH1D *h_tsu_exps = BUILD_PROJECTION(h2_tsu_exps, itag,
					Form("htsu_merge_exps_%s", vmerge_tag.at(imerge).Data()));
			TH1D *h_prl_exps_raw = BUILD_PROJECTION(h2_prl_exps, itag,
					Form("hprl_merge_exps_%s", vmerge_tag.at(imerge).Data()));
			TH1D *h_prl_exps = BUILD_ON_REF_TIME_AXIS(h_tsu, h_prl_exps_raw,
					Form("hprl_merge_exps_aligned_%s_ene%02d", vmerge_tag.at(imerge).Data(), itag));

			double xtmp_min = 0.0;
			double xtmp_max = 0.0;
			if(!FIND_COMMON_RANGE(h_tsu, h_prl, xtmp_min, xtmp_max)){
				cerr<<"ERR MAIN ===== no merge common range"
					<<" tag="<<vmerge_tag.at(imerge)
					<<" itag="<<itag
					<<endl;
				return 5;
			}
			if(is_first){
				xmin = xtmp_min;
				xmax = xtmp_max;
				is_first = false;
			}
			else{
				xmin = max(xmin, xtmp_min);
				xmax = min(xmax, xtmp_max);
			}

			TH1D *h_tsu_ma = nullptr;
			TH1D *h_prl_ma = nullptr;
			BUILD_PAIR_MOVING_AVERAGE_EXPS(h_tsu, h_prl,
					h_tsu_exps, h_prl_exps, h_tsu_ma, h_prl_ma,
					Form("htsu_ma_%s_srcene%02d", vmerge_tag.at(imerge).Data(), itag),
					Form("hprl_ma_%s_srcene%02d", vmerge_tag.at(imerge).Data(), itag),
					xtmp_min, xtmp_max);
			vh_tsu_ma.push_back(h_tsu_ma);
			vh_prl_ma.push_back(h_prl_ma);
			vh_tmp.push_back(h_tsu);
			vh_tmp.push_back(h_prl_raw);
			vh_tmp.push_back(h_prl);
			vh_tmp.push_back(h_tsu_exps);
			vh_tmp.push_back(h_prl_exps_raw);
			vh_tmp.push_back(h_prl_exps);
		}
		if(xmin >= xmax){
			cerr<<"ERR MAIN ===== bad merge x range"
				<<" tag="<<vmerge_tag.at(imerge)
				<<" xmin="<<xmin
				<<" xmax="<<xmax
				<<endl;
			return 6;
		}

		TString htitle = Form("flux time, %g to %g GeV",
				energy_bins[vitag.front()],
				energy_bins[vitag.back() + 1]);
		TH1D *h_tsu_merge = BUILD_EWIDTH_AVERAGE(vh_tsu_ma, vitag,
				Form("htsu_ma_%s", vmerge_tag.at(imerge).Data()), htitle);
		TH1D *h_prl_merge = BUILD_EWIDTH_AVERAGE(vh_prl_ma, vitag,
				Form("hprl_ma_%s", vmerge_tag.at(imerge).Data()), htitle);
		if(!DRAW_MERGE_PAIR(fout, h_tsu_merge, h_prl_merge, vmerge_tag.at(imerge), outdir, xmin, xmax)){
			return 7;
		}

		delete h_tsu_merge;
		delete h_prl_merge;
		for(size_t ih=0; ih<vh_tsu_ma.size(); ih++) delete vh_tsu_ma.at(ih);
		for(size_t ih=0; ih<vh_prl_ma.size(); ih++) delete vh_prl_ma.at(ih);
		for(size_t ih=0; ih<vh_tmp.size(); ih++) delete vh_tmp.at(ih);
	}

	fout->Write();
	fout->Close();
	f_tsu->Close();
	f_prl->Close();

	cout<<"IN MAIN ===== output root: "<<outdir + "/flux_tsu_prl_compare.root"<<endl;
	return 0;
}
