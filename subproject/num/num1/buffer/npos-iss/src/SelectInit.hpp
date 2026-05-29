_Pragma("once")

#include <iostream>
using namespace std;
#include "TChain.h"
#include "VarEneBin.hpp"
#include "VarBuffertree.hpp"

class SelectInit:virtual public VarBuffertree{
    public:
        SelectInit():VarBuffertree(){cout<<"Constructor_selectinit"<<endl;}
        ~SelectInit(){}
    public:
		void INIT_CHAIN_SetBranchAddress(TChain*);
        void INIT_TREEARRAY_BuildBrand(int,TTree**&);
};

void SelectInit::INIT_CHAIN_SetBranchAddress(TChain* fChain){
    fChain->SetBranchAddress("run", &run, &b_run);
    fChain->SetBranchAddress("event", &event, &b_event);
    fChain->SetBranchAddress("utime", &utime, &b_utime);
    fChain->SetBranchAddress("lf", &lf, &b_lf);
    // fChain->SetBranchAddress("lfc", &lfc, &b_lfc);
    fChain->SetBranchAddress("TTheta", &TTheta, &b_TTheta);
    fChain->SetBranchAddress("Phi", &Phi, &b_Phi);
    // fChain->SetBranchAddress("Glat", &Glat, &b_Glat);
    // fChain->SetBranchAddress("Glong", &Glong, &b_Glong);
    // fChain->SetBranchAddress("Mlat", &Mlat, &b_Mlat);
    // fChain->SetBranchAddress("Mlong", &Mlong, &b_Mlong);
    // fChain->SetBranchAddress("Gr", &Gr, &b_Gr);
    fChain->SetBranchAddress("cf", cf, &b_cf);
    fChain->SetBranchAddress("cfi", cfi, &b_cfi);
    fChain->SetBranchAddress("poscf", poscf, &b_poscf);
    fChain->SetBranchAddress("negcf", negcf, &b_negcf);
    fChain->SetBranchAddress("poscfi", poscfi, &b_poscfi);
    fChain->SetBranchAddress("negcfi", negcfi, &b_negcfi);
    fChain->SetBranchAddress("mcp", &mcp, &b_mcp);
    fChain->SetBranchAddress("mcq", &mcq, &b_mcq);
    fChain->SetBranchAddress("isbadrun", &isbadrun, &b_isbadrun);
    fChain->SetBranchAddress("isbad", isbad, &b_isbad);
    //----tsu
    fChain->SetBranchAddress("isbadrun_trd", &isbadrun_trd, &b_isbadrun_trd);
    fChain->SetBranchAddress("isbadrun_trk", &isbadrun_trk, &b_isbadrun_trk);
    fChain->SetBranchAddress("isbadrun_ecal", &isbadrun_ecal, &b_isbadrun_ecal);
    fChain->SetBranchAddress("isbadrun_general", &isbadrun_general, &b_isbadrun_general);
    fChain->SetBranchAddress("isbadrun_daq", &isbadrun_daq, &b_isbadrun_daq);
    fChain->SetBranchAddress("isbadrun_fov", &isbadrun_fov, &b_isbadrun_fov);
    //--------
    fChain->SetBranchAddress("lvl1_JMembPatt", &lvl1_JMembPatt, &b_lvl1_JMembPatt);
    fChain->SetBranchAddress("lvl1_PhysBPatt", &lvl1_PhysBPatt, &b_lvl1_PhysBPatt);
    fChain->SetBranchAddress("theta", &theta, &b_theta);
    fChain->SetBranchAddress("beta", &beta, &b_beta);
    fChain->SetBranchAddress("tofq", &tofq, &b_tofq);
    fChain->SetBranchAddress("tofqlow", &tofqlow, &b_tofqlow);
    fChain->SetBranchAddress("tofqup", &tofqup, &b_tofqup);
    fChain->SetBranchAddress("nclus_intime", &nclus_intime, &b_nclus_intime);
    fChain->SetBranchAddress("ntrk", &ntrk, &b_ntrk);
    fChain->SetBranchAddress("pat", &pat, &b_pat);
    fChain->SetBranchAddress("patx", &patx, &b_patx);
    fChain->SetBranchAddress("rig", &rig, &b_rig);
    fChain->SetBranchAddress("qin", &qin, &b_qin);
    fChain->SetBranchAddress("q1", &q1, &b_q1);
    fChain->SetBranchAddress("q9", &q9, &b_q9);
    fChain->SetBranchAddress("chi2x", &chi2x, &b_chi2x);
    fChain->SetBranchAddress("chi2y", &chi2y, &b_chi2y);
    fChain->SetBranchAddress("ccbdt0", &ccbdt0, &b_ccbdt0);
    fChain->SetBranchAddress("ccbdt1", &ccbdt1, &b_ccbdt1);
    fChain->SetBranchAddress("ccbdt2", &ccbdt2, &b_ccbdt2);
    fChain->SetBranchAddress("ccbdtx2", &ccbdtx2, &b_ccbdtx2);
    fChain->SetBranchAddress("bdt_chargepid", &bdt_chargepid, &b_bdt_chargepid);
    fChain->SetBranchAddress("trdnhits", &trdnhits, &b_trdnhits);
    fChain->SetBranchAddress("trdlkhd", &trdlkhd, &b_trdklkhd);
    fChain->SetBranchAddress("trdelprob", &trdelprob, &b_trdelprob);
    fChain->SetBranchAddress("trdlkhde", &trdlkhde, &b_trdklkhde);
    fChain->SetBranchAddress("trdlkhde_ehe", &trdlkhde_ehe, &b_trdklkhde_ehe);
    fChain->SetBranchAddress("trdnhits2", &trdnhits2, &b_trdnhits2);
    fChain->SetBranchAddress("trdlkhde2", &trdlkhde2, &b_trdklkhde2);
    fChain->SetBranchAddress("trdlkhde2_ehe", &trdlkhde2_ehe, &b_trdklkhde2_ehe);
    fChain->SetBranchAddress("trdlkhdnew", &trdlkhdnew, &b_trdlkhdnew);
    fChain->SetBranchAddress("trdelprobnew", &trdelprobnew, &b_trdelprobnew);
    fChain->SetBranchAddress("trdlkhdnew_lastbin", &trdlkhdnew_lastbin, &b_trdlkhdnew_lastbin);
    fChain->SetBranchAddress("trdelprobnew_lastbin", &trdelprobnew_lastbin, &b_trdelprobnew_lastbin);
    fChain->SetBranchAddress("ecal_nsh", &ecal_nsh, &b_ecal_nsh);
    fChain->SetBranchAddress("ecal_enc", &ecal_enc, &b_ecal_enc);
    fChain->SetBranchAddress("ecal_ene17", &ecal_ene17, &b_ecal_ene17);
    fChain->SetBranchAddress("ecal_bdt", &ecal_bdt, &b_ecal_bdt);
    fChain->SetBranchAddress("ecal_nchi2", &ecal_nchi2, &b_ecal_nchi2);
    fChain->SetBranchAddress("hadflag", &hadflag, &b_hadflag);
    fChain->SetBranchAddress("N_Shwr", &N_Shwr, &b_N_Shwr);
    fChain->SetBranchAddress("Ene", &Ene, &b_Ene);
    fChain->SetBranchAddress("E0", E0, &b_E0);
    fChain->SetBranchAddress("A0", A0, &b_A0);
    fChain->SetBranchAddress("Lkhd0", &Lkhd0, &b_Lkhd0);
    fChain->SetBranchAddress("Lkhd1", &Lkhd1, &b_Lkhd1);
    fChain->SetBranchAddress("EmBDT", &EmBDT, &b_EmBDT);
    fChain->SetBranchAddress("LkhdRatio", &LkhdRatio, &b_LkhdRatio);
    fChain->SetBranchAddress("NLatLeakCell", &NLatLeakCell, &b_NLatLeakCell);
    fChain->SetBranchAddress("LatLeakFrac", &LatLeakFrac, &b_LatLeakFrac);
    fChain->SetBranchAddress("RearLeakFrac", &RearLeakFrac, &b_RearLeakFrac);
    fChain->SetBranchAddress("xecal", xecal, &b_xecal);
    fChain->SetBranchAddress("yecal", yecal, &b_yecal);
    fChain->SetBranchAddress("trkecalmatch", &trkecalmatch, &b_trkecalmatch);
    fChain->SetBranchAddress("trdecalmatch", &trdecalmatch, &b_trdecalmatch);
    fChain->SetBranchAddress("ECAL_IsFocused", &ECAL_IsFocused, &b_ECAL_IsFocused);
    fChain->SetBranchAddress("ECAL_IsContained", &ECAL_IsContained, &b_ECAL_IsContained);
    fChain->SetBranchAddress("inecal", &inecal, &b_inecal);
    fChain->SetBranchAddress("trkecaldx", &trkecaldx, &b_trkecaldx);
    fChain->SetBranchAddress("trkecaldy", &trkecaldy, &b_trkecaldy);
    fChain->SetBranchAddress("trdecaldx", &trdecaldx, &b_trdecaldx);
    fChain->SetBranchAddress("trdecaldy", &trdecaldy, &b_trdecaldy);
    fChain->SetBranchAddress("nvertex_3d", &nvertex_3d, &b_nvertex_3d);
    fChain->SetBranchAddress("nvertex_2d_x", &nvertex_2d_x, &b_nvertex_2d_x);
    fChain->SetBranchAddress("nvertex_2d_y", &nvertex_2d_y, &b_nvertex_2d_y);
}


//==== INIT_TREEARRAY_BuildBrand
//-- cuz the ins_tree need to be reinit in this function, so we can't only transform the pointer(it'll lose the address) but ref
//-- 3rd parameter "xx/y" y means the data type
void SelectInit::INIT_TREEARRAY_BuildBrand(int ntree,TTree**& ins_treearray){
    // i: index for energy_bins
    for (int i = 0; i < ntree; i++){
        ins_treearray[i] = new TTree(Form("tbin%02d", i), Form("%g - %g GeV", energy_bins[i], energy_bins[i+1]));
        ins_treearray[i]->Branch("datatype", &datatype, "datatype/I");
        // ins_treearray[i]->Branch("weight", &ins_tempbranch->weight, "weight/F");
        ins_treearray[i]->Branch("amsday", &amsday, "amsday/I");
        ins_treearray[i]->Branch("day", &day, "day/I");
        ins_treearray[i]->Branch("br", &br, "br/I");
        ins_treearray[i]->Branch("dayID", &dayID, "dayID/I");
        ins_treearray[i]->Branch("run", &run, "run/i");
        ins_treearray[i]->Branch("event", &event, "event/i");
        ins_treearray[i]->Branch("utime", &utime, "utime/i");
        ins_treearray[i]->Branch("Ene", &Ene, "Ene/F");
        ins_treearray[i]->Branch("nsh", &ecal_nsh, "nsh/I");
        ins_treearray[i]->Branch("enc", &ecal_enc, "enc/F");
        ins_treearray[i]->Branch("ene17", &ecal_ene17, "ene17/F");
        ins_treearray[i]->Branch("mcp", &mcp, "mcp/F");
        ins_treearray[i]->Branch("rig", &rig, "rig/F");
        // ins_treearray[i]->Branch("eop", &ins_tempbranch->eop, "eop/F"); // Ene/rig
        // ins_treearray[i]->Branch("ecop", &ins_tempbranch->ecop, "ecop/F"); // ecal_enc/rig
        ins_treearray[i]->Branch("sf", &sf, "sf/F"); // energy_bins[bin] / cf[0]
        ins_treearray[i]->Branch("sfi", &sfi, "sfi/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("cf", &cf, "cf[4]/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("cfi", &cfi, "cfi[4]/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("poscfi", &poscfi, "poscfi[4]/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("negcfi", &negcfi, "negcfi[4]/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("TTheta", &TTheta, "TTheta/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("Phi", &Phi, "Phi/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Glat", &Glat, "Glat/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Glong", &Glong, "Glong/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Mlat", &Mlat, "Mlat/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Mlong", &Mlong, "Mlong/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Gr", &Gr, "Gr/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("mlat", &mlat, "mlat/D");
        ins_treearray[i]->Branch("mlon", &mlon, "mlon/D");
        ins_treearray[i]->Branch("rr", &rr, "rr/D");
        //===
        ins_treearray[i]->Branch("trdlkhde", &trdlkhde, "trdlkhde/F");
        ins_treearray[i]->Branch("trdlkhdnew", &trdlkhdnew, "trdlkhdnew/F");
        ins_treearray[i]->Branch("trdelprob", &trdelprob, "trdelprob/F");
        ins_treearray[i]->Branch("trdelprobnew", &trdelprobnew, "trdelprobnew/F");
        // ins_treearray[i]->Branch("trdlkhde_ehe", &ins_tempbranch->trdlkhde_ehe, "trdlkhde_ehe/F");
        ins_treearray[i]->Branch("trdnhits", &trdnhits, "trdnhits/I");
        ins_treearray[i]->Branch("trdnhits2", &trdnhits2, "trdnhits2/I"); // trd standalone refit
        ins_treearray[i]->Branch("trdlkhde2", &trdlkhde2, "trdlkhde2/F"); // trd standalone refit
        ins_treearray[i]->Branch("trdlkhde2_ehe", &trdlkhde2_ehe, "trdlkhde2_ehe/F"); // trd standalone refit
        //===
        ins_treearray[i]->Branch("theta", &theta, "theta/F");
        ins_treearray[i]->Branch("lvl1_PhysBPatt", &lvl1_PhysBPatt, "lvl1_PhysBPatt/I");
        ins_treearray[i]->Branch("ecal_bdt", &ecal_bdt, "ecal_bdt/F");
        ins_treearray[i]->Branch("beta", &beta, "beta/F");
        ins_treearray[i]->Branch("qin", &qin, "qin/F");
        ins_treearray[i]->Branch("tofqup", &tofqup, "tofqup/F");
        ins_treearray[i]->Branch("tofqlow", &tofqlow, "tofqlow/F");
        ins_treearray[i]->Branch("chi2x", &chi2x, "chi2x/F");
        ins_treearray[i]->Branch("chi2y", &chi2y, "chi2y/F");
        ins_treearray[i]->Branch("NShwr", &N_Shwr, "NShwr/I");
        ins_treearray[i]->Branch("hadflag", &hadflag, "hadflag/I");
        ins_treearray[i]->Branch("ntrk", &ntrk, "ntrk/I");
        ins_treearray[i]->Branch("trkecalmatch", &trkecalmatch, "trkecalmatch/I");
        ins_treearray[i]->Branch("pat", &pat, "pat/I");
        ins_treearray[i]->Branch("patx", &patx, "patx/I");
        ins_treearray[i]->Branch("inecal", &inecal, "inecal/I");
        //===
        ins_treearray[i]->Branch("ccbdt2", &ccbdt2, "ccbdt2/F"); // combined
        ins_treearray[i]->Branch("ccbdt0", &ccbdt0, "ccbdt0/F"); // interaction
        ins_treearray[i]->Branch("ccbdtx2", &ccbdtx2, "ccbdtx2/F"); // combined
        ins_treearray[i]->Branch("ccbdt", &ccbdt, "ccbdt/F"); // combined
        ins_treearray[i]->Branch("ccbdtv", &ccbdtv, "ccbdtv/F"); // combined
        ins_treearray[i]->Branch("bdt_chargepid", &bdt_chargepid, "bdt_chargepid/F"); // charge id
        //===
        ins_treearray[i]->Branch("Lkhd0", &Lkhd0, "Lkhd0/F");
        ins_treearray[i]->Branch("LkhdRatio", &LkhdRatio, "LkhdRatio/F");
        ins_treearray[i]->Branch("nvertex_3d", &nvertex_3d, "nvertex_3d/I");
        ins_treearray[i]->Branch("EmBDT", &EmBDT, "EmBDT/F");
        // cout<<i<<endl;
        //==== detect eff type
        ins_treearray[i]->Branch("EventSelectEffType", &EventSelectEffType, "EventSelectEffType/I");
        //==== live time 250227
        ins_treearray[i]->Branch("lf", &lf, "lf/F");
        // ins_treearray[i]->Branch("lfc", &lfc, "lfc/F");
        //---- 260208--badrun
        ins_treearray[i]->Branch("isbadrun", &isbadrun, "isbadrun/I");
        ins_treearray[i]->Branch("isbadrun_trd", &isbadrun_trd, "isbadrun_trd/I");
        ins_treearray[i]->Branch("isbadrun_trk", &isbadrun_trk, "isbadrun_trk/I");
        ins_treearray[i]->Branch("isbadrun_ecal", &isbadrun_ecal, "isbadrun_ecal/I");
        ins_treearray[i]->Branch("isbadrun_general", &isbadrun_general, "isbadrun_general/I");
        ins_treearray[i]->Branch("isbadrun_daq", &isbadrun_daq, "isbadrun_daq/I");
        ins_treearray[i]->Branch("isbadrun_fov", &isbadrun_fov, "isbadrun_fov/I");
    }
}
