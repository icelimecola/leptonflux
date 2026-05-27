#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TString.h"
#include "TMath.h"
#include "TF1.h"
#include <vector>
#include <iostream>
using namespace std;
#include "TGaxis.h"
#include "TROOT.h"


int IsPositron = 0;
int IsFineBinning = 0;

TGraphErrors *convert_TH1_to_graph(TH1* h1, double xmin=0){
	int np=0;
	//xmin=3;
	TGraphErrors *gret = new TGraphErrors();
	for(int i=0; i<h1->GetNbinsX(); i++){
		if( h1->GetBinError(i+1)>0 ){
			if( h1->GetBinCenter(i+1)<xmin) continue;
			gret->SetPoint(np, h1->GetBinCenter(i+1), h1->GetBinContent(i+1));
			gret->SetPointError(np, 0, h1->GetBinError(i+1));
			np++;
		}
	}
	return gret;
}

TGraphErrors* divideGraph(TGraphErrors *g1, TGraphErrors *g2){
	TGraphErrors *graph_ratio = new TGraphErrors();
	graph_ratio->SetNameTitle(Form("%s_%s_ratio", g1->GetName(), g2->GetName()), g1->GetTitle());
	int np=0;
	double *x1 = g1->GetX();
	double *y1 = g1->GetY();
	double *ey1 = g1->GetEY();
	double *x2 = g2->GetX();
	double *y2 = g2->GetY();
	double *ey2 = g2->GetEY();
	if( g1->GetN() > g2->GetN() ){
		cerr << "Bins incompatible, exit" << endl;
		return 0;
	}else{
		for(int i=0; i<g1->GetN(); i++){
			graph_ratio->SetPoint(i, x1[i], y1[i]/y2[i]);
			graph_ratio->SetPointError(i, 0, sqrt( ey1[i]*ey1[i]/y1[i]/y1[i] + ey2[i]*ey2[i]/y2[i]/y2[i] )*y1[i]/y2[i] );
		}
		return graph_ratio;
	}
}


void addGraphWithWeight(TGraphErrors *gout, TGraphErrors *gin, TH1D *hwgt ){
	double *x = gin->GetX();
	double *y = gin->GetY();
	double *ey = gin->GetEY();
	if( gout==0 || gout->GetN()<1 ){
		for(int i=0; i<gin->GetN(); i++){
			cout << "hwgt->FindBin( TMath::Min(60.0, x[i]) )=" << hwgt->GetBinContent( hwgt->FindBin( TMath::Min(60.0, x[i]) ) ) << endl;
			gout->SetPoint( i, x[i], y[i]*hwgt->GetBinContent( hwgt->FindBin( TMath::Min(60.0, x[i]) ) ) );
			gout->SetPointError( i, 0, ey[i]*hwgt->GetBinContent( hwgt->FindBin(TMath::Min(60.0, x[i])) ) );
		}
	}
	else{
		if( gout->GetN() != gin->GetN() ){
			cerr << "-- addGraphWithWeight -- Error -- Incompatible bins" << endl;
			return;
		}
		double *y1 = gout->GetY();
		double *ey1 = gout->GetEY();
		for(int i=0; i<gin->GetN(); i++){
			double wgt = hwgt->GetBinContent( hwgt->FindBin(TMath::Min(60.0, x[i])) );
			gout->SetPoint( i, x[i], (wgt*y[i] + y1[i]) );
			gout->SetPointError( i, 0, sqrt((wgt*wgt*ey[i]*ey[i] + ey1[i]*ey1[i])) );
		}
	}
}

TH1D *_calcAcceptance(TString fnm_sel, TString fnm_gen, int icut, double emin, double emax, double nplane=1.0){
	//======== geoacc
	double A0 = nplane*3.9*3.9*TMath::Pi()*1e4;	//----cm^2sr
	//======== hsel
	TFile *file_sel = new TFile(fnm_sel);
	TH1F *h1sel = dynamic_cast<TH1F*>( file_sel->Get(Form("h1MCEne_cut%02d", icut)) );
	//======== hgen
 	TFile *file_gen = new TFile( fnm_gen );
	TH1F *h1gen;
  	if( IsFineBinning ) h1gen = dynamic_cast<TH1F*>( file_gen->Get("hmcspectra_finebinning") );
	else h1gen = dynamic_cast<TH1F*>( file_gen->Get("hgen") );
	//======== hacc
	TH1D *hacc = (TH1D*)h1gen->Clone("hacc");
	hacc->SetNameTitle( "hacc", "Acceptance;Energy[GeV];Acceptance[cm^2sr]" );
	//======== traverse
	int np=0;
	for(int ix=0; ix<h1sel->GetNbinsX(); ix++){
		//==== getsel
		double elow = h1sel->GetBinLowEdge(ix+1);
		double eup  = h1sel->GetBinLowEdge(ix+2);
		double nsel = h1sel->GetBinContent(ix+1);
		double nsel_err = h1sel->GetBinError(ix+1)/nsel;
		//if( elow < emin || eup > emax ) continue;
		if( nsel==0 ) continue;
		//==== getgen
		double ngen = h1gen->GetBinContent(ix+1);
		if( ngen < 1 ) continue;
		//==== calc acc
		double acc = nsel/ngen*A0;
		double acc_err = nsel_err*acc;
		hacc->SetBinContent(ix+1, acc );
		hacc->SetBinError(ix+1, acc_err);
		//==== end
		np++;
	}
	//======== return
	return hacc;
}



void ReweightMCMatrix(TH2F *hmatrix, TH1D *hflux, TH1F *hgen, double xmin, double xmax, int icut=-1){
	double xlow, xup;
	double mc_integral, flux_integral, weight;
	//======== reweight matrix
	//====traverse
	for(int ix=0; ix<hmatrix->GetNbinsX(); ix++){
		//====ene
		xlow = hmatrix->GetXaxis()->GetBinLowEdge(ix+1);
		xup  = hmatrix->GetXaxis()->GetBinLowEdge(ix+2);
		// if( xup < xmin ) continue;
		// if( xlow > xmax ) continue;
		// if( xlow < xmin ) xlow = xmin;
		// if( xup  > xmax ) xup  = xmax;
		//====weight
		flux_integral = hflux->GetBinContent( hflux->FindBin(0.5*(xlow+xup)) )*(xup - xlow);
		mc_integral = hgen->GetBinContent(hgen->FindBin(0.5*(xlow+xup)));
		if( mc_integral<=0 ) continue;
		weight = flux_integral/mc_integral;
		//====reweight
		for(int iy=0; iy<hmatrix->GetNbinsY(); iy++){
			hmatrix->SetBinContent(ix+1, iy+1, hmatrix->GetBinContent(ix+1, iy+1)*weight);
			hmatrix->SetBinError(ix+1, iy+1, hmatrix->GetBinError(ix+1, iy+1)*weight);
		}
	}
}

double _FluxModel_Positron(double *x, double *par){
	// par[0]=1/Es [TeV^-1], par[1]=Cs, par[2]=gamma_s, par[3]=Cd, par[4]=gamma_d, par[5]=phi_e+ [GeV]
	const double E1 = 7.0;   // GeV
	const double E2 = 60.0;  // GeV
	double E = x[0];
	double Ehat = E + par[5];
	if( E<=0 || Ehat<=0 ) return 0;

	double invEs_GeVinv = par[0]*1e-3; // convert TeV^-1 to GeV^-1
	double diffuse = par[3]*pow(Ehat/E1, par[4]);
	double source = par[1]*pow(Ehat/E2, par[2])*exp(-Ehat*invEs_GeVinv);
	return (E*E)/(Ehat*Ehat)*(diffuse + source);
}

TF1* FitFluxModel(TH1D *hfluxmodel, double xmin=0.5, double xmax=1000.0){
	//======== check input
	if( hfluxmodel==0 ){
		cerr << "FitFluxModel Error: null histogram" << endl;
		return 0;
	}
	//======== ensure there are usable points for TH1::Fit (requires positive bin error)
	int nfit_points = 0;
	for(int ibin=1; ibin<=hfluxmodel->GetNbinsX(); ibin++){
		double xc = hfluxmodel->GetBinCenter(ibin);
		if( xc<xmin || xc>xmax ) continue;
		double yc = hfluxmodel->GetBinContent(ibin);
		double ey = hfluxmodel->GetBinError(ibin);
		if( yc>0 && ey>0 ) nfit_points++;
	}
	if( nfit_points<3 ){
		cerr << "FitFluxModel Warning: insufficient fit points (" << nfit_points << ")" << endl;
		return 0;
	}
	//======== init fit function
	TString fname = Form("%s_fit", hfluxmodel->GetName());
	TF1 *fflux_fit = new TF1(fname, _FluxModel_Positron, xmin, xmax, 6);
	fflux_fit->SetNpx(1000);
	fflux_fit->SetParNames("invEs", "Cs", "gamma_s", "Cd", "gamma_d", "phi_eplus");
	fflux_fit->SetParameters(1.23, 6.80e-5, -2.58, 6.51e-2, -4.07, 1.10);
	//======== parameter limits
	fflux_fit->SetParLimits(0, 1e-4, 20.0);      // invEs [TeV^-1]
	fflux_fit->SetParLimits(1, 1e-8, 1.0);       // Cs
	fflux_fit->SetParLimits(2, -8.0, -0.5);      // gamma_s
	fflux_fit->SetParLimits(3, 1e-6, 1.0);       // Cd
	fflux_fit->SetParLimits(4, -8.0, -0.5);      // gamma_d
	fflux_fit->SetParLimits(5, 0.0, 5.0);        // phi_eplus [GeV]
	//======== fit
	int fit_status = hfluxmodel->Fit(fflux_fit, "RQM0");
	if( fit_status!=0 ){
		cerr << "FitFluxModel Warning: fit status = " << fit_status << endl;
	}
	//======== return
	return fflux_fit;
}

// void ReweightMCMatrix(TH2F *hmatrix, TH1D *hflux){
// 	// x axis -> MC rigidity
// 	// y axis -> measured rigidity
// 	//
// 	double xlow, xup;
// 	double mc_integral, flux_integral, weight;
// 	// reweight matrix
// 	for(int ix=0; ix<hmatrix->GetNbinsX(); ix++){
// 		xlow = hmatrix->GetXaxis()->GetBinLowEdge(ix+1);
// 		xup  = hmatrix->GetXaxis()->GetBinLowEdge(ix+2);
// 		mc_integral = log(xup) - log(xlow);
// 		flux_integral = hflux->GetBinContent( hflux->FindBin(0.5*(xlow+xup)) )*(xup - xlow);
// 		if( mc_integral<=0 ) continue;
// 		weight = flux_integral/mc_integral;
// 		for(int iy=0; iy<hmatrix->GetNbinsY(); iy++){
// 			hmatrix->SetBinContent(ix+1, iy+1, hmatrix->GetBinContent(ix+1, iy+1)*weight);
// 			hmatrix->SetBinError(ix+1, iy+1, hmatrix->GetBinError(ix+1, iy+1)*weight);
// 		}
// 	}
// 	// done
// }

TH1D *_calcFoldedAcceptance(TString fnm_gen, TString fnm_sel, int icut, double emin, double emax, TFile* fout,double nplane=1.0){
	//======== geoacc
	double A0 = nplane*3.9*3.9*TMath::Pi()*1e4; // acceptance of generation plane [cm^2 sr]
	//======== hsel
	TFile *filesel = new TFile(fnm_sel,"read");
	TH2F *hmatrix = dynamic_cast<TH2F*>(filesel->Get( Form("h2Ene3D_MCEne_cut%02d", icut)));
	hmatrix->SetName("hmatrix");
	//======== hgen
	TFile *file_gen = new TFile(fnm_gen,"read");
	TH1F *hgen;
	if( IsFineBinning ) hgen = dynamic_cast<TH1F*>( file_gen->Get("hgen_finebin") );
	else hgen = dynamic_cast<TH1F*>( file_gen->Get("hgen") );
	//======== hmatrix_temp
	TH2F *hmatrix_temp = (TH2F*)hmatrix->Clone("h2d_temp");
	//======== hrec0
	TH1D *hrec0 = dynamic_cast<TH1D*>( hmatrix_temp->ProjectionY( "hrec0", 1,  hmatrix_temp->GetNbinsX() ) );
	TH1D *hrec = (TH1D*)hrec0->Clone("hrec");
	TH1D *hacc = (TH1D*)hrec0->Clone("hacc");
  	//======== flux--tsu
	// TFile *file_fluxmodel;
  	// // if( IsFineBinning )  file_fluxmodel = new TFile("/afs/cern.ch/work/w/wxu/work/ams/analysis/positron/Positron2017/WXU/Acceptance/fluxmodel_tsu.root");
	// if( IsFineBinning )  file_fluxmodel = new TFile("./fluxmodel_tsu_finebin.root");
	// else file_fluxmodel = new TFile("./fluxmodel_tsu.root");
	// TH1D *hflux;
	// if( IsPositron ) hflux = dynamic_cast<TH1D*>(file_fluxmodel->Get("hposflux_model"));
	// else             hflux = dynamic_cast<TH1D*>(file_fluxmodel->Get("heleflux_model"));
  	//======== flux--prl122 260322
	// TFile *file_fluxmodel = new TFile( "./datain/posflux.root", "read" );
	// TH1D *hfluxmodel = dynamic_cast<TH1D*>( file_fluxmodel->Get("hflux") );
  	//======== flux--gcx 260412
	TFile *file_fluxmodel = new TFile( "./datain/hrawflux.root", "read" );
	TH1D *hrawflux = dynamic_cast<TH1D*>( file_fluxmodel->Get("h_rawflux") );
	TH1D *hrawflux_noacc = dynamic_cast<TH1D*>( file_fluxmodel->Get("rawflux_noacc") );
	//======== hflux_temp
	TH1D *hflux_temp = (TH1D*)hrawflux->Clone("hflux_temp");
	TH1D *hflux_temp02 = (TH1D*)hrawflux->Clone("hflux_temp02");
	TF1 *hflux_fit = 0;
	TH1D *hfluxmodel_expand = (TH1D*)hrec0->Clone("hfluxmodel_expand");
	hfluxmodel_expand->Reset();
	// //======== flux--gcx
	// //----file_sel
	// TFile *file_rawflux;
	// // file_rawflux = new TFile("rawflux_ene.root");
	// file_rawflux = new TFile("./datain/rawflux_ene.root");
	// //----rawflux
	// // TH1D *hflux;
	// // hflux = dynamic_cast<TH1D*>(file_rawflux->Get("h_rawflux"));
	// // hflux->SetDirectory(0);
	// // hflux->SetName("hflux");
	// //----flux0
	// TH1D *hflux0;
	// hflux0 = dynamic_cast<TH1D*>(file_rawflux->Get("h_rawflux0"));
	// hflux0->SetDirectory(0);
	// hflux0->SetName("hflux0");
	// //----flux1
	// TH1D *hflux1;
	// hflux1 = dynamic_cast<TH1D*>(file_rawflux->Get("h_rawflux1"));
	// hflux1->SetDirectory(0);
	// hflux1->SetName("hflux1");
	// //----flux_diff
	// TH1D *hflux_diff;
	// hflux_diff = dynamic_cast<TH1D*>(file_rawflux->Get("h_rawflux_diff"));
	// hflux_diff->SetDirectory(0);
	// hflux_diff->SetName("hflux_diff");
	// //----rawflux2
	// TFile *file_rawflux2;
	// // file_rawflux2 = new TFile("unfoldedflux_ic0.root");
	// file_rawflux2 = new TFile("./datain/unfoldedflux_ic0.root");
	// TH1D *hflux;
	// hflux = dynamic_cast<TH1D*>(file_rawflux2->Get("hflux1_add"));
	// hflux->SetDirectory(0);
	// hflux->SetName("hflux");
	// //======== fluxfit--gcx
	// //----file_sel
	// TFile *file_fluxmodel;
	// // file_fluxmodel = new TFile("./fluxmodel_GCX01.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX02--0.98.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX03--0.97.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX04--0.95.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX05--gcx.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX06--gcx.root");
	// // file_fluxmodel = new TFile("./fluxmodel_GCX07--gcx.root");
	// // file_fluxmodel = new TFile("fluxmodel_GCX08--gcx.root");
	// file_fluxmodel = new TFile("./datain/fluxmodel_GCX08--gcx.root");
	// //----hflux_fit
	// TH1D *hflux_fit;
	// hflux_fit = dynamic_cast<TH1D*>(file_fluxmodel->Get("h_fluxfit"));
	// hflux_fit->SetDirectory(0);
	// hflux_fit->SetName("hflux_fit");
	// //======== flux--add
	// //----hflux_add
	// TH1D *hflux_add = (TH1D*)hflux_fit->Clone();
	// for(int ibin=1; ibin<=hflux->GetNbinsX(); ibin++){
	// 	double elow = hflux->GetBinLowEdge(ibin);
	// 	double eup = hflux->GetBinLowEdge(ibin+1);
	// 	int bin_fit = hflux_add->FindBin(0.5*(elow+eup));
	// 	hflux_add->SetBinContent(bin_fit, hflux->GetBinContent(ibin));
	// 	hflux_add->SetBinError(bin_fit, hflux->GetBinError(ibin));
	// }
	// hflux_add->SetDirectory(0);
	// hflux_add->SetName("hflux_add");
	// //----hflux0_add
	// TH1D *hflux0_add = (TH1D*)hflux_fit->Clone();
	// for(int ibin=1; ibin<=hflux0->GetNbinsX(); ibin++){
	// 	double elow = hflux0->GetBinLowEdge(ibin);
	// 	double eup = hflux0->GetBinLowEdge(ibin+1);
	// 	int bin_fit = hflux0_add->FindBin(0.5*(elow+eup));
	// 	hflux0_add->SetBinContent(bin_fit, hflux0->GetBinContent(ibin));
	// 	hflux0_add->SetBinError(bin_fit, hflux0->GetBinError(ibin));
	// }
	// hflux0_add->SetDirectory(0);
	// hflux0_add->SetName("hflux0_add");
	// //----hflux1_add
	// TH1D *hflux1_add = (TH1D*)hflux_fit->Clone();
	// for(int ibin=1; ibin<=hflux1->GetNbinsX(); ibin++){
	// 	double elow = hflux1->GetBinLowEdge(ibin);
	// 	double eup = hflux1->GetBinLowEdge(ibin+1);
	// 	int bin_fit = hflux1_add->FindBin(0.5*(elow+eup));
	// 	hflux1_add->SetBinContent(bin_fit, hflux1->GetBinContent(ibin));
	// 	hflux1_add->SetBinError(bin_fit, hflux1->GetBinError(ibin));
	// }
	// hflux1_add->SetDirectory(0);
	// hflux1_add->SetName("hflux1_add");
	// //----hflux_diff_add
	// TH1D *hflux_diff_add = (TH1D*)hflux_fit->Clone();
	// for(int ibin=1; ibin<=hflux_diff->GetNbinsX(); ibin++){
	// 	double elow = hflux_diff->GetBinLowEdge(ibin);
	// 	double eup = hflux_diff->GetBinLowEdge(ibin+1);
	// 	int bin_fit = hflux_diff_add->FindBin(0.5*(elow+eup));
	// 	hflux_diff_add->SetBinContent(bin_fit, hflux_diff->GetBinContent(ibin));
	// 	hflux_diff_add->SetBinError(bin_fit, hflux_diff->GetBinError(ibin));
	// }
	// hflux_diff_add->SetDirectory(0);
	// hflux_diff_add->SetName("hflux_diff_add");
	
	// //======== fluxtemp
	// TH1D* hflux_temp = (TH1D*)hflux_add->Clone();
	// hflux_temp->SetDirectory(0);
	// hflux_temp->SetName("hflux_temp");
	//======== loopcontrol
	// int ic_max = 1;
	int ic_max = 10;
	int ic_save = 0;
	//======== calc acc
	double elow,eup,nrec,nrec_err,ngen,flux,weight,acc,acc_err,flux0,flux1,flux_diff;
	double fit_xmin = hrec0->GetBinLowEdge(1);
	double fit_xmax = hrec0->GetBinLowEdge(hrec0->GetNbinsX()+1);
	int iter_converged = 0;
	for(int ic=0;ic<ic_max;ic++){
		//======== cut--index
		// if(icut!=15 || icut!=16) continue;
		if(icut<15 ) continue;
		//======== fit before reweight
		cout<<"01"<<endl;
		hflux_fit = FitFluxModel(hflux_temp, fit_xmin, fit_xmax);
		cout<<"02"<<endl;
		if( hflux_fit ) hflux_fit->SetName(Form("hflux_fit_iter%02d", ic));
		cout<<"03"<<endl;
		hfluxmodel_expand->Reset();
		cout<<"04"<<endl;
		for(int ibin=1; ibin<=hfluxmodel_expand->GetNbinsX(); ibin++){
			double elow_bin = hfluxmodel_expand->GetBinLowEdge(ibin);
			double eup_bin = hfluxmodel_expand->GetBinLowEdge(ibin+1);
			double flux_int = 0;
			if( hflux_fit ) flux_int = hflux_fit->Integral(elow_bin, eup_bin);
			hfluxmodel_expand->SetBinContent(ibin, flux_int);
			hfluxmodel_expand->SetBinError(ibin, 0);
		}
		cout<<"05"<<endl;
		//======== reset matrix for each iteration
		delete hmatrix_temp;
		hmatrix_temp = (TH2F*)hmatrix->Clone("h2d_temp");
		//======== reweight
		cout<<"ReweightMCMatrix"<<endl;
		// ReweightMCMatrix( h2d, hflux );
		ReweightMCMatrix( hmatrix_temp, hfluxmodel_expand, hgen, emin, emax ,icut);
		cout<<"ReweightMCMatrix02"<<endl;
		//======== calc acc
		hrec = dynamic_cast<TH1D*>( hmatrix_temp->ProjectionY( "hrec", 1,  hmatrix_temp->GetNbinsX() ) );
		hacc = (TH1D*)(hrec->Clone("hacc"));
		for(int ix=0; ix<hrec->GetNbinsX(); ix++){
			//====ene
			elow = hrec->GetBinLowEdge(ix+1);
			eup  = hrec->GetBinLowEdge(ix+2);
			// if( elow < emin || eup > emax ) continue;
			//====nrec
			nrec = hrec->GetBinContent(ix+1);
			if( nrec==0 ) continue;
			nrec_err = hrec->GetBinError(ix+1)/nrec;
			//====flux
			// flux = hflux_temp->GetBinContent( hflux_temp->FindBin( 0.5*(elow+eup) ) )*(eup - elow);
			flux = hfluxmodel_expand->GetBinContent( hfluxmodel_expand->FindBin( 0.5*(elow+eup) ) )*(eup - elow);
			//====calc acc
			hacc->SetBinContent(ix+1, 0 );
			hacc->SetBinError(ix+1, 0);
			if(flux>0){
				acc =nrec/flux*A0;
				acc_err = nrec_err*acc;
				hacc->SetBinContent(ix+1, acc );
				hacc->SetBinError(ix+1, acc_err);
			}
			// //----hflux1
			// int bin_hflux = hflux->FindBin(0.5*(elow+eup));
			// if(hflux->GetBinContent(bin_hflux) > 0 && acc > 0){
			// 	flux0 = hflux0_add->GetBinContent(hflux0_add->FindBin(0.5*(elow+eup)));
			// 	if(flux0 > 0){
			// 		flux1 = flux0/(acc/1e4);
			// 		int bin_fit = hflux1_add->FindBin(0.5*(elow+eup));
			// 		hflux1_add->SetBinContent(bin_fit, flux1);
			// 		hflux1_add->SetBinError(bin_fit, 0);
			// 	}
			// }
			// //----hflux_diff
			// double rawflux = hflux_temp->GetBinContent(hflux_temp->FindBin(0.5*(elow+eup)));
			// double unfoldedflux = hflux1_add->GetBinContent(hflux1_add->FindBin(0.5*(elow+eup)));
			// if(unfoldedflux > 0){
			// 	flux_diff = fabs(unfoldedflux-rawflux)/unfoldedflux;
			// 	hflux_diff_add->SetBinContent(hflux_diff_add->FindBin(0.5*(elow+eup)), flux_diff);
			// 	hflux_diff_add->SetBinError(hflux_diff_add->FindBin(0.5*(elow+eup)), 0);
			// }
			// 	// if(icut==15 && ix>60 && ix<70) cout<<"nacc "<<nacc<<" ngen "<<ngen<<" flux "<<flux<<" weight "<<weight<<" acc "<<acc<<endl;
			// 	if(ix>60 && ix<70) cout<<"nacc "<<nrec<<" ngen "<<ngen<<" flux "<<flux<<" weight "<<weight<<" acc "<<acc<<endl;
		}
		//======== update fluxtemp with rawflux_noacc / (acc/1e4)
		double max_rel_dev = 0;
		for(int ibin=1; ibin<=hflux_temp->GetNbinsX(); ibin++){
			hflux_temp02->SetBinContent(ibin, hflux_temp->GetBinContent(ibin));
			hflux_temp02->SetBinError(ibin, hflux_temp->GetBinError(ibin));
			double elow_bin = hflux_temp->GetBinLowEdge(ibin);
			double eup_bin = hflux_temp->GetBinLowEdge(ibin+1);
			double ecenter = 0.5*(elow_bin+eup_bin);
			double acc_bin = hacc->GetBinContent( hacc->FindBin(ecenter) );
			double raw_noacc_bin = hrawflux_noacc->GetBinContent( hrawflux_noacc->FindBin(ecenter) );
			if( acc_bin>0 ){
				double new_flux = raw_noacc_bin/(acc_bin/1e4);
				hflux_temp02->SetBinContent(ibin, new_flux);
				double old_flux = hflux_temp->GetBinContent(ibin);
				double old_err  = hflux_temp->GetBinError(ibin);
				double new_err  = 0;
				if( old_flux>0 && old_err>0 ){
					new_err = new_flux*(old_err/old_flux);
				}
				else if( new_flux>0 ){
					new_err = 0.05*new_flux;
				}
				hflux_temp02->SetBinError(ibin, new_err);
				if( old_flux>0 ){
					double rel_dev = fabs(new_flux-old_flux)/old_flux;
					if( rel_dev>max_rel_dev ) max_rel_dev = rel_dev;
				}
			}
		}
		cout<<"iter "<<ic<<" max_rel_dev="<<max_rel_dev<<endl;
		if( max_rel_dev<1e-3 ){
			iter_converged = 1;
			break;
		}
		hflux_temp->Reset();
		hflux_temp->Add(hflux_temp02);
		// //----save
		// // if(icut==15 && ic==ic_save){
		// if(ic==ic_save){
		// 	fout->cd();
		// 	hmatrix->Write();
		// 	hrec0->Write();
		// 	hgen->Write();
		// 	hflux->Write();
		// 	hflux0->Write();
		// 	hflux1->Write();
		// 	hflux_diff->Write();
		// 	hflux_fit->Write();
		// 	hflux0_add->Write();
		// 	hflux_add->Write();
		// 	hflux_temp->Write();
		// 	hmatrix_temp->Write();
		// 	hrec->Write();
		// 	hacc->Write();
		// 	hflux1_add->Write();
		// 	hflux_diff_add->Write();
		// }
		// //----control
		// cout<<"calc:icut: "<<icut<<" ic: "<<ic<<endl;
	}
	if(icut>=15 && !iter_converged) cout<<"Warning: iteration not converged within ic_max="<<ic_max<<endl;
	//======== save
	if(icut==15){
		fout->cd();
		hmatrix->Write();
		hrec0->Write();
		hgen->Write();
		hmatrix_temp->Write();
		hrec->Write();
		hacc->Write();
	}
	//======== return
	return hacc;
}


void DRAW_Acceptance(TH1D *h1,TString foutname,int icut){
    //============================ DEC
    TH1D *h1d_temp;
    TAxis *xaxis,*yaxis,*zaxis;
    //============================ INIT
    TCanvas ins_can("can_acc","can_acc", 1000,700);
    h1d_temp = h1;
    xaxis=h1d_temp->GetXaxis();
    yaxis=h1d_temp->GetYaxis();
    zaxis=h1d_temp->GetZaxis();
    //============================ CANVAS
    ins_can.SetTopMargin(0.13);
    ins_can.SetBottomMargin(0.15);
    ins_can.SetLeftMargin(0.13);
    ins_can.SetRightMargin(0.08);
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogx();
    //============================ HIST
    // h1d_temp->SetNameTitle("",Form("Energy %g to %g GeV",energy_bins[bin_ene],energy_bins[bin_ene+1]));
    // h1d_temp->SetNameTitle("",Form("cut%02d",icut));
    h1d_temp->SetStats(0);
    //============================ GSTYLE
	// gStyle->SetOptStat(0);
	gStyle->SetTitleFont(62, "T");
    gStyle->SetTitleSize(0.05,"t");
    gStyle->SetTitleAlign(22);       // 设置全局标题对齐方式为居中靠上
    gStyle->SetTitleX(0.5);          // 设置标题在X方向上的位置（归一化坐标，0.5居中）
    gStyle->SetTitleY(0.94);         // 设置标题在Y方向上的位置（归一化坐标，0.98靠近顶部）
    // gStyle->SetTitleOffset(5,"t");     // 设置全局标题偏移量
    //============================ X
    // xaxis->SetNameTitle("Date","Date");
    xaxis->SetNameTitle("Energy[GeV]","Energy[GeV]");
    xaxis->CenterTitle();
    xaxis->SetTitleFont(62);
    // xaxis->SetTitleSize(0.045);
    xaxis->SetTitleSize(0.05);
    // xaxis->SetTitleOffset(1.2); 
    xaxis->SetTitleOffset(1.2);
    xaxis->SetLabelOffset(0.025);
    xaxis->SetNdivisions(-505);
    //---- time display
    // gStyle->SetTimeOffset(0);
    // xaxis->SetTimeDisplay(1);
    // // xaxis->SetTimeFormat("%m/%d/%Y");
    // xaxis->SetTimeFormat("%b/%d/%Y");
    //---- xrange
    // double x1 = xaxis->GetBinLowEdge(1);
    // double x2 = xaxis->GetBinLowEdge(2369);
    // double x2 = xaxis->GetBinLowEdge(1700); //---- 2025-10-19
    // double x2 = xaxis->GetBinLowEdge(2295); //---- 2025-10-22
    // double x2 = xaxis->GetBinLowEdge(4949); //---- 2025-10-26
    // double x1 = xaxis->GetBinLowEdge(21);       //---- 2025-10-30\
    // double x2 = xaxis->GetBinLowEdge(4582); //---- 2025-10-30
    // xaxis->SetRangeUser(x1,x2);
    // xaxis->SetRangeUser(0.09,101);
	// gPad->Modified();
	// gPad->Update();
    //============================ Y
    yaxis->SetNameTitle("Acceptance","MC Truth Acceptance[cm^{2}sr]");
    yaxis->CenterTitle();
    yaxis->SetTitleFont(62);
    // yaxis->SetTitleSize(0.045);
    yaxis->SetTitleSize(0.05);
    yaxis->SetTitleOffset(1);
    yaxis->SetLabelOffset(0.012);
    TGaxis::SetMaxDigits(3);
    //============================ DRAW
        h1d_temp->SetMarkerStyle(20);  // 20 是实心圆
		h1d_temp->SetMarkerSize(1.5);  // 大小
		// h1d_temp->SetMarkerColor(kBlue+1); // 或 color[0]
		h1d_temp->SetMarkerColor(kRed);
		h1d_temp->SetLineColor(kRed);
	h1d_temp->Draw("E1X0P");
    //============================ SAVE
    // ins_can.Write();
    TString dir="";
    dir="./data/pdf_num/";
    dir="./";
    // ins_can.SaveAs(dir+Form("EleNum_enebin%02d_%gGeV.pdf", bin_ene, energy_bins[bin_ene]));
    ins_can.SaveAs(dir+foutname+".pdf");
}


int main(){
	//============ init ============
	//==== filename-in
	TString fname_mc[1][2] = {
		"./datain/mchselect.root","./datain/mchgen.root"
	};
	//====minmax
	double emin,emax;
	emin = 0.25, emax = 200;
	// emin = 0, emax = 100;
	//====hist
	const int nCut = 17;
	// const int nCut = 18;	//----mine
  	TH1D *hacc[nCut]={0}; 
	//====Hypothesis
	//---- 0-> MC truth, 1-> electron, 2-> positron
	int Hypothesis; 
	Hypothesis = 0; 
	// Hypothesis = 1;
	// Hypothesis = 2;
	//====IsPositron
	if( Hypothesis==2 ) IsPositron=1;
	else IsPositron=0;
	//==== filename-out
	TString foutname;
	if( Hypothesis==0 ) foutname = "mcacc.root";
	else if( Hypothesis==1 ) foutname = "unacc_ele.root";
	else foutname = "unacc_pos_CORR.root";
	TFile *fout = new TFile(foutname, "recreate");

	//============ calc&draw ============
	for(int i=0; i<nCut; i++){
		if(Hypothesis==0){
			hacc[i] = _calcAcceptance( fname_mc[0][0], fname_mc[0][1], i, emin, emax );
			hacc[i]->SetNameTitle( Form("hacc_cut%02d", i), Form("Acceptance;MC Energy[GeV];Acceptance[cm^{2}sr]") );
		}
		else{
			hacc[i] = _calcFoldedAcceptance( fname_mc[0][1], fname_mc[0][0], i, emin, emax, fout );
			hacc[i]->SetNameTitle( Form("hacc_cut%02d", i), Form("Acceptance;Reconstructed Energy[GeV];Acceptance[cm^{2}sr]") );
		}
		if(i==15||i==16) DRAW_Acceptance( hacc[i], Form("acceptance_cut%02d", i),i);
	}

	//============ save ============
	fout->cd();
	for(int i=0; i<nCut; i++){if(hacc[i]) hacc[i]->Write();}
	fout->Close();

	//============ return ============
	return 0;
}