#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TGraph.h>
#include "../include/general/ConsoleDisplay.h"

/*
void Analysis::BuildMCWeight(){
	cout << "build weight for proton " << endl;
	TString dir_pbar_model = "/afs/cern.ch/work/w/wxu/work/ams/analysis/antipr/AMS_Pbar/WXU/Counting/Acceptance/fluxModel/";
	h1d_mcprweight = new TH1D("h1d_mcprweight", "mc weight", nbin, energy_bins);
//	BuildMCWeightFromFluxModel("/afs/cern.ch/work/w/wxu/work/ams/analysis/antipr/results/proton_flux_interpolation.root", h1d_mcprweight);
	BuildMCWeightFromFluxModel( dir_pbar_model + "proton_flux_interpolation_Phi600.root", h1d_mcprweight);
	fOutput->Add( h1d_mcprweight );

	cout << "build weight for proton " << endl;
	h1d_mcpbarweight = new TH1D("h1d_mcpbarweight", "mc weight", nbin, energy_bins);
//	BuildMCWeightFromFluxModel("/afs/cern.ch/work/w/wxu/work/ams/analysis/antipr/results/antiproton_flux_interpolation.root", h1d_mcpbarweight);
	
	int iteration=4;
//	BuildMCWeightFromFluxModel( dir_pbar_model + Form("antiproton_flux_interpolation_iteration%d.root", iteration), h1d_mcpbarweight);
	BuildMCWeightFromFluxModel( dir_pbar_model + Form("antiproton_flux_interpolation_Phi600_iteration%d.root", iteration), h1d_mcpbarweight);
	fOutput->Add( h1d_mcpbarweight );
}

void Analysis::BuildMCWeightFromFluxModel(TString flux_model_file, TH1D *hweight){
	TH1F *h1d_flux=0;
	TDirectory *gtmp = gDirectory;
	TFile *file_fluxmodel = new TFile( flux_model_file );
	h1d_flux = dynamic_cast<TH1F*>(file_fluxmodel->Get("h1"));
	cout << "h1d_flux=" << h1d_flux << endl;
	if( gtmp ) gtmp->cd();
	cout << "h1d_flux=" << h1d_flux << endl;
	TF1 *f1_flux_mc = new TF1("f1_flux_mc", "1/x", 0.1, 2000);
	f1_flux_mc->SetNpx(1000);
	TH1F *h1d_flux_mc = dynamic_cast<TH1F*>( h1d_flux->Clone("h1d_flux_mc") );
	for(int i=0; i<h1d_flux_mc->GetNbinsX(); i++){
		double x1 = h1d_flux_mc->GetBinLowEdge(i+1);
		double x2 = h1d_flux_mc->GetBinLowEdge(i+2);
		h1d_flux_mc->SetBinContent( i+1, f1_flux_mc->Integral(x1, x2) ); 
	}

	for(int i=0; i<hweight->GetNbinsX(); i++){
		double x1 = hweight->GetBinLowEdge(i+1);
		double x2 = hweight->GetBinLowEdge(i+2);
		double intergral_flux = h1d_flux->Integral( h1d_flux->FindBin(x1), h1d_flux->FindBin(x2) );
		double integral_mc = h1d_flux_mc->Integral( h1d_flux->FindBin(x1), h1d_flux->FindBin(x2) );
//		double integral_mc = f1_flux_mc->Integral( x1, x2 );
		hweight->SetBinContent(i+1, intergral_flux/integral_mc);
		cout << hweight->GetBinContent(i+1) << ", ";			if( (i+1)%10 == 0 ) cout << endl;
	}
}
*/


void Analysis::DefineEcalFiducialVolume(){
	static const double ecal_entry_z0 = -143.2;
	static const double ecal_exit_z0 = -158.9;
	inecal=0;
	double ecalentry[3][2],
			 ecalexit[3][2];
	memset(ecalentry, 0, sizeof(ecalentry));
	memset(ecalexit, 0, sizeof(ecalexit));
	// extrapolate to ECAL entry and exit
	for(int ish=0; ish<N_Shwr; ish++){
		if( ShwrDir[ish][2]!=0 ){
			ecalentry[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalentry[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
			ecalexit[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_exit_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalexit[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_exit_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
		}else{
			continue;
		}
	}
	// both entry and exit are contained in ECAL
	if( N_Shwr>0 && TMath::Abs( ecalentry[0][0] ) < 31.5 && TMath::Abs( ecalentry[0][1] ) < 31.5 ) inecal |= 1;
	if( N_Shwr>0 && TMath::Abs( ecalexit[0][0] )  < 31.5 && TMath::Abs( ecalexit[0][1] )  < 31.5 ) inecal |= 2;
	if( N_Shwr>1 && TMath::Abs( ecalentry[1][0] ) < 31.5 && TMath::Abs( ecalentry[1][1] ) < 31.5 ) inecal |= 4;
	if( N_Shwr>1 && TMath::Abs( ecalexit[1][0] )  < 31.5 && TMath::Abs( ecalexit[1][1] )  < 31.5 ) inecal |= 8;
	if( N_Shwr>2 && TMath::Abs( ecalentry[2][0] ) < 31.5 && TMath::Abs( ecalentry[2][1] ) < 31.5 ) inecal |= 16;
	if( N_Shwr>2 && TMath::Abs( ecalexit[2][0] )  < 31.5 && TMath::Abs( ecalexit[2][1] )  < 31.5 ) inecal |= 32;
}

void Analysis::DefineEcalFiducialVolumeOld(){
	inecal=0;
	if( TMath::Abs( ecal_entry[0] ) < 30.6 )  inecal |= 1;
	if( TMath::Abs( ecal_entry[1] ) < 30.6 )  inecal |= 2;
	if( TMath::Abs( ecal_exit[0] ) < 30.6 ) inecal |= 4;
	if( TMath::Abs( ecal_exit[1] ) < 30.6 ) inecal |= 8;
}
void Analysis::TrackerEcalMatchOld(){
	trkecalmatch=0;
	if( TMath::Abs(trk_ecalentry[0]) < 32.4              ) trkecalmatch |= 1;
	if( TMath::Abs(trk_ecalentry[1]) < 32.4              ) trkecalmatch |= 2;
	if( TMath::Abs(trk_ecalentry[0] - ecal_entry[0]) < 3 ) trkecalmatch |= 4;
	if( TMath::Abs(trk_ecalentry[1] - ecal_entry[1]) < 10 ) trkecalmatch |= 8;
}


void Analysis::TrackerEcalMatch(){
	static const double ecal_entry_z0 = -143.2;

	trkecalmatch=0;
	if( TMath::Abs(trk_ecalentry[0]) < 32.4              ) trkecalmatch |= 1;
	if( TMath::Abs(trk_ecalentry[1]) < 32.4              ) trkecalmatch |= 2;
	if( N_Shwr<1 ) return;
	
	// find best match shower
	Float_t ecalentry[3][2];
	memset(ecalentry, 0, sizeof(ecalentry));
	int index_shwr = -1;
	float min_dist = FLT_MAX, dist=0, dx, dy;
	for(int ish=0; ish<N_Shwr; ish++){
		if( ShwrDir[ish][2]!=0 ){
			ecalentry[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalentry[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
		}else{
			continue;
		}
		dx = trk_ecalentry[0] - ecalentry[ish][0];
		dy = trk_ecalentry[1] - ecalentry[ish][1];
		dist = dx*dx + dy*dy;
		if( min_dist > dist ){
			min_dist = dist;
			index_shwr = ish;
		}
	}
	if( index_shwr<0 ) return;
	// matching between trk and ecal
	if( TMath::Abs(trk_ecalentry[0] - ecalentry[index_shwr][0]) < 3  ) trkecalmatch |= 4;
	if( TMath::Abs(trk_ecalentry[1] - ecalentry[index_shwr][1]) < 10 ) trkecalmatch |= 8;
	
}

void Analysis::TrdEcalMatch(){
	static const double ecal_entry_z0 = -143.2;

	trdecalmatch=0;
	if( TMath::Abs(trd_ecalentry[0]) < 32.4              ) trdecalmatch |= 1;
	if( TMath::Abs(trd_ecalentry[1]) < 32.4              ) trdecalmatch |= 2;
	if( N_Shwr<1 ) return;
	
	// find best match shower
	Float_t ecalentry[3][2];
	memset(ecalentry, 0, sizeof(ecalentry));
	int index_shwr = -1;
	float min_dist = FLT_MAX, dist=0, dx, dy;
	for(int ish=0; ish<N_Shwr; ish++){
		if( ShwrDir[ish][2]!=0 ){
			ecalentry[ish][0] = ShwrDir[ish][0]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][0];
			ecalentry[ish][1] = ShwrDir[ish][1]/ShwrDir[ish][2]*(ecal_entry_z0 - ShwrPos[ish][2]) + ShwrPos[ish][1];
		}else{
			continue;
		}
		dx = trd_ecalentry[0] - ecalentry[ish][0];
		dy = trd_ecalentry[1] - ecalentry[ish][1];
		dist = dx*dx + dy*dy;
		if( min_dist > dist ){
			min_dist = dist;
			index_shwr = ish;
		}
	}
	if( index_shwr<0 ) return;
	// matching between trk and ecal
	if( TMath::Abs(trd_ecalentry[0] - ecalentry[index_shwr][0]) < 3  ) trdecalmatch |= 4;
	if( TMath::Abs(trd_ecalentry[1] - ecalentry[index_shwr][1]) < 10 ) trdecalmatch |= 8;
	
}

void Analysis::SmearEnergy(double factor){
	double resolution = sqrt(10.4*10.4/Ene + 1.4*1.4)/100.; // published ECAL resolution
	Ene = gRandom->Gaus( Ene, Ene*resolution*factor );
}

void Analysis::EvalChargeBDT(){
	static bool first_call = 1;
	if( first_call ){
		first_call = 0;
		// mychargepidbdt.Config("/afs/cern.ch/work/t/tsu/BDT_ChargePID/MyBDT_ChargePID_v1.config");
		mychargepidbdt.Config("/afs/cern.ch/work/c/chguan/public/BDT_ChargePID/MyBDT_ChargePID_v1.config");
		mychargepidbdt.PrepareForReading();
	}
	mychargepidbdt.var[0]=tof_qhlay[0];
	mychargepidbdt.var[1]=tof_qhlay[1];
	mychargepidbdt.var[2]=tof_qhlay[2];
	mychargepidbdt.var[3]=tof_qhlay[3];
	mychargepidbdt.var[4]=trd_elprob[0];
	mychargepidbdt.var[5]=trd_prprob[0];;
	mychargepidbdt.var[6]=trd_heprob[0];
	mychargepidbdt.var[7]=trk_qin;
	mychargepidbdt.var[8]=trk_qinrms;
	mychargepidbdt.var[9]=trk_qinup;
	mychargepidbdt.var[10]=trk_qinlow;
	mychargepidbdt.var[11]=acc_NACC;
	mychargepidbdt.var[12]=rich_ntothits;

	bdt_chargepid=mychargepidbdt.Eval_BDT();
}

void Analysis::LoopChain(){
	//====init hist
	BookHistograms();
	//====
	long nentries = fChain->GetEntries();
	// long nprint = nentries/100;
	// if( nprint<1 ) nprint=1;
	ConsoleDisplay mydisplay(nentries);
	//====260315 read flux
	// TFile *file_fluxmodel = new TFile( "/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/04.101--ACC1/datain/fluxmodel_prl122.root", "read" );
	// TH1D *hfluxmodel = dynamic_cast<TH1D*>( file_fluxmodel->Get("h_fluxfit") );
	//----260413.01----rightfluxprl122
	TFile *file_fluxmodel = new TFile( "/eos/ams/user/c/chguan/public/250614.01-DAILYFLUX/04.101--ACC1/datain/fluxprl122.root", "read" );
	TH1D *hfluxmodel = dynamic_cast<TH1D*>( file_fluxmodel->Get("hfluxpos") );
	//====loop
	for(long entry=0; entry<nentries; entry++){
		// TDatime t;
		// if(entry%nprint==0 || entry==nentries-1){
		// 	double progress = 100.0*(entry+1)/nentries;
		// 	cout<<Form("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond())
		// 		<<" Processing entry "<<entry<<" / "<<nentries
		// 		<<" ("<<Form("%.2f", progress)<<"%)"<<endl;
		// }
		mydisplay.Update(entry);
		fChain->GetEntry( entry );
		//====init
		ecal_enc *= 0.975;
		// SmearEnergy(1); // worsen the resolution by 50%
		// if( mcinfo_p!=0 ){
		// 	lvl1_PhysBPatt = lvl1_PhysBPattMC;
		// 	lvl1_JMembPatt = lvl1_JMembPattMC;
		// }
		// else             
		// 	lvl1_PhysBPatt = RestorePhysBPat( lvl1_JMembPatt );
		DefineEcalFiducialVolume();
		TrackerEcalMatch();
		TrdEcalMatch();
		EvalChargeBDT();
		// minor modification for TOI correction
		// Ene -= 0.015; // 15 MeV
		double rigidity = trk_rig[1];
		double eleprob=-log(trd_Likelihood_ele)/10.0;
			eleprob = (eleprob-0.696)*1.05 + 0.695;
		if( !(trk_patx&256) && (trk_pat&256) ){ // Y-only L9 hit should not be used in rigidity calculation
			trk_pat &= ~256; //  
			if( trk_pat&1 ){ // InnerL1
				rigidity = trk_rig[2];
			}else{ // Inner
				rigidity = trk_rig[0]; 
			}   
		}
	  	int bin = GetBinIndex(Ene);
	  	// int bin = GetBinIndex_Fine(Ene);
		bool IsNoTrkCutExceptQin=0;
		bool IsLooseChargeCut=0;
		//========cut
		Bool_t cut[nCut] = { 0 };
		//----tsu
		// cut[0] = tof_betah >= 0.8 && hadflag==0; // eff = 0.998(200GeV) - 0.994(1TeV)
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 2.0;
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.0 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259);
		// // cut[7] = (trk_pat&2)&&(trk_pat&12)&&(trk_pat&48)&&(trk_pat&192);
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0; // eff = 0.97
		// cut[9] =  bdtx_combined>0.0; // eff = 0.96
		// // cut[10] = TMath::Abs(Ene/rigidity)>0.65; // new charge BDT
		// cut[10] = TMath::Abs(Ene/rigidity)>0.65&&bdt_chargepid>0; // new charge BDT
		// cut[11] = trk_ntrk<2 && trd_nhits[2]>=12;
		// cut[12] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[13] = trd_klkhd[2][1]<0.8;
		// cut[14] = 1;//eleprob>(trd_new+0.1);
		// cut[15] = trd_new<0.7;
		// cut[16] = trk_ntrk==1;
		//----mine 260310.01--past
		// cut[0] = tof_betah > 0.8 && hadflag==0; // eff = 0.998(200GeV) - 0.994(1TeV) //----mine
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 2.0;
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.0 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259);
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436; // eff = 0.97 //----mine
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>0; // new charge BDT //----mine
		// cut[10] = trk_ntrk<2 && trd_nhits[2]>=12;
		// cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = 1;//eleprob>(trd_new+0.1);
		// cut[14] = trd_new<0.8;	//----mine
		// cut[15] = rigidity<0;	//----mine
		
		// cut[16] = trk_ntrk==1;
		// cut[17] =  bdtx_combined>0.0; // eff = 0.96
		//----mine 260310.02--ccratio(wangyu)--newcut
		// cut[0] = tof_betah > 0.8;
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// // cut[7] = (trk_pat&259);
		// cut[7] = (trk_pat&259) && (lvl1_PhysBPattMC&62);	//----260318发现并修正
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// // cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[11] = trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = trd_new<0.6;
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----mine 260316.01--ccratio(mine)--oldcut
		// cut[0] = tof_betah > 0.8;
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 2.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259);
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk<2 && trd_nhits[2]>=12;	//*
		// cut[11] = trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = trd_new<0.6;
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----mine 260320.01--ccratio(wangyu+)--newcut2
		// cut[0] = tof_betah > 0.8;
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259) && (lvl1_PhysBPattMC&62);	//----260318发现并修正
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// // cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[11] = trd_klkhd[2][0]<1.5;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = trd_new<0.6;
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----mine 260320.01--ccratio(wangyu+)--newcut3
		// cut[0] = tof_betah > 0.8;
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259) && (lvl1_PhysBPattMC&62);	//----260318发现并修正
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// // cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[11] = trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// // cut[13] = trd_new<0.6;
		// cut[13] = trd_new<0.7;	//*
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----mine 260324.01--ccratio(wangyu+)--newcut4.1&4.2
		// // cut[0] = tof_betah > 0.8;
		// cut[0] = tof_betah > 0.8 && hadflag==0; //----260324发现并修正
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259) && (lvl1_PhysBPatt&62);	//----260318发现并修正
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// // cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[11] = trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = trd_new<0.6;
		// // cut[13] = trd_new<0.7;	//* 4.1
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----mine 260330.01--ccratio(wangyu+)--newcut5--ntrk1
		// // cut[0] = tof_betah > 0.8;
		// cut[0] = tof_betah > 0.8 && hadflag==0; //----260324发现并修正
		// cut[1] = (inecal&3)==3;
		// cut[2] = (trkecalmatch&12)==12;
		// cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		// cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		// cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		// cut[6] = EmBDT>-0.995;
		// cut[7] = (trk_pat&259) && (lvl1_PhysBPattMC&62);	//----260318发现并修正
		// cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		// cut[10] = trk_ntrk==1 && trd_nhits[2]>=12;	//*
		// // cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// // cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		// cut[11] = trd_klkhd[2][0]<1.5 ;
		// cut[12] = trd_klkhd[2][1]<0.8;
		// cut[13] = trd_new<0.6;
		// // cut[13] = trd_new<0.7;	//*
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		// cut[15] = rigidity<0;
		// cut[16] = rigidity>0;
		//----260517
		// cut[0] = tof_betah > 0.8;
		cut[0] = tof_betah > 0.8 && hadflag==0; //----260324发现并修正
		cut[1] = (inecal&3)==3;
		cut[2] = (trkecalmatch&12)==12;
		cut[3] = tof_qup >0. && tof_qup < 3.0;	//*
		cut[4] = tof_qlow>0. && tof_qlow< 5.0;
		cut[5] = trk_qin > 0.7 && trk_qin < 1.5; 
		cut[6] = EmBDT>-0.995;
		cut[7] = (trk_pat&259) && (lvl1_PhysBPatt&62);	//----260318发现并修正
		cut[8] = trk_chi2x[1] < 20. && trk_chi2y[1]<20.0 && trk_theta < 0.436;
		// cut[9] = TMath::Abs(Ene/rigidity)>0.65&&TMath::Abs(Ene/rigidity)<5.00&&bdt_chargepid>=0;
		cut[9] = bdt_chargepid>=0;
		cut[10] = trk_ntrk<=2 && trd_nhits[2]>=12;	//*
		// cut[11] = rigidity * mcinfo_q > 0 && trd_klkhd[2][0]<1.5 ;
		cut[11] = trd_klkhd[2][0]<1.5 ;
		cut[12] = trd_klkhd[2][1]<0.8;
		cut[13] = trd_new<0.6;
		// cut[13] = trd_new<0.7;	//* 4.1
		// cut[14] = bdtx_combined>0.0; // eff = 0.96
		cut[14] = trd_new<0.6; // eff = 0.96
		cut[15] = rigidity<0;
		cut[16] = rigidity>0;
		//========fill hist
		//----original
		// bool passed = 1;
		// for(int i=0; i<nCut; i++){
		// 	passed = passed && cut[i]; 
		// 	if( passed ){
		// 		h1Ene3D[i]->Fill( Ene, weight );
		// 		h1EneC[i]->Fill( ecal_enc, weight );
		// 		h1Ene17[i]->Fill( ecal_ene17, weight );
		// 		h1MCEne[i]->Fill( mcinfo_p, weight );
		// 		h2Ene3D_MCEne[i]->Fill( mcinfo_p, Ene, weight );
		// 		h2EneC_MCEne[i]->Fill( mcinfo_p, ecal_enc, weight );
		// 		h2Ene17_MCEne[i]->Fill( mcinfo_p, ecal_ene17, weight );
		// 		h2Ene3DMCEneRatio_MCEne[i]->Fill( mcinfo_p, Ene/mcinfo_p, weight );
		// 		h2EneCMCEneRatio_MCEne[i]->Fill( mcinfo_p, ecal_enc/mcinfo_p, weight );
		// 		h2Ene17MCEneRatio_MCEne[i]->Fill( mcinfo_p, ecal_ene17/mcinfo_p, weight );
		// 		h2EneCEne3DRatio_MCEne[i]->Fill( mcinfo_p, ecal_enc/Ene, weight );
		// 		h2Ene17Ene3DRatio_MCEne[i]->Fill( mcinfo_p, ecal_ene17/Ene, weight );
		// 		h2EneCEne3DRatio_Ene3D[i]->Fill( Ene, ecal_enc/Ene, weight );
		// 		h2Ene17Ene3DRatio_Ene3D[i]->Fill( Ene, ecal_ene17/Ene, weight );
		// 	}
		// }
		//----260311--bfre
		bool passed = 1;
		bool passed_temp = 1;
		for(int i=0; i<nCut; i++){
			passed = passed && cut[i]; 
			if(i==14) passed_temp = passed;
			if(i==15) passed = passed_temp && cut[15];
			else if(i==16) passed = passed_temp && cut[16];
			if( passed ){
				h1Ene3D[i]->Fill( Ene, weight );
				// h1EneC[i]->Fill( ecal_enc, weight );
				// h1Ene17[i]->Fill( ecal_ene17, weight );
				h1MCEne[i]->Fill( mcinfo_p, weight );
				h2Ene3D_MCEne[i]->Fill( mcinfo_p, Ene, weight );
				// h2EneC_MCEne[i]->Fill( mcinfo_p, ecal_enc, weight );
				// h2Ene17_MCEne[i]->Fill( mcinfo_p, ecal_ene17, weight );
				// h2Ene3DMCEneRatio_MCEne[i]->Fill( mcinfo_p, Ene/mcinfo_p, weight );
				// h2EneCMCEneRatio_MCEne[i]->Fill( mcinfo_p, ecal_enc/mcinfo_p, weight );
				// h2Ene17MCEneRatio_MCEne[i]->Fill( mcinfo_p, ecal_ene17/mcinfo_p, weight );
				// h2EneCEne3DRatio_MCEne[i]->Fill( mcinfo_p, ecal_enc/Ene, weight );
				// h2Ene17Ene3DRatio_MCEne[i]->Fill( mcinfo_p, ecal_ene17/Ene, weight );
				// h2EneCEne3DRatio_Ene3D[i]->Fill( Ene, ecal_enc/Ene, weight );
				// h2Ene17Ene3DRatio_Ene3D[i]->Fill( Ene, ecal_ene17/Ene, weight );
			}
		}
		// ----260315--reweight
		// //----flux
		// double flux = hfluxmodel->GetBinContent( hfluxmodel->FindBin(mcinfo_p) );
		// //----binwidth
		// int mcpbin = GetBinIndex(mcinfo_p);
		// double mcpbinlow = energy_bins[mcpbin];
		// double mcpbinup = energy_bins[mcpbin+1]; 
		// //----reweight
		// double reweight = 1.0;
		// reweight = weight * mcinfo_p * flux;
		// // reweight = weight * mcinfo_p * flux * (mcpbinup - mcpbinlow);
		// //----fill
		// bool passed = 1;
		// bool passed_temp = 1;
		// for(int i=0; i<nCut; i++){
		// 	passed = passed && cut[i]; 
		// 	if(i==14) passed_temp = passed;
		// 	if(i==15) passed = passed_temp && cut[i];
		// 	if(i==16) passed = passed_temp && cut[i];
		// 	if( passed ){
		// 		h1MCEne[i]->Fill( mcinfo_p, reweight );
		// 		h1Ene3D[i]->Fill( Ene, reweight );
		// 		h2Ene3D_MCEne[i]->Fill( mcinfo_p, Ene, reweight );
		// 	}
		// }
	}
	// TH1D *cc_ene = new TH1D("cc_ene", "cc_ene", nbin, energy_bins);
	// double nele,nelecc,cc,cc_err;
	// for(int i=0; i<nbin; i++){
	// 	nele = h1Ene3D[15]->GetBinContent(i+1);
	// 	nelecc = h1Ene3D[16]->GetBinContent(i+1);
	// 	cc = nelecc / (nelecc + nele);
	// 	cc_err = sqrt( cc*(1-cc)/nelecc + cc*cc*nele/(nelecc*nele) );
	// 	cc_ene->SetBinContent(i+1, cc);
	// 	cc_ene->SetBinError(i+1, cc_err);
	// }
	//====save
	mydisplay.Finish();
	fout->cd();
	fout->Write();
	// cc_ene->Write();
	fout->Close();
}

void Analysis::BookHistograms(){
	cout << "Book histograms ..." << endl;
	fChain->GetEntry(0);
	TDirectory *gtmp = gDirectory;
	//----original
	// fout = new TFile( Form("histo_acceptance_run%i_event%i.root", info_run, info_event), "recreate");
	// for(int i=0; i<nCut; i++){
	// 	if( IsFineBinning ){
	// 		h1MCEne[i] = new TH1F( Form("h1MCEnergy_cut%02d", i), "h1MCEnergy;MC Energy [GeV];#Events", nbin_fine, energy_bins_fine);
	// 		h1Ene3D[i] = new TH1F( Form("h1Energy3D_cut%02d", i), "h1Energy3D;Energy3D [GeV];#Events", nbin_fine, energy_bins_fine);
	// 		h1EneC[i]  = new TH1F( Form("h1EnergyC_cut%02d", i),  "h1EnergyC; EnergyC [GeV]; #Events", nbin_fine, energy_bins_fine);
	// 		h1Ene17[i]  = new TH1F( Form("h1Energy17_cut%02d", i),  "h1Energy17; Energy17 [GeV]; #Events", nbin_fine, energy_bins_fine);
	// 		h2Ene3D_MCEne[i] = new TH2F( Form("h2Energy3D_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy3D;MC Energy[GeV];Energy3D [GeV]; #Events", nbin_fine, energy_bins_fine, nbin_fine, energy_bins_fine);
	// 		h2EneC_MCEne[i]  = new TH2F( Form("h2EnergyC_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC [GeV]; #Events", nbin_fine, energy_bins_fine, nbin_fine, energy_bins_fine);
	// 		h2Ene17_MCEne[i]  = new TH2F( Form("h2Energy17_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17 [GeV]; #Events", nbin_fine, energy_bins_fine, nbin_fine, energy_bins_fine);
	// 		h2Ene3DMCEneRatio_MCEne[i] = new TH2F( Form("h2Energy3D_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy3D;MC Energy[GeV];Energy3D/MCEnergy; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2EneCMCEneRatio_MCEne[i]  = new TH2F( Form("h2EnergyC_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC/MCEnergy; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2Ene17MCEneRatio_MCEne[i]  = new TH2F( Form("h2Energy17_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17/MCEnergy; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2EneCEne3DRatio_MCEne[i]  = new TH2F( Form("h2EnergyC_Energy3D_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC/Energy3D; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2Ene17Ene3DRatio_MCEne[i]  = new TH2F( Form("h2Energy17_Energy3D_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17/Energy3D; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2EneCEne3DRatio_Ene3D[i]  = new TH2F( Form("h2EnergyC_Energy3D_Ratio_vs_Energy3D_cut%02d", i), "h2Energy3DVsEnergyC;MC Energy[GeV];EnergyC/Energy3D; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 		h2Ene17Ene3DRatio_Ene3D[i]  = new TH2F( Form("h2Energy17_Energy3D_Ratio_vs_Energy3D_cut%02d", i), "h2Energy3DVsEnergy17;MC Energy[GeV];Energy17/Energy3D; #Events", nbin_fine, energy_bins_fine, 200, 0, 2);
	// 	}else{
	// 		h1MCEne[i] = new TH1F( Form("h1MCEnergy_cut%02d", i), "h1MCEnergy;MC Energy [GeV];#Events", nbin, energy_bins);
	// 		h1Ene3D[i] = new TH1F( Form("h1Energy3D_cut%02d", i), "h1Energy3D;Energy3D [GeV];#Events", nbin, energy_bins);
	// 		h1EneC[i]  = new TH1F( Form("h1EnergyC_cut%02d", i),  "h1EnergyC; EnergyC [GeV]; #Events", nbin, energy_bins);
	// 		h1Ene17[i]  = new TH1F( Form("h1Energy17_cut%02d", i),  "h1Energy17; Energy17 [GeV]; #Events", nbin, energy_bins);
	// 		h2Ene3D_MCEne[i] = new TH2F( Form("h2Energy3D_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy3D;MC Energy[GeV];Energy3D [GeV]; #Events", nbin, energy_bins, nbin, energy_bins);
	// 		h2EneC_MCEne[i]  = new TH2F( Form("h2EnergyC_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC [GeV]; #Events", nbin, energy_bins, nbin, energy_bins);
	// 		h2Ene17_MCEne[i]  = new TH2F( Form("h2Energy17_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17 [GeV]; #Events", nbin, energy_bins, nbin, energy_bins);
	// 		h2Ene3DMCEneRatio_MCEne[i] = new TH2F( Form("h2Energy3D_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy3D;MC Energy[GeV];Energy3D/MCEnergy; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2EneCMCEneRatio_MCEne[i]  = new TH2F( Form("h2EnergyC_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC/MCEnergy; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2Ene17MCEneRatio_MCEne[i]  = new TH2F( Form("h2Energy17_MCEnergy_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17/MCEnergy; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2EneCEne3DRatio_MCEne[i]  = new TH2F( Form("h2EnergyC_Energy3D_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergyC;MC Energy[GeV];EnergyC/Energy3D; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2Ene17Ene3DRatio_MCEne[i]  = new TH2F( Form("h2Energy17_Energy3D_Ratio_vs_MCEnergy_cut%02d", i), "h2MCEnergyVsEnergy17;MC Energy[GeV];Energy17/Energy3D; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2EneCEne3DRatio_Ene3D[i]  = new TH2F( Form("h2EnergyC_Energy3D_Ratio_vs_Energy3D_cut%02d", i), "h2Energy3DVsEnergyC;MC Energy[GeV];EnergyC/Energy3D; #Events", nbin, energy_bins, 200, 0, 2);
	// 		h2Ene17Ene3DRatio_Ene3D[i]  = new TH2F( Form("h2Energy17_Energy3D_Ratio_vs_Energy3D_cut%02d", i), "h2Energy3DVsEnergy17;MC Energy[GeV];Energy17/Energy3D; #Events", nbin, energy_bins, 200, 0, 2);
	// 	}
	// }
	//----260311
	fout = new TFile( Form("mchselect_run%i.root", info_run), "recreate");
	for(int i=0; i<nCut; i++){
		if( IsFineBinning ){
			h1MCEne[i] = new TH1F( Form("h1MCEne_cut%02d", i), "h1MCEne;MC Energy [GeV];#Events", nbin_fine, energy_bins_fine);
			h1Ene3D[i] = new TH1F( Form("h1Ene3D_cut%02d", i), "h1Ene3D;Energy3D [GeV];#Events", nbin_fine, energy_bins_fine);
			h2Ene3D_MCEne[i] = new TH2F( Form("h2Ene3D_MCEne_cut%02d", i), "h2Ene3D_MCEne;MC Energy[GeV];Energy3D [GeV]; #Events", nbin_fine, energy_bins_fine, nbin_fine, energy_bins_fine);
		}else{
			h1MCEne[i] = new TH1F( Form("h1MCEne_cut%02d", i), "h1MCEne;MC Energy [GeV];#Events", nbin, energy_bins);
			h1Ene3D[i] = new TH1F( Form("h1Ene3D_cut%02d", i), "h1Ene3D;Energy3D [GeV];#Events", nbin, energy_bins);
			h2Ene3D_MCEne[i] = new TH2F( Form("h2Ene3D_MCEne_cut%02d", i), "h2Ene3D_MCEne;MC Energy[GeV];Energy3D [GeV]; #Events", nbin, energy_bins, nbin, energy_bins);
		}
	}
	if( gtmp ) gtmp->cd();
	cout << "Book histograms OK" << endl;
}

int Analysis::selectDataFrom(TString fname, int id, int NFilesPerJob,TString treename="t", TString jobname=""){
	if(fChain){
		delete fChain;
		fChain=0;
	}

	int JobID=id;

	if(fname=="")return 0;
	vector<TString> v_fname;

	TChain* myChain = new TChain(treename);
	TString prefix = "root://castorpublic.cern.ch//";
	TString server = "?svcClass=amsuser";
	// TString eosprefix = "root://eosams.cern.ch//";
	TString eosprefix = "";

	if(fname.Contains("root")){
		TString fnm=fname;
		if( fnm.EndsWith("root") ){
			if( fnm.BeginsWith("/castor") )
				fnm = prefix + fnm + server;
			else if( fnm.BeginsWith("/eos"))
				fnm = eosprefix + fnm;
		}
		cout<<"Add Single Root File: " <<fnm<<endl;

		myChain->Add( fnm );

	}else{
		ifstream fin;
		fin.open( fname );
		TString fnm;
		while(1){
			fin >> fnm;
			if( fin.eof() ) break;
			if( fnm.BeginsWith("#") ) continue;
			if( fnm.EndsWith("root") ){
				if( fnm.BeginsWith("/castor") )
					fnm = prefix + fnm + server;
				else if( fnm.BeginsWith("/eos"))
					fnm = eosprefix + fnm;
			}
			v_fname.push_back(fnm);
		}

		cout<<"=============================================="<<endl;
		cout<<"Select "<<NFilesPerJob<<" Runs, From "<<v_fname.size()<<" Files, JobID: "<< JobID<<endl;
		cout<<"======================="<<endl;

		for(int i=NFilesPerJob*(JobID-1);i<NFilesPerJob*(JobID);i++){
			if(i<0)break;
			if(i>=v_fname.size())break;
			cout<<"Adding File : "<<v_fname.at(i)<<endl;
			myChain->Add( v_fname.at(i) );
		}
		cout<<"======================="<<endl;

	}
	cout<<"=============================================="<<endl;
	TTree* mynewtree=(TTree*)myChain;
	Init(mynewtree);
	cout << "Entries in fChain :" << fChain->GetEntries() << " data events " << endl;
	cout<<"=============================================="<<endl;

	return fChain->GetEntries();
}

int main(int argc, char *argv[]){
	//====init vari
	TString flist;
	int JobID=1;
	int NfilesPerJob=10;
	//====read value
	if( argc > 1 ) flist = argv[1];
	if( argc > 2 ) JobID = atoi(argv[2]);
	if( argc > 3 ) NfilesPerJob = atoi(argv[3]);
	//====init chain
	Analysis T;
	T.IsFineBinning=0;
	cout<<"Run: " <<flist<<", "<<JobID<<", "<<NfilesPerJob<<endl;
	if(T.selectDataFrom( flist,JobID,NfilesPerJob, "t_add")<=0){
		cout<<"Job have no input, Abort ,  JobID: " <<JobID<<endl;
		return -100;
	}
	//====mainprocess
	T.LoopChain();
}
