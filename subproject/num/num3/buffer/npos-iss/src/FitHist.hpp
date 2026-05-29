_Pragma("once")

#include<iostream>
using namespace std;
#include "TH1D.h"
#include "TH2D.h"
#include "TTimeStamp.h"
#include "TMath.h"
//==== 
#include "VarFittree.hpp"

class FitHist:virtual public VarFittree{
    public:
        FitHist():VarFittree(){cout<<"IN FitHist ===== Constructor_fithist"<<endl;}
        ~FitHist(){}
    public:
        int ilat=0;
        int iperiod=0;
        int rotation_number;
        int hist_index;
    //====================================================
    public:
        //======== pass
        //==== template
        TH1D* hEleSample;
        TH1D* hProSample;
        //==== sample
        TH1D* hPosiSample[nlat][nt];
        TH1D* hNegaSample[nlat][nt];
        TH1D* hNegaSample_phy[nlat][nt];
        TH1D* hNegaSample_unphy[nlat][nt];
        //==== nsample
        TH1D* hNegaSample_n[nlat][nt];
        TH1D* hPosiSample_n[nlat][nt];
    public:
        void FITHIST_InitH_Template();
        void FITHIST_FillH_Template();
        void FITHIST_SaveH_Template();
        void FITHIST_InitH();
        bool FITHIST_cut_PreSelect(int,int);
        void FITHIST_FillH(int,int,int,int);
        bool FITHIST_cut_FitCondition(int,int);
        void FITHIST_ResetH(int,int);
        void FITHIST_SaveH();
        void FITHIST_SaveH(int,int);
        void FITHIST_FillFitR(int,int);
    public:
        //----test
        TH1D* h1_EoP;
        TH2D* h2_EoP_embdt;
    public:
        //----260407
        TH1D* hTOFpass[nlat][nt];
        TH1D* hTOFtotal[nlat][nt];
        TH1D* hTRDpass[nlat][nt];
        TH1D* hTRDtotal[nlat][nt];
        TH1D* hECALpass[nlat][nt];
        TH1D* hECALtotal[nlat][nt];
        TH1D* hTrkpass[nlat][nt];
        TH1D* hTrktotal[nlat][nt];
        //----260517
        TH1D* hTrkpass_pat[nlat][nt];
        TH1D* hTrktotal_pat[nlat][nt];
        TH1D* hTrkpass_match[nlat][nt];
        TH1D* hTrktotal_match[nlat][nt];
        TH1D* hTrkpass_chi2[nlat][nt];
        TH1D* hTrktotal_chi2[nlat][nt];
        TH1D* hTrkpass_qin[nlat][nt];
        TH1D* hTrktotal_qin[nlat][nt];
        TH1D* hTrkpass_ntrk[nlat][nt];
        TH1D* hTrktotal_ntrk[nlat][nt];
};


bool FitHist::FITHIST_cut_PreSelect(int SelEffType = 0, int passcut = 0){
    //---- SelEffType
    //* 0-all 1-TOF 2-TRD 3-trk 4-ecal
    //* 31-pat 32-match 33-chi2 34-qin 35-ntrk
    //---- passcut
    //* 0-totalcut; 1-passcut; both--num&trig 
    //==================== TOF
    bool cut_tof = 
            // 1;
            beta > 0.8 
            && tofqup  > 0. && tofqup < 3.
            && tofqlow > 0. && tofqlow < 5.;
    //====tof--eff_tof
    if(SelEffType == 1){
        if(passcut) cut_tof = 
            beta > 0.8 
            && tofqup  > 0. && tofqup < 3.
            && tofqlow > 0. && tofqlow < 5.;
        else cut_tof=1;
    }
    //====tof--eff_trd&trk&ecal
    if(SelEffType == 2 || SelEffType == 3 || SelEffType == 4 || SelEffType/10 == 3){
        cut_tof = 
            beta > 0.8 
            && tofqup  > 0.7 && tofqup < 1.7
            && tofqlow > 0.7 && tofqlow < 3.0;
    }

    //==================== TRD
    bool cut_trd = 
            // 1;
            trdnhits2 >= 12 
            && trdlkhde2 < 1.5     // tail
            && trdlkhde2_ehe < 0.8;    // helium
    //====trd--eff_trd
    if(SelEffType == 2){
        if(passcut) cut_trd =  
            trdnhits2 >= 12
            && trdlkhde2 < 1.5
            && trdlkhde2_ehe < 0.8
            // && trdlkhdnew < 0.6;
            && trdlkhdnew < 0.7;
            // && trdlkhdnew < 0.8;
        else cut_trd=1;
    }
    //====trd--eff_tof&trk&ecal
    if(SelEffType == 1 || SelEffType == 3 || SelEffType == 4 || SelEffType/10 == 3){
        cut_trd =
            trdnhits2 >= 12 
            && trdlkhde2 < 1.5
            && trdlkhde2_ehe < 0.5;
    }

    //==================== Tracker
    bool cut_trk = 
            // 1;
            (pat&259) 
            && (trkecalmatch&12)==12  //-- |dX|<3 and |dY|<10
            && chi2x < 20 && chi2y < 20
            // && qin > 0. && qin < 1.5
            && qin > 0.7 && qin < 1.5
            // && ntrk < 2;
            // && ntrk == 1;
            && ntrk <= 2;
    //====trk--eff_tof&trd&ecal
    if(SelEffType == 1 || SelEffType == 2 || SelEffType == 4){
        cut_trk= 
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk == 1;
    }
    //====trk--eff_trk
    if(SelEffType == 3){
        if(passcut) cut_trk= 
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk=1;
    }
    //====trk--eff_trk----pat
    if(SelEffType == 31){
        if(passcut) cut_trk= 
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk= 
            (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
    }
    //====trk--eff_trk----match
    if(SelEffType == 32){
        if(passcut) cut_trk= 
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk=
            (pat&259)
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
    }
    //====trk--eff_trk----chi2
    if(SelEffType == 33){
        if(passcut) cut_trk=
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk=
            (pat&259)
            && (trkecalmatch&12)==12
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
    }
    //====trk--eff_trk----qin
    if(SelEffType == 34){
        if(passcut) cut_trk=
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk=
            (pat&259)
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && ntrk <= 2;
    }
    //====trk--eff_trk----ntrk
    if(SelEffType == 35){
        if(passcut) cut_trk=
            (pat&259) 
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5
            && ntrk <= 2;
        else cut_trk=
            (pat&259)
            && (trkecalmatch&12)==12
            && chi2x < 20 && chi2y < 20
            && qin > 0.7 && qin < 1.5;
    }
    
    //==================== ECAL
    bool cut_ecal = 
            // 1;
            hadflag==0;
    //====ecal--eff_ecal
    if(SelEffType == 4 ){
        cut_ecal = 
            hadflag==0;
    }
    //====ecal--eff_tof&trd&trk
    if(SelEffType == 1 || SelEffType == 2 || SelEffType == 3 || SelEffType/10 == 3){
        cut_ecal = 
            hadflag==0 
            && Lkhd0<2.8;
    }

    //==================== BDT
    bool cut_bdt = EmBDT>-0.995;
    //====bdt--eff_ecal
    if(SelEffType == 4 ){
        if(passcut) cut_bdt = 
            EmBDT>-0.995;
        else cut_bdt=1;
    }
    
    //==================== other
    bool cut_other = 1;
    // bool cut_other = mcp==0 && hadflag==0 && sf>sf_cutoff;
    // bool cut_other = hadflag==0 && sf>sf_cutoff;
    // bool cut_other = sf>sf_cutoff;

    //==================== total
    bool temp= cut_trd && cut_ecal && cut_tof && cut_trk && cut_bdt && cut_other;
    return temp;
}




void FitHist::FITHIST_InitH_Template(){
    //======== template--num
    hEleSample=new TH1D("EleSample","ElectronSample",lkhd_nbin,lkhd_min,lkhd_max);
    hProSample=new TH1D("ProSample","ProtonSample",lkhd_nbin,lkhd_min,lkhd_max);
}


void FitHist::FITHIST_FillH_Template(){
    //======== init
    double fillvar=Lkhd0;
    //======== template--num
    if(FITHIST_cut_PreSelect(0,1)){
        if(rig<0 && trdlkhdnew<0.5) hEleSample->Fill(fillvar);//-- e/r>0.8
        if(rig>0 && trdlkhdnew>0.8) hProSample->Fill(fillvar);
    }
}


void FitHist::FITHIST_SaveH_Template(){
    hEleSample->Write();
    hProSample->Write();
}




void FitHist::FITHIST_InitH(){
    for(int ilat = 0; ilat < nlat; ilat++){
    for(int it = 0; it < nt; it++){
        //==== original
        hPosiSample[ilat][it]=new TH1D(Form("PosiSample_ilat%d_it%d",ilat,it),"PositiveSample",lkhd_nbin,lkhd_min,lkhd_max);
        hNegaSample[ilat][it]=new TH1D(Form("NegaSample_ilat%d_it%d",ilat,it),"NegativeSample",lkhd_nbin,lkhd_min,lkhd_max);
        hNegaSample_phy[ilat][it]=new TH1D(Form("NegaSample_phy_ilat%d_it%d",ilat,it),"NegativeSample with physical trigger",lkhd_nbin,lkhd_min,lkhd_max);
        hNegaSample_unphy[ilat][it]=new TH1D(Form("NegaSample_unphy_ilat%d_it%d",ilat,it),"NegativeSample with unphysical trigger",lkhd_nbin,lkhd_min,lkhd_max);
        hPosiSample_n[ilat][it]=new TH1D(Form("nPosiSample_ilat%d_it%d",ilat,it),"PositiveSample entries",1,0,2);
        hNegaSample_n[ilat][it]=new TH1D(Form("nNegaSample_ilat%d_it%d",ilat,it),"NegativeSample entries ",1,0,2);
    }
    }
    //----ecaltest
    h1_EoP=new TH1D("EoP","E over P",150,0,15);
    h2_EoP_embdt=new TH2D("EoP_embdt","E over P vs EmbDT",150,0,15,60,-3,3);
    //----260407 seleffhist
    for(int ilat = 0; ilat < nlat; ilat++){
    for(int it = 0; it < nt; it++){
        //----seleff
        hTOFpass[ilat][it]=new TH1D(Form("TOFpass_ilat%d_it%d",ilat,it),"TOF Sample Pass",1,0,2);
        hTOFtotal[ilat][it]=new TH1D(Form("TOFtotal_ilat%d_it%d",ilat,it),"TOF Sample Total",1,0,2);
        hTRDpass[ilat][it]=new TH1D(Form("TRDpass_ilat%d_it%d",ilat,it),"TRD Sample Pass",1,0,2);
        hTRDtotal[ilat][it]=new TH1D(Form("TRDtotal_ilat%d_it%d",ilat,it),"TRD Sample Total",1,0,2);
        hECALpass[ilat][it]=new TH1D(Form("ECALpass_ilat%d_it%d",ilat,it),"ECAL Sample Pass",1,0,2);
        hECALtotal[ilat][it]=new TH1D(Form("ECALtotal_ilat%d_it%d",ilat,it),"ECAL Sample Total",1,0,2);
        hTrkpass[ilat][it]=new TH1D(Form("Trkpass_ilat%d_it%d",ilat,it),"Tracker Sample Pass",1,0,2);
        hTrktotal[ilat][it]=new TH1D(Form("Trktotal_ilat%d_it%d",ilat,it),"Tracker Sample Total",1,0,2);
        //----trkeff
        hTrkpass_pat[ilat][it]=new TH1D(Form("Trkpass_pat_ilat%d_it%d",ilat,it),"Tracker Sample Pass with pat",1,0,2);
        hTrktotal_pat[ilat][it]=new TH1D(Form("Trktotal_pat_ilat%d_it%d",ilat,it),"Tracker Sample Total with pat",1,0,2);
        hTrkpass_match[ilat][it]=new TH1D(Form("Trkpass_match_ilat%d_it%d",ilat,it),"Tracker Sample Pass with match",1,0,2);
        hTrktotal_match[ilat][it]=new TH1D(Form("Trktotal_match_ilat%d_it%d",ilat,it),"Tracker Sample Total with match",1,0,2);
        hTrkpass_chi2[ilat][it]=new TH1D(Form("Trkpass_chi2_ilat%d_it%d",ilat,it),"Tracker Sample Pass with chi2",1,0,2);
        hTrktotal_chi2[ilat][it]=new TH1D(Form("Trktotal_chi2_ilat%d_it%d",ilat,it),"Tracker Sample Total with chi2",1,0,2);
        hTrkpass_qin[ilat][it]=new TH1D(Form("Trkpass_qin_ilat%d_it%d",ilat,it),"Tracker Sample Pass with qin",1,0,2);
        hTrktotal_qin[ilat][it]=new TH1D(Form("Trktotal_qin_ilat%d_it%d",ilat,it),"Tracker Sample Total with qin",1,0,2);
        hTrkpass_ntrk[ilat][it]=new TH1D(Form("Trkpass_ntrk_ilat%d_it%d",ilat,it),"Tracker Sample Pass with ntrk",1,0,2);
        hTrktotal_ntrk[ilat][it]=new TH1D(Form("Trktotal_ntrk_ilat%d_it%d",ilat,it),"Tracker Sample Total with ntrk",1,0,2);
    }
    }
}


void FitHist::FITHIST_FillH(int iene,int ilat,int it,int entry){
    //======== init
    double fillvar=Lkhd0;
    //============ passhist ============
    //======== samplecut
    bool samplecut=
        //====trig+trdlkhd
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6;
        lvl1_PhysBPatt&62 && trdlkhdnew<0.7;
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.8;
        //====trig+eop
        // lvl1_PhysBPatt&62 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00; //----260506 
        //====trig+trdlkhd+eop
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00;
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.7 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00; //----260322 
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.8 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00;
    //======== trigsamplecut
    bool trigsamplecut = 
        //====rig+trdlkhd
        rig<0 && trdlkhdnew<0.8;
        //====rig+eop
        // rig<0 && Ene/TMath::Abs(rig)>0.65;
        //====rig+trdlkhd+eop01
        // rig<0 && trdlkhdnew<0.6 && Ene/TMath::Abs(rig)>0.65;
        // rig<0 && trdlkhdnew<0.7 && Ene/TMath::Abs(rig)>0.65;
        //====rig+trdlkhd+eop02
        // rig>0 && trdlkhdnew<0.6 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00; //----260401
        // rig<0 && trdlkhdnew<0.8 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00;
    //======== fillhist01
    if(FITHIST_cut_PreSelect(0,1)){
        //==== sample
        if(samplecut){    //---- for ele<0.8, posi<0.6
            if(rig>0){
                hPosiSample[ilat][it]->Fill(fillvar);
                hPosiSample_n[ilat][it]->Fill(1,1);
            }
            else if(rig<0){
                hNegaSample[ilat][it]->Fill(fillvar);
                hNegaSample_n[ilat][it]->Fill(1,1);
            }
        }
        //==== trigsample
        if(trigsamplecut){
            if(lvl1_PhysBPatt&62) hNegaSample_phy[ilat][it]->Fill(fillvar);
            else hNegaSample_unphy[ilat][it]->Fill(fillvar);
        }
    }


    //============ ecaltest ============
    if(EventSelectEffType && FITHIST_cut_PreSelect(0,0)){
        if(rig<0 && samplecut) h1_EoP->Fill(Ene/TMath::Abs(rig));
        if(rig<0 && samplecut) h2_EoP_embdt->Fill(Ene/TMath::Abs(rig),EmBDT);
    }

 
    //============ seleffhist ============
    //====tof
    samplecut=
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6;
        lvl1_PhysBPatt&62 && trdlkhdnew<0.7;
    if(FITHIST_cut_PreSelect(1,1) && samplecut){
        if(rig<0) hTOFpass[ilat][it]->Fill(1,1);
    }
    if(FITHIST_cut_PreSelect(1,0) && samplecut){
        if(rig<0) hTOFtotal[ilat][it]->Fill(1,1);
    }
    //====trd
    samplecut=
        lvl1_PhysBPatt&62;
    if(FITHIST_cut_PreSelect(2,1) && samplecut){
        if(rig<0) hTRDpass[ilat][it]->Fill(1,1);
    }
    if(FITHIST_cut_PreSelect(2,0) && samplecut){
        if(rig<0) hTRDtotal[ilat][it]->Fill(1,1);
    }
    //====trk
    samplecut=
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6;
        lvl1_PhysBPatt&62 && trdlkhdnew<0.7;
    if(FITHIST_cut_PreSelect(3,1) && samplecut){
        if(rig<0) hTrkpass[ilat][it]->Fill(1,1);
    }
    if(FITHIST_cut_PreSelect(3,0) && samplecut){
        if(rig<0) hTrktotal[ilat][it]->Fill(1,1);
    }
    //========trk--pat
    if(FITHIST_cut_PreSelect(31,1) && samplecut && rig<0) hTrkpass_pat[ilat][it]->Fill(1,1);
    if(FITHIST_cut_PreSelect(31,0) && samplecut && rig<0) hTrktotal_pat[ilat][it]->Fill(1,1);
    //========trk--match
    if(FITHIST_cut_PreSelect(32,1) && samplecut && rig<0) hTrkpass_match[ilat][it]->Fill(1,1);
    if(FITHIST_cut_PreSelect(32,0) && samplecut && rig<0) hTrktotal_match[ilat][it]->Fill(1,1);
    //========trk--chi2
    if(FITHIST_cut_PreSelect(33,1) && samplecut && rig<0) hTrkpass_chi2[ilat][it]->Fill(1,1);
    if(FITHIST_cut_PreSelect(33,0) && samplecut && rig<0) hTrktotal_chi2[ilat][it]->Fill(1,1);
    //========trk--qin
    if(FITHIST_cut_PreSelect(34,1) && samplecut && rig<0) hTrkpass_qin[ilat][it]->Fill(1,1);
    if(FITHIST_cut_PreSelect(34,0) && samplecut && rig<0) hTrktotal_qin[ilat][it]->Fill(1,1);
    //========trk--ntrk
    if(FITHIST_cut_PreSelect(35,1) && samplecut && rig<0) hTrkpass_ntrk[ilat][it]->Fill(1,1);
    if(FITHIST_cut_PreSelect(35,0) && samplecut && rig<0) hTrktotal_ntrk[ilat][it]->Fill(1,1);
    //====ecal
    samplecut=
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6 && TMath::Abs(Ene/rig)>0.65 && TMath::Abs(Ene/rig)<5.00;
        lvl1_PhysBPatt&62 && trdlkhdnew<0.7;
    if(FITHIST_cut_PreSelect(4,1) && samplecut){
        if(rig<0) hECALpass[ilat][it]->Fill(1,1);
    }
    samplecut=
        // lvl1_PhysBPatt&62 && trdlkhdnew<0.6;
        lvl1_PhysBPatt&62 && trdlkhdnew<0.7;
    if(FITHIST_cut_PreSelect(4,0) && samplecut){
        if(rig<0) hECALtotal[ilat][it]->Fill(1,1);
    }
}


void FitHist::FITHIST_SaveH(){
    for (int ilat = 0; ilat < nlat; ilat++){
    for (int it = 0; it < nt; it++){
        //============ sample
        if(hNegaSample[ilat][it]->GetEntries()>0){
            //==== sample
            hPosiSample[ilat][it]->Write();
            hNegaSample[ilat][it]->Write();
            //==== nsample
            hPosiSample_n[ilat][it]->Write();
            hNegaSample_n[ilat][it]->Write();
            //==== sampletrigger
            hNegaSample_phy[ilat][it]->Write();
            hNegaSample_unphy[ilat][it]->Write();
        }
        //============ seleff
        hTOFpass[ilat][it]->Write();
        hTOFtotal[ilat][it]->Write();
        hTRDpass[ilat][it]->Write();
        hTRDtotal[ilat][it]->Write();
        hECALpass[ilat][it]->Write();
        hECALtotal[ilat][it]->Write();
        hTrkpass[ilat][it]->Write();
        hTrktotal[ilat][it]->Write();
        //============ trkeff
        hTrkpass_pat[ilat][it]->Write();
        hTrktotal_pat[ilat][it]->Write();
        hTrkpass_match[ilat][it]->Write();
        hTrktotal_match[ilat][it]->Write();
        hTrkpass_chi2[ilat][it]->Write();
        hTrktotal_chi2[ilat][it]->Write();
        hTrkpass_qin[ilat][it]->Write();
        hTrktotal_qin[ilat][it]->Write();
        hTrkpass_ntrk[ilat][it]->Write();
        hTrktotal_ntrk[ilat][it]->Write();
    }
    }
    h1_EoP->Write();
    h2_EoP_embdt->Write();
}


void FitHist::FITHIST_SaveH(int ilat,int it){
    //============ sample
    if(hNegaSample[ilat][it]->GetEntries()>0){
        //==== sample
        hPosiSample[ilat][it]->Write();
        hNegaSample[ilat][it]->Write();
        hNegaSample_phy[ilat][it]->Write();
        hNegaSample_unphy[ilat][it]->Write();
        //==== sample entries
        hPosiSample_n[ilat][it]->Write();
        hNegaSample_n[ilat][it]->Write();
    }
    h1_EoP->Write();
    h2_EoP_embdt->Write();
    //============ seleff
    hTOFpass[ilat][it]->Write();
    hTOFtotal[ilat][it]->Write();
    hTRDpass[ilat][it]->Write();
    hTRDtotal[ilat][it]->Write();
    hECALpass[ilat][it]->Write();
    hECALtotal[ilat][it]->Write();
    hTrkpass[ilat][it]->Write();
    hTrktotal[ilat][it]->Write();
    //============ trkeff
    hTrkpass_pat[ilat][it]->Write();
    hTrktotal_pat[ilat][it]->Write();
    hTrkpass_match[ilat][it]->Write();
    hTrktotal_match[ilat][it]->Write();
    hTrkpass_chi2[ilat][it]->Write();
    hTrktotal_chi2[ilat][it]->Write();
    hTrkpass_qin[ilat][it]->Write();
    hTrktotal_qin[ilat][it]->Write();
    hTrkpass_ntrk[ilat][it]->Write();
    hTrktotal_ntrk[ilat][it]->Write();
}


//-- 遍历所有hist的lkhdbin,如果模板在该bin取值<1且样本在该bin有值,将样本置0
//-- 也就是说,把所有的模板无取值的bin对应的样本bin全部置0
void FitHist::FITHIST_ResetH(int ilat,int it){
    for (int i_lkhdbin = 1; i_lkhdbin <= lkhd_nbin; i_lkhdbin++){
        if (hEleSample->GetBinContent(i_lkhdbin)<1 && hProSample->GetBinContent(i_lkhdbin)<1){
            if(hPosiSample[ilat][it]->GetBinContent(i_lkhdbin)) hPosiSample[ilat][it]->SetBinContent(i_lkhdbin,0);
            if(hNegaSample[ilat][it]->GetBinContent(i_lkhdbin)) hNegaSample[ilat][it]->SetBinContent(i_lkhdbin,0);
            if(hNegaSample_phy[ilat][it]->GetBinContent(i_lkhdbin)) hNegaSample_phy[ilat][it]->SetBinContent(i_lkhdbin,0);
            if(hNegaSample_unphy[ilat][it]->GetBinContent(i_lkhdbin)) hNegaSample_unphy[ilat][it]->SetBinContent(i_lkhdbin,0);
        }
    }
}


//-- 250518
void FitHist::FITHIST_FillFitR(int ilat,int it){
    //==== SampleEntries
    fitR.n_PosiSample=hPosiSample_n[ilat][it]->GetBinContent(1);
    fitR.n_PosiSample_err=hPosiSample_n[ilat][it]->GetBinError(1);
    fitR.n_NegaSample=hNegaSample_n[ilat][it]->GetBinContent(1);
    fitR.n_NegaSample_err=hNegaSample_n[ilat][it]->GetBinError(1);
    //==== calc cc ratio 01
    // if(fitR.n_PosiSample + fitR.n_NegaSample > 0){
    //     fitR.cc = fitR.n_PosiSample / (fitR.n_PosiSample + fitR.n_NegaSample);
    //     fitR.cc_err = TMath::Sqrt(
    //         TMath::Power(fitR.n_NegaSample * fitR.n_PosiSample_err,2) + TMath::Power(fitR.n_PosiSample * fitR.n_NegaSample_err,2)
    //     ) / TMath::Power(fitR.n_PosiSample + fitR.n_NegaSample,2);
    // }
    //==== calc cc ratio 02
    if(fitR.npos + fitR.nele > 0){
        fitR.cc = fitR.npos / (fitR.npos + fitR.nele);
        fitR.cc_err = TMath::Sqrt(
            TMath::Power(fitR.nele * fitR.npos_err,2) + TMath::Power(fitR.npos * fitR.nele_err,2)
        ) / TMath::Power(fitR.npos + fitR.nele,2);
    }
    //==== seleff SampleEntries 260407
    fitR.nTOFpass=hTOFpass[ilat][it]->GetBinContent(1);
    fitR.nTOFpass_err=hTOFpass[ilat][it]->GetBinError(1);
    fitR.nTOFtotal=hTOFtotal[ilat][it]->GetBinContent(1);
    fitR.nTOFtotal_err=hTOFtotal[ilat][it]->GetBinError(1);
    fitR.nTRDpass=hTRDpass[ilat][it]->GetBinContent(1);
    fitR.nTRDpass_err=hTRDpass[ilat][it]->GetBinError(1);
    fitR.nTRDtotal=hTRDtotal[ilat][it]->GetBinContent(1);
    fitR.nTRDtotal_err=hTRDtotal[ilat][it]->GetBinError(1);
    fitR.nECALpass=hECALpass[ilat][it]->GetBinContent(1);
    fitR.nECALpass_err=hECALpass[ilat][it]->GetBinError(1);
    fitR.nECALtotal=hECALtotal[ilat][it]->GetBinContent(1);
    fitR.nECALtotal_err=hECALtotal[ilat][it]->GetBinError(1);
    fitR.nTrkpass=hTrkpass[ilat][it]->GetBinContent(1);
    fitR.nTrkpass_err=hTrkpass[ilat][it]->GetBinError(1);
    fitR.nTrktotal=hTrktotal[ilat][it]->GetBinContent(1);
    fitR.nTrktotal_err=hTrktotal[ilat][it]->GetBinError(1);
    //==== trkeff SampleEntries 260517
    fitR.nPATpass=hTrkpass_pat[ilat][it]->GetBinContent(1);
    fitR.nPATpass_err=hTrkpass_pat[ilat][it]->GetBinError(1);
    fitR.nPATtotal=hTrktotal_pat[ilat][it]->GetBinContent(1);
    fitR.nPATtotal_err=hTrktotal_pat[ilat][it]->GetBinError(1);
    fitR.nMATCHpass=hTrkpass_match[ilat][it]->GetBinContent(1);
    fitR.nMATCHpass_err=hTrkpass_match[ilat][it]->GetBinError(1);
    fitR.nMATCHtotal=hTrktotal_match[ilat][it]->GetBinContent(1);
    fitR.nMATCHtotal_err=hTrktotal_match[ilat][it]->GetBinError(1);
    fitR.nCHI2pass=hTrkpass_chi2[ilat][it]->GetBinContent(1);
    fitR.nCHI2pass_err=hTrkpass_chi2[ilat][it]->GetBinError(1);
    fitR.nCHI2total=hTrktotal_chi2[ilat][it]->GetBinContent(1);
    fitR.nCHI2total_err=hTrktotal_chi2[ilat][it]->GetBinError(1);
    fitR.nQINpass=hTrkpass_qin[ilat][it]->GetBinContent(1);
    fitR.nQINpass_err=hTrkpass_qin[ilat][it]->GetBinError(1);
    fitR.nQINtotal=hTrktotal_qin[ilat][it]->GetBinContent(1);
    fitR.nQINtotal_err=hTrktotal_qin[ilat][it]->GetBinError(1);
    fitR.nNTRKpass=hTrkpass_ntrk[ilat][it]->GetBinContent(1);
    fitR.nNTRKpass_err=hTrkpass_ntrk[ilat][it]->GetBinError(1);
    fitR.nNTRKtotal=hTrktotal_ntrk[ilat][it]->GetBinContent(1);
    fitR.nNTRKtotal_err=hTrktotal_ntrk[ilat][it]->GetBinError(1);
}


bool FitHist::FITHIST_cut_FitCondition(int ilat,int it){
    double n_ele = hEleSample->Integral(1,lkhd_nbin);
    double n_pro = hProSample->Integral(1,lkhd_nbin);
    double n_nega = hNegaSample[ilat][it]->Integral(1,lkhd_nbin);
    double n_posi = hPosiSample[ilat][it]->Integral(1,lkhd_nbin);
    bool temp = 
        // hEleSample->GetEntries()>0
        // && hProSample->GetEntries()>0
        // && hNegaSample[ilat][it]->GetEntries()>0
        // && hPosiSample[ilat][it]->GetEntries()>0;
        n_ele>0
        && n_pro>0
        && n_nega>0
        && n_posi>0;
    return temp;
}