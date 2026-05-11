_Pragma("once")

#include <iostream>
using namespace std;
#include "TMath.h"
#include "VarBuffertree.hpp"
#include "VarEneBin.hpp"

class SelectProcEvent:virtual public VarBuffertree{
    public:
        SelectProcEvent():VarBuffertree(){cout<<"Constructor_selectprocevent"<<endl;}
        ~SelectProcEvent(){}
    public:
        double myEne;
        int i_enebin;
    public:
        //-- enebin
        void PROCESS_CalcEnebinIndex();
        int PROCESS_GetEnebinIndex();
        bool PROCESS_SEL_IsWrongEnebin();
        //-- selection
        bool PROCESS_SELECT_IsISS();
        bool PROCESS_SELECT_IsBadlf();
        bool PROCESS_SELECT_Preselect();
        //-- process
        void PROCESS_ResetVar();
        void PROCESS_InitVar();
        void PROCESS_Classify();
        //-- fill tree
        bool PROCESS_SELECT_ShouldFill();
};

//=============================================================================== ENEBIN
void SelectProcEvent::PROCESS_CalcEnebinIndex(){
    //==== Ene & Bin
    myEne = Ene;//ecal_enc*0.975;
    //double myEne = ecal_ene17;
    i_enebin=ENEBIN_GetBinIndex(myEne);
    // cout<<"ene: "<<myEne<<" i: "<<i_enebin<<endl;
}
int SelectProcEvent::PROCESS_GetEnebinIndex(){
    return i_enebin;
}
bool SelectProcEvent::PROCESS_SEL_IsWrongEnebin(){
    return i_enebin < 0;
}

//=============================================================================== SELECTION
//-- iss
bool SelectProcEvent::PROCESS_SELECT_IsISS(){
    return mcp==0;
}
//-- badlf
bool SelectProcEvent::PROCESS_SELECT_IsBadlf(){
    return lf<0.05;
}
//-- pre
bool SelectProcEvent::PROCESS_SELECT_Preselect(){
    //==================================== TRD
    bool cut_trd=1;
    // bool cut_trd=
    //     trdnhits2 >= 12 
    //     && trdlkhde2 < 1.5     // tail
    //     && trdlkhde2_ehe < 0.8;    // helium
    //==================================== ECAL
    // bool cut_ecal=1;
    bool cut_ecal=
        (inecal&3)==3;        // |X0|<31.5 and |Y0|<31.5 in ECAL
        // (inecal&3)==3 && TMath::Abs(Ene/rig)>0.65;
    //==================================== TOF
    bool cut_tof=1;
    // bool cut_tof=
    //     beta > 0.8 
    //     && tofqup  > 0. && tofqup < 3
    //     && tofqlow > 0. && tofqlow < 5;
    //==================================== TRK
    // bool cut_trk=1;
    bool cut_trk=theta < 0.436;
    // bool cut_trk=
    //     theta < 0.436
    //     && qin > 0. && qin < 2.0 
    //     && (pat&259) 
    //     && (trkecalmatch&12)==12  // |dX|<3 and |dY|<10
    //     && chi2x < 30 && chi2y < 30;
    //     // && chi2x < 50 && chi2y < 50;
    //==================================== BDT
    // bool cut_bdt=1;
    bool cut_bdt=
        // bdt_chargepid > 0;
        // bdt_chargepid >= 0;
        // bdt_chargepid >= 0 && EmBDT>-0.995;
        bdt_chargepid >= 0 && ccbdt>0.0;
        // bdt_chargepid >= 0 && ccbdt>0.0 && EmBDT>-0.995;
    //==================================== TOTAL
    bool totalcut= cut_trd && cut_ecal && cut_tof && cut_trk && cut_bdt;
    return totalcut;
}

//=============================================================================== PROCESS
void SelectProcEvent::PROCESS_ResetVar(){
    amsday = -1;
    day = -1;
    dayID = -1;
    br = -1;
    //==== datatype
    datatype=0;
    //==== detecteff_ttpe
    EventSelectEffType=0;
    // cout<<"resetbranch "<<amsday<<day<<dayID<<br<<endl;
    mlat = mlon = rr = 0;
}
void SelectProcEvent::PROCESS_InitVar(){
    //==== 
    if( myEne<2 ){//bug in MakeSkimNtuple v13
        trdlkhdnew = trdlkhde;
        trdelprobnew = trdelprob;
    }
    // if( bin<bin_lowenergy_analysis ){ // use TRD refit track at low energy
    // 	trdnhits = trdnhits2;
    // 	trdlkhdnew = trdlkhde2;
    // }
    //====
    EmLkhd = Lkhd0;
    trdelprob = -log(trdelprob)/10.; 
    trdelprobnew = -log(trdelprobnew)/10.; 
    //====
    eop = TMath::Log10( Ene/TMath::Abs(rig) );
    ecop = TMath::Log10( ecal_enc/TMath::Abs(rig) );
    //==== ccbdt
    if( myEne<100 ) ccbdt = ccbdtx2;
    else ccbdt = ccbdt2;
    ccbdtv = log((1+ccbdt)/(1-ccbdt))/7;
    //==== SetSaftyFactor
    // apply cutoff and saa cut
    if( PROCESS_SELECT_IsISS() ){
        sf = energy_bins[i_enebin] / poscf[0];
        sf40 = energy_bins[i_enebin] / poscf[3];
        sfi = energy_bins[i_enebin] / poscfi[0];
        sfi40 = energy_bins[i_enebin] / poscfi[3];
        //==========↓original
        //if( sf<1 ) continue;
        //if( sfi<1 ) continue;
        //==========↑original
        // cout<<"sf"<<sf<<"sf40"<<sf40<<"sfi"<<sfi<<"sfi40"<<sfi40<<endl;
    }else{ 
        // MC
        sf40 = sf = 10000;
        sfi40 = sfi = 10000;
        Lkhd0 /= 1.04;
    }
}
void SelectProcEvent::PROCESS_Classify(){
    //====
    enum EmDataType{
        kGoodProton=1,
        kChargeConfusedProton=2,
        kGoodElectron=4,
        kChargeConfusedElectron=8,
        kPositiveData=16,
        kNegativeData=32,
        kGoodProtonMC=64,
        kChargeConfusedProtonMC=128,
        kGoodElectronMC=256
    };
    //====
    static const double data_EmLkhd_precut = 10.; // for efficiency estimation
    if( mcp==0 ){
        if( rig>0 && EmLkhd>3. && EmLkhd<6 ){
            datatype |= kGoodProton;
        }
        if( rig<0 && EmLkhd>3. && EmLkhd<6 ){
            datatype |= kChargeConfusedProton;
        }
        if( rig<0 && EmLkhd<3 ){ // tight cut to select pure electron
            datatype |= kGoodElectron;
        }
        if( rig<0 && EmLkhd<data_EmLkhd_precut ){
            datatype |= kNegativeData;
        }
        if( rig>0 && EmLkhd<data_EmLkhd_precut ){
            datatype |= kPositiveData;
        }
    }else{
        trdlkhde /= 1.03;
        trdelprobnew = (trdelprobnew-0.696)*1.05 + 0.695; // data-mc correction
        if( mcq<0 ){ 
            // e- MC
            if( rig>0 && EmLkhd<data_EmLkhd_precut ){
                datatype |= kChargeConfusedElectron;
            }
            if( rig<0 && EmLkhd<data_EmLkhd_precut ){
                datatype |= kGoodElectronMC;
            }
        }	
    }
    // cout<<"datatype_classify "<<datatype<<endl;
}

//=============================================================================== FILLTREE
//-- fill
bool SelectProcEvent::PROCESS_SELECT_ShouldFill(){
    bool temp=
        datatype>0 && i_enebin>=0;
    return temp;
}