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
#include "TLegend.h"
#include "TLine.h"
#include "TPad.h"
#include "TROOT.h"
#include "TString.h"
#include "TStyle.h"

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

void STYLE_HIST(TH1 *h, int color, int mstyle){
	h->SetStats(0);
	h->SetMarkerStyle(mstyle);
	h->SetMarkerSize(0.75);
	h->SetMarkerColor(color);
	h->SetLineColor(color);
	h->SetLineWidth(1);
}

void STYLE_ENE_AXIS(TAxis *xaxis, double title_size, double label_size, double title_offset){
	xaxis->SetTitle("Energy [GeV]");
	xaxis->CenterTitle();
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(title_size);
	xaxis->SetTitleOffset(title_offset);
	xaxis->SetLabelSize(label_size);
	xaxis->SetLabelOffset(0.015);
	xaxis->SetMoreLogLabels();
	xaxis->SetNoExponent();
}

void RESET_TSU_ERR(TH1 *h_tsu){
	for(int ibin=1; ibin<=h_tsu->GetNbinsX(); ibin++){
		double y = h_tsu->GetBinContent(ibin);
		h_tsu->SetBinError(ibin, y > 0.0 ? 0.001 : 0.0);
	}
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
		double x = h_gcx->GetBinCenter(ibin);
		int jbin = h_tsu->GetXaxis()->FindFixBin(x);
		if(jbin < 1 || jbin > h_tsu->GetNbinsX()) continue;

		double y_gcx = h_gcx->GetBinContent(ibin);
		double y_tsu = h_tsu->GetBinContent(jbin);
		if(y_gcx == 0.0 || y_tsu == 0.0) continue;

		int rbin = ibin - ibin_min + 1;
		hratio->SetBinContent(rbin, y_gcx / y_tsu);
		hratio->SetBinError(rbin, 0.0);
	}

	return hratio;
}

bool DRAW_PAIR(TFile *fout, TH1 *h_gcx_in, TH1 *h_tsu_in, const TString &outdir){
	TH1 *h_gcx = dynamic_cast<TH1*>(h_gcx_in->Clone("hgcx_trig_draw"));
	TH1 *h_tsu = dynamic_cast<TH1*>(h_tsu_in->Clone("htsu_trig_draw"));
	h_gcx->SetDirectory(nullptr);
	h_tsu->SetDirectory(nullptr);
	h_gcx->SetTitle("");
	h_tsu->SetTitle("");

	double xmin = max(h_gcx->GetXaxis()->GetXmin(), h_tsu->GetXaxis()->GetXmin());
	double xmax = min(h_gcx->GetXaxis()->GetXmax(), h_tsu->GetXaxis()->GetXmax());
	TH1D *hratio = BUILD_RATIO(h_gcx, h_tsu, "hratio_trig", xmin, xmax);

	TCanvas *c = new TCanvas("c_trig_compare", "c_trig_compare", 900, 720);
	TPad *pad_top = new TPad("pad_top", "", 0.0, 0.32, 1.0, 1.0);
	TPad *pad_ratio = new TPad("pad_ratio", "", 0.0, 0.0, 1.0, 0.32);

	pad_top->SetTopMargin(0.08);
	pad_top->SetBottomMargin(0.03);
	pad_top->SetLeftMargin(0.12);
	pad_top->SetRightMargin(0.06);
	pad_ratio->SetTopMargin(0.03);
	pad_ratio->SetBottomMargin(0.32);
	pad_ratio->SetLeftMargin(0.12);
	pad_ratio->SetRightMargin(0.06);
	pad_top->Draw();
	pad_ratio->Draw();

	pad_top->cd();
	gPad->SetGridx();
	gPad->SetGridy();
	gPad->SetLogx();
	STYLE_HIST(h_gcx, kRed + 1, 20);
	STYLE_HIST(h_tsu, kBlue + 1, 24);

	h_gcx->GetXaxis()->SetRangeUser(xmin, xmax);
	h_tsu->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_ENE_AXIS(h_gcx->GetXaxis(), 0.0, 0.0, 1.2);
	h_gcx->GetYaxis()->SetTitle("Trigger efficiency");
	h_gcx->GetYaxis()->CenterTitle();
	h_gcx->GetYaxis()->SetTitleFont(62);
	h_gcx->GetYaxis()->SetTitleSize(0.052);
	h_gcx->GetYaxis()->SetTitleOffset(0.82);
	h_gcx->GetYaxis()->SetLabelOffset(0.012);

	h_gcx->SetMinimum(0.8);
	h_gcx->SetMaximum(1.1);

	h_gcx->Draw("E1X0P");
	h_tsu->Draw("E1X0P same");

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
	gPad->SetLogx();

	hratio->GetXaxis()->SetRangeUser(xmin, xmax);
	STYLE_ENE_AXIS(hratio->GetXaxis(), 0.11, 0.10, 1.1);

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
	TString foutpdf = outdir + "/trig_compare.pdf";
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

	cout<<"IN DRAW_PAIR ===== x=["<<xmin<<","<<xmax<<"]"
		<<" out="<<foutpdf
		<<endl;
	return true;
}

int main(){
	gROOT->SetBatch(kTRUE);
	gStyle->SetOptStat(0);
	gStyle->SetEndErrorSize(0);
	TGaxis::SetMaxDigits(3);

	TString fpath_gcx = "datain/trig_gcx.root";
	TString fpath_tsu = "datain/trig_tsu.root";
	TString outdir = "dataout";

	TFile *f_gcx = new TFile(fpath_gcx, "read");
	TFile *f_tsu = new TFile(fpath_tsu, "read");
	TFile *fout = new TFile(outdir + "/trig_compare.root", "recreate");

	TH1D *h_gcx_in = dynamic_cast<TH1D*>(f_gcx->Get("hene_iss"));
	TH1D *h_tsu_in = dynamic_cast<TH1D*>(f_tsu->Get("h_trig_ave"));
	if(h_gcx_in == nullptr || h_tsu_in == nullptr){
		cerr<<"ERR MAIN ===== missing trig hist"<<endl;
		return 1;
	}

	TH1D *h_gcx = dynamic_cast<TH1D*>(h_gcx_in->Clone("hgcx_trig"));
	TH1D *h_tsu = dynamic_cast<TH1D*>(h_tsu_in->Clone("htsu_trig"));
	h_gcx->SetDirectory(nullptr);
	h_tsu->SetDirectory(nullptr);
	RESET_TSU_ERR(h_tsu);

	cout<<"IN MAIN ===== input gcx: "<<fpath_gcx<<endl
		<<"IN MAIN ===== input tsu: "<<fpath_tsu<<endl
		<<"IN MAIN ===== gcx hist: "<<h_gcx_in->GetName()
		<<" class="<<h_gcx_in->ClassName()
		<<" nb="<<h_gcx_in->GetNbinsX()
		<<" x=["<<h_gcx_in->GetXaxis()->GetXmin()<<","<<h_gcx_in->GetXaxis()->GetXmax()<<"]"<<endl
		<<"IN MAIN ===== tsu hist: "<<h_tsu_in->GetName()
		<<" class="<<h_tsu_in->ClassName()
		<<" nb="<<h_tsu_in->GetNbinsX()
		<<" x=["<<h_tsu_in->GetXaxis()->GetXmin()<<","<<h_tsu_in->GetXaxis()->GetXmax()<<"]"<<endl
		<<endl;

	if(!DRAW_PAIR(fout, h_gcx, h_tsu, outdir)){
		delete h_gcx;
		delete h_tsu;
		fout->Close();
		f_gcx->Close();
		f_tsu->Close();
		return 2;
	}

	delete h_gcx;
	delete h_tsu;

	fout->Write();
	fout->Close();
	f_gcx->Close();
	f_tsu->Close();

	cout<<"IN MAIN ===== output root: "<<outdir + "/trig_compare.root"<<endl;
	return 0;
}
