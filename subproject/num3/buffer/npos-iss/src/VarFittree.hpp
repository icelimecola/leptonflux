_Pragma("once")

#include<iostream>
using namespace std;
#include<TBranch.h>
#include "VarBuffertree.hpp"
#include "VarEneBin.hpp"

class VarFittree: public VarBuffertree,public VarEneBin{
    public:
    VarFittree():VarBuffertree(),VarEneBin(){cout<<"IN VarFittree ===== Constructor_VarFittree"<<endl;}
    ~VarFittree(){}
    //============ hist ============
    int lkhd_nbin=40;
    int lkhd_min=-2;
    int lkhd_max=6;
    //============ fitr ============
    struct FitResult{
        int Eid;
        double Elow,Eup;
        int day;
        int tid;
        int tlow, tmid, tup;
        double npos,npos_err;
        double npro,npro_err;
        double nele,nele_err;
        double nccpro,nccpro_err;
        double nelephy, nelephy_err;
        double neleunphy, neleunphy_err;
        double trigeff, trigeff_err;
        double chi2pos,ndfpos,chi2neg,ndfneg;
        //-- 
        double n_NegaSample,n_NegaSample_err;
        double n_PosiSample,n_PosiSample_err;
        Float_t lf,lfc;
        //--
        double sf;
        //----260211
        double cc, cc_err;
        //----260303
        double posfrac,posfrac_err;
        //----260407
        double nTOFpass,nTOFpass_err;
        double nTOFtotal,nTOFtotal_err;
        double nTRDpass,nTRDpass_err;
        double nTRDtotal,nTRDtotal_err;
        double nECALpass,nECALpass_err;
        double nECALtotal,nECALtotal_err;
        double nTrkpass,nTrkpass_err;
        double nTrktotal,nTrktotal_err;
        //----260421 seleff
        double tofeff,tofeff_err;
        double trdeff,trdeff_err;
        double ecaleff,ecaleff_err;
        double trkeff,trkeff_err;
        //----260517 trkeff
        //--------N
        double nPATpass, nPATpass_err;
        double nPATtotal, nPATtotal_err;
        double nMATCHpass, nMATCHpass_err;
        double nMATCHtotal, nMATCHtotal_err;
        double nCHI2pass, nCHI2pass_err;
        double nCHI2total, nCHI2total_err;
        double nQINpass, nQINpass_err;
        double nQINtotal, nQINtotal_err;
        double nNTRKpass, nNTRKpass_err;
        double nNTRKtotal, nNTRKtotal_err;
        //--------EFF
        double pateff,pateff_err;
        double matcheff,matcheff_err;
        double chi2eff,chi2eff_err;
        double qineff,qineff_err;
        double ntrkeff,ntrkeff_err;
    }fitR{};
    //============ latbin ============
    //==== nlat
    static const int nlat=1;
    // static const int nlat=12;
    // static const int nlat=36;
    //==== other
    double latmin=-90;
    double latmax=90;
    double wlat=(latmax-latmin)/nlat;
    int nolat=0;
    //==== func
    int GetNLat(){return nlat;}
    void NoLatBin(int nolatbin = 0){nolat=nolatbin;}
    //============ tbin ============
    //==== noper
    // const double wt = 60*60*24*6000;
    // static const int nt=1;
    // const double tmin = 1305417600;
    // const double tmax = tmin+nt*wt;
    //==== 1day
    //----prl130posi--260419
    const double wt = 60*60*24;  //---- 86400[s]
    static const int nt=6000;
    const double tmin = 1305417600;
    const double tmax = tmin+nt*wt;	    //----1823817600
    //==== 27days
    //----prl130posi--260421
    // const double wt = 60*60*24*27;
    // static const int nt=225;
    // const double tmin = 1305417600;
    // const double tmax = tmin+nt*wt;	    //----1830297600
    //====other
    int noperiod=0;
    //==== func
    int GetNPeriod(){return nt;}
    void NoPeriod(int noperiodbin = 0){noperiod=noperiodbin;}
};