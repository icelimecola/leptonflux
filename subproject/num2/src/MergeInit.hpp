_Pragma("once")

#include <iostream>
using namespace std;
#include "TChain.h"
#include "VarEneBin.hpp"
#include "VarBuffertree.hpp"

class MergeInit:virtual public VarBuffertree{
    public:
        MergeInit():VarBuffertree(){cout<<"Constructor_selectinit"<<endl;}
        ~MergeInit(){}
    public:
		void INIT_CHAIN_SetBranchAddress(TChain*);
        void INIT_TREEARRAY_BuildBrand(int,TTree**&,int);
};

void MergeInit::INIT_CHAIN_SetBranchAddress(TChain* fChain){
    fChain->SetBranchAddress("datatype", &datatype);
	fChain->SetBranchAddress("amsday", &amsday);
	fChain->SetBranchAddress("day", &day);
	fChain->SetBranchAddress("br", &br);
	fChain->SetBranchAddress("dayID", &dayID);
	fChain->SetBranchAddress("run", &run);
	fChain->SetBranchAddress("event", &event);
	fChain->SetBranchAddress("utime", &utime);
	fChain->SetBranchAddress("Ene", &Ene);
	fChain->SetBranchAddress("nsh", &ecal_nsh);
	fChain->SetBranchAddress("enc", &ecal_enc);
	fChain->SetBranchAddress("ene17", &ecal_ene17);
	fChain->SetBranchAddress("mcp", &mcp);
	fChain->SetBranchAddress("rig", &rig);
	fChain->SetBranchAddress("sf", &sf); // energy_bins[bin] / cf[0]
	fChain->SetBranchAddress("sfi", &sfi); // energy_bins[bin] / cfi[0], IGRF cutoff

	fChain->SetBranchAddress("trdlkhde", &trdlkhde);
	fChain->SetBranchAddress("trdlkhdnew", &trdlkhdnew);
	fChain->SetBranchAddress("trdelprob", &trdelprob);
	fChain->SetBranchAddress("trdelprobnew", &trdelprobnew);
	fChain->SetBranchAddress("trdnhits", &trdnhits);
	fChain->SetBranchAddress("trdnhits2", &trdnhits2); // trd standalone refit
	fChain->SetBranchAddress("trdlkhde2", &trdlkhde2); // trd standalone refit
	fChain->SetBranchAddress("trdlkhde2_ehe", &trdlkhde2_ehe); // trd standalone refit

	fChain->SetBranchAddress("theta", &theta);
	fChain->SetBranchAddress("lvl1_PhysBPatt", &lvl1_PhysBPatt);
	fChain->SetBranchAddress("ecal_bdt", &ecal_bdt);
	fChain->SetBranchAddress("beta", &beta);
	fChain->SetBranchAddress("qin", &qin);
	fChain->SetBranchAddress("tofqup", &tofqup);
	fChain->SetBranchAddress("tofqlow", &tofqlow);
	fChain->SetBranchAddress("chi2x", &chi2x);
	fChain->SetBranchAddress("chi2y", &chi2y);
	fChain->SetBranchAddress("NShwr", &N_Shwr);
	fChain->SetBranchAddress("hadflag", &hadflag);
	fChain->SetBranchAddress("ntrk", &ntrk);
	fChain->SetBranchAddress("trkecalmatch", &trkecalmatch);
	fChain->SetBranchAddress("pat", &pat);
	fChain->SetBranchAddress("patx", &patx);

	fChain->SetBranchAddress("ccbdt2", &ccbdt2); // combined
	fChain->SetBranchAddress("ccbdt0", &ccbdt0); // interaction
	fChain->SetBranchAddress("ccbdtx2", &ccbdtx2); // combined
	fChain->SetBranchAddress("ccbdt", &ccbdt); // combined
	fChain->SetBranchAddress("ccbdtv", &ccbdtv); // combined
	fChain->SetBranchAddress("bdt_chargepid", &bdt_chargepid); // charge id

	fChain->SetBranchAddress("Lkhd0", &Lkhd0);
	fChain->SetBranchAddress("LkhdRatio", &LkhdRatio);
	fChain->SetBranchAddress("nvertex_3d", &nvertex_3d);
	fChain->SetBranchAddress("EmBDT", &EmBDT);
    fChain->SetBranchAddress("mlat", &mlat);
    fChain->SetBranchAddress("mlon", &mlon);
    fChain->SetBranchAddress("rr", &rr);
	// fChain->SetBranchAddress("Mlong", &Mlong);
	// fChain->SetBranchAddress("Mlat", &Mlat);
	// fChain->SetBranchAddress("Glong", &Glong);
	fChain->SetBranchAddress("TTheta", &TTheta);
	// fChain->SetBranchAddress("Glat", &Glat);
	// fChain->SetBranchAddress("Gr", &Gr);
	//==== detect eff type
	fChain->SetBranchAddress("EventSelectEffType", &EventSelectEffType);
	//==== lf
	fChain->SetBranchAddress("lf", &lf);
	// fChain->SetBranchAddress("lfc", &lfc);
    // cout<<"fitin_transchain"<<endl;
    //==== sf--260124
    fChain->SetBranchAddress("cf", cf, &b_cf);
    fChain->SetBranchAddress("cfi", cfi, &b_cfi);
    fChain->SetBranchAddress("poscfi", poscfi, &b_poscfi);
    //---- 260208--badrun
    fChain->SetBranchAddress("isbadrun", &isbadrun);
    fChain->SetBranchAddress("isbadrun_trd", &isbadrun_trd);
    fChain->SetBranchAddress("isbadrun_trk", &isbadrun_trk);
    fChain->SetBranchAddress("isbadrun_ecal", &isbadrun_ecal);
    fChain->SetBranchAddress("isbadrun_general", &isbadrun_general);
    fChain->SetBranchAddress("isbadrun_daq", &isbadrun_daq);
    fChain->SetBranchAddress("isbadrun_fov", &isbadrun_fov);
}


//==== INIT_TREEARRAY_BuildBrand
//-- cuz the ins_tree need to be reinit in this function, so we can't only transform the pointer(it'll lose the address) but ref
//-- 3rd parameter "xx/y" y means the data type
void MergeInit::INIT_TREEARRAY_BuildBrand(int ntree,TTree**& ins_treearray,int enebin){
    // i: index for energy_bins
    for (int i = 0; i < ntree; i++){
        // ins_treearray[i] = new TTree(Form("tbin%02d", i), Form("%g - %g GeV", energy_bins[i], energy_bins[i+1]));
        ins_treearray[i] = new TTree(Form("tbin%02d", enebin), Form("%g - %g GeV", energy_bins[enebin], energy_bins[enebin+1]));
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
        ins_treearray[i]->Branch("TTheta", &TTheta, "TTheta/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        ins_treearray[i]->Branch("Phi", &Phi, "Phi/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Glat", &Glat, "Glat/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Glong", &Glong, "Glong/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Mlat", &Mlat, "Mlat/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Mlong", &Mlong, "Mlong/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
        // ins_treearray[i]->Branch("Gr", &Gr, "Gr/F"); // energy_bins[bin] / cfi[0], IGRF cutoff
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
        ins_treearray[i]->Branch("mlat", &mlat, "mlat/D");
        ins_treearray[i]->Branch("mlon", &mlon, "mlon/D");
        ins_treearray[i]->Branch("rr", &rr, "rr/D");
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
