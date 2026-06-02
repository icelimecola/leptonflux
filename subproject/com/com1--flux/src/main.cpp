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

TH1D *BUILD_RATIO(TH1 *h_gcx, TH1 *h_prl, int itag, double xmin, double xmax){
	TAxis *xaxis = h_gcx->GetXaxis();
	int ibin_min = xaxis->FindFixBin(xmin);
	int ibin_max = xaxis->FindFixBin(xmax);
	if(ibin_min < 1) ibin_min = 1;
	if(ibin_max > h_gcx->GetNbinsX()) ibin_max = h_gcx->GetNbinsX();

	vector<double> xbins;
	for(int ibin=ibin_min; ibin<=ibin_max + 1; ibin++){
		xbins.push_back(xaxis->GetBinLowEdge(ibin));
	}

	TH1D *hratio = new TH1D(Form("hratio_ene%02d", itag),
			Form("hratio_ene%02d", itag), xbins.size() - 1, &xbins[0]);
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

bool DRAW_PAIR(TFile *fout, TH1 *h_gcx_in, TH1 *h_prl_in, int itag, const TString &outdir){
	TH1 *h_gcx = dynamic_cast<TH1*>(h_gcx_in->Clone(Form("hgcx_ene%02d", itag)));
	TH1 *h_prl = dynamic_cast<TH1*>(h_prl_in->Clone(Form("hprl_ene%02d", itag)));
	h_gcx->SetDirectory(nullptr);
	h_prl->SetDirectory(nullptr);

	double xmin = max(h_gcx->GetXaxis()->GetXmin(), h_prl->GetXaxis()->GetXmin());
	double xmax = min(h_gcx->GetXaxis()->GetXmax(), h_prl->GetXaxis()->GetXmax());
	if(xmin >= xmax){
		cerr<<"ERR DRAW_PAIR ===== empty x range for ene"<<Form("%02d", itag)<<endl;
		delete h_gcx;
		delete h_prl;
		return false;
	}

	TH1D *hratio = BUILD_RATIO(h_gcx, h_prl, itag, xmin, xmax);

	TCanvas *c = new TCanvas(Form("c_ene%02d", itag), Form("c_ene%02d", itag), 1100, 720);
	TPad *pad_top = new TPad(Form("pad_top_ene%02d", itag), "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad(Form("pad_ratio_ene%02d", itag), "", 0.0, 0.0, 1.0, 0.32);

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
	hratio->SetMinimum(0.0);
	hratio->SetMaximum(2.0);
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
	TString foutpdf = Form("%s/flux_compare_ene%02d.pdf", outdir.Data(), itag);
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
	delete h_gcx;
	delete h_prl;

	cout<<"IN DRAW_PAIR ===== ene"<<Form("%02d", itag)
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

	fout->Write();
	fout->Close();
	f_gcx->Close();
	f_prl->Close();

	cout<<"IN MAIN ===== output root: "<<outdir + "/flux_compare.root"<<endl;
	return 0;
}
