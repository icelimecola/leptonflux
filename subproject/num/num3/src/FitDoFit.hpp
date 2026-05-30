_Pragma("once")

#include<iostream>
using namespace std;
#include<TF1.h>
#include<TChain.h>
#include "RooRealVar.h"
#include<RooDataHist.h>
#include<RooHistPdf.h>
#include<RooAddPdf.h>
#include<RooArgList.h>
#include <RooFitResult.h>
using namespace RooFit;
//====
#include "FitHist.hpp"

class FitDoFit:public FitHist{
    public:
        FitDoFit():FitHist(){cout<<"IN FitDoFit ===== Constructor_fitdofit"<<endl;}
        ~FitDoFit(){}
    public:
        RooRealVar *FitVar_lkhd;
        //-- count
        RooDataHist *FitH_PosiSample;
        RooDataHist *FitH_NegaSample;
        RooDataHist *FitH_NegaSample_phy;
        RooDataHist *FitH_NegaSample_unphy;
        RooDataHist *FitH_EleSample;
        RooDataHist *FitH_ProSample;
        RooHistPdf *FitPdf_ele;
        RooHistPdf *FitPdf_pro;
        RooRealVar *FitPara_nele;
        RooRealVar *FitPara_nccpro;
        RooRealVar *FitPara_npositron;
        RooRealVar *FitPara_nproton;
        RooAddPdf *FitAddPdf_ele;
        RooAddPdf *FitAddPdf_pos;

    public:
        void DOFIT_InitRHist(int,int,int printlv=0);
        void DOFIT_DoFit(int,int);
        void DOFIT_CalTrigEff(int,int,int);
};


    void FitDoFit::DOFIT_InitRHist(int ilat,int it,int printlv){
    //======== output
    // if(printlv==0) RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
    if(printlv==0) RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
    //======== fitvar
    // RVar_lkhd=new RooRealVar("trdlkhde2","TRD likelihood",lkhd_min,lkhd_max);
    FitVar_lkhd=new RooRealVar("Lkhd0","ECAL likelihood",lkhd_min,lkhd_max);
    FitVar_lkhd->setBins(lkhd_nbin);
    //======== fithist
    //---- template
    FitH_EleSample=new RooDataHist("PureEle","electron template",*FitVar_lkhd,hEleSample);
    FitH_ProSample=new RooDataHist("PurePro","proton template",*FitVar_lkhd,hProSample);
    //---- sample
    FitH_PosiSample=new RooDataHist("FitH_PosiSample","positive sample",*FitVar_lkhd,hPosiSample[ilat][it]);
    FitH_NegaSample=new RooDataHist("FitH_NegaSample","negative sample",*FitVar_lkhd,hNegaSample[ilat][it]);
    FitH_NegaSample_phy=new RooDataHist("FitH_NegaSample_phy","negative sample with physical trigger",*FitVar_lkhd,hNegaSample_phy[ilat][it]);
    FitH_NegaSample_unphy=new RooDataHist("FitH_NegaSample_unphy","negative sample with unphysical trigger",*FitVar_lkhd,hNegaSample_unphy[ilat][it]);
}


void FitDoFit::DOFIT_DoFit(int ilat,int it){
    //======== pdf
    FitPdf_ele=new RooHistPdf("FitPdf_ele","electron",*FitVar_lkhd,*FitH_EleSample);
    FitPdf_pro=new RooHistPdf("FitPdf_pro","proton",*FitVar_lkhd,*FitH_ProSample);
    //======== fit--negasample
    if(it%1000==0) cout<<"========fit--negasample======== ilat "<<ilat<<" ; it "<<it<<endl;
    // if(hNegaSample[ilat][it]->GetEntries()>0){
    if(hNegaSample[ilat][it]->Integral(1,lkhd_nbin)>0){
        //==== numvar
        FitPara_nele=new RooRealVar("FitPara_nele", "N_{e^{-}}", FitH_NegaSample->sumEntries()*0.9, 0, FitH_NegaSample->sumEntries()*2);
        FitPara_nccpro=new RooRealVar("FitPara_nccpro", "N_{p->e^{-}}", FitH_NegaSample->sumEntries()*0.1, 0, FitH_NegaSample->sumEntries()*2);
        //==== dofit
        FitAddPdf_ele=new RooAddPdf("FitAddPdf_ele", "model for electron fit", RooArgList(*FitPdf_pro, *FitPdf_ele), RooArgList(*FitPara_nccpro, *FitPara_nele));
        RooFitResult *FitResult_ele=FitAddPdf_ele->fitTo(*FitH_NegaSample, Extended(1), Save(1), Minos(1), PrintLevel(-1));
        // RooFitResult *FitResult_ele=FitAddPdf_ele->fitTo(*FitH_NegaSample, Extended(1), Save(1), PrintLevel(-1));
        //==== save num
        fitR.nele=FitPara_nele->getVal();
        fitR.nele_err=FitPara_nele->getError();
        fitR.nccpro=FitPara_nccpro->getVal();
        fitR.nccpro_err=FitPara_nccpro->getError();
    }
    else{
        fitR.nele=0;
        fitR.nele_err=0;
        fitR.nccpro=0;
        fitR.nccpro_err=0;
    }
    //======== fit--posisample
    if(it%1000==0) cout<<"========fit--posisample======== ilat "<<ilat<<" ; it "<<it<<endl;
    // if(hPosiSample[ilat][it]->GetEntries()>0){
    if(hPosiSample[ilat][it]->Integral(1,lkhd_nbin)>0){
        //==== numvar
        FitPara_npositron=new RooRealVar("FitPara_npositron", "N_{e^{+}}", FitH_PosiSample->sumEntries()*0.1, 0, FitH_PosiSample->sumEntries()*2);
        FitPara_nproton=new RooRealVar("FitPara_nproton", "N_{p->e^{+}}", FitH_PosiSample->sumEntries()*0.9, 0, FitH_PosiSample->sumEntries()*2);
        //==== dofit
        FitAddPdf_pos=new RooAddPdf("FitAddPdf_pos", "model for positron fit", RooArgList(*FitPdf_pro, *FitPdf_ele), RooArgList(*FitPara_nproton, *FitPara_npositron));
        RooFitResult *FitResult_pos=FitAddPdf_pos->fitTo(*FitH_PosiSample, Extended(1), Save(1), Minos(1), PrintLevel(-1));
        // RooFitResult *FitResult_pos=FitAddPdf_pos->fitTo(*FitH_PosiSample, Extended(1), Save(1), PrintLevel(-1));
        //==== save num
        fitR.npos=FitPara_npositron->getVal();
        fitR.npos_err=FitPara_npositron->getError();
        fitR.npro=FitPara_nproton->getVal();
        fitR.npro_err=FitPara_nproton->getError();
    }
    else{
        fitR.npos=0;
        fitR.npos_err=0;
        fitR.npro=0;
        fitR.npro_err=0;
    }
}


void FitDoFit::DOFIT_CalTrigEff(int iene,int ilat,int it){
    // if(hNegaSample_phy[ilat][it]->GetEntries()>0){
    if(hNegaSample_phy[ilat][it]->Integral(1,lkhd_nbin)>0){
        //======== fit--phy
        if(it%1000==0) cout<<"========fit--phy======== ilat "<<ilat<<" ; it "<<it<<endl;
        FitAddPdf_ele->fitTo( *FitH_NegaSample_phy, Extended(1), Save(1), Minos(1), PrintLevel(-1) );
        // FitAddPdf_ele->fitTo( *FitH_NegaSample_phy, Extended(1), Save(1), PrintLevel(-1) );
        fitR.nelephy = FitPara_nele->getVal();
        fitR.nelephy_err = FitPara_nele->getError();
        //======== fit--unphy
        if(it%1000==0) cout<<"========fit--unphy======== ilat "<<ilat<<" ; it "<<it<<endl;
        if( FitH_NegaSample_unphy->sumEntries() > 1 ){
            FitAddPdf_ele->fitTo( *FitH_NegaSample_unphy, Extended(1), Save(1), Minos(1), PrintLevel(-1) );
            // FitAddPdf_ele->fitTo( *FitH_NegaSample_unphy, Extended(1), Save(1), PrintLevel(-1) );
            fitR.neleunphy = FitPara_nele->getVal();
            fitR.neleunphy_err = FitPara_nele->getError();
        }else{
            fitR.neleunphy = 0;
            fitR.neleunphy_err = 0;
        }
        //======== scale--unphy
        const double trig_scale = 100;
        fitR.neleunphy *= trig_scale;
        fitR.neleunphy_err *= trig_scale;
        //======== trigeff
        if( fitR.neleunphy >= 1 ){
            fitR.trigeff = fitR.nelephy / (fitR.nelephy + fitR.neleunphy);
            fitR.trigeff_err = 
            sqrt(pow(fitR.neleunphy_err/fitR.neleunphy,2)+pow(fitR.nelephy_err/fitR.nelephy,2))*fitR.trigeff*(1 - fitR.trigeff);
        }else{
            fitR.trigeff = 1.;
            fitR.trigeff_err = 0.;
        }
    }
    else{
        fitR.nelephy=0;
        fitR.nelephy_err=0;
        fitR.neleunphy=0;
        fitR.neleunphy_err=0;
        fitR.trigeff=0.;
        fitR.trigeff_err=0.;
    }
}
