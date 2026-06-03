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
#include "TKey.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"

static const int nenebin = 29;
static const double energy_bins[nenebin + 1] = {
	0.80, 1.00, 1.16, 1.33, 1.51,
	1.71, 1.92, 2.15, 2.40, 2.67,
	2.97, 3.29, 3.64, 4.02, 4.43,
	4.88, 5.37, 5.90, 6.47, 7.09,
	7.76, 8.48, 9.26, 10.10, 11.0,
	13.0, 16.6, 22.8, 41.9, 45.10
};

struct HistPair{
	int itag;
	TString hname_gcx;
	TString hname_prl;
};

vector<HistPair> GET_HIST_PAIRS(TFile *f_gcx, TFile *f_prl){
	vector<HistPair> vpairs;

	for(int itag=0; itag<100; itag++){
		TString hname_gcx = Form("hflux_t_ene%02d", itag);
		TString hname_prl = Form("hfluxt_ene%02d", itag);
		if(f_gcx->Get(hname_gcx) == nullptr) continue;
		if(f_prl->Get(hname_prl) == nullptr) continue;
		vpairs.push_back({itag, hname_gcx, hname_prl});
	}

	return vpairs;
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

TH1D *BUILD_MOVING_AVERAGE(TH1 *h, const TString &hname, double xmin, double xmax){
	int nbin = h->GetNbinsX();
	int nside = 7 * 27;
	//====xbins
	vector<double> xbins;
	for(int ibin=1; ibin<=nbin + 1; ibin++){
		xbins.push_back(h->GetXaxis()->GetBinLowEdge(ibin));
	}
	//====init hma
	TH1D *hma = new TH1D(hname, h->GetTitle(), nbin, &xbins[0]);
	hma->SetDirectory(nullptr);
	hma->SetStats(0);
	//====ma
	for(int ibin=1; ibin<=nbin; ibin++){
		double x = h->GetBinCenter(ibin);
		if(x < xmin || x > xmax) continue;
		//====init jmin&jmax
		int jmin = ibin - nside;
		int jmax = ibin + nside;
		if(jmin < 1) jmin = 1;
		if(jmax > nbin) jmax = nbin;
		//====calc sumwy&sumw
		double sumw = 0.0;
		double sumwy = 0.0;
		for(int jbin=jmin; jbin<=jmax; jbin++){
			double xj = h->GetBinCenter(jbin);
			if(xj < xmin || xj > xmax) continue;
			double y = h->GetBinContent(jbin);
			double ye = h->GetBinError(jbin);
			if(ye <= 0.0) continue;
			double w = 1.0 / (ye * ye);
			sumw += w;
			sumwy += w * y;
		}
		if(sumw <= 0.0) continue;
		//====calc ma
		hma->SetBinContent(ibin, sumwy / sumw);
		hma->SetBinError(ibin, 1.0 / sqrt(sumw));
	}
	cout<<"======== clac ma ======== "<<endl
		<<"hname="<<hname<<endl
		<<"nside="<<nside<<endl
		<<"nbin="<<nbin<<endl
		<<"xmin="<<xmin<<endl
		<<"xmax="<<xmax<<endl
		<<endl;
	return hma;
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
		double sumwe2 = 0.0;
		for(size_t ih=0; ih<vh.size(); ih++){
			double width = energy_bins[vitag.at(ih) + 1] - energy_bins[vitag.at(ih)];
			double y = vh.at(ih)->GetBinContent(ibin);
			double ye = vh.at(ih)->GetBinError(ibin);
			sumwy += width * y;
			sumwe2 += pow(width * ye, 2);
		}
		if(width_total <= 0.0) continue;
		hout->SetBinContent(ibin, sumwy / width_total);
		hout->SetBinError(ibin, sqrt(sumwe2) / width_total);
	}

	cout<<"======== clac ewidth avg ======== "<<endl
		<<"hname="<<hname<<endl
		<<"n_hist="<<vh.size()<<endl
		<<"width_total="<<width_total<<endl
		<<endl;
	return hout;
}

TH1D *BUILD_RATIO(TH1 *h_gcx, TH1 *h_prl, const TString &hname, double xmin, double xmax){
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
		double x = h_gcx->GetBinCenter(ibin);
		int jbin = h_prl->GetXaxis()->FindFixBin(x);
		if(jbin < 1 || jbin > h_prl->GetNbinsX()) continue;

		double y_gcx = h_gcx->GetBinContent(ibin);
		double e_gcx = h_gcx->GetBinError(ibin);
		double y_prl = h_prl->GetBinContent(jbin);
		double e_prl = h_prl->GetBinError(jbin);
		if(y_gcx == 0.0 || y_prl == 0.0) continue;

		int rbin = ibin - ibin_min + 1;
		double ratio = y_gcx / y_prl;
		double eratio = ratio * sqrt(pow(e_gcx / y_gcx, 2) + pow(e_prl / y_prl, 2));
		hratio->SetBinContent(rbin, ratio);
		hratio->SetBinError(rbin, 0);
		// hratio->SetBinError(rbin, eratio);
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

bool DRAW_COMPARE(TFile *fout, TH1 *h_gcx, TH1 *h_prl, const TString &tag, const TString &foutbase, double xmin, double xmax, const TString &outdir){
	TH1D *hratio = BUILD_RATIO(h_gcx, h_prl, "hratio_" + tag, xmin, xmax);
	TCanvas *c = new TCanvas("c_" + tag, "c_" + tag, 1100, 720);
	TPad *pad_top = new TPad("pad_top_" + tag, "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad("pad_ratio_" + tag, "", 0.0, 0.0, 1.0, 0.32);

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
	STYLE_HIST(h_prl, kBlue + 1, 24);

	h_gcx->GetXaxis()->SetRangeUser(xmin, xmax);
	h_prl->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(h_gcx->GetXaxis(), 0.0, 0.0, 1.2);
	h_gcx->GetYaxis()->SetTitle("Flux");
	h_gcx->GetYaxis()->CenterTitle();
	h_gcx->GetYaxis()->SetTitleFont(62);
	h_gcx->GetYaxis()->SetTitleSize(0.052);
	h_gcx->GetYaxis()->SetTitleOffset(0.82);
	h_gcx->GetYaxis()->SetLabelOffset(0.012);

	double hmax = max(GET_HMAX(h_gcx, xmin, xmax), GET_HMAX(h_prl, xmin, xmax));
	h_gcx->SetMinimum(0.0);
	h_gcx->SetMaximum(hmax > 0.0 ? hmax * 1.25 : 1.0);

	h_gcx->Draw("E1X0P");
	h_prl->Draw("E1X0P same");

	TLegend *leg = new TLegend(0.15, 0.74, 0.31, 0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(62);
	leg->SetTextSize(0.04);
	leg->AddEntry(h_gcx, "GCX", "ep");
	leg->AddEntry(h_prl, "PRL", "ep");
	leg->Draw();

	TLatex latex;
	latex.SetNDC();
	latex.SetTextFont(62);
	latex.SetTextSize(0.035);
	// latex.DrawLatex(0.68, 0.84, Form("Energy bin %02d", itag));

	pad_ratio->cd();
	gPad->SetGridx();
	gPad->SetGridy();

	hratio->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_TIME_AXIS(hratio->GetXaxis(), 0.11, 0.10, 1.1);

	hratio->GetYaxis()->SetTitle("GCX / PRL");
	hratio->GetYaxis()->CenterTitle();
	hratio->GetYaxis()->SetTitleFont(62);
	hratio->GetYaxis()->SetTitleSize(0.10);
	hratio->GetYaxis()->SetTitleOffset(0.48);
	hratio->GetYaxis()->SetLabelSize(0.085);
	hratio->GetYaxis()->SetLabelOffset(0.012);
	hratio->GetYaxis()->SetNdivisions(505);
	hratio->SetMinimum(0.8);
	hratio->SetMaximum(1.2);
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
	TString foutpdf = outdir + "/" + foutbase + ".pdf";
	c->SaveAs(foutpdf);

	fout->cd();
	h_gcx->Write();
	h_prl->Write();
	hratio->Write();
	c->Write();

	delete line1;
	delete box3;
	delete box5;
	delete leg;
	delete hratio;
	delete c;

	cout<<"======== draw compare ======== "<<endl
		<<"tag="<<tag<<endl
		<<"out="<<foutpdf<<endl
		<<"xmin="<<xmin<<endl
		<<"xmax="<<xmax<<endl
		<<endl;
	return true;
}

bool DRAW_PAIR(TFile *fout, TH1 *h_gcx_in, TH1 *h_prl_in, int itag, const TString &outdir){
	// double xmin = min(h_gcx_in->GetXaxis()->GetXmin(), h_prl_in->GetXaxis()->GetXmin());
	// double xmax = max(h_gcx_in->GetXaxis()->GetXmax(), h_prl_in->GetXaxis()->GetXmax());
	double xmin = max(h_gcx_in->GetXaxis()->GetXmin(), h_prl_in->GetXaxis()->GetXmin());
	double xmax = min(h_gcx_in->GetXaxis()->GetXmax(), h_prl_in->GetXaxis()->GetXmax());
	bool is_find_xmin = false;
	bool is_find_xmax = false;
	for(int ibin=1; ibin<=h_gcx_in->GetNbinsX(); ibin++){
		double x = h_gcx_in->GetBinCenter(ibin);
		int jbin = h_prl_in->GetXaxis()->FindFixBin(x);
		if(x < xmin || x > xmax) continue;
		if(jbin < 1 || jbin > h_prl_in->GetNbinsX()) continue;
		if(h_gcx_in->GetBinContent(ibin) == 0.0) continue;
		if(h_prl_in->GetBinContent(jbin) == 0.0) continue;
		xmin = x;
		is_find_xmin = true;
		break;
	}
	for(int ibin=h_gcx_in->GetNbinsX(); ibin>=1; ibin--){
		double x = h_gcx_in->GetBinCenter(ibin);
		int jbin = h_prl_in->GetXaxis()->FindFixBin(x);
		if(x < xmin || x > xmax) continue;
		if(jbin < 1 || jbin > h_prl_in->GetNbinsX()) continue;
		if(h_gcx_in->GetBinContent(ibin) == 0.0) continue;
		if(h_prl_in->GetBinContent(jbin) == 0.0) continue;
		xmax = x;
		is_find_xmax = true;
		break;
	}
	if(!is_find_xmin || !is_find_xmax){
		cerr<<"ERR DRAW_PAIR ===== no common nonzero x range for ene"<<Form("%02d", itag)<<endl;
		return false;
	}
	if(xmin >= xmax){
		cerr<<"ERR DRAW_PAIR ===== empty x range for ene"<<Form("%02d", itag)<<endl;
		return false;
	}

	TH1D *h_gcx = BUILD_MOVING_AVERAGE(h_gcx_in, Form("hgcx_ma_ene%02d", itag), xmin, xmax);
	TH1D *h_prl = BUILD_MOVING_AVERAGE(h_prl_in, Form("hprl_ma_ene%02d", itag), xmin, xmax);
	h_gcx->SetDirectory(nullptr);
	h_prl->SetDirectory(nullptr);

	if(!DRAW_COMPARE(fout, h_gcx, h_prl, Form("ene%02d", itag), Form("flux_compare_ene%02d", itag), xmin, xmax, outdir)){
		delete h_gcx;
		delete h_prl;
		return false;
	}

	delete h_gcx;
	delete h_prl;

	cout<<"IN DRAW_PAIR ===== ene"<<Form("%02d", itag)
		<<" x=["<<xmin<<","<<xmax<<"]"
		<<endl;
	return true;
}

int main(){
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	gStyle->SetTimeOffset(0);
	TGaxis::SetMaxDigits(3);

	TString fpath_gcx = "datain/hflux_gcx.root";
	TString fpath_prl = "datain/hflux_prl.root";
	TString outdir = "dataout";

	TFile *f_gcx = new TFile(fpath_gcx, "read");
	TFile *f_prl = new TFile(fpath_prl, "read");
	TFile *fout = new TFile(outdir + "/flux_compare.root", "recreate");

	vector<HistPair> vpairs = GET_HIST_PAIRS(f_gcx, f_prl);
	cout<<"IN MAIN ===== n_pair="<<vpairs.size()<<endl;

	for(size_t ipair=0; ipair<vpairs.size(); ipair++){
		TH1 *h_gcx = dynamic_cast<TH1*>(f_gcx->Get(vpairs.at(ipair).hname_gcx));
		TH1 *h_prl = dynamic_cast<TH1*>(f_prl->Get(vpairs.at(ipair).hname_prl));
		if(h_gcx == nullptr || h_prl == nullptr) continue;
		if(!DRAW_PAIR(fout, h_gcx, h_prl, vpairs.at(ipair).itag, outdir)){
			fout->Close();
			f_gcx->Close();
			f_prl->Close();
			return 1;
		}
	}

	//======== enebin merge ========
	//====init merge tags
	vector<vector<int>> vmerge_itag = {
		{1, 2},
		{1, 2, 3, 4}
	};
	vector<TString> vmerge_tag = {
		"ene01_02",
		"ene01_04"
	};
	//====merge
	for(size_t imerge=0; imerge<vmerge_itag.size(); imerge++){
		vector<int> vitag = vmerge_itag.at(imerge);
		vector<TH1D*> vh_gcx_ma;
		vector<TH1D*> vh_prl_ma;
		double xmin = 0.0;
		double xmax = 0.0;
		bool is_first = true;

		for(size_t i=0; i<vitag.size(); i++){
			int itag = vitag.at(i);
			TH1 *h_gcx = dynamic_cast<TH1*>(f_gcx->Get(Form("hflux_t_ene%02d", itag)));
			TH1 *h_prl = dynamic_cast<TH1*>(f_prl->Get(Form("hfluxt_ene%02d", itag)));
			if(h_gcx == nullptr || h_prl == nullptr){
				cerr<<"ERR MAIN ===== missing merge hist"
					<<" tag="<<vmerge_tag.at(imerge)
					<<" itag="<<itag
					<<endl;
				return 2;
			}
			if(is_first){
				xmin = max(h_gcx->GetXaxis()->GetXmin(), h_prl->GetXaxis()->GetXmin());
				xmax = min(h_gcx->GetXaxis()->GetXmax(), h_prl->GetXaxis()->GetXmax());
				is_first = false;
			}
			else{
				xmin = max(xmin, max(h_gcx->GetXaxis()->GetXmin(), h_prl->GetXaxis()->GetXmin()));
				xmax = min(xmax, min(h_gcx->GetXaxis()->GetXmax(), h_prl->GetXaxis()->GetXmax()));
			}
			bool is_find_xmin = false;
			bool is_find_xmax = false;
			double xmin_now = max(h_gcx->GetXaxis()->GetXmin(), h_prl->GetXaxis()->GetXmin());
			double xmax_now = min(h_gcx->GetXaxis()->GetXmax(), h_prl->GetXaxis()->GetXmax());
			for(int ibin=1; ibin<=h_gcx->GetNbinsX(); ibin++){
				double x = h_gcx->GetBinCenter(ibin);
				int jbin = h_prl->GetXaxis()->FindFixBin(x);
				if(x < xmin_now || x > xmax_now) continue;
				if(jbin < 1 || jbin > h_prl->GetNbinsX()) continue;
				if(h_gcx->GetBinContent(ibin) == 0.0) continue;
				if(h_prl->GetBinContent(jbin) == 0.0) continue;
				xmin_now = x;
				is_find_xmin = true;
				break;
			}
			for(int ibin=h_gcx->GetNbinsX(); ibin>=1; ibin--){
				double x = h_gcx->GetBinCenter(ibin);
				int jbin = h_prl->GetXaxis()->FindFixBin(x);
				if(x < xmin_now || x > xmax_now) continue;
				if(jbin < 1 || jbin > h_prl->GetNbinsX()) continue;
				if(h_gcx->GetBinContent(ibin) == 0.0) continue;
				if(h_prl->GetBinContent(jbin) == 0.0) continue;
				xmax_now = x;
				is_find_xmax = true;
				break;
			}
			if(!is_find_xmin || !is_find_xmax){
				cerr<<"ERR MAIN ===== no common nonzero merge x range"
					<<" tag="<<vmerge_tag.at(imerge)
					<<" itag="<<itag
					<<endl;
				return 3;
			}
			xmin = max(xmin, xmin_now);
			xmax = min(xmax, xmax_now);
		}
		if(xmin >= xmax){
			cerr<<"ERR MAIN ===== empty merge x range"
				<<" tag="<<vmerge_tag.at(imerge)
				<<endl;
			return 3;
		}

		for(size_t i=0; i<vitag.size(); i++){
			int itag = vitag.at(i);
			TH1 *h_gcx = dynamic_cast<TH1*>(f_gcx->Get(Form("hflux_t_ene%02d", itag)));
			TH1 *h_prl = dynamic_cast<TH1*>(f_prl->Get(Form("hfluxt_ene%02d", itag)));
			TH1D *h_gcx_ma = BUILD_MOVING_AVERAGE(h_gcx,
					Form("hgcx_ma_%s_src%02d", vmerge_tag.at(imerge).Data(), itag),
					xmin, xmax);
			TH1D *h_prl_ma = BUILD_MOVING_AVERAGE(h_prl,
					Form("hprl_ma_%s_src%02d", vmerge_tag.at(imerge).Data(), itag),
					xmin, xmax);
			vh_gcx_ma.push_back(h_gcx_ma);
			vh_prl_ma.push_back(h_prl_ma);
		}

		TString htitle = Form("flux time, %g to %g GeV",
				energy_bins[vitag.front()],
				energy_bins[vitag.back() + 1]);
		TH1D *h_gcx_merge = BUILD_EWIDTH_AVERAGE(vh_gcx_ma, vitag,
				"hgcx_ma_" + vmerge_tag.at(imerge), htitle);
		TH1D *h_prl_merge = BUILD_EWIDTH_AVERAGE(vh_prl_ma, vitag,
				"hprl_ma_" + vmerge_tag.at(imerge), htitle);

		if(!DRAW_COMPARE(fout, h_gcx_merge, h_prl_merge, vmerge_tag.at(imerge),
					"flux_compare_" + vmerge_tag.at(imerge), xmin, xmax, outdir)){
			return 4;
		}

		cout<<"======== draw merge ======== "<<endl
			<<"tag="<<vmerge_tag.at(imerge)<<endl
			<<"n_hist="<<vitag.size()<<endl
			<<"elow="<<energy_bins[vitag.front()]<<endl
			<<"eup="<<energy_bins[vitag.back() + 1]<<endl
			<<endl;

		delete h_gcx_merge;
		delete h_prl_merge;
		for(size_t i=0; i<vh_gcx_ma.size(); i++){
			delete vh_gcx_ma.at(i);
			delete vh_prl_ma.at(i);
		}
	}
	//======== save ========
	fout->Write();
	fout->Close();
	f_gcx->Close();
	f_prl->Close();

	cout<<"IN MAIN ===== output root: "<<outdir + "/flux_compare.root"<<endl;
	return 0;
}
