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
#include <memory>
#include <iostream>
using namespace std;
#include "TGaxis.h"
#include "TROOT.h"


int IsPositron = 0;
int IsFineBinning = 0;
static const int NENEBIN_TDEP = 54;
static const int FIRST_ENEBIN_TDEP_RAW = 1;
static const int NENEBIN_TDEP_RAW = NENEBIN_TDEP - FIRST_ENEBIN_TDEP_RAW;
static const int NTBIN_27D = 225;
static const double WT_27D = 60.0*60.0*24.0*27.0;
static const double TMIN_27D = 1305417600.0;
static const double TMAX_27D = TMIN_27D + NTBIN_27D*WT_27D;
static const double ENERGY_BINS_TDEP[NENEBIN_TDEP + 1] = {
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

TH1D *_calcMCAcc(TString fnm_sel, TString fnm_gen, int icut, double emin, double emax, double nplane=1.0){
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

TH1D *unacct_init_hene(const TString &hname, const TString &htitle){
	TH1D *hout = new TH1D(hname, htitle, NENEBIN_TDEP_RAW, ENERGY_BINS_TDEP + FIRST_ENEBIN_TDEP_RAW);
	hout->SetDirectory(0);
	hout->Sumw2();
	hout->Reset();
	return hout;
}

TH1D *BUILD_RawFluxFrom1GeV(TH1D *hin, const TString &hname){
	if( hin==0 ) return 0;
	TH1D *hout = unacct_init_hene(hname, hin->GetTitle());
	for(int ibin=1; ibin<=hout->GetNbinsX(); ibin++){
		double ecenter = hout->GetBinCenter(ibin);
		int ibin_in = hin->FindBin(ecenter);
		hout->SetBinContent(ibin, hin->GetBinContent(ibin_in));
		hout->SetBinError(ibin, hin->GetBinError(ibin_in));
	}
	return hout;
}

bool unacct_get_rawflux(TFile *file_flux, vector<TH1D*> &vrawflux, vector<TH1D*> &vrawflux_noacc){
	if( file_flux==0 ) return false;
	//====inithene
	vrawflux.clear();
	vrawflux_noacc.clear();
	for(int it=0; it<NTBIN_27D; it++){
		vrawflux.push_back( unacct_init_hene(Form("hrawflux_t%03d", it), Form("hrawflux_t%03d", it)) );
		vrawflux_noacc.push_back( unacct_init_hene(Form("hrawflux_noacc_t%03d", it), Form("hrawflux_noacc_t%03d", it)) );
	}
	for(int ie=FIRST_ENEBIN_TDEP_RAW; ie<NENEBIN_TDEP; ie++){
		//====getht (by ene)
		TH1D *hflux_t = dynamic_cast<TH1D*>( file_flux->Get(Form("hflux_27d_t_ene%02d", ie)) );
		TH1D *hflux_noacc_t = dynamic_cast<TH1D*>( file_flux->Get(Form("hflux_noacc_27d_t_ene%02d", ie)) );
		if( hflux_t==0 || hflux_noacc_t==0 ){
			cerr << "ERR unacct_get_rawflux ===== missing hist for enebin " << ie << endl;
			return false;
		}
		//====fillhene (by time)
		//----use the startbin as bin1
		int ibin_out = ie - FIRST_ENEBIN_TDEP_RAW + 1;
		for(int it=0; it<NTBIN_27D; it++){
			vrawflux[it]->SetBinContent(ibin_out, hflux_t->GetBinContent(it+1));
			vrawflux[it]->SetBinError(ibin_out, hflux_t->GetBinError(it+1));
			vrawflux_noacc[it]->SetBinContent(ibin_out, hflux_noacc_t->GetBinContent(it+1));
			vrawflux_noacc[it]->SetBinError(ibin_out, hflux_noacc_t->GetBinError(it+1));
		}
	}
	return true;
}

TH1D *unacct_init_ht(const TString &hname, const TString &htitle){
	TH1D *hout = new TH1D(hname, htitle, NTBIN_27D, TMIN_27D, TMAX_27D);
	hout->SetDirectory(0);
	hout->Sumw2();
	return hout;
}

TH1D *_calc_unacc_core(TString fnm_sel, TString fnm_gen, int icut, double emin, double emax, TFile *fout, TH1D *hrawflux, TH1D *hrawflux_noacc, TString htag="", int is_save_detail=1, double nplane=1.0){
	//============ init ============
	//==== geoacc
	double A0 = nplane*3.9*3.9*TMath::Pi()*1e4; // acceptance of generation plane [cm^2 sr]
	//==== hgen
	TFile *file_gen = new TFile(fnm_gen,"read");
	TH1F *hgen;
	if( IsFineBinning ) hgen = dynamic_cast<TH1F*>( file_gen->Get("hgen_finebin") );
	else hgen = dynamic_cast<TH1F*>( file_gen->Get("hgen") );
	hgen->SetName(Form("hgen%s", htag.Data()));
	//==== hmatrix
	TFile *file_sel = new TFile(fnm_sel,"read");
	TH2F *hmatrix = dynamic_cast<TH2F*>(file_sel->Get( Form("h2Ene3D_MCEne_cut%02d", icut)));
	// hmatrix->SetName("hmatrix");
	hmatrix->SetName(Form("hmatrix%s", htag.Data()));
	TH2F *hmatrix_re = (TH2F*)hmatrix->Clone(Form("hmatrix_re%s", htag.Data()));
	//==== hrec
	TH1D *hrec_bfre = dynamic_cast<TH1D*>( hmatrix_re->ProjectionY( Form("hrec_bfre%s", htag.Data()), 1,  hmatrix_re->GetNbinsX() ) );
	TH1D *hrec_afre = (TH1D*)hrec_bfre->Clone(Form("hrec_afre%s", htag.Data()));
	TH1D *hunacc = (TH1D*)hrec_bfre->Clone(Form("hunacc%s", htag.Data()));
  	//==== hflux
	//---- tsu
	// TFile *file_fluxmodel;
  	// // if( IsFineBinning )  file_fluxmodel = new TFile("/afs/cern.ch/work/w/wxu/work/ams/analysis/positron/Positron2017/WXU/Acceptance/fluxmodel_tsu.root");
	// if( IsFineBinning )  file_fluxmodel = new TFile("./fluxmodel_tsu_finebin.root");
	// else file_fluxmodel = new TFile("./fluxmodel_tsu.root");
	// TH1D *hflux;
	// if( IsPositron ) hflux = dynamic_cast<TH1D*>(file_fluxmodel->Get("hposflux_model"));
	// else             hflux = dynamic_cast<TH1D*>(file_fluxmodel->Get("heleflux_model"));
	//----gcx 260412
	TH1D *hflux_raw = (TH1D*)hrawflux->Clone(Form("hflux_raw%s", htag.Data()));
	TH1D *hflux_unfold = (TH1D*)hrawflux->Clone(Form("hflux_unfold%s", htag.Data()));
  	//==== hfluxfit
	TF1 *fflux_fit = 0;
	TH1D *hflux_fit = (TH1D*)hrec_bfre->Clone(Form("hflux_fit%s", htag.Data()));
	hflux_fit->Reset();
	//============ traversal ============
	//==== init1
	int ic_max = 1;
	// int ic_max = 10;
	//==== init2
	double elow,eup,nrec,nrec_err,flux,acc,acc_err;
	double rawflux_xmin = hrawflux->GetBinLowEdge(1);
	double fit_xmin = rawflux_xmin;
	double fit_xmax = hrec_bfre->GetBinLowEdge(hrec_bfre->GetNbinsX()+1);
	int iter_converged = 0;
	for(int ic=0;ic<ic_max;ic++){
		//==== cut--index
		// if(icut!=15 || icut!=16) continue;
		if(icut<15 ) continue;
		//============ fit ============
		fflux_fit = FitFluxModel(hflux_raw, fit_xmin, fit_xmax);
		if( fflux_fit ) fflux_fit->SetName(Form("fflux_fit_iter%02d%s", ic, htag.Data()));
		hflux_fit->Reset();
		for(int ibin=1; ibin<=hflux_fit->GetNbinsX(); ibin++){
			double elow_bin = hflux_fit->GetBinLowEdge(ibin);
			double eup_bin = hflux_fit->GetBinLowEdge(ibin+1);
			double flux_int = 0;
			if( fflux_fit ) flux_int = fflux_fit->Integral(elow_bin, eup_bin);
			hflux_fit->SetBinContent(ibin, flux_int);
			hflux_fit->SetBinError(ibin, 0);
		}
		//============ reweight ============
		//======== reset hmatrix_re
		delete hmatrix_re;
		hmatrix_re = (TH2F*)hmatrix->Clone(Form("hmatrix_re%s", htag.Data()));
		//======== reweight
		// ReweightMCMatrix( h2d, hflux );
		ReweightMCMatrix( hmatrix_re, hflux_fit, hgen, emin, emax ,icut);
		//============ acc calc ============
		delete hrec_afre;delete hunacc;
		hrec_afre = dynamic_cast<TH1D*>( hmatrix_re->ProjectionY( Form("hrec_afre%s", htag.Data()), 1,  hmatrix_re->GetNbinsX() ) );
		hunacc = (TH1D*)(hrec_afre->Clone(Form("hunacc%s", htag.Data())));
		for(int ix=0; ix<hrec_afre->GetNbinsX(); ix++){
			//====ene
			elow = hrec_afre->GetBinLowEdge(ix+1);
			eup  = hrec_afre->GetBinLowEdge(ix+2);
			hunacc->SetBinContent(ix+1, 0 );
			hunacc->SetBinError(ix+1, 0);
			//====nrec
			nrec = hrec_afre->GetBinContent(ix+1);
			if( nrec==0 ) continue;
			nrec_err = hrec_afre->GetBinError(ix+1)/nrec;
			//====flux
			// flux = hflux_temp->GetBinContent( hflux_temp->FindBin( 0.5*(elow+eup) ) )*(eup - elow);
			flux = hflux_fit->GetBinContent( hflux_fit->FindBin( 0.5*(elow+eup) ) )*(eup - elow);
			//====calc acc
			if(flux>0){
				acc =nrec/flux*A0;
				acc_err = nrec_err*acc;
				hunacc->SetBinContent(ix+1, acc );
				hunacc->SetBinError(ix+1, acc_err);
			}
		}
		//============ unflux&fluxdiff ============
		//======== update fluxtemp with rawflux_noacc / (acc/1e4)
		double max_fluxreldiff = 0;
		for(int ibin=1; ibin<=hflux_raw->GetNbinsX(); ibin++){
			//====reset
			hflux_unfold->SetBinContent(ibin, hflux_raw->GetBinContent(ibin));
			hflux_unfold->SetBinError(ibin, hflux_raw->GetBinError(ibin));
			//====ecenter
			double elow = hflux_raw->GetBinLowEdge(ibin);
			double eup = hflux_raw->GetBinLowEdge(ibin+1);
			double ecenter = 0.5*(elow+eup);
			//====acc
			double acc = hunacc->GetBinContent( hunacc->FindBin(ecenter) );
			//====flux_noacc
			double rawflux_noacc = hrawflux_noacc->GetBinContent( hrawflux_noacc->FindBin(ecenter) );
			if( acc>0 ){
				//====unflux
				double unflux = rawflux_noacc/(acc/1e4);
				hflux_unfold->SetBinContent(ibin, unflux);
				//====rawflux
				double rawflux = hflux_raw->GetBinContent(ibin);
				double rawflux_err  = hflux_raw->GetBinError(ibin);
				double unflux_err  = 0;
				//====unflux_err
				if( rawflux>0 && rawflux_err>0 ) unflux_err = unflux*(rawflux_err/rawflux);
				else if( unflux>0 ) unflux_err = 0.05*unflux;
				hflux_unfold->SetBinError(ibin, unflux_err);
				//====flux relative diff
				if( rawflux>0 ){
					double fluxreldiff = fabs(unflux-rawflux)/rawflux;
					if( fluxreldiff>max_fluxreldiff ) max_fluxreldiff = fluxreldiff;
				}
			}
		}
		cout<<"iter "<<ic<<htag<<" max_fluxreldiff="<<max_fluxreldiff<<endl;
		//============ iteration convergence check ============
		if( max_fluxreldiff<1e-3 ){
			iter_converged = 1;
			break;
		}
		//============ reset ============
		hflux_raw->Reset();
		hflux_raw->Add(hflux_unfold);
	}
	//====iteration failed warning
	if(icut>=15 && !iter_converged) cout<<"Warning: iteration not converged within ic_max="<<ic_max<<", "<<htag<<endl;
	//============ save ============
	if(icut==15 && is_save_detail && fout!=0){
		fout->cd();
		hmatrix->Write();
		hrec_bfre->Write();
		hgen->Write();
		hrawflux->Write();
		hrawflux_noacc->Write();
		hmatrix_re->Write();
		hrec_afre->Write();
		if( fflux_fit ) fflux_fit->Write();
		hflux_fit->Write();
		hunacc->Write();
	}
	//============ return ============
	hunacc->SetDirectory(0);
	file_sel->Close();
	file_gen->Close();
	delete file_sel;
	delete file_gen;
	return hunacc;
}


TH1D *_calc_unacc(TString fnm_sel, TString fnm_gen, int icut, double emin, double emax, TFile* fout,double nplane=1.0){
	TFile *file_flux = new TFile( "./datain/hrawflux.root", "read" );
	TH1D *hrawflux_in = dynamic_cast<TH1D*>( file_flux->Get("h_rawflux") );
	TH1D *hrawflux_noacc_in = dynamic_cast<TH1D*>( file_flux->Get("rawflux_noacc") );
	TH1D *hrawflux = BUILD_RawFluxFrom1GeV(hrawflux_in, "h_rawflux_from1GeV");
	TH1D *hrawflux_noacc = BUILD_RawFluxFrom1GeV(hrawflux_noacc_in, "rawflux_noacc_from1GeV");
	if( hrawflux==0 || hrawflux_noacc==0 ){
		cerr << "ERR _calcFoldedAcc ===== missing h_rawflux or rawflux_noacc" << endl;
		return 0;
	}
	TH1D *hunacc = _calc_unacc_core(fnm_sel, fnm_gen, icut, emin, emax, fout, hrawflux, hrawflux_noacc, "", 1, nplane);
	file_flux->Close();
	delete file_flux;
	return hunacc;
}

void _calc_unacc_tdep(TString fnm_sel, TString fnm_gen, int icut, double emin, double emax, TFile *fout){
	vector<TH1D*> vrawflux, vrawflux_noacc, vunacc, vunfactor;
	//============ read fluxt ============
	TFile *file_flux = new TFile("./datain/hrawflux_t.root", "read");
	if( !unacct_get_rawflux(file_flux, vrawflux, vrawflux_noacc) ) return;
	//============ calcunacc ============
	for(int it=0; it<NTBIN_27D; it++){
		int save=0;
		if( it==1 ) save=1;
		TH1D *hunacc = _calc_unacc_core(
			fnm_sel, fnm_gen, icut, emin, emax, fout,
			vrawflux[it], vrawflux_noacc[it],
			// Form("_tbin%03d", it), 0
			Form("_t%03d", it), save
		);
		if( hunacc ){
			hunacc->SetNameTitle(
				Form("hunacc_tbin%03d", it),
				Form("Acceptance cut%02d tbin%03d;ECAL Energy[GeV];Acceptance[cm^{2}sr]", icut, it)
			);
			vunacc.push_back(hunacc);
		}
	}
	//============ get mcacc ============
	TFile *file_mcacc = new TFile("./datain/mcacc.root", "read");
	TH1F *hmcacc = dynamic_cast<TH1F*>( file_mcacc->Get("hacc_cut15") );
		if( hmcacc==0 ){
			cerr << "ERR CALC_TimeDependentAcceptanceCut15 ===== missing hacc_cut15 in mcacc.root" << endl;
			return;
		}
	//============ calc unfactor ============
	for(int it=0; it<NTBIN_27D; it++){
		if( it>=(int)vunacc.size() || vunacc[it]==0 ) continue;
		TH1D *hunfactor = (TH1D*)vunacc[it]->Clone( Form("hunfactor%03d", it) );
		hunfactor->SetTitle( Form("unacc/mcacc t%03d;ECAL Energy[GeV];unacc/mcacc", it) );
		for(int ie=0; ie<hunfactor->GetNbinsX(); ie++){
			double unacc = vunacc[it]->GetBinContent(ie);
			double mcacc = hmcacc->GetBinContent(ie);
			hunfactor->Reset();
			if( mcacc>0 ){
				hunfactor->SetBinContent(ie+1, unacc/mcacc);
				hunfactor->SetBinError(ie+1, 0);
			}
		}
		vunfactor.push_back(hunfactor);
	}
	//============ save ============
	// fout->cd();
	// for(int it=0; it<NTBIN_27D; it++){
	// 	if( vrawflux_noacc[it] ) vrawflux_noacc[it]->Write();
	// 	if( vrawflux[it] ) vrawflux[it]->Write();
	// 	if( it>=(int)vunacc.size() || vunacc[it]==0 ) continue;
	// 	vunacc[it]->Write();
	// 	vunfactor[it]->Write();
	// }
	//============ edep to tdep ============
	fout->cd();
	for(int ie=FIRST_ENEBIN_TDEP_RAW; ie<NENEBIN_TDEP; ie++){
		//====init hunacc&hunfactor 
		double elow = ENERGY_BINS_TDEP[ie];
		double eup = ENERGY_BINS_TDEP[ie+1];
		TH1D *hunacc_t = unacct_init_ht(
			Form("hunacc_t_ene%02d", ie),
			Form("Acceptance %.2f-%.2f GeV;time;Acceptance[cm^{2}sr]", elow, eup)
		);
		TH1D *hunfactor_t = unacct_init_ht(
			Form("hunfactor_t%02d", ie),
			Form("unacc/mcacc %.2f-%.2f GeV;time;unacc/mcacc", elow, eup)
		);
		//====fill
		for(int it=0; it<NTBIN_27D; it++){
			if( it>=(int)vunacc.size() || vunacc[it]==0 ) continue;
			double acc = vunacc[it]->GetBinContent(ie+1);
			double acc_err = vunacc[it]->GetBinError(ie+1);
			hunacc_t->Reset();
			if( it<(int)vunfactor.size() && vunfactor[it] ){
				hunfactor_t->SetBinContent(it+1, vunfactor[it]->GetBinContent(ie+1));
				hunfactor_t->SetBinError(it+1, 0);
			}
		}
		// hunacc_t->Write();
		// hunfactor_t->Write();
	}
	file_mcacc->Close();
	file_flux->Close();
	delete file_mcacc;
	delete file_flux;
}


void draw_acc(TH1D *h1,TString foutname,int icut){
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
  	TH1D *hacc[nCut]={0}; 
	//====Hypothesis
	//---- 0-> MC truth, 1-> electron, 2-> positron
	int Hypothesis; 
	Hypothesis = 0; 
	Hypothesis = 1;
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
	// for(int i=0; i<nCut; i++){
	// 	if(Hypothesis==0){
	// 		hacc[i] = _calcMCAcc( fname_mc[0][0], fname_mc[0][1], i, emin, emax );
	// 		hacc[i]->SetNameTitle( Form("hacc_cut%02d", i), Form("Acceptance;MC Energy[GeV];Acceptance[cm^{2}sr]") );
	// 	}
	// 	else{
	// 		hacc[i] = _calcFoldedAcc( fname_mc[0][0], fname_mc[0][1], i, emin, emax, fout );
	// 		hacc[i]->SetNameTitle( Form("hacc_cut%02d", i), Form("Acceptance;ECAL Energy[GeV];Acceptance[cm^{2}sr]") );
	// 	}
	// 	if(i==15||i==16) DRAW_Acceptance( hacc[i], Form("acceptance_cut%02d", i),i);
	// }
	//----260527 tdep cut15
	if( Hypothesis!=0 ){
		_calc_unacc_tdep( fname_mc[0][0], fname_mc[0][1], 15, emin, emax, fout);
	}

	//============ save ============
	// fout->cd();
	// for(int i=0; i<nCut; i++){if(hacc[i]) hacc[i]->Write();}
	// fout->Close();

	//============ return ============
	return 0;
}
