//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 15 12:23:12 2017 by ROOT version 5.34/09
// from TTree t_add/t_add
// found on file: /eos/ams/user/w/wxu/Ntuple/AMSMIT/Positron2017/el.B1091.el.pl1.2002000.ForPositron_newECALRec_v4/Skim_Add_EcalKRec_CCBDT_Ene_TrdK_v11/Skimmed_Lepton_MC_Time_1209209327_92.root
//////////////////////////////////////////////////////////

#ifndef t_add_h
#define t_add_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class t_add {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
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
   Float_t         Ene;
   Float_t         Lkhd0;
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

   t_add(TTree *tree=0);
   virtual ~t_add();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef t_add_cxx
t_add::t_add(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/ams/user/w/wxu/Ntuple/AMSMIT/Positron2017/el.B1091.el.pl1.2002000.ForPositron_newECALRec_v4/Skim_Add_EcalKRec_CCBDT_Ene_TrdK_v11/Skimmed_Lepton_MC_Time_1209209327_92.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/ams/user/w/wxu/Ntuple/AMSMIT/Positron2017/el.B1091.el.pl1.2002000.ForPositron_newECALRec_v4/Skim_Add_EcalKRec_CCBDT_Ene_TrdK_v11/Skimmed_Lepton_MC_Time_1209209327_92.root");
      }
      f->GetObject("t_add",tree);

   }
   Init(tree);
}

t_add::~t_add()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t t_add::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t t_add::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void t_add::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("dst", &dst, &b_dst);
   fChain->SetBranchAddress("ibeam", &ibeam, &b_ibeam);
   fChain->SetBranchAddress("version", &version, &b_verion);
   fChain->SetBranchAddress("npart", &npart, &b_npart);
   fChain->SetBranchAddress("ntrd", &ntrd, &b_ntrd);
   fChain->SetBranchAddress("ntrdh", &ntrdh, &b_ntrdh);
   fChain->SetBranchAddress("nbetah", &nbetah, &b_nbetah);
   fChain->SetBranchAddress("trktype", &trktype, &b_trktype);
   fChain->SetBranchAddress("info.run", &info_run, &b_info_run);
   fChain->SetBranchAddress("info.event", &info_event, &b_info_event);
   fChain->SetBranchAddress("info.utime", &info_utime, &b_info_utime);
   fChain->SetBranchAddress("info.lf", &info_lf, &b_info_lf);
   fChain->SetBranchAddress("info.maxcf", info_maxcf, &b_info_maxcf);
   fChain->SetBranchAddress("info.maxcfi", info_maxcfi, &b_info_maxcfi);
   fChain->SetBranchAddress("info.momentum", &info_momentum, &b_info_momentum);
   fChain->SetBranchAddress("info.velocity", &info_velocity, &b_info_velocity);
   fChain->SetBranchAddress("info.isinshadow", &info_isinshadow, &b_info_isinshadow);
   fChain->SetBranchAddress("info.saa", &info_saa, &b_info_saa);
   fChain->SetBranchAddress("info.badsec", &info_badsec, &b_info_badsec);
   fChain->SetBranchAddress("info.badalign", &info_badalign, &b_info_badalign);
   fChain->SetBranchAddress("info.runtype", &info_runtype, &b_info_runtype);
   fChain->SetBranchAddress("info.ntrig", &info_ntrig, &b_info_ntrig);
   fChain->SetBranchAddress("info.npart", &info_npart, &b_info_npart);
   fChain->SetBranchAddress("info.nev", &info_nev, &b_info_nev);
   fChain->SetBranchAddress("info.nerr", &info_nerr, &b_info_nerr);
   fChain->SetBranchAddress("info.zenith", &info_zenith, &b_info_zenith);
   fChain->SetBranchAddress("info.bt_status_max", &info_bt_status_max, &b_info_bt_status_max);
   fChain->SetBranchAddress("info.bt_status_max_sf", &info_bt_status_max_sf, &b_info_bt_status_max_sf);
   fChain->SetBranchAddress("info.bt_result", &info_bt_result, &b_info_bt_result);
   fChain->SetBranchAddress("mcinfo.p", &mcinfo_p, &b_mcinfo_p);
   fChain->SetBranchAddress("mcinfo.q", &mcinfo_q, &b_mcinfo_q);
   fChain->SetBranchAddress("mcinfo.theta", &mcinfo_theta, &b_mcinfo_theta);
   fChain->SetBranchAddress("mcinfo.phi", &mcinfo_phi, &b_mcinfo_phi);
   fChain->SetBranchAddress("mcinfo.x", &mcinfo_x, &b_mcinfo_x);
   fChain->SetBranchAddress("mcinfo.y", &mcinfo_y, &b_mcinfo_y);
   fChain->SetBranchAddress("mcinfo.isPassTOF", mcinfo_isPassTOF, &b_mcinfo_isPassTOF);
   fChain->SetBranchAddress("mcinfo.nanti", &mcinfo_nanti, &b_mcinfo_nanti);
   fChain->SetBranchAddress("mcinfo.ntof", &mcinfo_ntof, &b_mcinfo_ntof);
   fChain->SetBranchAddress("mcinfo.ntoflay", mcinfo_ntoflay, &b_mcinfo_ntoflay);
   fChain->SetBranchAddress("mcinfo.ntrkclus", &mcinfo_ntrkclus, &b_mcinfo_ntrkclus);
   fChain->SetBranchAddress("mcinfo.ntrklay", mcinfo_ntrklay, &b_mcinfo_ntrklay);
   fChain->SetBranchAddress("mcinfo.energy_TOI", &mcinfo_energy_TOI, &b_mcinfo_energy_TOI);
   fChain->SetBranchAddress("mcinfo.energy_BOI", &mcinfo_energy_BOI, &b_mcinfo_energy_BOI);
   fChain->SetBranchAddress("mcinfo.Nbacksplash", &mcinfo_Nbacksplash, &b_mcinfo_Nbacksplash);
   fChain->SetBranchAddress("mcinfo.Nsecondary", &mcinfo_Nsecondary, &b_mcinfo_Nsecondary);
   fChain->SetBranchAddress("mcinfo.Nbrem", &mcinfo_Nbrem, &b_mcinfo_Nbrem);
   fChain->SetBranchAddress("mcinfo.Nconversion", &mcinfo_Nconversion, &b_mcinfo_Nconversion);
   fChain->SetBranchAddress("lvl1.JMembPatt", &lvl1_JMembPatt, &b_lvl1_JMembPatt);
   fChain->SetBranchAddress("lvl1.PhysBPatt", &lvl1_PhysBPatt, &b_lvl1_PhysBPatt);
   fChain->SetBranchAddress("lvl1.JMembPattMC", &lvl1_JMembPattMC, &b_lvl1_JMembPattMC);
   fChain->SetBranchAddress("lvl1.PhysBPattMC", &lvl1_PhysBPattMC, &b_lvl1_PhysBPattMC);
   fChain->SetBranchAddress("ecal.ish", &ecal_ish, &b_ecal_ish);
   fChain->SetBranchAddress("ecal.nsh", &ecal_nsh, &b_ecal_nsh);
   fChain->SetBranchAddress("ecal.theta", &ecal_theta, &b_ecal_theta);
   fChain->SetBranchAddress("ecal.phi", &ecal_phi, &b_ecal_phi);
   fChain->SetBranchAddress("ecal.nhits", &ecal_nhits, &b_ecal_nhits);
   fChain->SetBranchAddress("ecal.nhitsall", &ecal_nhitsall, &b_ecal_nhitsall);
   fChain->SetBranchAddress("ecal.stat", &ecal_stat, &b_ecal_stat);
   fChain->SetBranchAddress("ecal.edep", &ecal_edep, &b_ecal_edep);
   fChain->SetBranchAddress("ecal.ene17", &ecal_ene17, &b_ecal_ene17);
   fChain->SetBranchAddress("ecal.ene", &ecal_ene, &b_ecal_ene);
   fChain->SetBranchAddress("ecal.enc", &ecal_enc, &b_ecal_enc);
   fChain->SetBranchAddress("ecal.enp", &ecal_enp, &b_ecal_enp);
   fChain->SetBranchAddress("ecal.cog", ecal_cog, &b_ecal_cog);
   fChain->SetBranchAddress("ecal.entry", ecal_entry, &b_ecal_entry);
   fChain->SetBranchAddress("ecal.exit", ecal_exit, &b_ecal_exit);
   fChain->SetBranchAddress("ecal.cog_ecax", ecal_cog_ecax, &b_ecal_cog_ecax);
   fChain->SetBranchAddress("ecal.theta_ecax", &ecal_theta_ecax, &b_ecal_theta_ecax);
   fChain->SetBranchAddress("ecal.phi_ecax", &ecal_phi_ecax, &b_ecal_phi_ecax);
   fChain->SetBranchAddress("ecal.cellstat", ecal_cellstat, &b_ecal_cellstat);
   fChain->SetBranchAddress("ecal.adcl", ecal_adcl, &b_ecal_adcl);
   fChain->SetBranchAddress("ecal.adch", ecal_adch, &b_ecal_adch);
   fChain->SetBranchAddress("ecal.bdtp", &ecal_bdtp, &b_ecal_bdtp);
   fChain->SetBranchAddress("ecal.bdtp5", &ecal_bdtp5, &b_ecal_bdtp5);
   fChain->SetBranchAddress("ecal.bdtp5s", &ecal_bdtp5s, &b_ecal_bdtp5s);
   fChain->SetBranchAddress("ecal.nchi2", &ecal_nchi2, &b_ecal_nchi2);
   fChain->SetBranchAddress("trk.itrk", &trk_itrk, &b_trk_itrk);
   fChain->SetBranchAddress("trk.ntrk", &trk_ntrk, &b_trk_ntrk);
   fChain->SetBranchAddress("trk.mdist", &trk_mdist, &b_trk_mdist);
   fChain->SetBranchAddress("trk.mdcos", &trk_mdcos, &b_trk_mdcos);
   fChain->SetBranchAddress("trk.nhitx", &trk_nhitx, &b_trk_nhitx);
   fChain->SetBranchAddress("trk.nhity", &trk_nhity, &b_trk_nhity);
   fChain->SetBranchAddress("trk.hasnoisehit", &trk_hasnoisehit, &b_trk_hasnoisehit);
   fChain->SetBranchAddress("trk.hasnoisehit_ext", &trk_hasnoisehit_ext, &b_trk_hasnoisehit_ext);
   fChain->SetBranchAddress("trk.pat", &trk_pat, &b_trk_pat);
   fChain->SetBranchAddress("trk.patx", &trk_patx, &b_trk_patx);
   fChain->SetBranchAddress("trk.theta", &trk_theta, &b_trk_theta);
   fChain->SetBranchAddress("trk.phi", &trk_phi, &b_trk_phi);
   fChain->SetBranchAddress("trk.bcor", &trk_bcor, &b_trk_bcor);
   fChain->SetBranchAddress("trk.bcorinfit", &trk_bcorinfit, &b_trk_bcorinfit);
   fChain->SetBranchAddress("trk.rig", trk_rig, &b_trk_rig);
   fChain->SetBranchAddress("trk.rigerr", trk_rigerr, &b_trk_rigerr);
   fChain->SetBranchAddress("trk.rig_al", trk_rig_al, &b_trk_rig_al);
   fChain->SetBranchAddress("trk.rig_ck", trk_rig_ck, &b_trk_rig_ck);
   fChain->SetBranchAddress("trk.chi2x", trk_chi2x, &b_trk_chi2x);
   fChain->SetBranchAddress("trk.chi2y", trk_chi2y, &b_trk_chi2y);
   fChain->SetBranchAddress("trk.qin", &trk_qin, &b_trk_qin);
   fChain->SetBranchAddress("trk.qinrms", &trk_qinrms, &b_trk_qinrms);
   fChain->SetBranchAddress("trk.qinup", &trk_qinup, &b_trk_qinup);
   fChain->SetBranchAddress("trk.qinlow", &trk_qinlow, &b_trk_qinlow);
   fChain->SetBranchAddress("trk.qlay", trk_qlay, &b_trk_qlay);
   fChain->SetBranchAddress("trk.qmin", &trk_qmin, &b_trk_qmin);
   fChain->SetBranchAddress("trk.qmax", &trk_qmax, &b_trk_qmax);
   fChain->SetBranchAddress("trk.yqlay", trk_yqlay, &b_trk_yqlay);
   fChain->SetBranchAddress("trk.yqmin", &trk_yqmin, &b_trk_yqmin);
   fChain->SetBranchAddress("trk.yqmax", &trk_yqmax, &b_trk_yqmax);
   fChain->SetBranchAddress("trk.edeplay", trk_edeplay, &b_trk_edeplay);
   fChain->SetBranchAddress("trk.ecalentry", trk_ecalentry, &b_trk_ecalentry);
   fChain->SetBranchAddress("trk.x0", &trk_x0, &b_x0);
   fChain->SetBranchAddress("trk.y0", &trk_y0, &b_y0);
   fChain->SetBranchAddress("trk.xL1", &trk_xL1, &b_xL1);
   fChain->SetBranchAddress("trk.yL1", &trk_yL1, &b_yL1);
   fChain->SetBranchAddress("trk.xL9", &trk_xL9, &b_xL9);
   fChain->SetBranchAddress("trk.yL9", &trk_yL9, &b_yL9);
   fChain->SetBranchAddress("trk.mindisty", &trk_mindisty, &b_trk_mindisty);
   fChain->SetBranchAddress("trk.maxresy", &trk_maxresy, &b_trk_maxresy);
   fChain->SetBranchAddress("trk.maxresx", &trk_maxresx, &b_trk_maxresx);
   fChain->SetBranchAddress("trk.srmin", &trk_srmin, &b_trk_srmin);
   fChain->SetBranchAddress("trk.sr", trk_sr, &b_trk_sr);
   fChain->SetBranchAddress("trk.eleccbdt", &trk_eleccbdt, &b_trk_eleccbdt);
   fChain->SetBranchAddress("trk.eleccbdtmdr", &trk_eleccbdtmdr, &b_trk_eleccbdtmdr);
   fChain->SetBranchAddress("trk.bendarea", &trk_bendarea, &b_trk_bendarea);
   fChain->SetBranchAddress("trk.chi2y_op", &trk_chi2y_op, &b_trk_chi2y_op);
   fChain->SetBranchAddress("trk.hitcoo", trk_hitcoo, &b_trk_hitcoo);
   fChain->SetBranchAddress("trk.layresy", trk_layresy, &b_trk_layresy);
   fChain->SetBranchAddress("trk.layresy_exc", trk_layresy_exc, &b_trk_layresy_exc);
   fChain->SetBranchAddress("trk.layresx", trk_layresx, &b_trk_layresx);
   fChain->SetBranchAddress("trk.layresx_exc", trk_layresx_exc, &b_trk_layresx_exc);
   fChain->SetBranchAddress("trk.hittkid", trk_hittkid, &b_trk_hittkid);
   fChain->SetBranchAddress("trk.hitsenid", trk_hitsenid, &b_trk_hitsenid);
   fChain->SetBranchAddress("trk.laypos", trk_laypos, &b_trk_laypos);
   fChain->SetBranchAddress("trk.lay_dymin", trk_lay_dymin, &b_trk_lay_dymin);
   fChain->SetBranchAddress("trk.lay_dxmin", trk_lay_dxmin, &b_trk_lay_dxmin);
   fChain->SetBranchAddress("trk.lay_mccoo", trk_lay_mccoo, &b_trk_lay_mccoo);
   fChain->SetBranchAddress("trk.lay_propcoo", trk_lay_propcoo, &b_trk_lay_propcoo);
   fChain->SetBranchAddress("trk.lay_propcoo_eloss", trk_lay_propcoo_eloss, &b_trk_lay_propcoo_eloss);
   fChain->SetBranchAddress("trk.lay_linecoo", trk_lay_linecoo, &b_trk_lay_linecoo);
   fChain->SetBranchAddress("trk.TrackHit_EdepX", trk_TrackHit_EdepX, &b_trk_TrackHit_EdepX);
   fChain->SetBranchAddress("trk.TrackHit_EdepY", trk_TrackHit_EdepY, &b_trk_TrackHit_EdepY);
   fChain->SetBranchAddress("trk.TrackHit_OnlyX", trk_TrackHit_OnlyX, &b_trk_TrackHit_OnlyX);
   fChain->SetBranchAddress("trk.TrackHit_OnlyY", trk_TrackHit_OnlyY, &b_trk_TrackHit_OnlyY);
   fChain->SetBranchAddress("trk.TrackHit_Coord", trk_TrackHit_Coord, &b_trk_TrackHit_Coord);
   fChain->SetBranchAddress("trk.TrackHit_ECoord", trk_TrackHit_ECoord, &b_trk_TrackHit_ECoord);
   fChain->SetBranchAddress("trk.TrackHit_Q", trk_TrackHit_Q, &b_trk_TrackHit_Q);
   fChain->SetBranchAddress("trk.MaxNHit_offtrack", &trk_MaxNHit_offtrack, &b_trk_MaxNHit_offtrack);
   fChain->SetBranchAddress("trk.NHitLay_offtrack", trk_NHitLay_offtrack, &b_trk_NHitLay_offtrack);
   fChain->SetBranchAddress("trk.EdepXLay_offtrack", trk_EdepXLay_offtrack, &b_trk_EdepXLay_offtrack);
   fChain->SetBranchAddress("trk.EdepYLay_offtrack", trk_EdepYLay_offtrack, &b_trk_EdepYLay_offtrack);
   fChain->SetBranchAddress("trk.NHitLay_neighbor", trk_NHitLay_neighbor, &b_trk_NHitLay_neighbor);
   fChain->SetBranchAddress("trk.EdepXLay_neighbor", trk_EdepXLay_neighbor, &b_trk_EdepXLay_neighbor);
   fChain->SetBranchAddress("trk.EdepYLay_neighbor", trk_EdepYLay_neighbor, &b_trk_EdepYLay_neighbor);
   fChain->SetBranchAddress("trk.NsumHit_radius", trk_NsumHit_radius, &b_trk_NsumHit_radius);
   fChain->SetBranchAddress("trk.EsumXHit_radius", trk_EsumXHit_radius, &b_trk_EsumXHit_radius);
   fChain->SetBranchAddress("trk.EsumYHit_radius", trk_EsumYHit_radius, &b_trk_EsumYHit_radius);
   fChain->SetBranchAddress("trk.NStrip", trk_NStrip, &b_trk_NStrip);
   fChain->SetBranchAddress("trk.NHit_OffTrackVC", &trk_NHit_OffTrackVC, &b_trk_NHit_OffTrackVC);
   fChain->SetBranchAddress("tof.ibeta", &tof_ibeta, &b_tof_ibeta);
   fChain->SetBranchAddress("tof.beta", &tof_beta, &b_tof_beta);
   fChain->SetBranchAddress("tof.nclus_intime", &tof_nclus_intime, &b_tof_nclus_intime);
   fChain->SetBranchAddress("tof.q", &tof_q, &b_tof_q);
   fChain->SetBranchAddress("tof.qlow", &tof_qlow, &b_tof_qlow);
   fChain->SetBranchAddress("tof.qup", &tof_qup, &b_tof_qup);
   fChain->SetBranchAddress("tof.ibetah", &tof_ibetah, &b_tof_ibetah);
   fChain->SetBranchAddress("tof.buildtype", &tof_buildtype, &b_tof_buildtype);
   fChain->SetBranchAddress("tof.match", &tof_match, &b_tof_match);
   fChain->SetBranchAddress("tof.match_ecal", &tof_match_ecal, &b_tof_match_ecal);
   fChain->SetBranchAddress("tof.match_trd", &tof_match_trd, &b_tof_match_trd);
   fChain->SetBranchAddress("tof.goodbeta", &tof_goodbeta, &b_tof_goodbeta);
   fChain->SetBranchAddress("tof.pattern", &tof_pattern, &b_tof_pattern);
   fChain->SetBranchAddress("tof.pattern_betaR", &tof_pattern_betaR, &b_tof_pattern_betaR);
   fChain->SetBranchAddress("tof.nlay", &tof_nlay, &b_tof_nlay);
   fChain->SetBranchAddress("tof.betah", &tof_betah, &b_tof_betah);
   fChain->SetBranchAddress("tof.xL1", &tof_xL1, &b_tof_xL1);
   fChain->SetBranchAddress("tof.yL1", &tof_yL1, &b_tof_yL1);
   fChain->SetBranchAddress("tof.xL9", &tof_xL9, &b_tof_xL9);
   fChain->SetBranchAddress("tof.yL9", &tof_yL9, &b_tof_yL9);
   fChain->SetBranchAddress("tof.ebetav", &tof_ebetav, &b_tof_ebetav);
   fChain->SetBranchAddress("tof.betahs", &tof_betahs, &b_tof_betahs);
   fChain->SetBranchAddress("tof.qh", &tof_qh, &b_tof_qh);
   fChain->SetBranchAddress("tof.qhrms", &tof_qhrms, &b_tof_qhrms);
   fChain->SetBranchAddress("tof.qhlay", tof_qhlay, &b_tof_qhlay);
   fChain->SetBranchAddress("tof.z", &tof_z, &b_tof_z);
   fChain->SetBranchAddress("tof.probz", &tof_probz, &b_tof_probz);
   fChain->SetBranchAddress("tof.chi2c", &tof_chi2c, &b_tof_chi2c);
   fChain->SetBranchAddress("tof.chi2t", &tof_chi2t, &b_tof_chi2t);
   fChain->SetBranchAddress("tof.mass", &tof_mass, &b_tof_mass);
   fChain->SetBranchAddress("tof.mass_err", &tof_mass_err, &b_tof_mass_err);
   fChain->SetBranchAddress("tof.edeplay", tof_edeplay, &b_tof_edeplay);
   fChain->SetBranchAddress("tof.nclslay_offtrack", tof_nclslay_offtrack, &b_tof_nclslay_offtrack);
   fChain->SetBranchAddress("tof.edeplay_offtrack", tof_edeplay_offtrack, &b_tof_edeplay_offtrack);
   fChain->SetBranchAddress("tof.edeplay_2nd", tof_edeplay_2nd, &b_tof_edeplay_2nd);
   fChain->SetBranchAddress("tof.nclslay_2nd", tof_nclslay_2nd, &b_tof_nclslay_2nd);
   fChain->SetBranchAddress("trd.type", &trd_type, &b_trd_type);
   fChain->SetBranchAddress("trd.nhtrd", &trd_nhtrd, &b_trd_nhtrd);
   fChain->SetBranchAddress("trd.nsegx", &trd_nsegx, &b_trd_nsegx);
   fChain->SetBranchAddress("trd.nsegy", &trd_nsegy, &b_trd_nsegy);
   fChain->SetBranchAddress("trd.itrd", &trd_itrd, &b_trd_itrd);
   fChain->SetBranchAddress("trd.ihtrd", &trd_ihtrd, &b_trd_ihtrd);
   fChain->SetBranchAddress("trd.nhits", trd_nhits, &b_trd_nhits);
   fChain->SetBranchAddress("trd.elprob", trd_elprob, &b_trd_elprob);
   fChain->SetBranchAddress("trd.prprob", trd_prprob, &b_trd_prprob);
   fChain->SetBranchAddress("trd.heprob", trd_heprob, &b_trd_heprob);
   fChain->SetBranchAddress("trd.klkhd", trd_klkhd, &b_trd_klkhd);
   fChain->SetBranchAddress("trd.theta", &trd_theta, &b_trd_theta);
   fChain->SetBranchAddress("trd.phi", &trd_phi, &b_trd_phi);
   fChain->SetBranchAddress("trd.coo", trd_coo, &b_trd_coo);
   fChain->SetBranchAddress("trd.ecalentry", trd_ecalentry, &b_trd_ecalentry);
   fChain->SetBranchAddress("trd.ecalexit", trd_ecalexit, &b_trd_ecalexit);
   fChain->SetBranchAddress("trd.xL1", &trd_xL1, &b_trd_xL1);
   fChain->SetBranchAddress("trd.yL1", &trd_yL1, &b_trd_yL1);
   fChain->SetBranchAddress("trd.xL9", &trd_xL9, &b_trd_xL9);
   fChain->SetBranchAddress("trd.yL9", &trd_yL9, &b_trd_yL9);
   fChain->SetBranchAddress("trd.isPassTOF", trd_isPassTOF, &b_trd_isPassTOF);
   fChain->SetBranchAddress("rich.ntothits", &rich_ntothits, &b_rich_ntothits);
   fChain->SetBranchAddress("rich.npexp_el", &rich_npexp_el, &b_rich_npexp_el);
   fChain->SetBranchAddress("acc.nclus", &acc_nclus, &b_acc_nclus);
   fChain->SetBranchAddress("acc.NACC", &acc_NACC, &b_acc_NACC);
   fChain->SetBranchAddress("acc.NBackSplashHits", &acc_NBackSplashHits, &b_acc_NBackSplashHits);
   fChain->SetBranchAddress("acc.NInteractionHits", &acc_NInteractionHits, &b_acc_NInteractionHits);
   fChain->SetBranchAddress("bdt_interaction", &bdt_interaction, &b_bdt_interaction);
   fChain->SetBranchAddress("bdt_spillover", &bdt_spillover, &b_bdt_spillover);
   fChain->SetBranchAddress("bdt_combined", &bdt_combined, &b_bdt_combined);
   fChain->SetBranchAddress("bdta_interaction", &bdta_interaction, &b_bdta_interaction);
   fChain->SetBranchAddress("bdta_spillover", &bdta_spillover, &b_bdta_spillover);
   fChain->SetBranchAddress("bdta_combined", &bdta_combined, &b_bdta_combined);
   fChain->SetBranchAddress("bdtx_interaction", &bdtx_interaction, &b_bdtx_interaction);
   fChain->SetBranchAddress("bdtx_spillover", &bdtx_spillover, &b_bdtx_spillover);
   fChain->SetBranchAddress("bdtx_combined", &bdtx_combined, &b_bdtx_combined);
   fChain->SetBranchAddress("bdtax_interaction", &bdtax_interaction, &b_bdtax_interaction);
   fChain->SetBranchAddress("bdtax_spillover", &bdtax_spillover, &b_bdtax_spillover);
   fChain->SetBranchAddress("bdtax_combined", &bdtax_combined, &b_bdtax_combined);
   fChain->SetBranchAddress("Match_TRDECAL", &Match_TRDECAL, &b_Match_TRDECAL);
   fChain->SetBranchAddress("Pass_TRKECAL", &Pass_TRKECAL, &b_Pass_TRKECAL);
   fChain->SetBranchAddress("Pass_TRDECAL", &Pass_TRDECAL, &b_Pass_TRDECAL);
   fChain->SetBranchAddress("ECAL_IsFocused", &ECAL_IsFocused, &b_ECAL_IsFocused);
   fChain->SetBranchAddress("ECAL_IsContained", &ECAL_IsContained, &b_ECAL_IsContained);
   fChain->SetBranchAddress("tcpu", &tcpu, &b_tcpu);
   fChain->SetBranchAddress("N_Shwr", &N_Shwr, &b_N_Shwr);
   fChain->SetBranchAddress("hadflag", &hadflag, &b_hadflag);
   fChain->SetBranchAddress("E0", E0, &b_E0);
   fChain->SetBranchAddress("A0", A0, &b_A0);
   fChain->SetBranchAddress("ShwrPos", ShwrPos, &b_ShwrPos);
   fChain->SetBranchAddress("ShwrDir", ShwrDir, &b_ShwrDir);
   fChain->SetBranchAddress("Ene", &Ene, &b_Ene);
   fChain->SetBranchAddress("Lkhd0", &Lkhd0, &b_Lkhd0);
   fChain->SetBranchAddress("Lkhd1", &Lkhd1, &b_Lkhd1);
   fChain->SetBranchAddress("EmBDT", &EmBDT, &b_EmBDT);
   fChain->SetBranchAddress("NLatLeakCell", &NLatLeakCell, &b_NLatLeakCell);
   fChain->SetBranchAddress("LatLeakFrac", &LatLeakFrac, &b_LatLeakFrac);
   fChain->SetBranchAddress("RearLeakFrac", &RearLeakFrac, &b_RearLeakFrac);
   fChain->SetBranchAddress("trd_new", &trd_new, &b_trd_new);
   fChain->SetBranchAddress("trd_Likelihood_ele", &trd_Likelihood_ele, &b_trd_Likelihood_ele);
   fChain->SetBranchAddress("trd_Likelihood_pr", &trd_Likelihood_pr, &b_trd_Likelihood_pr);
   fChain->SetBranchAddress("trd_lastbin", &trd_lastbin, &b_trd_lastbin);
   fChain->SetBranchAddress("trd_Likelihood_ele_lastbin", &trd_Likelihood_ele_lastbin, &b_trd_Likelihood_ele_lastbin);
   fChain->SetBranchAddress("trd_Likelihood_pr_lastbin", &trd_Likelihood_pr_lastbin, &b_trd_Likelihood_pr_lastbin);
   Notify();
}

Bool_t t_add::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void t_add::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t t_add::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef t_add_cxx
