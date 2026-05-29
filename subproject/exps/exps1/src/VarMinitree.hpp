_Pragma("once")

#include <iostream>
using namespace std;
#include "VarSkimtree.hpp"
#include "VarEneBin.hpp"

class VarMinitree: public VarSkimtree,public VarEneBin{
    public:
        VarMinitree(){cout<<"exposurevariable-cont"<<endl;};
        ~VarMinitree(){};
    public:
        enum EmDataType{
            _ISS_Data=1,
            _TB_Data=2,
            _MC_Data=3
        };
        static const int nbadruntype=19;
        const char *badruntyps[nbadruntype] = {
            "DAQ_pulser_on",              // 0
            "Ecal_EIB_RP3_LV_power_cycle",// 1
            "Ecal_Trigger_test",          // 2
            "Ecal_EIB_RP3_wrong_setting", // 3
            "Ecal_EDR_1_0_A_wrong_config",// 4
            "Ecal_E0_event_mismatch",    // 5
            "FieldOfView_SSRMS",         // 6
            "General_CanadinArmInFOV",   // 7
            "General_commissioning",     // 8
            "General_desync",            // 9
            "General_events_with_error", // 10
            "General_events_without_particle", // 11
            "General_missing_events",   // 12
            "General_not_vertical",     // 13
            "General_ShortRun", 			 // 14
            "Tracker_BadStrips",        // 15
            "TRD_BadForEPseparation",  // 16
            "TRD_UnusableForAnalysis", // 17
            "TRD_WeakForEPseparation"  // 18
        };
        Int_t   isbadrun, isbad[nbadruntype];
		Int_t   isbadrun_trd, isbadrun_ecal, isbadrun_trk, isbadrun_general, isbadrun_daq, isbadrun_fov;
    public:
        static const int    nfov = 4, // 25, 30, 35, 40
                            nsf = 5; // 1.0, 1.1, 1.2, 1.3, 1.4
		float   maxcf_ts05[4],
				cf_ts05[4][2];
            float        info_poscf[4];
            float        info_poscfi[4];
            float        info_negcf[4];
            float        info_negcfi[4];
            float        info_theta;
            float        info_phi;
		//
        float        info_glat;
		float        info_glong;
            float        info_r;
            float        info_mlat;
            float        info_mlon;
        //
		//TH1D *h1d_exposure_fluxbin_stormer[nfov][nsf];
		//TH1D *h1d_exposure_fluxbin_igrf[nfov][nsf];
		//TH1D *h1d_exposure_fluxbin_stormer_finebin[nfov][nsf];
		//TH1D *h1d_exposure_fluxbin_igrf_finebin[nfov][nsf];
		//TH2D *h2d_exposure_fluxbin_stormer_daily[nsf]; // fov=25
		//TH2D *h2d_exposure_fluxbin_igrf_daily[nsf]; // fov=25
		//TH2D *h2d_exposure_fluxbin_ts05_daily[nsf]; // fov=25
        //me
        // TH2D *h2d_exposure_mLongVSmLat;
        // TH1D *h1d_exposure_mLat;
        // TH1D *h1d_exposure_mLong;
        //me
        // TH2D *h2d_exposure_gLongVSgLat;
        // TH1D *h1d_exposure_gLat;
        // TH1D *h1d_exposure_gLong;
        //me
		// TH2F *htmpl_trd_ccbdt_sig[nbin];
		// TH2F *htmpl_trd_ccbdt_pr[nbin];
		// TH2F *hmc_trd_ccbdt_sig[nbin];
		// TH2F *hmc_trd_ccbdt_cc[nbin];
		// TH2F *hdata_trd_ccbdt_neg[nbin];
		// TH2F *hdata_trd_ccbdt_pos[nbin];
		// TH2F *hdata_trd_ccbdt_neg_cut[nbin];
		// TH2F *hdata_trd_ccbdt_pos_cut[nbin];
	// private:
	public:
		// TFile *file_output;
		// TTree *tree_output;
		// TString filename;
		// Int_t JobID;
		bool    is_splitbin;
        //
		Int_t   dataType;
		Int_t   inecal,
                trkecalmatch,
                trdecalmatch;
		Float_t xecal[2],
                yecal[2];
		Float_t rigidity,
                trkecaldx,
                trkecaldy,
                trdecaldx,
                trdecaldy;
        //
		Float_t bdt_chargepid;
};