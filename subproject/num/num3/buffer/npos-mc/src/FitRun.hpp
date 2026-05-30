_Pragma("once")

#include<iostream>
using namespace std;
#include<TF1.h>
#include<TChain.h>
#include "TStyle.h"
//====
#include "../include/rootclass_gcx/ToolFileName.hpp"
#include "../include/rootclass_gcx/ToolChain.hpp"
#include "../include/rootclass_gcx/ToolFileOut.hpp"
#include "../include/rootclass_gcx/ToolTreeArray.hpp"
#include "../include/general/ConsoleDisplay.h"
//====
#include "Fit.hpp"


class FitRun{
    public:
        FitRun(){cout<<"IN FitRun ===== Constructor_fitrun"<<endl;}
        FitRun(TString fn):
        filename(fn),
        ins_filename(fn),ins_chain(),ins_treearray(),ins_fileout(),
        ins_fit()
        {cout<<"IN FitRun ===== Constructor_fitrun"<<endl;}
        ~FitRun(){}
    public:
        TString filename;
    public:
        ToolFileName ins_filename;
        ToolChain ins_chain;
        ToolFileOut ins_fileout;
        ToolTreeArray ins_treearray;
    public:
        Fit ins_fit;
    public:
        void FIT_RUN_iene(int,vector<int>,int printlv=0);
};

void FitRun::FIT_RUN_iene(int i_enebin,vector<int>bin_division,int printlv){
    //============ init--selefftype
    //---- 0-all 1-TOF 2-TRD 3-trk 4-ecal 
    ins_fit.EventSelectEffType=0;
    //============ init--bindivision
    //---- Bin_Division
    //* 1. nolatbin--1; 2. NoPerBin--1; 3. DayPeriod--1
    // ins_fit.NoLatBin(bin_division.at(0));
    // ins_fit.NoPeriod(bin_division.at(1));
    // ins_fit.DayPeriod(bin_division.at(2));
    //============ initlength
    int nlat=ins_fit.GetNLat();
    int nt=ins_fit.GetNPeriod();
    //============ saveoption
    // bool saveall=1;
    bool saveall=0;
    int it_save=0;
    if(ins_fit.wt==60*60*24) it_save=3000;   //----for 1 day
    else if(ins_fit.wt==60*60*24*27) it_save=80;    //----for 27 days
    else if(ins_fit.wt==60*60*24*6000) it_save=0;    //----for notbin
    int ilat_save=0;
    
    
    //============ init ============
    ins_filename.InitFileName();
    TString treename=Form("tbin%02d", i_enebin);
    ins_chain.InitChain(treename);
    ins_chain.AddTree(ins_filename.GetFullName(),i_enebin+1,1);
    ins_fit.INIT_CHAIN_SetBranchAddress(ins_chain.GetChain());
    //----260419.01
    ins_treearray.InitTreeArray(vector<int>{nlat,1});
    ins_fit.INIT_TREEARRAY_BuildBrand(vector<int>{nlat,1},ins_treearray.GetTreeArray());
    ins_fileout.InitFileOut(Form("fitresult_enebin%02d_%gGeV.root",i_enebin,ins_fit.energy_bins[i_enebin]));
    
    
    //============ hist ============
    //======== inithist
    ins_fit.FITHIST_InitH_Template();
    ins_fit.FITHIST_InitH();
    //======== traversal
    long nentries = ins_chain.GetChain()->GetEntries();
    ConsoleDisplay mydisplay_hist(nentries);
    for(long entry=0; entry<nentries; entry++){
        //======== print
        mydisplay_hist.Update(entry);
        ins_chain.GetChain()->GetEntry(entry);
            // if(entry%1000000==0) cout<<"ene: "<<ins_fit.energy_bins[i_enebin]<<"; cfi[0]: "<<ins_fit.cfi[0]<<endl;
        //======== cut--time
        // if(ins_select.utime<1305763200) continue;
        // if(ins_select.utime>1385510400) continue;
        // if(ins_select.utime>1510444800) continue;
        //----260419
        // if(ins_fit.utime<1305849600) continue;
        // if(ins_fit.utime>1635811200) continue;
        //----260510
        // if(ins_fit.utime<1305849600) continue;
        // if(ins_fit.utime>1761955200) continue;
        //======== cut--geomag
        //====stormer
        // if(ins_fit.energy_bins[i_enebin]<1.2*ins_fit.cf[0]) continue;
        //====igrf
        // if(ins_fit.energy_bins[i_enebin]<1.1*ins_fit.cfi[0]) continue;
        // if(ins_fit.energy_bins[i_enebin]<1.2*ins_fit.cfi[0]) continue;
        //======== cut--badrun
        // if(ins_fit.isbadrun_daq || ins_fit.isbadrun_fov || ins_fit.isbadrun_ecal || ins_fit.isbadrun_trk) continue;
        // if(ins_fit.utime>=1305417600 && ins_fit.utime<=1307750400 && ins_fit.isbadrun_general) continue;
        //======== it
        // int it = (ins_fit.utime-ins_fit.tmin)/ins_fit.wt;
        // if(it<0 || it>=nt) continue;
        int it = 0;
        //======== imlat
        // int imlat = -1;
        // double wlat = ins_fit.wlat;
        // if(info_mlat >= ins_fit.latmin && info_mlat < ins_fit.latmax){
        //     imlat = static_cast<int>((info_mlat - ins_fit.latmin) / wlat);
        // }else if(info_mlat == ins_fit.latmax){
        //     imlat = nlat - 1;
        // }
        // if(imlat < 0 || imlat >= nlat) continue;
        // // //----260416
        // if(entry%10000==0) cout<<"info_mlat: "<<info_mlat<<" ; imlat: "<<imlat<<endl;
        int imlat=0;
        //======== fill
        ins_fit.FITHIST_FillH_Template();
        ins_fit.FITHIST_FillH(i_enebin,imlat,it,entry);
    }
    mydisplay_hist.Finish();
    ins_fit.FITHIST_SaveH_Template();
    if(saveall) ins_fit.FITHIST_SaveH();
    else ins_fit.FITHIST_SaveH(ilat_save,it_save);
    
    
    //============ fit ============
    cout<<"=============================================="<<endl;
    cout<<"========fit========"<<endl;
    ConsoleDisplay mydisplay_fit((long)nlat*nt);
    long nfitcond_fail = 0;
    for (int ilat = 0; ilat < nlat; ilat++){
    for (int it = 0; it < nt; it++){
        //============ output
        mydisplay_fit.Update((long)ilat*nt+it);
        //============ init
        //==== resetfitR
        ins_fit.fitR = {};
        //==== fillfitR01--ene
        ins_fit.fitR.Eid=i_enebin;
        ins_fit.fitR.Elow=ins_fit.energy_bins[i_enebin];
        ins_fit.fitR.Eup=ins_fit.energy_bins[i_enebin+1];
        //==== fillfitR01--time
        double tlow=ins_fit.tmin+(it)*ins_fit.wt;   //---- 251101 for l.j. t0unix(16pm)
        int tmid=ins_fit.tmin+(it+0.5)*ins_fit.wt;
        int tup=ins_fit.tmin+(it+1)*ins_fit.wt;
        ins_fit.fitR.tid=it;
        ins_fit.fitR.tlow=tlow;
        ins_fit.fitR.tmid=tmid;
        ins_fit.fitR.tup=tup;
        //==== fillfitR02
        ins_fit.FITHIST_FillFitR(ilat,it);
        //==== fillfitR03--seleff
        if(ins_fit.fitR.nTOFtotal>0){
            ins_fit.fitR.tofeff = ins_fit.fitR.nTOFpass/ins_fit.fitR.nTOFtotal;
            ins_fit.fitR.tofeff_err = TMath::Sqrt(ins_fit.fitR.tofeff*(1-ins_fit.fitR.tofeff)/ins_fit.fitR.nTOFtotal);
        }
        if(ins_fit.fitR.nTRDtotal>0){
            ins_fit.fitR.trdeff = ins_fit.fitR.nTRDpass/ins_fit.fitR.nTRDtotal;
            ins_fit.fitR.trdeff_err = TMath::Sqrt(ins_fit.fitR.trdeff*(1-ins_fit.fitR.trdeff)/ins_fit.fitR.nTRDtotal);
        }
        if(ins_fit.fitR.nECALtotal>0){
            ins_fit.fitR.ecaleff = ins_fit.fitR.nECALpass/ins_fit.fitR.nECALtotal;
            ins_fit.fitR.ecaleff_err = TMath::Sqrt(ins_fit.fitR.ecaleff*(1-ins_fit.fitR.ecaleff)/ins_fit.fitR.nECALtotal);
        }
        if(ins_fit.fitR.nTrktotal>0){
            ins_fit.fitR.trkeff = ins_fit.fitR.nTrkpass/ins_fit.fitR.nTrktotal;
            ins_fit.fitR.trkeff_err = TMath::Sqrt(ins_fit.fitR.trkeff*(1-ins_fit.fitR.trkeff)/ins_fit.fitR.nTrktotal);
        }
        //----260517 trkeff
        if(ins_fit.fitR.nPATtotal>0){
            ins_fit.fitR.pateff = ins_fit.fitR.nPATpass/ins_fit.fitR.nPATtotal;
            ins_fit.fitR.pateff_err = TMath::Sqrt(ins_fit.fitR.pateff*(1-ins_fit.fitR.pateff)/ins_fit.fitR.nPATtotal);
        }
        if(ins_fit.fitR.nMATCHtotal>0){
            ins_fit.fitR.matcheff = ins_fit.fitR.nMATCHpass/ins_fit.fitR.nMATCHtotal;
            ins_fit.fitR.matcheff_err = TMath::Sqrt(ins_fit.fitR.matcheff*(1-ins_fit.fitR.matcheff)/ins_fit.fitR.nMATCHtotal);
        }
        if(ins_fit.fitR.nCHI2total>0){
            ins_fit.fitR.chi2eff = ins_fit.fitR.nCHI2pass/ins_fit.fitR.nCHI2total;
            ins_fit.fitR.chi2eff_err = TMath::Sqrt(ins_fit.fitR.chi2eff*(1-ins_fit.fitR.chi2eff)/ins_fit.fitR.nCHI2total);
        }
        if(ins_fit.fitR.nQINtotal>0){
            ins_fit.fitR.qineff = ins_fit.fitR.nQINpass/ins_fit.fitR.nQINtotal;
            ins_fit.fitR.qineff_err = TMath::Sqrt(ins_fit.fitR.qineff*(1-ins_fit.fitR.qineff)/ins_fit.fitR.nQINtotal);
        }
        if(ins_fit.fitR.nNTRKtotal>0){
            ins_fit.fitR.ntrkeff = ins_fit.fitR.nNTRKpass/ins_fit.fitR.nNTRKtotal;
            ins_fit.fitR.ntrkeff_err = TMath::Sqrt(ins_fit.fitR.ntrkeff*(1-ins_fit.fitR.ntrkeff)/ins_fit.fitR.nNTRKtotal);
        }
        //----260518 totaleff
        // double totaleff = ins_fit.fitR.tofeff * ins_fit.fitR.trdeff * ins_fit.fitR.ecaleff * ins_fit.fitR.trkeff;
        // double totaleff_err = totaleff * TMath::Sqrt(
        //     TMath::Power(ins_fit.fitR.tofeff_err/ins_fit.fitR.tofeff,2) +
        //     TMath::Power(ins_fit.fitR.trdeff_err/ins_fit.fitR.trdeff,2) +
        //     TMath::Power(ins_fit.fitR.ecaleff_err/ins_fit.fitR.ecaleff,2) +
        //     TMath::Power(ins_fit.fitR.trkeff_err/ins_fit.fitR.trkeff,2)
        // );
        double totaleff = ins_fit.fitR.tofeff * ins_fit.fitR.trdeff * ins_fit.fitR.ecaleff * ins_fit.fitR.pateff * ins_fit.fitR.matcheff * ins_fit.fitR.chi2eff * ins_fit.fitR.qineff * ins_fit.fitR.ntrkeff;
        double totaleff_err = totaleff * TMath::Sqrt(
            TMath::Power(ins_fit.fitR.tofeff_err/ins_fit.fitR.tofeff,2) +
            TMath::Power(ins_fit.fitR.trdeff_err/ins_fit.fitR.trdeff,2) +
            TMath::Power(ins_fit.fitR.ecaleff_err/ins_fit.fitR.ecaleff,2) +
            TMath::Power(ins_fit.fitR.pateff_err/ins_fit.fitR.pateff,2) +
            TMath::Power(ins_fit.fitR.matcheff_err/ins_fit.fitR.matcheff,2) +
            TMath::Power(ins_fit.fitR.chi2eff_err/ins_fit.fitR.chi2eff,2) +
            TMath::Power(ins_fit.fitR.qineff_err/ins_fit.fitR.qineff,2) +
            TMath::Power(ins_fit.fitR.ntrkeff_err/ins_fit.fitR.ntrkeff,2)
        );
        ins_fit.fitR.totaleff = totaleff;
        ins_fit.fitR.totaleff_err = totaleff_err;
        //============ fit
        // cout<<"========fit--index======== ilat "<<ilat<<" ; it "<<it<<endl;
        //====loop skip
        // if(1) continue;
        // if(ins_fit.EventSelectEffType) continue;
        if(!ins_fit.FITHIST_cut_FitCondition(ilat,it)){
            nfitcond_fail++;
            if(ins_fit.EventSelectEffType==0) ins_treearray.FillTreeArray(0);
            continue;
        }
        //==== dofit
        ins_fit.FITHIST_ResetH(ilat,it);
        ins_fit.DOFIT_InitRHist(ilat,it,0);
        ins_fit.DOFIT_DoFit(ilat,it);
        //==== calc eff
        if(it%1000==0) cout<<"========trig========"<<endl;
        ins_fit.DOFIT_CalTrigEff(i_enebin,ilat,it);
        //==== calc posfrac 260304
        if(ins_fit.fitR.nele + ins_fit.fitR.npos > 0){
            ins_fit.fitR.posfrac = ins_fit.fitR.npos / (ins_fit.fitR.nele + ins_fit.fitR.npos);
            ins_fit.fitR.posfrac_err = TMath::Sqrt(
                TMath::Power((ins_fit.fitR.nele + ins_fit.fitR.npos) * ins_fit.fitR.npos_err,2) + TMath::Power(ins_fit.fitR.npos * TMath::Sqrt(ins_fit.fitR.nele + ins_fit.fitR.npos),2)
            ) / TMath::Power(ins_fit.fitR.nele + ins_fit.fitR.npos,2);
        }
        //==== filltree
        ins_treearray.FillTreeArray(0);
        //============ draw
        ins_fit.FITDRAW_InitDraw(i_enebin,it,it_save);
        ins_fit.FITDRAW_DoDraw(i_enebin,it,tlow);
        // ins_fit.FITDRAW_DoDraw_ESEFF(i_perbin);
        ins_fit.FITDRAW_DoChi2Test();
        // ins_fileout.GetFileOut()->cd();
        // if(always_save_flag || i_perbin==i_perbin_save) ins_fit.FITDRAW_SaveDraw(i_enebin,i_latbin,i_perbin);
        if(ilat==ilat_save && it==it_save) ins_fit.FITDRAW_SaveDraw(i_enebin,it);
        // ins_fit.FITDRAW_Clear();
        // ins_fit.DOFIT_Clear();
        // ins_fileout.GetFileOut()->Close();
    }
    }
    mydisplay_fit.Finish();
    cout<<"========fit condition failed: "<<nfitcond_fail<<" / "<<((long)nlat*nt)<<endl;
    
    
    //============ save ============
    ins_fileout.GetFileOut()->cd();
    ins_treearray.WriteTreeArray();
    ins_fileout.GetFileOut()->Close();
}
