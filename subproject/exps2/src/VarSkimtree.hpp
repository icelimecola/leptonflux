_Pragma("once")

#include<iostream>
using namespace std;
#include<TBranch.h>

class VarSkimtree{
    public:
        VarSkimtree(){cout<<"VarSkimtree-cont"<<endl;};
        ~VarSkimtree(){};
    public:
        Float_t         weight;
        Float_t         dst;
        Int_t           ibeam;
        Int_t           version;
        Int_t           npart;
        Int_t           ntrd;
        Int_t           ntrdh;
        Int_t           nbetah;
        Int_t           trktype;
        UInt_t          info_run;
        UInt_t          info_event;
        UInt_t          info_utime;
        Float_t         info_lf;
        Float_t         info_lfc;
        Float_t         info_maxcf[4];
        Float_t         info_maxcfi[4];
        Float_t         info_momentum;
        Float_t         info_velocity;
        Int_t           info_isinshadow;
        Int_t           info_saa;
        Int_t           info_badsec;
        Int_t           info_badalign;
        Int_t           info_runtype;
        Float_t         info_ntrig;
        Float_t         info_npart;
        Float_t         info_nev;
        Float_t         info_nerr;
        Float_t         info_zenith;
        Int_t           info_bt_status_max;
        Int_t           info_bt_status_max_sf;
        Int_t           info_bt_result;
        Float_t         mcinfo_p;
        Float_t         mcinfo_q;
        Float_t         mcinfo_theta;
        Float_t         mcinfo_phi;
        Float_t         mcinfo_x;
        Float_t         mcinfo_y;
        Int_t           mcinfo_isPassTOF[4];
        Int_t           mcinfo_nanti;
        Int_t           mcinfo_ntof;
        Int_t           mcinfo_ntoflay[4];
        Int_t           mcinfo_ntrkclus;
        Int_t           mcinfo_ntrklay[9];
        Float_t         mcinfo_energy_TOI;
        Float_t         mcinfo_energy_BOI;
        Int_t           mcinfo_Nbacksplash;
        Int_t           mcinfo_Nsecondary;
        Int_t           mcinfo_Nbrem;
        Int_t           mcinfo_Nconversion;
        Int_t           lvl1_JMembPatt;
        Int_t           lvl1_PhysBPatt;
        Int_t           lvl1_JMembPattMC;
        Int_t           lvl1_PhysBPattMC;
        Int_t           ecal_ish;
        Int_t           ecal_nsh;
        Float_t         ecal_theta;
        Float_t         ecal_phi;
        Int_t           ecal_nhits;
        Int_t           ecal_nhitsall;
        Int_t           ecal_stat;
        Float_t         ecal_edep;
        Float_t         ecal_ene17;
        Float_t         ecal_ene;
        Float_t         ecal_enc;
        Float_t         ecal_enp;
        Float_t         ecal_cog[3];
        Float_t         ecal_entry[3];
        Float_t         ecal_exit[3];
        Float_t         ecal_cog_ecax[3];
        Float_t         ecal_theta_ecax;
        Float_t         ecal_phi_ecax;
        Int_t           ecal_cellstat[18][72];
        Float_t         ecal_adcl[18][72];
        Float_t         ecal_adch[18][72];
        Float_t         ecal_bdtp;
        Float_t         ecal_bdtp5;
        Float_t         ecal_bdtp5s;
        Float_t         ecal_nchi2;
        Int_t           trk_itrk;
        Int_t           trk_ntrk;
        Float_t         trk_mdist;
        Float_t         trk_mdcos;
        Int_t           trk_nhitx;
        Int_t           trk_nhity;
        Int_t           trk_hasnoisehit;
        Int_t           trk_hasnoisehit_ext;
        Int_t           trk_pat;
        Int_t           trk_patx;
        Float_t         trk_theta;
        Float_t         trk_phi;
        Float_t         trk_bcor;
        Float_t         trk_bcorinfit;
        Float_t         trk_rig[6];
        Float_t         trk_rigerr[2];
        Float_t         trk_rig_al[2];
        Float_t         trk_rig_ck[2];
        Float_t         trk_chi2x[4];
        Float_t         trk_chi2y[4];
        Float_t         trk_qin;
        Float_t         trk_qinrms;
        Float_t         trk_qinup;
        Float_t         trk_qinlow;
        Float_t         trk_qlay[9];
        Float_t         trk_qmin;
        Float_t         trk_qmax;
        Float_t         trk_yqlay[9];
        Float_t         trk_yqmin;
        Float_t         trk_yqmax;
        Float_t         trk_edeplay[9];
        Float_t         trk_ecalentry[3];
        Float_t         trk_x0;
        Float_t         trk_y0;
        Float_t         trk_xL1;
        Float_t         trk_yL1;
        Float_t         trk_xL9;
        Float_t         trk_yL9;
        Float_t         trk_mindisty;
        Float_t         trk_maxresy;
        Float_t         trk_maxresx;
        Float_t         trk_srmin;
        Float_t         trk_sr[9];
        Float_t         trk_eleccbdt;
        Float_t         trk_eleccbdtmdr;
        Float_t         trk_bendarea;
        Float_t         trk_chi2y_op;
        Float_t         trk_hitcoo[9][3];
        Float_t         trk_layresy[9];
        Float_t         trk_layresy_exc[9];
        Float_t         trk_layresx[9];
        Float_t         trk_layresx_exc[9];
        Int_t           trk_hittkid[9];
        Int_t           trk_hitsenid[9];
        Float_t         trk_laypos[9][2];
        Float_t         trk_lay_dymin[9];
        Float_t         trk_lay_dxmin[9];
        Float_t         trk_lay_mccoo[9];
        Float_t         trk_lay_propcoo[9];
        Float_t         trk_lay_propcoo_eloss[9];
        Float_t         trk_lay_linecoo[9];
        Float_t         trk_TrackHit_EdepX[10];
        Float_t         trk_TrackHit_EdepY[10];
        Bool_t          trk_TrackHit_OnlyX[10];
        Bool_t          trk_TrackHit_OnlyY[10];
        Float_t         trk_TrackHit_Coord[10][3];
        Float_t         trk_TrackHit_ECoord[10][3];
        Float_t         trk_TrackHit_Q[10];
        Int_t           trk_MaxNHit_offtrack;
        Int_t           trk_NHitLay_offtrack[10];
        Float_t         trk_EdepXLay_offtrack[10];
        Float_t         trk_EdepYLay_offtrack[10];
        Int_t           trk_NHitLay_neighbor[10];
        Float_t         trk_EdepXLay_neighbor[10];
        Float_t         trk_EdepYLay_neighbor[10];
        Int_t           trk_NsumHit_radius[8];
        Float_t         trk_EsumXHit_radius[8];
        Float_t         trk_EsumYHit_radius[8];
        Int_t           trk_NStrip[10];
        Int_t           trk_NHit_OffTrackVC;
        Int_t           tof_ibeta;
        Float_t         tof_beta;
        Int_t           tof_nclus_intime;
        Float_t         tof_q;
        Float_t         tof_qlow;
        Float_t         tof_qup;
        Int_t           tof_ibetah;
        Int_t           tof_buildtype;
        Int_t           tof_match;
        Int_t           tof_match_ecal;
        Int_t           tof_match_trd;
        Int_t           tof_goodbeta;
        Int_t           tof_pattern;
        Int_t           tof_pattern_betaR;
        Int_t           tof_nlay;
        Float_t         tof_betah;
        Float_t         tof_xL1;
        Float_t         tof_yL1;
        Float_t         tof_xL9;
        Float_t         tof_yL9;
        Float_t         tof_ebetav;
        Float_t         tof_betahs;
        Float_t         tof_qh;
        Float_t         tof_qhrms;
        Float_t         tof_qhlay[4];
        Float_t         tof_z;
        Float_t         tof_probz;
        Float_t         tof_chi2c;
        Float_t         tof_chi2t;
        Float_t         tof_mass;
        Float_t         tof_mass_err;
        Float_t         tof_edeplay[4];
        Int_t           tof_nclslay_offtrack[4];
        Float_t         tof_edeplay_offtrack[4];
        Float_t         tof_edeplay_2nd[4];
        Int_t           tof_nclslay_2nd[4];
        Int_t           trd_type;
        Int_t           trd_nhtrd;
        Int_t           trd_nsegx;
        Int_t           trd_nsegy;
        Int_t           trd_itrd;
        Int_t           trd_ihtrd;
        Int_t           trd_nhits[3];
        Float_t         trd_elprob[3];
        Float_t         trd_prprob[3];
        Float_t         trd_heprob[3];
        Float_t         trd_klkhd[3][3];
        Float_t         trd_theta;
        Float_t         trd_phi;
        Float_t         trd_coo[3];
        Float_t         trd_ecalentry[3];
        Float_t         trd_ecalexit[3];
        Float_t         trd_xL1;
        Float_t         trd_yL1;
        Float_t         trd_xL9;
        Float_t         trd_yL9;
        Int_t           trd_isPassTOF[4];
        Int_t           rich_ntothits;
        Float_t         rich_npexp_el;
        Int_t           acc_nclus;
        Int_t           acc_NACC;
        Int_t           acc_NBackSplashHits;
        Int_t           acc_NInteractionHits;
        Float_t         bdt_interaction;
        Float_t         bdt_spillover;
        Float_t         bdt_combined;
        Float_t         bdta_interaction;
        Float_t         bdta_spillover;
        Float_t         bdta_combined;
        Float_t         bdtx_interaction;
        Float_t         bdtx_spillover;
        Float_t         bdtx_combined;
        Float_t         bdtax_interaction;
        Float_t         bdtax_spillover;
        Float_t         bdtax_combined;
        Int_t           Match_TRDECAL;
        Int_t           Pass_TRKECAL;
        Int_t           Pass_TRDECAL;
        Int_t           ECAL_IsFocused;
        Int_t           ECAL_IsContained;
        Float_t         tcpu;
        Int_t           N_Shwr;
        Int_t           hadflag;
        Float_t         E0[3];
        Float_t         A0[3];
        Float_t         ShwrPos[3][3];
        Float_t         ShwrDir[3][3];
        Float_t         Ene;    //--ECAL ENE
        Float_t         Lkhd0;
        Float_t         LkhdRatio;
        Float_t         Lkhd1;
        Float_t         EmBDT;
        Int_t           NLatLeakCell;
        Float_t         LatLeakFrac;
        Float_t         RearLeakFrac;
        Float_t         trd_new;
        Float_t         trd_Likelihood_ele;
        Float_t         trd_Likelihood_pr;
        Float_t         trd_lastbin;
        Float_t         trd_Likelihood_ele_lastbin;
        Float_t         trd_Likelihood_pr_lastbin;
        Int_t           ntrdtrack_x;
        Int_t           ntrdtrack_y;
        Int_t           nvertex_2d_x;
        Int_t           nvertex_2d_y;
        Int_t           nvertex_3d;
        Double_t        vertex_x;
        Double_t        vertex_y;
        Double_t        vertex_z;
        Double_t        vertex_x_err;
        Double_t        vertex_y_err;
        Double_t        vertex_z_err;
        Int_t           vertex_ntrack;
        Int_t           vertex_nhit;
        Double_t        vertex_chi2;
        Int_t           vertex_ntrack_x;
        Int_t           vertex_nhit_x;
        Double_t        vertex_chi2_x;
        Int_t           vertex_ntrack_y;
        Int_t           vertex_nhit_y;
        Double_t        vertex_chi2_y;
        Int_t           vertex_is2d;
    public:
        // List of branches
        TBranch        *b_weight;   //!
        TBranch        *b_dst;   //!
        TBranch        *b_ibeam;   //!
        TBranch        *b_verion;   //!
        TBranch        *b_npart;   //!
        TBranch        *b_ntrd;   //!
        TBranch        *b_ntrdh;   //!
        TBranch        *b_nbetah;   //!
        TBranch        *b_trktype;   //!
        TBranch        *b_info_run;   //!
        TBranch        *b_info_event;   //!
        TBranch        *b_info_utime;   //!
        TBranch        *b_info_lf;   //!
        TBranch        *b_info_maxcf;   //!
        TBranch        *b_info_maxcfi;   //!
        TBranch        *b_info_momentum;   //!
        TBranch        *b_info_velocity;   //!
        TBranch        *b_info_isinshadow;   //!
        TBranch        *b_info_saa;   //!
        TBranch        *b_info_badsec;   //!
        TBranch        *b_info_badalign;   //!
        TBranch        *b_info_runtype;   //!
        TBranch        *b_info_ntrig;   //!
        TBranch        *b_info_npart;   //!
        TBranch        *b_info_nev;   //!
        TBranch        *b_info_nerr;   //!
        TBranch        *b_info_zenith;   //!
        TBranch        *b_info_bt_status_max;   //!
        TBranch        *b_info_bt_status_max_sf;   //!
        TBranch        *b_info_bt_result;   //!
        TBranch        *b_mcinfo_p;   //!
        TBranch        *b_mcinfo_q;   //!
        TBranch        *b_mcinfo_theta;   //!
        TBranch        *b_mcinfo_phi;   //!
        TBranch        *b_mcinfo_x;   //!
        TBranch        *b_mcinfo_y;   //!
        TBranch        *b_mcinfo_isPassTOF;   //!
        TBranch        *b_mcinfo_nanti;   //!
        TBranch        *b_mcinfo_ntof;   //!
        TBranch        *b_mcinfo_ntoflay;   //!
        TBranch        *b_mcinfo_ntrkclus;   //!
        TBranch        *b_mcinfo_ntrklay;   //!
        TBranch        *b_mcinfo_energy_TOI;   //!
        TBranch        *b_mcinfo_energy_BOI;   //!
        TBranch        *b_mcinfo_Nbacksplash;   //!
        TBranch        *b_mcinfo_Nsecondary;   //!
        TBranch        *b_mcinfo_Nbrem;   //!
        TBranch        *b_mcinfo_Nconversion;   //!
        TBranch        *b_lvl1_JMembPatt;   //!
        TBranch        *b_lvl1_PhysBPatt;   //!
        TBranch        *b_lvl1_JMembPattMC;   //!
        TBranch        *b_lvl1_PhysBPattMC;   //!
        TBranch        *b_ecal_ish;   //!
        TBranch        *b_ecal_nsh;   //!
        TBranch        *b_ecal_theta;   //!
        TBranch        *b_ecal_phi;   //!
        TBranch        *b_ecal_nhits;   //!
        TBranch        *b_ecal_nhitsall;   //!
        TBranch        *b_ecal_stat;   //!
        TBranch        *b_ecal_edep;   //!
        TBranch        *b_ecal_ene17;   //!
        TBranch        *b_ecal_ene;   //!
        TBranch        *b_ecal_enc;   //!
        TBranch        *b_ecal_enp;   //!
        TBranch        *b_ecal_cog;   //!
        TBranch        *b_ecal_entry;   //!
        TBranch        *b_ecal_exit;   //!
        TBranch        *b_ecal_cog_ecax;   //!
        TBranch        *b_ecal_theta_ecax;   //!
        TBranch        *b_ecal_phi_ecax;   //!
        TBranch        *b_ecal_cellstat;   //!
        TBranch        *b_ecal_adcl;   //!
        TBranch        *b_ecal_adch;   //!
        TBranch        *b_ecal_bdtp;   //!
        TBranch        *b_ecal_bdtp5;   //!
        TBranch        *b_ecal_bdtp5s;   //!
        TBranch        *b_ecal_nchi2;   //!
        TBranch        *b_trk_itrk;   //!
        TBranch        *b_trk_ntrk;   //!
        TBranch        *b_trk_mdist;   //!
        TBranch        *b_trk_mdcos;   //!
        TBranch        *b_trk_nhitx;   //!
        TBranch        *b_trk_nhity;   //!
        TBranch        *b_trk_hasnoisehit;   //!
        TBranch        *b_trk_hasnoisehit_ext;   //!
        TBranch        *b_trk_pat;   //!
        TBranch        *b_trk_patx;   //!
        TBranch        *b_trk_theta;   //!
        TBranch        *b_trk_phi;   //!
        TBranch        *b_trk_bcor;   //!
        TBranch        *b_trk_bcorinfit;   //!
        TBranch        *b_trk_rig;   //!
        TBranch        *b_trk_rigerr;   //!
        TBranch        *b_trk_rig_al;   //!
        TBranch        *b_trk_rig_ck;   //!
        TBranch        *b_trk_chi2x;   //!
        TBranch        *b_trk_chi2y;   //!
        TBranch        *b_trk_qin;   //!
        TBranch        *b_trk_qinrms;   //!
        TBranch        *b_trk_qinup;   //!
        TBranch        *b_trk_qinlow;   //!
        TBranch        *b_trk_qlay;   //!
        TBranch        *b_trk_qmin;   //!
        TBranch        *b_trk_qmax;   //!
        TBranch        *b_trk_yqlay;   //!
        TBranch        *b_trk_yqmin;   //!
        TBranch        *b_trk_yqmax;   //!
        TBranch        *b_trk_edeplay;   //!
        TBranch        *b_trk_ecalentry;   //!
        TBranch        *b_x0;   //!
        TBranch        *b_y0;   //!
        TBranch        *b_xL1;   //!
        TBranch        *b_yL1;   //!
        TBranch        *b_xL9;   //!
        TBranch        *b_yL9;   //!
        TBranch        *b_trk_mindisty;   //!
        TBranch        *b_trk_maxresy;   //!
        TBranch        *b_trk_maxresx;   //!
        TBranch        *b_trk_srmin;   //!
        TBranch        *b_trk_sr;   //!
        TBranch        *b_trk_eleccbdt;   //!
        TBranch        *b_trk_eleccbdtmdr;   //!
        TBranch        *b_trk_bendarea;   //!
        TBranch        *b_trk_chi2y_op;   //!
        TBranch        *b_trk_hitcoo;   //!
        TBranch        *b_trk_layresy;   //!
        TBranch        *b_trk_layresy_exc;   //!
        TBranch        *b_trk_layresx;   //!
        TBranch        *b_trk_layresx_exc;   //!
        TBranch        *b_trk_hittkid;   //!
        TBranch        *b_trk_hitsenid;   //!
        TBranch        *b_trk_laypos;   //!
        TBranch        *b_trk_lay_dymin;   //!
        TBranch        *b_trk_lay_dxmin;   //!
        TBranch        *b_trk_lay_mccoo;   //!
        TBranch        *b_trk_lay_propcoo;   //!
        TBranch        *b_trk_lay_propcoo_eloss;   //!
        TBranch        *b_trk_lay_linecoo;   //!
        TBranch        *b_trk_TrackHit_EdepX;   //!
        TBranch        *b_trk_TrackHit_EdepY;   //!
        TBranch        *b_trk_TrackHit_OnlyX;   //!
        TBranch        *b_trk_TrackHit_OnlyY;   //!
        TBranch        *b_trk_TrackHit_Coord;   //!
        TBranch        *b_trk_TrackHit_ECoord;   //!
        TBranch        *b_trk_TrackHit_Q;   //!
        TBranch        *b_trk_MaxNHit_offtrack;   //!
        TBranch        *b_trk_NHitLay_offtrack;   //!
        TBranch        *b_trk_EdepXLay_offtrack;   //!
        TBranch        *b_trk_EdepYLay_offtrack;   //!
        TBranch        *b_trk_NHitLay_neighbor;   //!
        TBranch        *b_trk_EdepXLay_neighbor;   //!
        TBranch        *b_trk_EdepYLay_neighbor;   //!
        TBranch        *b_trk_NsumHit_radius;   //!
        TBranch        *b_trk_EsumXHit_radius;   //!
        TBranch        *b_trk_EsumYHit_radius;   //!
        TBranch        *b_trk_NStrip;   //!
        TBranch        *b_trk_NHit_OffTrackVC;   //!
        TBranch        *b_tof_ibeta;   //!
        TBranch        *b_tof_beta;   //!
        TBranch        *b_tof_nclus_intime;   //!
        TBranch        *b_tof_q;   //!
        TBranch        *b_tof_qlow;   //!
        TBranch        *b_tof_qup;   //!
        TBranch        *b_tof_ibetah;   //!
        TBranch        *b_tof_buildtype;   //!
        TBranch        *b_tof_match;   //!
        TBranch        *b_tof_match_ecal;   //!
        TBranch        *b_tof_match_trd;   //!
        TBranch        *b_tof_goodbeta;   //!
        TBranch        *b_tof_pattern;   //!
        TBranch        *b_tof_pattern_betaR;   //!
        TBranch        *b_tof_nlay;   //!
        TBranch        *b_tof_betah;   //!
        TBranch        *b_tof_xL1;   //!
        TBranch        *b_tof_yL1;   //!
        TBranch        *b_tof_xL9;   //!
        TBranch        *b_tof_yL9;   //!
        TBranch        *b_tof_ebetav;   //!
        TBranch        *b_tof_betahs;   //!
        TBranch        *b_tof_qh;   //!
        TBranch        *b_tof_qhrms;   //!
        TBranch        *b_tof_qhlay;   //!
        TBranch        *b_tof_z;   //!
        TBranch        *b_tof_probz;   //!
        TBranch        *b_tof_chi2c;   //!
        TBranch        *b_tof_chi2t;   //!
        TBranch        *b_tof_mass;   //!
        TBranch        *b_tof_mass_err;   //!
        TBranch        *b_tof_edeplay;   //!
        TBranch        *b_tof_nclslay_offtrack;   //!
        TBranch        *b_tof_edeplay_offtrack;   //!
        TBranch        *b_tof_edeplay_2nd;   //!
        TBranch        *b_tof_nclslay_2nd;   //!
        TBranch        *b_trd_type;   //!
        TBranch        *b_trd_nhtrd;   //!
        TBranch        *b_trd_nsegx;   //!
        TBranch        *b_trd_nsegy;   //!
        TBranch        *b_trd_itrd;   //!
        TBranch        *b_trd_ihtrd;   //!
        TBranch        *b_trd_nhits;   //!
        TBranch        *b_trd_elprob;   //!
        TBranch        *b_trd_prprob;   //!
        TBranch        *b_trd_heprob;   //!
        TBranch        *b_trd_klkhd;   //!
        TBranch        *b_trd_theta;   //!
        TBranch        *b_trd_phi;   //!
        TBranch        *b_trd_coo;   //!
        TBranch        *b_trd_ecalentry;   //!
        TBranch        *b_trd_ecalexit;   //!
        TBranch        *b_trd_xL1;   //!
        TBranch        *b_trd_yL1;   //!
        TBranch        *b_trd_xL9;   //!
        TBranch        *b_trd_yL9;   //!
        TBranch        *b_trd_isPassTOF;   //!
        TBranch        *b_rich_ntothits;   //!
        TBranch        *b_rich_npexp_el;   //!
        TBranch        *b_acc_nclus;   //!
        TBranch        *b_acc_NACC;   //!
        TBranch        *b_acc_NBackSplashHits;   //!
        TBranch        *b_acc_NInteractionHits;   //!
        TBranch        *b_bdt_interaction;   //!
        TBranch        *b_bdt_spillover;   //!
        TBranch        *b_bdt_combined;   //!
        TBranch        *b_bdta_interaction;   //!
        TBranch        *b_bdta_spillover;   //!
        TBranch        *b_bdta_combined;   //!
        TBranch        *b_bdtx_interaction;   //!
        TBranch        *b_bdtx_spillover;   //!
        TBranch        *b_bdtx_combined;   //!
        TBranch        *b_bdtax_interaction;   //!
        TBranch        *b_bdtax_spillover;   //!
        TBranch        *b_bdtax_combined;   //!
        TBranch        *b_Match_TRDECAL;   //!
        TBranch        *b_Pass_TRKECAL;   //!
        TBranch        *b_Pass_TRDECAL;   //!
        TBranch        *b_ECAL_IsFocused;   //!
        TBranch        *b_ECAL_IsContained;   //!
        TBranch        *b_tcpu;   //!
        TBranch        *b_N_Shwr;   //!
        TBranch        *b_hadflag;   //!
        TBranch        *b_E0;   //!
        TBranch        *b_A0;   //!
        TBranch        *b_ShwrPos;   //!
        TBranch        *b_ShwrDir;   //!
        TBranch        *b_Ene;   //!
        TBranch        *b_Lkhd0;   //!
        TBranch        *b_LkhdRatio;   //!
        TBranch        *b_Lkhd1;   //!
        TBranch        *b_EmBDT;   //!
        TBranch        *b_NLatLeakCell;   //!
        TBranch        *b_LatLeakFrac;   //!
        TBranch        *b_RearLeakFrac;   //!
        TBranch        *b_trd_new;   //!
        TBranch        *b_trd_Likelihood_ele;   //!
        TBranch        *b_trd_Likelihood_pr;   //!
        TBranch        *b_trd_lastbin;   //!
        TBranch        *b_trd_Likelihood_ele_lastbin;   //!
        TBranch        *b_trd_Likelihood_pr_lastbin;   //!
        TBranch        *b_ntrdtrack_x;   //!
        TBranch        *b_ntrdtrack_y;   //!
        TBranch        *b_nvertex_2d_x;   //!
        TBranch        *b_nvertex_2d_y;   //!
        TBranch        *b_nvertex_3d;   //!
        TBranch        *b_vertex_x;   //!
        TBranch        *b_vertex_y;   //!
        TBranch        *b_vertex_z;   //!
        TBranch        *b_vertex_x_err;   //!
        TBranch        *b_vertex_y_err;   //!
        TBranch        *b_vertex_z_err;   //!
        TBranch        *b_vertex_ntrack;   //!
        TBranch        *b_vertex_nhit;   //!
        TBranch        *b_vertex_chi2;   //!
        TBranch        *b_vertex_ntrack_x;   //!
        TBranch        *b_vertex_nhit_x;   //!
        TBranch        *b_vertex_chi2_x;   //!
        TBranch        *b_vertex_ntrack_y;   //!
        TBranch        *b_vertex_nhit_y;   //!
        TBranch        *b_vertex_chi2_y;   //!
        TBranch        *b_vertex_is2d;   //!
};