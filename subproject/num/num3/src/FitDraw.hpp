_Pragma("once")

#include<iostream>
using namespace std;
#include<TString.h>
#include<TFile.h>
#include<TF1.h>
#include<TH1F.h>
#include<TRandom.h>
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TGraphErrors.h"
#include "TGaxis.h"
#include "TLatex.h"
//==== 
#include "../include/general/RootSaveDir.hpp"
#include "FitDoFit.hpp"

class FitDraw:public FitDoFit{
	public:
        FitDraw():FitDoFit(){}
        ~FitDraw(){}
	public:
		//-- npass
		TH1 *h1d_PosiSample,*h1d_NegaSample;
		TH1 *h1d_PDFproton,*h1d_PDFelectron;
		TH1 *h1d_nproton,*h1d_npositron,*h1d_nSum_pospro;
		TH1 *h1d_nccproton,*h1d_nelectron,*h1d_nSum_eleccpro;
		TCanvas *ins_can;
		//-- ntotal
		TH1 *h1d_PosiSample_total,*h1d_NegaSample_total;
		TH1 *h1d_PDFproton_total,*h1d_PDFelectron_total;
		TH1 *h1d_nproton_total,*h1d_npositron_total,*h1d_nSum_pospro_total;
		TH1 *h1d_nccproton_total,*h1d_nelectron_total,*h1d_nSum_eleccpro_total;
		TCanvas *ins_can_ESEFF;
    public:
        // int *color = new int[4]{ kBlack, kRed, kBlue+1, kGreen+1};
        int *color = new int[4]{ kBlack, kRed, kBlue, kGreen+1};
    public:
        void FITDRAW_InitDraw(int,int,int);
		void FITDRAW_DoDraw(int,int,double);
		void FITDRAW_DoDraw_ESEFF(int);
		void FITDRAW_SaveDraw(int,int);
		int FITDRAW_Chi2Test(TH1*,TH1*,double,double,int);
		void FITDRAW_DoChi2Test();
		void FITDRAW_Clear();
};

//-- init draw 需要绘制的h1d
//* i_enebin 用于标题命名
void FitDraw::FITDRAW_InitDraw(int i_enebin,int i_perbin,int i_perbin_save){
	//==================================================== style
    gStyle->SetOptStat(0);
	// gStyle->SetLegendTextSize(0.025);
	//--
	// gStyle->SetPadTopMargin(0.08);
	// gStyle->SetPadRightMargin(0.05);
	// gStyle->SetOptFit(0);
	// gStyle->SetOptLogx(1);
	// gStyle->SetOptLogy(1);
	gStyle->SetFrameLineWidth(2);//和linewidth一起能加粗右上//单独使用全加宽但不包含刻度
	// gStyle->SetHistLineWidth(5);//单独使用没效果
    // gStyle->SetHatchesLineWidth(3.5);
    gStyle->SetLineWidth(2);//单独使用加粗左下坐标轴及刻度
	//==================================================== init_npass
	//==================================== init_Sample
	TString RealVarName=FitVar_lkhd->GetName();
	//-- PosiSample
	//* don't use the original hist because the bin dividing is different between this 2 types of hist
	//* 从拟合的roodatahist里创建h1d
    h1d_PosiSample = FitH_PosiSample->createHistogram(RealVarName, *FitVar_lkhd);
	// h1d_PosiSample = dynamic_cast<TH1*>(h1_PosiSample_lkhd[i_latbin]->Clone("h1d_PosiSample"));
    h1d_PosiSample->SetNameTitle( "h1d_PosiSample", "R>0 sample;#Lambda_{TRD};Probability" );
    h1d_PosiSample->SetTitle( Form("%g < E < %g GeV, R > 0", energy_bins[i_enebin], energy_bins[i_enebin+1]) );
    h1d_PosiSample->SetLineColor(color[0]);
    h1d_PosiSample->SetMarkerColor(color[0]);
	//-- NegaSample
    h1d_NegaSample = FitH_NegaSample->createHistogram(RealVarName, *FitVar_lkhd);
    h1d_NegaSample->SetNameTitle( "h1d_NegaSample", "R<0 sample;#Lambda_{TRD};Probability" );
    h1d_NegaSample->SetTitle( Form("%g < E < %g GeV, R < 0", energy_bins[i_enebin], energy_bins[i_enebin+1]) );
    h1d_NegaSample->SetLineColor(color[0]);
    h1d_NegaSample->SetMarkerColor(color[0]);
	//==================================== init_PDF
	//-- PDFpro
	h1d_PDFproton = FitPdf_pro->createHistogram(RealVarName, *FitVar_lkhd);
    h1d_PDFproton->SetNameTitle( "h1d_PDFproton", "R>0 proton;#Lambda_{TRD};Probability" );
    h1d_PDFproton->SetLineColor( color[1] );
    h1d_PDFproton->SetMarkerColor( color[1] );
	//-- PDFele
    h1d_PDFelectron = FitPdf_ele->createHistogram(RealVarName, *FitVar_lkhd);
    h1d_PDFelectron->SetNameTitle( "h1d_PDFelectron", "R<0 electron;#Lambda_{TRD};Probability" );
    h1d_PDFelectron->SetLineColor( color[2] );
    h1d_PDFelectron->SetMarkerColor( color[2] );
	if(i_perbin==i_perbin_save){
		h1d_PDFproton->Write();
		h1d_PDFelectron->Write();
	}
	//==================================== init_Number_pos
	//-- h1d_nproton
    //* normalization to compare with positive sample
    h1d_nproton = dynamic_cast<TH1*>(h1d_PDFproton->Clone("h1d_nproton"));
    // h1d_nproton->Scale( fitR.npro/h1d_nproton->Integral() );
    h1d_nproton->Scale( fitR.npro/h1d_nproton->Integral(),"nosw2" );
	//-- h1d_npositron
	h1d_npositron = dynamic_cast<TH1*>(h1d_PDFelectron->Clone("h1d_npositron"));
    // h1d_npositron->Scale( fitR.npos/h1d_npositron->Integral() );
    h1d_npositron->Scale( fitR.npos/h1d_npositron->Integral(),"nosw2" );
	//-- h1d_nSum_pospro
	h1d_nSum_pospro = dynamic_cast<TH1*>( h1d_nproton->Clone("h1d_nSum_pospro") );
    h1d_nSum_pospro->Add( h1d_npositron );
    h1d_nSum_pospro->SetLineColor( color[3] );
	h1d_nSum_pospro->SetLineWidth(2);
    h1d_nSum_pospro->SetMarkerColor( color[3] );
	if(i_perbin==i_perbin_save){
		h1d_nproton->Write();
		h1d_npositron->Write();
		h1d_nSum_pospro->Write();
	}
	//==================================== init_Number_neg
	//-- h1d_nccproton
	//* normalization to compare with negative sample
    h1d_nccproton = dynamic_cast<TH1*>(h1d_PDFproton->Clone("h1d_nccproton")); 
    // h1d_nccproton->Scale( fitR.nccpro/h1d_nccproton->Integral(),"nosw2" );
    h1d_nccproton->Scale( fitR.nccpro/h1d_nccproton->Integral());
	//-- h1d_nelectron
	h1d_nelectron = dynamic_cast<TH1*>(h1d_PDFelectron->Clone("h1d_nelectron"));
    // h1d_nelectron->Scale( fitR.nele/h1d_nelectron->Integral(),"nosw2" );
    h1d_nelectron->Scale( fitR.nele/h1d_nelectron->Integral());
	//-- h1d_nSum_eleccpro
    h1d_nSum_eleccpro = dynamic_cast<TH1*>( h1d_nccproton->Clone("h1d_nSum_eleccpro") );
    h1d_nSum_eleccpro->Add( h1d_nelectron );
    h1d_nSum_eleccpro->SetLineColor( color[3] );
	h1d_nSum_eleccpro->SetLineWidth(2);
    h1d_nSum_eleccpro->SetMarkerColor( color[3] );
	if(i_perbin==i_perbin_save){
		h1d_nccproton->Write();
		h1d_nelectron->Write();
		h1d_nSum_eleccpro->Write();
	}
}

//-- 主要用于设置canvas/h1d绘图细节/具体绘制
void FitDraw::FITDRAW_DoDraw(int i_enebin,int i_t,double unixtime){
	//-- init
	// ins_can = new TCanvas( Form("canvas_latbin%02d_perbin%02d", i_latbin, i_perbin), Form("latbin=%d_perbin=%02d", i_latbin, i_perbin), 1000, 500);
	ins_can = new TCanvas( Form("canvas_it%02d", i_t), Form("it=%02d", i_t), 1000, 800);
	ins_can->Divide(2,1);
	TH1 *h1d_temp;
	TAxis *xaxis,*yaxis,*zaxis;
	//==================================================== canvas2_negasample
	ins_can->cd(1);
	gPad->SetLeftMargin(0.15);
	//============================ negasample
	//---- INIT
	h1d_temp=h1d_NegaSample;
	xaxis=h1d_temp->GetXaxis();
	yaxis=h1d_temp->GetYaxis();
	zaxis=h1d_temp->GetZaxis();
	//---- HIST
	gStyle->SetTitleFont(62, "T");
	h1d_temp->SetTitle("Electron");
	//---- X
	xaxis->SetTitle("#Lambda_{ECAL}");
	// xaxis->SetRangeUser(0,10e2*0.3);
	xaxis->CenterTitle();
	xaxis->SetTitleOffset(1);
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(0.042);
	xaxis->SetLabelSize(0.03);
	//---- Y
	yaxis->SetTitle("Number");
	yaxis->CenterTitle(0);
	yaxis->SetTitleOffset(1.5);
	yaxis->SetTitleFont(62);
	yaxis->SetLabelSize(0.03);
	yaxis->SetLabelOffset(0.008);
	TGaxis::SetMaxDigits(4);
	//============================ draw
		h1d_temp->SetMarkerStyle(20);  // 20 是实心圆
		h1d_temp->SetMarkerSize(0.9);  // 大小
		h1d_temp->SetMarkerColor(kBlack); // 或 color[0]
	h1d_temp->Draw("E0X0P");
	h1d_nelectron->SetFillStyle(1001);
	h1d_nelectron->SetFillColor(color[2]);
	// h1d_nelectron->SetLineColor(kBlue+3);
	// h1d_nelectron->SetLineWidth(3);
	h1d_nelectron->Draw("h1same");
	h1d_nccproton->SetFillStyle(1001);
	h1d_nccproton->SetFillColor(color[1]);
	// h1d_nccproton->SetLineColor(kRed+3);
	// h1d_nccproton->SetLineWidth(3);
	h1d_nccproton->Draw("h1same");
	h1d_nSum_eleccpro->Draw("h1same");
	h1d_temp->Draw("E0X0P same");
	//============================ legend
	// TLegend* ins_legend2=new TLegend(0.13,0.6,0.4,0.88);
	TLegend* ins_legend1=new TLegend(0.13+0.05,0.68,0.4+0.05,0.88);
	TLegendEntry *l11=ins_legend1->AddEntry("h1d_NegaSample","NegaSample", "p");
	l11->SetTextColor(color[0]);
	TLegendEntry *l13=ins_legend1->AddEntry("h1d_nelectron","e^{-} signal");
	l13->SetTextColor(color[2]);
	TLegendEntry *l12=ins_legend1->AddEntry("h1d_nccproton","p background");
	l12->SetTextColor(color[1]);
	TLegendEntry *l14=ins_legend1->AddEntry("h1d_nSum_eleccpro","Fit");
	l14->SetTextColor(color[3]);
	ins_legend1->SetTextFont(62);
	ins_legend1->SetTextSize(0.03);
	ins_legend1->SetFillStyle(0);
	ins_legend1->SetBorderSize(0);
	ins_legend1->Draw();
	//==================================================== canvas1_posisample
	ins_can->cd(2);
	//============================ h1_PosiSample_lkhd
	//---- INIT
	h1d_temp=h1d_PosiSample;
	xaxis=h1d_temp->GetXaxis();
	yaxis=h1d_temp->GetYaxis();
	zaxis=h1d_temp->GetZaxis();
	//---- HIST
	gStyle->SetTitleFont(62, "T");
	h1d_temp->SetTitle("Positron");
	//---- X
	xaxis->SetTitle("#Lambda_{ECAL}");
	// xaxis->SetRangeUser(0,10e2*0.3);
	xaxis->CenterTitle();
	xaxis->SetTitleOffset(1);
	xaxis->SetTitleFont(62);
	xaxis->SetTitleSize(0.042);
	xaxis->SetLabelSize(0.03);
	//---- Y
	yaxis->SetTitle("Number");
	yaxis->CenterTitle(0);
	yaxis->SetTitleOffset(1.5);
	yaxis->SetTitleFont(62);
	yaxis->SetLabelSize(0.03);
	yaxis->SetLabelOffset(0.008);
	TGaxis::SetMaxDigits(4);
	//============================ draw
		h1d_temp->SetMarkerStyle(20);  // 20 是实心圆
		h1d_temp->SetMarkerSize(0.9);  // 大小
		h1d_temp->SetMarkerColor(kBlack); // 或 color[0]
	h1d_temp->Draw("E0X0P");
	h1d_npositron->SetFillStyle(1001);
	h1d_npositron->SetFillColor(color[2]);
	// h1d_npositron->SetLineColor(kBlue+3);
	// h1d_npositron->SetLineWidth(3);
	h1d_npositron->Draw("h1same");
	h1d_nproton->SetFillStyle(1001);
	h1d_nproton->SetFillColor(color[1]);
	// h1d_nproton->SetLineColor(kRed+3);
	// h1d_nproton->SetLineWidth(3);
	h1d_nproton->Draw("h1same");
	h1d_nSum_pospro->Draw("h1same");
	h1d_temp->Draw("E0X0P same");
	//============================ legend
	TLegend* ins_legend2=new TLegend(0.13,0.68,0.4,0.88);
	TLegendEntry *l21=ins_legend2->AddEntry("h1d_PosiSample","PosiSample", "p");
	l21->SetTextColor(color[0]);
	TLegendEntry *l23=ins_legend2->AddEntry("h1d_npositron","e^{+} signal");
	l23->SetTextColor(color[2]);
	TLegendEntry *l22=ins_legend2->AddEntry("h1d_nproton","p background");
	l22->SetTextColor(color[1]);
	TLegendEntry *l24=ins_legend2->AddEntry("h1d_nSum_pospro","Fit");
	l24->SetTextColor(color[3]);
	ins_legend2->SetTextFont(62);
	ins_legend2->SetTextSize(0.03);
	ins_legend2->SetFillStyle(0);
	ins_legend2->SetBorderSize(0);
	ins_legend2->Draw();
	//==================================================== 添加总标题
	ins_can->cd(0);  // 回到整个画布（而不是子 pad）
	TLatex latex;
	latex.SetNDC();  // 使用归一化坐标（0~1 之间，独立于 pad）
	latex.SetTextFont(62);  // 粗体
	latex.SetTextSize(0.033);  // 字体大小可调整
	latex.SetTextAlign(22);  // 22 表示居中对齐（水平和垂直都居中）
	latex.DrawLatex(0.5, 0.97, Form("Energy %g to %g GeV",energy_bins[i_enebin],energy_bins[i_enebin+1]));
	// 第二行：时间（从 Unix 时间戳转化）
	time_t t = unixtime;  // 你的 Unix 时间戳变量
	struct tm *tm_info = gmtime(&t); // 若想要GMT+0
	char time_str[32];
	strftime(time_str, sizeof(time_str), "%b/%d/%Y", tm_info);
	latex.SetTextSize(0.028);  // 第二行稍小
	latex.DrawLatex(0.5, 0.935, time_str);  // Y坐标稍往下
}
//===============================================================================  chi2test
int FitDraw::FITDRAW_Chi2Test(TH1* h1,TH1* h2,double chi2,double ndf,int printlv=0){
	if(h1->GetNbinsX() != h2->GetNbinsX() || h1->GetNbinsY() != h2->GetNbinsY()){
		if(printlv==1) cout << "incompatible binning , return " << endl;
		return -1;
	}
	double n1,n2;
	double delta;
	chi2 = 0;
	ndf = 0;
	for(int ix=1; ix<=h1->GetNbinsX(); ix++){
		for(int iy=1; iy<=h1->GetNbinsY(); iy++){
			n1 = h1->GetBinContent(ix,iy);
			n2 = h2->GetBinContent(ix,iy);
			if( n1 > 1 ){
				delta = (n1 - n2)/sqrt(n1); // ignore the statistic error of templates
				chi2 += delta*delta;
				ndf ++;
			}
		}
	}
	return 0;
}
void FitDraw::FITDRAW_DoChi2Test(){
	FITDRAW_Chi2Test( h1d_PosiSample, h1d_nSum_pospro, fitR.chi2pos, fitR.ndfpos );
	// cout<<fitR.ndfpos<<endl;
	fitR.ndfpos -= 2;
	// cout<<fitR.ndfpos<<endl;
	FITDRAW_Chi2Test( h1d_NegaSample, h1d_nSum_eleccpro, fitR.chi2neg, fitR.ndfneg );
	fitR.ndfneg -= 2;
	// cout<<"fitdraw_chi2test--end "<<endl;
}
//=============================================================================== save
void FitDraw::FITDRAW_SaveDraw(int i_enebin,int i_perbin){
		RootSaveDir::Enter("draw");
	ins_can->Write();
		RootSaveDir::Exit();
	// ins_can_ESEFF->Write();
	// cout<<"fitdraw_savecanvas--end"<<endl;
	//==== save file
	int i_latbin=0;
	// TString canvasname=Form("fitresult_draw_enebin%02d_latbin%02d_perbin%02d",i_enebin,i_latbin,i_perbin);
	TString canvasname=Form("fitresult_draw_enebin%02d_perbin%02d",i_enebin,i_perbin);
	// cout<<"canvas name: "<<canvasname<<endl;
	// ins_can->SaveAs(canvasname+".svg");
	ins_can->SaveAs(canvasname+".pdf");
	// delete ins_can;
}
//=============================================================================== delete
void FitDraw::FITDRAW_Clear(){
	delete h1d_PosiSample;
	delete h1d_NegaSample;
	delete h1d_PDFproton;
	delete h1d_PDFelectron;
	delete h1d_nproton;
	delete h1d_npositron;
	delete h1d_nSum_pospro;
	delete h1d_nccproton;
	delete h1d_nelectron;
	delete h1d_nSum_eleccpro;
	ins_can->Close();
	delete ins_can;
	//---- total
	// delete h1d_PosiSample_total;
	// delete h1d_NegaSample_total;
	// delete h1d_PDFproton_total;
	// delete h1d_PDFelectron_total;
	// delete h1d_nproton_total;
	// delete h1d_npositron_total;
	// delete h1d_nSum_pospro_total;
	// delete h1d_nccproton_total;
	// delete h1d_nelectron_total;
	// delete h1d_nSum_eleccpro_total;
	// ins_can_ESEFF->Close();
	// delete ins_can_ESEFF;
}
