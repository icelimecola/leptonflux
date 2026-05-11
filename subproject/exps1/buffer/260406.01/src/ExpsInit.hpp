_Pragma("once")

#include <iostream>
using namespace std;
#include "TChain.h"
#include "VarMinitree.hpp"

class ExpsInit:virtual public VarMinitree{
    public:
        ExpsInit():VarMinitree(){cout<<"Constructor_expsinit"<<endl;}
        ~ExpsInit(){}
    public:
        void INIT_SetZero();
		void INIT_SetDatatype();
		void INIT_CHAIN_SetBranchAddress(TChain*);
        void INIT_TREE_BuildBrand(TTree*&);
};

void ExpsInit::INIT_SetZero(){
	//==== DATATYPE
    dataType=0;
    //==== BADRUN
	isbadrun = isbadrun_trd = isbadrun_ecal = isbadrun_trk = isbadrun_general = isbadrun_daq = isbadrun_fov = 0;
	memset(isbad, 0, sizeof(isbad));
    //==== ECAL
    inecal=0;
	memset(xecal, 0, sizeof(xecal));
	memset(yecal, 0, sizeof(yecal));
    //==== ECALMATCH
	trkecalmatch=0;
	trkecaldx = trkecaldy = 0;
	trdecalmatch=0;
	trdecaldx = trdecaldy = 0;
}

void ExpsInit::INIT_SetDatatype(){
    if( mcinfo_p > 0 ) dataType = _MC_Data;
	else if( info_run < 1305756000 ) dataType = _TB_Data;
	else dataType = _ISS_Data;
}

void ExpsInit::INIT_CHAIN_SetBranchAddress(TChain* fChain){
    fChain->SetBranchStatus("MySimpleHits*",0);
    fChain->SetBranchStatus("MyMatchedSimpleHits*",0);
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
    if(fChain->FindBranch("mcinfo.p")){
        fChain->SetBranchAddress("mcinfo.p", &mcinfo_p, &b_mcinfo_p);
        fChain->SetBranchAddress("mcinfo.q", &mcinfo_q, &b_mcinfo_q);
        fChain->SetBranchAddress("lvl1.JMembPattMC", &lvl1_JMembPattMC, &b_lvl1_JMembPattMC);
        fChain->SetBranchAddress("lvl1.PhysBPattMC", &lvl1_PhysBPattMC, &b_lvl1_PhysBPattMC);
    }else{
        mcinfo_p = 0;
        mcinfo_q = 0;
        lvl1_JMembPattMC = 0;
        lvl1_PhysBPattMC = 0;
    }
    //==== original↓
    // fChain->SetBranchAddress("mcinfo.theta", &mcinfo_theta, &b_mcinfo_theta);
    // fChain->SetBranchAddress("mcinfo.phi", &mcinfo_phi, &b_mcinfo_phi);
    // fChain->SetBranchAddress("mcinfo.x", &mcinfo_x, &b_mcinfo_x);
    // fChain->SetBranchAddress("mcinfo.y", &mcinfo_y, &b_mcinfo_y);
    // fChain->SetBranchAddress("mcinfo.isPassTOF", mcinfo_isPassTOF, &b_mcinfo_isPassTOF);
    // fChain->SetBranchAddress("mcinfo.nanti", &mcinfo_nanti, &b_mcinfo_nanti);
    // fChain->SetBranchAddress("mcinfo.ntof", &mcinfo_ntof, &b_mcinfo_ntof);
    // fChain->SetBranchAddress("mcinfo.ntoflay", mcinfo_ntoflay, &b_mcinfo_ntoflay);
    // fChain->SetBranchAddress("mcinfo.ntrkclus", &mcinfo_ntrkclus, &b_mcinfo_ntrkclus);
    // fChain->SetBranchAddress("mcinfo.ntrklay", mcinfo_ntrklay, &b_mcinfo_ntrklay);
    // fChain->SetBranchAddress("mcinfo.energy_TOI", &mcinfo_energy_TOI, &b_mcinfo_energy_TOI);
    // fChain->SetBranchAddress("mcinfo.energy_BOI", &mcinfo_energy_BOI, &b_mcinfo_energy_BOI);
    // fChain->SetBranchAddress("mcinfo.Nbacksplash", &mcinfo_Nbacksplash, &b_mcinfo_Nbacksplash);
    // fChain->SetBranchAddress("mcinfo.Nsecondary", &mcinfo_Nsecondary, &b_mcinfo_Nsecondary);
    // fChain->SetBranchAddress("mcinfo.Nbrem", &mcinfo_Nbrem, &b_mcinfo_Nbrem);
    // fChain->SetBranchAddress("mcinfo.Nconversion", &mcinfo_Nconversion, &b_mcinfo_Nconversion);
    //==== original↑
    fChain->SetBranchAddress("lvl1.JMembPatt", &lvl1_JMembPatt, &b_lvl1_JMembPatt);
    fChain->SetBranchAddress("lvl1.PhysBPatt", &lvl1_PhysBPatt, &b_lvl1_PhysBPatt);
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
    //==== original↓
    // fChain->SetBranchAddress("ecal.cog_ecax", ecal_cog_ecax, &b_ecal_cog_ecax);
    // fChain->SetBranchAddress("ecal.theta_ecax", &ecal_theta_ecax, &b_ecal_theta_ecax);
    // fChain->SetBranchAddress("ecal.phi_ecax", &ecal_phi_ecax, &b_ecal_phi_ecax);
    // fChain->SetBranchAddress("ecal.cellstat", ecal_cellstat, &b_ecal_cellstat);
    // fChain->SetBranchAddress("ecal.adcl", ecal_adcl, &b_ecal_adcl);
    // fChain->SetBranchAddress("ecal.adch", ecal_adch, &b_ecal_adch);
    //==== original↑
    fChain->SetBranchAddress("ecal.bdtp", &ecal_bdtp, &b_ecal_bdtp);
    fChain->SetBranchAddress("ecal.bdtp5", &ecal_bdtp5, &b_ecal_bdtp5);
    fChain->SetBranchAddress("ecal.bdtp5s", &ecal_bdtp5s, &b_ecal_bdtp5s);
    fChain->SetBranchAddress("trk.ntrk", &trk_ntrk, &b_trk_ntrk);
    // fChain->SetBranchAddress("trk.mdist", &trk_mdist, &b_trk_mdist);
    // fChain->SetBranchAddress("trk.mdcos", &trk_mdcos, &b_trk_mdcos);
    fChain->SetBranchAddress("trk.nhitx", &trk_nhitx, &b_trk_nhitx);
    fChain->SetBranchAddress("trk.nhity", &trk_nhity, &b_trk_nhity);
    fChain->SetBranchAddress("trk.hasnoisehit", &trk_hasnoisehit, &b_trk_hasnoisehit);
    fChain->SetBranchAddress("trk.hasnoisehit_ext", &trk_hasnoisehit_ext, &b_trk_hasnoisehit_ext);
    fChain->SetBranchAddress("trk.pat", &trk_pat, &b_trk_pat);
    fChain->SetBranchAddress("trk.patx", &trk_patx, &b_trk_patx);
    fChain->SetBranchAddress("trk.theta", &trk_theta, &b_trk_theta);
    fChain->SetBranchAddress("trk.phi", &trk_phi, &b_trk_phi);
    //==== original↓
    // fChain->SetBranchAddress("trk.bcor", &trk_bcor, &b_trk_bcor);
    // fChain->SetBranchAddress("trk.bcorinfit", &trk_bcorinfit, &b_trk_bcorinfit);
    //==== original↑
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
    //==== original↓
    /*
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
    */
    //==== original↑
    fChain->SetBranchAddress("tof.ibeta", &tof_ibeta, &b_tof_ibeta);
    fChain->SetBranchAddress("tof.beta", &tof_beta, &b_tof_beta);
    fChain->SetBranchAddress("tof.nclus_intime", &tof_nclus_intime, &b_tof_nclus_intime);
    fChain->SetBranchAddress("tof.q", &tof_q, &b_tof_q);
    fChain->SetBranchAddress("tof.qlow", &tof_qlow, &b_tof_qlow);
    fChain->SetBranchAddress("tof.qup", &tof_qup, &b_tof_qup);
    //   fChain->SetBranchAddress("tof.ibetah", &tof_ibetah, &b_tof_ibetah);
    //   fChain->SetBranchAddress("tof.buildtype", &tof_buildtype, &b_tof_buildtype);
    //   fChain->SetBranchAddress("tof.match", &tof_match, &b_tof_match);
    //   fChain->SetBranchAddress("tof.match_ecal", &tof_match_ecal, &b_tof_match_ecal);
    //   fChain->SetBranchAddress("tof.match_trd", &tof_match_trd, &b_tof_match_trd);
    //   fChain->SetBranchAddress("tof.goodbeta", &tof_goodbeta, &b_tof_goodbeta);
    //   fChain->SetBranchAddress("tof.pattern", &tof_pattern, &b_tof_pattern);
    //   fChain->SetBranchAddress("tof.pattern_betaR", &tof_pattern_betaR, &b_tof_pattern_betaR);
    //   fChain->SetBranchAddress("tof.nlay", &tof_nlay, &b_tof_nlay);
    fChain->SetBranchAddress("tof.betah", &tof_betah, &b_tof_betah);
    //   fChain->SetBranchAddress("tof.xL1", &tof_xL1, &b_tof_xL1);
    //   fChain->SetBranchAddress("tof.yL1", &tof_yL1, &b_tof_yL1);
    //   fChain->SetBranchAddress("tof.xL9", &tof_xL9, &b_tof_xL9);
    //   fChain->SetBranchAddress("tof.yL9", &tof_yL9, &b_tof_yL9);
    //   fChain->SetBranchAddress("tof.ebetav", &tof_ebetav, &b_tof_ebetav);
    //   fChain->SetBranchAddress("tof.betahs", &tof_betahs, &b_tof_betahs);
    //   fChain->SetBranchAddress("tof.qh", &tof_qh, &b_tof_qh);
    fChain->SetBranchAddress("tof.qhrms", &tof_qhrms, &b_tof_qhrms);
    fChain->SetBranchAddress("tof.qhlay", tof_qhlay, &b_tof_qhlay);
    //   fChain->SetBranchAddress("tof.z", &tof_z, &b_tof_z);
    //   fChain->SetBranchAddress("tof.probz", &tof_probz, &b_tof_probz);
    //   fChain->SetBranchAddress("tof.chi2c", &tof_chi2c, &b_tof_chi2c);
    //   fChain->SetBranchAddress("tof.chi2t", &tof_chi2t, &b_tof_chi2t);
    //   fChain->SetBranchAddress("tof.mass", &tof_mass, &b_tof_mass);
    //   fChain->SetBranchAddress("tof.mass_err", &tof_mass_err, &b_tof_mass_err);
    //   fChain->SetBranchAddress("tof.edeplay", tof_edeplay, &b_tof_edeplay);
    //   fChain->SetBranchAddress("tof.nclslay_offtrack", tof_nclslay_offtrack, &b_tof_nclslay_offtrack);
    //   fChain->SetBranchAddress("tof.edeplay_offtrack", tof_edeplay_offtrack, &b_tof_edeplay_offtrack);
    //   fChain->SetBranchAddress("tof.edeplay_2nd", tof_edeplay_2nd, &b_tof_edeplay_2nd);
    //   fChain->SetBranchAddress("tof.nclslay_2nd", tof_nclslay_2nd, &b_tof_nclslay_2nd);
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
    //   fChain->SetBranchAddress("trd.ecalentry", trd_ecalentry, &b_trd_ecalentry);
    //   fChain->SetBranchAddress("trd.ecalexit", trd_ecalexit, &b_trd_ecalexit);
    //   fChain->SetBranchAddress("trd.xL1", &trd_xL1, &b_trd_xL1);
    //   fChain->SetBranchAddress("trd.yL1", &trd_yL1, &b_trd_yL1);
    //   fChain->SetBranchAddress("trd.xL9", &trd_xL9, &b_trd_xL9);
    //   fChain->SetBranchAddress("trd.yL9", &trd_yL9, &b_trd_yL9);
    fChain->SetBranchAddress("trd.isPassTOF", trd_isPassTOF, &b_trd_isPassTOF);
    // fChain->SetBranchAddress("rich.ntothits", &rich_ntothits, &b_rich_ntothits);     //----GCX 151026
    // fChain->SetBranchAddress("rich.npexp_el", &rich_npexp_el, &b_rich_npexp_el);     //----GCX 151026
    // fChain->SetBranchAddress("acc.nclus", &acc_nclus, &b_acc_nclus);     //----GCX 151026
    // fChain->SetBranchAddress("acc.NACC", &acc_NACC, &b_acc_NACC);        //----GCX 151026
    // fChain->SetBranchAddress("acc.NBackSplashHits", &acc_NBackSplashHits, &b_acc_NBackSplashHits);   //----GCX 151026
    // fChain->SetBranchAddress("acc.NInteractionHits", &acc_NInteractionHits, &b_acc_NInteractionHits);    //----GCX 151026
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
    if( fChain->FindBranch("LkhdRatio") ) fChain->SetBranchAddress("LkhdRatio", &LkhdRatio, &b_LkhdRatio);
    fChain->SetBranchAddress("Lkhd1", &Lkhd1, &b_Lkhd1);
    fChain->SetBranchAddress("EmBDT", &EmBDT, &b_EmBDT);
    fChain->SetBranchAddress("NLatLeakCell", &NLatLeakCell, &b_NLatLeakCell);
    fChain->SetBranchAddress("LatLeakFrac", &LatLeakFrac, &b_LatLeakFrac);
    fChain->SetBranchAddress("RearLeakFrac", &RearLeakFrac, &b_RearLeakFrac);
    fChain->SetBranchAddress("trd_new", &trd_new, &b_trd_new);
    fChain->SetBranchAddress("trd_Likelihood_ele", &trd_Likelihood_ele, &b_trd_Likelihood_ele);
    fChain->SetBranchAddress("trd_Likelihood_pr", &trd_Likelihood_pr, &b_trd_Likelihood_pr);
    // fChain->SetBranchAddress("trd_lastbin", &trd_lastbin, &b_trd_lastbin);
    // fChain->SetBranchAddress("trd_Likelihood_ele_lastbin", &trd_Likelihood_ele_lastbin, &b_trd_Likelihood_ele_lastbin);
    // fChain->SetBranchAddress("trd_Likelihood_pr_lastbin", &trd_Likelihood_pr_lastbin, &b_trd_Likelihood_pr_lastbin);
    if( fChain->FindBranch( "ntrdtrack_x" ) ){ // since v4
        fChain->SetBranchAddress("ntrdtrack_x", &ntrdtrack_x, &b_ntrdtrack_x);
        fChain->SetBranchAddress("ntrdtrack_y", &ntrdtrack_y, &b_ntrdtrack_y);
        fChain->SetBranchAddress("nvertex_2d_x", &nvertex_2d_x, &b_nvertex_2d_x);
        fChain->SetBranchAddress("nvertex_2d_y", &nvertex_2d_y, &b_nvertex_2d_y);
        fChain->SetBranchAddress("nvertex_3d", &nvertex_3d, &b_nvertex_3d);
        fChain->SetBranchAddress("vertex_x", &vertex_x, &b_vertex_x);
        fChain->SetBranchAddress("vertex_y", &vertex_y, &b_vertex_y);
        fChain->SetBranchAddress("vertex_z", &vertex_z, &b_vertex_z);
        fChain->SetBranchAddress("vertex_x_err", &vertex_x_err, &b_vertex_x_err);
        fChain->SetBranchAddress("vertex_y_err", &vertex_y_err, &b_vertex_y_err);
        fChain->SetBranchAddress("vertex_z_err", &vertex_z_err, &b_vertex_z_err);
        fChain->SetBranchAddress("vertex_ntrack", &vertex_ntrack, &b_vertex_ntrack);
        fChain->SetBranchAddress("vertex_nhit", &vertex_nhit, &b_vertex_nhit);
        fChain->SetBranchAddress("vertex_chi2", &vertex_chi2, &b_vertex_chi2);
        fChain->SetBranchAddress("vertex_ntrack_x", &vertex_ntrack_x, &b_vertex_ntrack_x);
        fChain->SetBranchAddress("vertex_nhit_x", &vertex_nhit_x, &b_vertex_nhit_x);
        fChain->SetBranchAddress("vertex_chi2_x", &vertex_chi2_x, &b_vertex_chi2_x);
        fChain->SetBranchAddress("vertex_ntrack_y", &vertex_ntrack_y, &b_vertex_ntrack_y);
        fChain->SetBranchAddress("vertex_nhit_y", &vertex_nhit_y, &b_vertex_nhit_y);
        fChain->SetBranchAddress("vertex_chi2_y", &vertex_chi2_y, &b_vertex_chi2_y);
        fChain->SetBranchAddress("vertex_is2d", &vertex_is2d, &b_vertex_is2d);
    }
}


//==== INIT_TREE_BuildBrand
//-- cuz the ins_tree need to be reinit in this function, so we can't only transform the pointer(it'll lose the address) but ref
void ExpsInit::INIT_TREE_BuildBrand(TTree*& ins_tree){
	ins_tree = new TTree( "mt", "mini tree" );
	ins_tree->Branch("run", &info_run, "run/i");
	ins_tree->Branch("event", &info_event, "event/i");
	ins_tree->Branch("utime", &info_utime, "utime/i");
	ins_tree->Branch("lf", &info_lf, "lf/F");
	ins_tree->Branch("lfc", &info_lfc, "lfc/F");
	ins_tree->Branch("cf", info_maxcf, "cf[4]/F");
	ins_tree->Branch("cfi", info_maxcfi, "cfi[4]/F");
	ins_tree->Branch("poscf", info_poscf, "poscf[4]/F");
	ins_tree->Branch("negcf", info_negcf, "negcf[4]/F");
	ins_tree->Branch("poscfi", info_poscfi, "poscfi[4]/F");
	ins_tree->Branch("negcfi", info_negcfi, "negcfi[4]/F");
	ins_tree->Branch("maxcf", info_maxcf, "maxcf[4]/F");
	ins_tree->Branch("maxcfi", info_maxcfi, "maxcfi[4]/F");
	ins_tree->Branch("TTheta", &info_theta, "TTheta/F");
	ins_tree->Branch("Phi", &info_phi, "Phi/F");
	ins_tree->Branch("Glat", &info_glat, "Glat/F");
	ins_tree->Branch("Glong", &info_glong, "Glong/F");
	ins_tree->Branch("Mlong", &info_mlon, "Mlong/F");
	ins_tree->Branch("Mlat", &info_mlat, "Mlat/F");
	ins_tree->Branch("Gr", &info_r, "Gr/F");
    //
	ins_tree->Branch("cfts", maxcf_ts05, "cfts[4]/F"); // TS05
	ins_tree->Branch("mcp", &mcinfo_p, "mcp/F");
	ins_tree->Branch("mcq", &mcinfo_q, "mcq/F");
	//
	ins_tree->Branch("isbadrun", &isbadrun, "isbadrun/I");
	ins_tree->Branch("isbadrun_trd", &isbadrun_trd, "isbadrun_trd/I");
	ins_tree->Branch("isbadrun_trk", &isbadrun_trk, "isbadrun_trk/I");
	ins_tree->Branch("isbadrun_ecal", &isbadrun_ecal, "isbadrun_ecal/I");
	ins_tree->Branch("isbadrun_general", &isbadrun_general, "isbadrun_general/I");
	ins_tree->Branch("isbadrun_daq", &isbadrun_daq, "isbadrun_daq/I");
	ins_tree->Branch("isbadrun_fov", &isbadrun_fov, "isbadrun_fov/I");
	ins_tree->Branch("isbad", isbad, Form("isbad[%d]/I", nbadruntype));
	ins_tree->Branch("lvl1_JMembPatt", &lvl1_JMembPatt, "lvl1_JMembPatt/I");
	ins_tree->Branch("lvl1_PhysBPatt", &lvl1_PhysBPatt, "lvl1_PhysBPatt/I");
	ins_tree->Branch("theta", &(trk_theta), "theta/F");
    //
	ins_tree->Branch("beta", &(tof_betah), "beta/F");
	ins_tree->Branch("tofq", &(tof_qh), "tofq/F");
	ins_tree->Branch("tofqlow", &(tof_qlow), "tofqlow/F");
	ins_tree->Branch("tofqup", &(tof_qup), "tofqup/F");
	ins_tree->Branch("nclus_intime", &(tof_nclus_intime), "nclus_intime/I");
	//
	ins_tree->Branch("trk_qlay", &(trk_qlay[0]), "trk_qlay[9]/F");
	ins_tree->Branch("ntrk", &trk_ntrk, "ntrk/I");
	ins_tree->Branch("pat", &trk_pat, "pat/I");
	ins_tree->Branch("patx", &trk_patx, "patx/I");
	ins_tree->Branch("rig", &rigidity, "rig/F");
	ins_tree->Branch("qin", &trk_qin, "qin/F");
	ins_tree->Branch("q1", &(trk_qlay[0]), "q1/F");
	ins_tree->Branch("q9", &(trk_qlay[8]), "q9/F");
	ins_tree->Branch("chi2x", &(trk_chi2x[1]), "chi2x/F");
	ins_tree->Branch("chi2y", &(trk_chi2y[1]), "chi2y/F");
	// tree_output->Branch("resy", trk_layresy, "resy[9]/F");
	// tree_output->Branch("excresy", trk_layresy_exc, "excresy[9]/F");
	// tree_output->Branch("trk_ecalentry", trk_ecalentry, "trk_ecalentry[3]/F");
    //
	ins_tree->Branch("ccbdt0", &bdt_interaction, "ccbdt0/F");
	ins_tree->Branch("ccbdt1", &bdt_spillover, "ccbdt1/F");
	ins_tree->Branch("ccbdt2", &bdt_combined, "ccbdt2/F");
	// ins_tree->Branch("ccbdtx0", &bdtx_interaction, "ccbdtx0/F");
	// ins_tree->Branch("ccbdtx1", &bdtx_spillover, "ccbdtx1/F");
	ins_tree->Branch("ccbdtx2", &bdtx_combined, "ccbdtx2/F"); // new training  
	ins_tree->Branch("bdt_chargepid", &bdt_chargepid, "bdt_chargepid/F");
    //
	// ins_tree->Branch("eleccbdt", &trk_eleccbdt, "eleccbdt/F");
	// ins_tree->Branch("eleccbdtmdr", &trk_eleccbdtmdr, "eleccbdtmdr/F");
    //
	ins_tree->Branch("trdnhits", &(trd_nhits[0]), "trdnhits/I");
	ins_tree->Branch("trdlkhd", &(trd_klkhd[1][0]), "trdklkhd/F");
	ins_tree->Branch("trdelprob", &(trd_elprob[1]), "trdelprob/F");
	ins_tree->Branch("trdlkhde", &(trd_klkhd[0][0]), "trdklkhde/F"); // energy hypothesis
	ins_tree->Branch("trdlkhde_ehe", &(trd_klkhd[0][1]), "trdklkhde_ehe/F");
	ins_tree->Branch("trdnhits2", &(trd_nhits[2]), "trdnhits2/I"); // trd refit
	ins_tree->Branch("trdlkhde2", &(trd_klkhd[2][0]), "trdklkhde2/F"); // trd refit
	ins_tree->Branch("trdlkhde2_ehe", &(trd_klkhd[2][1]), "trdklkhde2_ehe/F"); // trd refit
    //
	ins_tree->Branch("trdlkhdnew", &(trd_new), "trdlkhdnew/F"); // energy hypothesis with EnergyK
	ins_tree->Branch("trdelprobnew", &(trd_Likelihood_ele), "trdelprobnew/F"); // energy hypothesis with EnergyK
	ins_tree->Branch("trdlkhdnew_lastbin", &(trd_lastbin), "trdlkhdnew_lastbin/F"); // energy hypothesis with EnergyK
	ins_tree->Branch("trdelprobnew_lastbin", &(trd_Likelihood_ele_lastbin), "trdelprobnew_lastbin/F"); // energy hypothesis with EnergyK
    //
	ins_tree->Branch("ecal_nsh", &ecal_nsh, "ecal_nsh/I");
	ins_tree->Branch("ecal_enc", &ecal_enc, "ecal_enc/F");
	ins_tree->Branch("ecal_ene17", &ecal_ene17, "ecal_ene17/F");
	ins_tree->Branch("ecal_bdt", &ecal_bdtp5s, "ecal_bdt/F");
	ins_tree->Branch("ecal_nchi2", &ecal_nchi2, "ecal_nchi2/F");
	ins_tree->Branch("hadflag", &hadflag, "hadflag/I");
	ins_tree->Branch("N_Shwr", &N_Shwr, "N_Shwr/I");
	ins_tree->Branch("Ene", &Ene, "Ene/F");
	ins_tree->Branch("E0", E0, "E0[3]/F");
	ins_tree->Branch("A0", A0, "A0[3]/F");
	// tree_output->Branch("EmVar", EmVar, "EmVar[20]/F");
	ins_tree->Branch("Lkhd0", &Lkhd0, "Lkhd0/F");
	ins_tree->Branch("Lkhd1", &Lkhd1, "Lkhd1/F");
	ins_tree->Branch("EmBDT", &EmBDT, "EmBDT/F");
	ins_tree->Branch("LkhdRatio", &LkhdRatio, "LkhdRatio/F");
	ins_tree->Branch("NLatLeakCell", &NLatLeakCell, "NLatLeakCell/I");
	ins_tree->Branch("LatLeakFrac", &LatLeakFrac, "LatLeakFrac/F");
	ins_tree->Branch("RearLeakFrac", &RearLeakFrac, "RearLeakFrac/F");
	ins_tree->Branch("xecal", xecal, "xecal[2]/F");
	ins_tree->Branch("yecal", yecal, "yecal[2]/F");
	ins_tree->Branch("trkecalmatch", &trkecalmatch, "trkecalmatch/I");
	ins_tree->Branch("trdecalmatch", &trdecalmatch, "trdecalmatch/I");
	ins_tree->Branch("ECAL_IsFocused", &ECAL_IsFocused, "ECAL_IsFocused/I");
	ins_tree->Branch("ECAL_IsContained", &ECAL_IsContained, "ECAL_IsContained/I");
	ins_tree->Branch("inecal", &inecal, "inecal/I");
	ins_tree->Branch("trkecaldx", &trkecaldx, "trkecaldx/F");
	ins_tree->Branch("trkecaldy", &trkecaldy, "trkecaldy/F");
	ins_tree->Branch("trdecaldx", &trdecaldx, "trdecaldx/F");
	ins_tree->Branch("trdecaldy", &trdecaldy, "trdecaldy/F");
	//
	ins_tree->Branch("nvertex_3d", &nvertex_3d, "nvertex_3d/I");
	ins_tree->Branch("nvertex_2d_x", &nvertex_2d_x, "nvertex_2d_x/I");
	ins_tree->Branch("nvertex_2d_y", &nvertex_2d_y, "nvertex_2d_y/I");
}