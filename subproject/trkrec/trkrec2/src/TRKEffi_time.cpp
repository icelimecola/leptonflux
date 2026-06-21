#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TChain.h"
#include "TSystem.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
using namespace std;

const int nbin=56;
double energy_bins[nbin+1] = {
    0.08,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,
    2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,
    7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,
    18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,
    38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,
    80.5,86.5,93.0,100.,108.,116.,125.
};
/*const int nbin=23;
  double energy_bins[nbin+1] = {
  0.08,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,
  2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,7.09,8.48,
  11.0,16.6,22.8,41.9
  };*/
const int nday = 5000;
const double t0 = 1305417600;//1304179200;//1305417600;
//const int nday = 200;
//const double t0 = 1318982400;//1326326400;
const int iv=5;

class TRKEffi{
    public:

        int ibetah,iecal,itrk;
        int itrtracks,ibetahs;
        UInt_t info_run,info_event,info_utime;
        Float_t info_lf,info_lfc;
        Float_t info_maxcf[4],info_maxcfi[4];

        int lvl1_PhysBPatt;

        int tof_nhits,tof_nusehit;
        int tof_nlay,tof_patt;
        Float_t tof_z,tof_probz;
        Float_t tof_chi2t, tof_chi2c;
        Float_t tof_betah;
        Float_t tof_qhlay[4];
        Float_t tof_trkxL[9],tof_trkyL[9];
        Float_t tof_ecalentry[2],tof_ecalexit[2];
        Float_t tof_trkl1qr,tof_trkl1qs;
        Float_t tof_trkl9qr,tof_trkl9qs;
        Float_t tof_tkl1qxy[2],tof_tkl9qxy[2];

        int ecal_nsh;
        Float_t ecal_dis,ecal_edep;
        Float_t ecal_bdtp,ecal_bdtp5s;
        Float_t ecal_enc,ecal_ene;
        Float_t ecal_enp,ecal_ene17;
        Float_t ecal_entry[2],ecal_exit[2];

        int trk_ntrk,trk_pat,trk_patx;
        Float_t trk_rigkf[2];
        Float_t trk_rig[2];
        Float_t trk_chi2x[2],trk_chi2y[2];
        Float_t trk_qin,trk_qinrms;
        Float_t trk_theta;
        Float_t trk_ecalentry[2],trk_ecalexit[2];
        Float_t trk_xL[9],trk_yL[9];

        int trd_nhits;
        Float_t trd_klkhd[3];

        bool prodc[iv];
        bool Inner,L1Inner,Fullspan,Innerhit;
        bool trkchi;
        int trkecalmatch;
        bool tofcharge,ecalvolume;

        TH2D *TH2_pro_cut[iv], *TH2_pro_refcut[iv];
        TH2D *TH2_pro_beta[iv], *TH2_pro_refbeta[iv];
        TH2D *TH2_pro_ene[iv], *TH2_pro_refene[iv];
        TH2D *TH2_ele_ene[iv], *TH2_ele_refene[iv];
        TH2D *TH2_ele_cut[iv], *TH2_ele_refcut[iv];

        TH2D *TH2_pro_beta1[iv],*TH2_pro_cut1[iv],*TH2_pro_ene1[iv];
        TH2D *TH2_pro_beta2[iv],*TH2_pro_cut2[iv],*TH2_pro_ene2[iv];
        TH2D *TH2_ele_cut1[iv];

        TH2D *PrEffi_time_beta;
        TH2D *PrEffi_time_cut;
        TH2D *PrEffi_time_ene;
        TChain *fChain;
        void FillBufferFrom(TString fname, int jobID, int NFilesPerJob);
        int ReadFiles(TString fname, int jobID, int NFilesPerJob);
        void Init_tree();
        void Trk_fiducialvolume();
        int GetBartelsRotationNumber(UInt_t gmt_time);
        int ConvertUtime2Day(UInt_t utime);
        void BookHistogram();

};

int TRKEffi::GetBartelsRotationNumber(UInt_t gmt_time){
    int BR0 = 2427;
    double T0 = 1307750400; // Bartel rotation n. 2427
    double bin_width = 86400*27; // 27 days
    return (gmt_time - T0)/bin_width + BR0;
}

int TRKEffi::ConvertUtime2Day(UInt_t utime){
    time_t _time( utime );
    struct tm *_gmt_time = gmtime( &_time );
    int year = 1900 + _gmt_time->tm_year; // tm_year since 1900
    int month = _gmt_time->tm_mon + 1; // tm_mon 0-11
    int day = _gmt_time->tm_mday; // tm_mday 1-31
    return year*10000 + month*100 + day;
};

void TRKEffi::Init_tree(){
    // fChain->SetBranchAddress("itrtracks", &itrtracks);
    // fChain->SetBranchAddress("ibetahs", &ibetahs);
    fChain->SetBranchAddress("info_run", &info_run);
    fChain->SetBranchAddress("info_event", &info_event);
    fChain->SetBranchAddress("info_utime", &info_utime);
    // fChain->SetBranchAddress("info_lf", &info_lf);
    // fChain->SetBranchAddress("info_lfc", &info_lfc);
    fChain->SetBranchAddress("info_maxcf", info_maxcf);
    fChain->SetBranchAddress("info_maxcfi", info_maxcfi);
    fChain->SetBranchAddress("lvl1_PhysBPatt", &lvl1_PhysBPatt);
    // fChain->SetBranchAddress("tof_patt", &tof_patt);
    fChain->SetBranchAddress("tof_z", &tof_z);
    fChain->SetBranchAddress("tof_chi2t", &tof_chi2t);
    fChain->SetBranchAddress("tof_chi2c", &tof_chi2c);
    fChain->SetBranchAddress("tof_betah", &tof_betah);
    fChain->SetBranchAddress("tof_qhlay", tof_qhlay);
    fChain->SetBranchAddress("tof_trkxL", tof_trkxL);
    fChain->SetBranchAddress("tof_trkyL", tof_trkyL);
    fChain->SetBranchAddress("tof_ecalentry", tof_ecalentry);
    fChain->SetBranchAddress("tof_ecalexit", tof_ecalexit);
    // fChain->SetBranchAddress("tof_trkl1qr", &tof_trkl1qr);
    //fChain->SetBranchAddress("tof_trkl1qs", &tof_trkl1qs);
    // fChain->SetBranchAddress("tof_trkl9qr", &tof_trkl9qr);
    //fChain->SetBranchAddress("tof_trkl9qs", &tof_trkl9qs);
    //fChain->SetBranchAddress("tof_tkl1qxy", tof_tkl1qxy);
    //fChain->SetBranchAddress("tof_tkl9qxy", tof_tkl9qxy);
    fChain->SetBranchAddress("ecal_nsh", &ecal_nsh);
    fChain->SetBranchAddress("ecal_bdtp5s", &ecal_bdtp5s);
    fChain->SetBranchAddress("ecal_edep", &ecal_edep);
    // fChain->SetBranchAddress("ecal_enc", &ecal_enc);
    fChain->SetBranchAddress("ecal_ene", &ecal_ene);
    // fChain->SetBranchAddress("ecal_enp", &ecal_enp);
    fChain->SetBranchAddress("ecal_ene17", &ecal_ene17);
    fChain->SetBranchAddress("ecal_entry", ecal_entry);
    fChain->SetBranchAddress("ecal_exit", ecal_exit);
    fChain->SetBranchAddress("trk_ntrk", &trk_ntrk);
    fChain->SetBranchAddress("trk_pat", &trk_pat);
    fChain->SetBranchAddress("trk_patx", &trk_patx);
    fChain->SetBranchAddress("trk_rigkf", trk_rigkf);
    fChain->SetBranchAddress("trk_rig", trk_rig);
    fChain->SetBranchAddress("trk_chi2x", trk_chi2x);
    fChain->SetBranchAddress("trk_chi2y", trk_chi2y);
    fChain->SetBranchAddress("trk_qin", &trk_qin);
    fChain->SetBranchAddress("trk_ecalentry", trk_ecalentry);
    fChain->SetBranchAddress("trk_ecalexit", trk_ecalexit);
    fChain->SetBranchAddress("trk_xL", trk_xL);
    fChain->SetBranchAddress("trk_yL", trk_yL);
    fChain->SetBranchAddress("trd_nhits", &trd_nhits);
    fChain->SetBranchAddress("trd_klkhd", trd_klkhd);
}

void TRKEffi::BookHistogram(){

    TString name[5]={"Inner","Inner+Ecal","InnerL1","InnerL1+Ecal","Fullspan"};
    // TString cutname[3]={"ntrk>0","ntrk>0&&chi2xy<20","ntrk>0&&ntrk<=2&&chi2xy<20"};
    TString cutname[3]={"kf0","kf1","kf0&1"};
    // TString cutname[3]={"chi2xy,pat","chi2xy,pat,ntrk2","chi2xy,pat,ntrk1"};
    for(int i=0;i<iv;i++){
        TH2_pro_refcut[i]=new TH2D(Form("TH2_pro_refcut_%02d",i),Form("TH2_pro_refcut_%s",gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_refbeta[i]=new TH2D(Form("TH2_pro_refbeta_%02d",i),Form("TH2_pro_refbeta_%s",gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_refene[i]=new TH2D(Form("TH2_pro_refene_%02d",i),Form("TH2_pro_refene_%s",gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);

        TH2_pro_cut[i]=new TH2D(Form("TH2_pro_cut_%02d",i),Form("[%s]TH2_pro_cut_%s",gSystem->BaseName( cutname[0]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_beta[i]=new TH2D(Form("TH2_pro_beta_%02d",i),Form("[%s]TH2_pro_beta_%s",gSystem->BaseName( cutname[0]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_ene[i]=new TH2D(Form("TH2_pro_ene_%02d",i),Form("[%s]TH2_pro_ene_%s",gSystem->BaseName( cutname[0]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);

        TH2_pro_cut1[i]=new TH2D(Form("TH2_pro_cut1_%02d",i),Form("[%s]TH2_pro_cut_%s",gSystem->BaseName( cutname[1]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_beta1[i]=new TH2D(Form("TH2_pro_beta1_%02d",i),Form("[%s]TH2_pro_beta_%s",gSystem->BaseName( cutname[1]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_ene1[i]=new TH2D(Form("TH2_pro_ene1_%02d",i),Form("[%s]TH2_pro_ene_%s",gSystem->BaseName( cutname[1]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);

        TH2_pro_cut2[i]=new TH2D(Form("TH2_pro_cut2_%02d",i),Form("[%s]TH2_pro_cut_%s",gSystem->BaseName( cutname[2]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_beta2[i]=new TH2D(Form("TH2_pro_beta2_%02d",i),Form("[%s]TH2_pro_beta_%s",gSystem->BaseName( cutname[2]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_pro_ene2[i]=new TH2D(Form("TH2_pro_ene2_%02d",i),Form("[%s]TH2_pro_ene_%s",gSystem->BaseName( cutname[2]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);

        TH2_ele_refene[i]=new TH2D(Form("TH2_ele_refene_%02d",i),Form("TH2_ele_refene_%s",gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_ele_ene[i]=new TH2D(Form("TH2_ele_ene_%02d",i),Form("[%s]TH2_ele_ene_%s",gSystem->BaseName( cutname[0]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_ele_cut[i]=new TH2D(Form("TH2_ele_cut_%02d",i),Form("[%s]TH2_ele_cut_%s",gSystem->BaseName( cutname[1]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
        TH2_ele_cut1[i]=new TH2D(Form("TH2_ele_cut1_%02d",i),Form("[%s]TH2_ele_cut_%s",gSystem->BaseName( cutname[2]),gSystem->BaseName( name[i])),nday,t0,t0+nday*86400,nbin,energy_bins);
    }

}

int TRKEffi::ReadFiles(TString fname, int jobID, int NFilesPerJob){
    cout<<"---ReadFiles---"<<endl;

    fChain=new TChain("t");
    if(fname=="")return 0;
    vector<TString> v_fname;

    TList *inputlist=new TList();
    TString prefix = "root://castorpublic.cern.ch//";
    TString server = "?svcClass=amsuser";
    TString eosprefix = "root://eosams.cern.ch//";
    //TString eosprefix="";

    if(fname.Contains("root")){
        TString fnm=fname;
        if( fnm.EndsWith("root") ){
            if( fnm.BeginsWith("/castor") )
                fnm = prefix + fnm + server;
            else if( fnm.BeginsWith("/eos"))
                fnm = eosprefix + fnm;
        }
        cout<<"Add Single Root File: " <<fnm<<endl;
        fChain->Add( fnm );
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
        cout<<"Select "<<NFilesPerJob<<" Runs, From "<<v_fname.size()<<" Files, jobID: "<< jobID<<endl;
        cout<<"======================="<<endl;

        for(int i=NFilesPerJob*(jobID-1);i<NFilesPerJob*(jobID);i++){
            if(i<0)break;
            if(i>=v_fname.size())break;
            cout<<"Adding File : "<<v_fname.at(i)<<endl;
            fChain->Add( v_fname.at(i) );
            inputlist->Add( TFile::Open(v_fname.at(i)) );
        }
        cout<<"======================="<<endl;
    }
    cout << "In Total :" << fChain->GetEntries() << " data events " << endl;
    cout<<"=============================================="<<endl;

    return fChain->GetEntries();
}

void TRKEffi::Trk_fiducialvolume(){
    double trkR[9],trky[9];
    for(int i=0;i<9;i++){
        trkR[i]=pow(pow(tof_trkxL[i],2)+pow(tof_trkyL[i],2),0.5);
        trky[i]=fabs(tof_trkyL[i]);
    }
    prodc[iv]={0};
    prodc[0]=trkR[1]<62&&trky[1]<40 && ((trkR[2]<46&&trky[2]<44)||(trkR[3]<46&&trky[3]<44)) && ((trkR[4]<46&&trky[4]<36)||(trkR[5]<46&&trky[5]<36)) && ((trkR[6]<46&&trky[6]<44)||(trkR[7]<46&&trky[7]<44));
    //Inner
    prodc[1]=trkR[1]<62&&trky[1]<40 && ((trkR[2]<46&&trky[2]<44)||(trkR[3]<46&&trky[3]<44)) && ((trkR[4]<46&&trky[4]<36)||(trkR[5]<46&&trky[5]<36)) && ((trkR[6]<46&&trky[6]<44)||(trkR[7]<46&&trky[7]<44)) && (fabs(tof_ecalexit[0])<31.5) && (fabs(tof_ecalexit[1])<31.5) && (fabs(tof_ecalentry[0])<31.5) && (fabs(tof_ecalentry[1])<31.5);
    //Inner+Ecal
    prodc[2]=trkR[0]<62&&trky[0]<47 && trkR[1]<62&&trky[1]<40 && ((trkR[2]<46&&trky[2]<44)||(trkR[3]<46&&trky[3]<44)) && ((trkR[4]<46&&trky[4]<36)||(trkR[5]<46&&trky[5]<36)) && ((trkR[6]<46&&trky[6]<44)||(trkR[7]<46&&trky[7]<44));
    //InnerL1 tof_trkl1qs>0.6&&tof_trkl1qs<2 &&
    prodc[3]=trkR[0]<62&&trky[0]<47 && trkR[1]<62&&trky[1]<40 && ((trkR[2]<46&&trky[2]<44)||(trkR[3]<46&&trky[3]<44)) && ((trkR[4]<46&&trky[4]<36)||(trkR[5]<46&&trky[5]<36)) && ((trkR[6]<46&&trky[6]<44)||(trkR[7]<46&&trky[7]<44)) && (fabs(tof_ecalexit[0])<31.5) && (fabs(tof_ecalexit[1])<31.5) && (fabs(tof_ecalentry[0])<31.5) && (fabs(tof_ecalentry[1])<31.5);
    //InnerL1+Ecal tof_trkl1qs>0.6&&tof_trkl1qs<2 &&
    prodc[4]=trkR[0]<62&&trky[0]<47 && trkR[1]<62&&trky[1]<40 && ((trkR[2]<46&&trky[2]<44)||(trkR[3]<46&&trky[3]<44)) && ((trkR[4]<46&&trky[4]<36)||(trkR[5]<46&&trky[5]<36)) && ((trkR[6]<46&&trky[6]<44)||(trkR[7]<46&&trky[7]<44)) && ((trkR[8]<43&&trky[8]<29));
    //Fullspan tof_trkl1qs>0.6&&tof_trkl1qs<2 && tof_trkl9qs>0.6&&tof_trkl9qs<1.3 &&

    double trkR_1[9],trky_1[9];
    for(int i=0;i<9;i++){
        trkR_1[i]=pow(pow(trk_xL[i],2)+pow(trk_yL[i],2),0.5);
        trky_1[i]=fabs(trk_yL[i]);
    }
    bool hitL[9]={0};
    int nhity=0;
    for(int i=0;i<9;i++){
        if( trk_pat&(1<<i) ){
            hitL[i]=1;
            if(i>0 && i<8) nhity++;
        }
    }
    //Innerhit= (trkR_1[1]<62&&trky_1[1]<40&&hitL[1]) && ((trkR_1[2]<46&&trky_1[2]<44&&hitL[2])||(trkR_1[3]<46&&trky_1[3]<44&&hitL[3])) && ((trkR_1[4]<46&&trky_1[4]<36&&hitL[4])||(trkR_1[5]<46&&trky_1[5]<36&&hitL[5])) && ((trkR_1[6]<46&&trky_1[6]<44&&hitL[6])||(trkR_1[7]<46&&trky_1[7]<44&&hitL[7]));

    // ecalvolume=(fabs(tof_ecalexit[0])<31.5) && (fabs(tof_ecalexit[1])<31.5) && (fabs(tof_ecalentry[0])<31.5) && (fabs(tof_ecalentry[1])<31.5);

    //trkchi=trk_chi2y[0]<10 && trk_chi2y[0]>0;
    //trkchi=trk_chi2x[0]<10 && trk_chi2x[0]>0 && trk_chi2y[0]<10 && trk_chi2y[0]>0;
    //trkchi=trk_chi2x[0]<20 && trk_chi2x[0]>0 && trk_chi2y[0]<20 && trk_chi2y[0]>0;

    tofcharge=tof_qhlay[0]>0.5 && tof_qhlay[0]<1.5 && tof_qhlay[1]>0.5 && tof_qhlay[1]<1.5 && tof_qhlay[2]>0.5 && tof_qhlay[2]<1.5 && tof_qhlay[3]>0.5 && tof_qhlay[3]<1.5;

  	static const double ecal_entry_z0 = -143.2;
  	trkecalmatch=0;
  	if( TMath::Abs(ecal_entry[0]) < 32.4 - 1.8        ) trkecalmatch |= 1;
  	if( TMath::Abs(ecal_entry[1]) < 32.4 - 1.8        ) trkecalmatch |= 2;
  	if( TMath::Abs(trk_ecalentry[0] - ecal_entry[0]) < 3 ) trkecalmatch |= 4;
  	if( TMath::Abs(trk_ecalentry[1] - ecal_entry[1]) < 10 ) trkecalmatch |=8;
}

void TRKEffi::FillBufferFrom(TString fname, int jobID, int NFilesPerJob){
    ReadFiles(fname,jobID,NFilesPerJob);
    //fChain->Add("/eos/home-j/jliang/TrRecon_B1130_pass7/ntuple_*");
    cout<<"fChian entries: "<<fChain->GetEntries()<<endl;
    Init_tree();
    BookHistogram();
    // int fov[iv]={40,25,30,25,25};
    int icut[iv]={3,0,1,0,0};
    // *** Stormer
    double cuta[4]={1.00537,0.874157,0.833474,0.771803};
    double cutb[4]={0.765555,0.657035,0.694287,0.91866};
    double ecuta[4]={0.736731,0.72442,0.644218,0.617034};
    double ecutb[4]={0.769293,0.782504,1.07814,1.08221};
    // *** IGRF
    // double cuta[4]={0.87548,0.874411,0.800326,0.767972};
    // double cutb[4]={0.568947,0.501461,0.62969,0.728691};
    // double ecuta[4]={0.667856,0.66413,0.639081,0.616215};
    // double ecutb[4]={0.444627,0.400116,0.433888,0.442649};
    long entries=fChain->GetEntries();
    for(long entry=0;entry<entries;entry++){
        fChain->GetEntry(entry);
        double rig_pr=0,cut_pr=0,ecal_e=0;
        double cut_el=0;
        Trk_fiducialvolume();
        trkchi=trk_ntrk>0 && trk_ntrk<=2 /*&& (trk_pat&259)*/ && trk_chi2y[1]<20 && trk_chi2x[1]<20 /*&& fabs(ecal_ene/trk_rig[1])>0.65 && fabs(ecal_ene/trk_rig[1])<5.*/ && (trkecalmatch&12)==12;


        bool cut0=/*(trk_pat&259)&&trk_chi2y[1]<20&&trk_chi2x[1]<20&&*/trk_rigkf[0]!=0;
        bool cut1=/*trk_ntrk<=2&&(trk_pat&259)&&trk_chi2y[1]<20&&trk_chi2x[1]<20&&*/trk_rigkf[1]!=0;
        bool cut2=/*trk_ntrk==1&&(trk_pat&259)&&trk_chi2y[1]<20&&trk_chi2x[1]<20&&*/(trk_rigkf[0]!=0)&&(trk_rigkf[1]!=0);
        if(trkchi && tofcharge && tof_betah>0.4 && trd_nhits>=8 && trd_klkhd[0]>0.95 && tof_chi2t<5 && tof_chi2c<10){
            for(int i=0;i<iv;i++){
                if(prodc[i]){
                    rig_pr=0.932*tof_betah/(1*pow((1-tof_betah*tof_betah),0.5));
                    cut_pr=cuta[icut[i]]*info_maxcf[icut[i]]+cutb[icut[i]];
                    ecal_e=2.43488*ecal_edep+0.402629;
                    bool _isrig=1;//rig_pr/info_maxcf[3]>1.;
                    bool _iscut=1;//cut_pr/info_maxcf[3]>1.;
                    bool _iseca=1;//ecal_e/info_maxcf[3]>1.;
                    if(_isrig) TH2_pro_refbeta[i]->Fill(info_utime, rig_pr);
                    if(_iscut) TH2_pro_refcut[i]->Fill(info_utime, cut_pr);
                    if(_iseca) TH2_pro_refene[i]->Fill(info_utime, ecal_e);
                    if(cut0){
                        if(_isrig) TH2_pro_beta[i]->Fill(info_utime, rig_pr);
                        if(_iscut) TH2_pro_cut[i]->Fill(info_utime, cut_pr);
                        if(_iseca) TH2_pro_ene[i]->Fill(info_utime, ecal_e);
                    }

                    if(cut1){
                        if(_isrig) TH2_pro_beta1[i]->Fill( info_utime, rig_pr);
                        if(_iscut) TH2_pro_cut1[i]->Fill( info_utime, cut_pr);
                        if(_iseca) TH2_pro_ene1[i]->Fill(info_utime, ecal_e);
                    }

                    if(cut2){
                        if(_isrig) TH2_pro_beta2[i]->Fill( info_utime, rig_pr);
                        if(_iscut) TH2_pro_cut2[i]->Fill( info_utime, cut_pr);
                        if(_iseca) TH2_pro_ene2[i]->Fill(info_utime, ecal_e);
                    }
                }
            }
        }
        if(entry%10000==0 || entry==entries-1){
            int day=ConvertUtime2Day(info_utime);
            cout<<"entry: "<<entry<<"/"<<entries<<"; day:"<<day<<"--"<<trk_rigkf[1]<<endl;
        }

        //if(entry%1000==0 && (trkecalmatch&12)==12) cout<<trkecalmatch<<"--!!--"<<fabs(ecal_ene/trk_rig[1])<<endl;

        if(ecal_ene/info_maxcf[0]<0.9) continue;
        if(trkchi && tofcharge && tof_betah>0.8 && trd_nhits>=8 && trd_klkhd[0]>0 && trd_klkhd[0]<0.55 && tof_chi2t<5 && tof_chi2c<10){
            for(int i=0;i<iv;i++){
                if(prodc[i]){
                    //cut_el=0.635353*info_maxcf[3]+1.04634;
                    // cut_el=ecuta[icut[i]]*info_maxcf[icut[i]]+ecutb[icut[i]];
                    if(i==1||i==3){//ecal
                        if(ecal_bdtp5s>0){
                            TH2_ele_refene[i]->Fill( info_utime, ecal_ene );
                            // TH2_ele_refcut[i]->Fill( info_utime, ecal_ene);
                            if(cut0){
                                TH2_ele_ene[i]->Fill( info_utime, ecal_ene );
                            }
                            if(cut1){
                                TH2_ele_cut[i]->Fill( info_utime, ecal_ene);
                            }
                            if(cut2){
                                TH2_ele_cut1[i]->Fill( info_utime, ecal_ene);
                            }

                        }
                    }
                    else{
                        TH2_ele_refene[i]->Fill( info_utime, ecal_ene );
                        // TH2_ele_refcut[i]->Fill( info_utime, cut_el);
                        if(cut0){
                            TH2_ele_ene[i]->Fill( info_utime, ecal_ene );
                        }
                        if(cut1){
                            TH2_ele_cut[i]->Fill( info_utime, ecal_ene);
                        }
                        if(cut2){
                            TH2_ele_cut1[i]->Fill( info_utime, ecal_ene);
                        }
                    }
                }
            }
        }
    }



    cout<<"=====fill complete, calculate efficiency====="<<endl;

    TFile *file=new TFile(Form("TimeCorr_%03d.root",jobID),"recreate");
    file->cd();
    for(int i=0;i<iv;i++){
        TH2_pro_refbeta[i]->Write();
        TH2_pro_beta[i]->Write();
        TH2_pro_refcut[i]->Write();
        TH2_pro_cut[i]->Write();
        TH2_pro_refene[i]->Write();
        TH2_pro_ene[i]->Write();

        TH2_pro_beta1[i]->Write();
        TH2_pro_cut1[i]->Write();
        TH2_pro_ene1[i]->Write();
        TH2_pro_beta2[i]->Write();
        TH2_pro_cut2[i]->Write();
        TH2_pro_ene2[i]->Write();
    }
    for(int i=0;i<iv;i++){
        TH2_ele_refene[i]->Write();
        TH2_ele_ene[i]->Write();
        // TH2_ele_refcut[i]->Write();
        TH2_ele_cut[i]->Write();
        TH2_ele_cut1[i]->Write();
    }
    file->Close();
}

void main(int argc, char *argv[]){
    TRKEffi T;
    TString filename = argv[1];
    int jobID=1;
    int nFilesPerJob=10;
    if( argc>2 ){
        jobID = atoi(argv[2]);
    }
    if( argc>3 ){
        nFilesPerJob = atoi(argv[3]);
    }
    T.FillBufferFrom(filename,jobID,nFilesPerJob);
}
