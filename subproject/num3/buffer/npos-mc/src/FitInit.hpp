_Pragma("once")

#include <iostream>
using namespace std;
#include <vector>
#include "TChain.h"
#include "VarFittree.hpp"

class FitInit:virtual public VarFittree{
    public:
        FitInit():VarFittree(){cout<<"IN FitInit ===== Constructor_FitInit"<<endl;}
        ~FitInit(){}
    public:
		void INIT_CHAIN_SetBranchAddress(TChain*);
		void INIT_TREEARRAY_BuildBrand(const vector<int>&,TTree**&);
        // void INIT_TREEARRAY_BuildBrand(int,TTree**&);
        void INIT_TREEARRAY_BuildBrand(const vector<int>&,int,TTree**&);
};

void FitInit::INIT_CHAIN_SetBranchAddress(TChain* fChain){
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
	// fChain->SetBranchAddress("EventSelectEffType", &EventSelectEffType);
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

void FitInit::INIT_TREEARRAY_BuildBrand(const vector<int>& length,TTree**& ins_treearray){
	//====check
	// if(ins_treearray == nullptr) return;
	// if(length.empty()) return;
	// for(size_t d = 0; d < length.size(); d++){
	// 	if(length[d] <= 0) return;
	// }
	//====run
	vector<int> index(length.size(), 0);
	bool done = false;
	while(!done){
		//====calc dist_total
		int dist_total = 0;
		int dist_pernextdim = 1;
		for(int idim = (int)length.size() - 1; idim >= 0; idim--){
			dist_total += index[idim] * dist_pernextdim;
			dist_pernextdim *= length[idim];
		}
		//====pass dist_total
		INIT_TREEARRAY_BuildBrand(index, dist_total, ins_treearray);
		//====calc index
		for(int idim = (int)length.size() - 1; idim >= 0; idim--){
			index[idim]++;
			if(index[idim] < length[idim]) break;
			index[idim] = 0;
			if(idim == 0) done = true;
		}
	}
}

//==== INIT_TREEARRAY_BuildBrand
//-- cuz the ins_tree need to be reinit in this function, so we can't only transform the pointer(it'll lose the address) but ref
void FitInit::INIT_TREEARRAY_BuildBrand(const vector<int>& index, int i,TTree**& ins_treearray){
	// ins_treearray[i] = new TTree(Form("tfit_latbin%02d", i), "fit result");
	// ins_treearray[i] = new TTree(Form("tfit_latbin%02d", i_latbin), "fit result");
	// ins_treearray[i] = new TTree("tfit", "fit result");
	TString treename= "tfit";
	if(i==0) treename = "tfit";
	else if(index.size()==1) treename = Form("tfit_t%02d", index[0]);
	else if(index.size()==2) treename = Form("tfit_lat%02d_t%02d", index[0], index[1]);
	ins_treearray[i] = new TTree(treename, "fit result");
	// ins_treearray[i]->Branch("exposure", &(fitR.exposure), "exposure/D");
	//ins_treearray[i]->Branch("tbegin", &tbegin, "tbegin/I");
	//-- from now
	ins_treearray[i]->Branch("Eid", &(fitR.Eid), "Eid/I");
	ins_treearray[i]->Branch("Elow", &(fitR.Elow), "Elow/D");
	ins_treearray[i]->Branch("Eup", &(fitR.Eup), "Eup/D");
	ins_treearray[i]->Branch("tid", &(fitR.tid), "tid/I");
	ins_treearray[i]->Branch("tlow", &(fitR.tlow), "tlow/i");
	ins_treearray[i]->Branch("tmid", &(fitR.tmid), "tmid/i");
	ins_treearray[i]->Branch("tup", &(fitR.tup), "tup/i");
	ins_treearray[i]->Branch("npos", &(fitR.npos), "npos/D");
	ins_treearray[i]->Branch("npos_err", &(fitR.npos_err), "npos_err/D");
	ins_treearray[i]->Branch("npro", &(fitR.npro), "npro/D");
	ins_treearray[i]->Branch("npro_err", &(fitR.npro_err), "npro_err/D");
	ins_treearray[i]->Branch("nele", &(fitR.nele), "nele/D");
	ins_treearray[i]->Branch("nele_err", &(fitR.nele_err), "nele_err/D");
	ins_treearray[i]->Branch("nccpro", &(fitR.nccpro), "nccpro/D");
	ins_treearray[i]->Branch("nccpro_err", &(fitR.nccpro_err), "nccpro_err/D");
	ins_treearray[i]->Branch("chi2pos", &(fitR.chi2pos), "chi2pos/D");
	ins_treearray[i]->Branch("ndfpos", &(fitR.ndfpos), "ndfpos/D");
	ins_treearray[i]->Branch("chi2neg", &(fitR.chi2neg), "chi2neg/D");
	ins_treearray[i]->Branch("ndfneg", &(fitR.ndfneg), "ndfneg/D");
	//==== trigger eff
	ins_treearray[i]->Branch("nelephy", &(fitR.nelephy), "nelephy/D");
	ins_treearray[i]->Branch("nelephy_err", &(fitR.nelephy_err), "nelephy_err/D");
	ins_treearray[i]->Branch("neleunphy", &(fitR.neleunphy), "neleunphy/D");
	ins_treearray[i]->Branch("neleunphy_err", &(fitR.neleunphy_err), "neleunphy_err/D");
	ins_treearray[i]->Branch("trigeff", &(fitR.trigeff), "trigeff/D");
	ins_treearray[i]->Branch("trigeff_err", &(fitR.trigeff_err), "trigeff_err/D");
	//==== daily lf
	ins_treearray[i]->Branch("lf", &(fitR.lf), "lf/F");
	// ins_treearray[i]->Branch("lfc", &(fitR.lfc), "lfc/F");
	//==== Entries
	ins_treearray[i]->Branch("n_NegaSample", &(fitR.n_NegaSample), "n_NegaSample/D");
	ins_treearray[i]->Branch("n_NegaSample_err", &(fitR.n_NegaSample_err), "n_NegaSample_err/D");
	ins_treearray[i]->Branch("n_PosiSample", &(fitR.n_PosiSample), "n_PosiSample/D");
	ins_treearray[i]->Branch("n_PosiSample_err", &(fitR.n_PosiSample_err), "n_PosiSample_err/D");
	//==== sf
	ins_treearray[i]->Branch("sf", &(fitR.sf), "sf/D");
	//----260211
	ins_treearray[i]->Branch("cc", &(fitR.cc), "cc/D");
	ins_treearray[i]->Branch("cc_err", &(fitR.cc_err), "cc_err/D");
	//----260303
	ins_treearray[i]->Branch("posfrac", &(fitR.posfrac), "posfrac/D");
	ins_treearray[i]->Branch("posfrac_err", &(fitR.posfrac_err), "posfrac_err/D");
	//----260407
	//----tof
	ins_treearray[i]->Branch("nTOFpass", &(fitR.nTOFpass), "nTOFpass/D");
	ins_treearray[i]->Branch("nTOFpass_err", &(fitR.nTOFpass_err), "nTOFpass_err/D");
	ins_treearray[i]->Branch("nTOFtotal", &(fitR.nTOFtotal), "nTOFtotal/D");
	ins_treearray[i]->Branch("nTOFtotal_err", &(fitR.nTOFtotal_err), "nTOFtotal_err/D");
	ins_treearray[i]->Branch("tofeff", &(fitR.tofeff), "tofeff/D");
	ins_treearray[i]->Branch("tofeff_err", &(fitR.tofeff_err), "tofeff_err/D");
	//----trd
	ins_treearray[i]->Branch("nTRDpass", &(fitR.nTRDpass), "nTRDpass/D");
	ins_treearray[i]->Branch("nTRDpass_err", &(fitR.nTRDpass_err), "nTRDpass_err/D");
	ins_treearray[i]->Branch("nTRDtotal", &(fitR.nTRDtotal), "nTRDtotal/D");
	ins_treearray[i]->Branch("nTRDtotal_err", &(fitR.nTRDtotal_err), "nTRDtotal_err/D");
	ins_treearray[i]->Branch("trdeff", &(fitR.trdeff), "trdeff/D");
	ins_treearray[i]->Branch("trdeff_err", &(fitR.trdeff_err), "trdeff_err/D");
	//----ecal
	ins_treearray[i]->Branch("nECALpass", &(fitR.nECALpass), "nECALpass/D");
	ins_treearray[i]->Branch("nECALpass_err", &(fitR.nECALpass_err), "nECALpass_err/D");
	ins_treearray[i]->Branch("nECALtotal", &(fitR.nECALtotal), "nECALtotal/D");
	ins_treearray[i]->Branch("nECALtotal_err", &(fitR.nECALtotal_err), "nECALtotal_err/D");
	ins_treearray[i]->Branch("ecaleff", &(fitR.ecaleff), "ecaleff/D");
	ins_treearray[i]->Branch("ecaleff_err", &(fitR.ecaleff_err), "ecaleff_err/D");
	//----trk
	ins_treearray[i]->Branch("nTrkpass", &(fitR.nTrkpass), "nTrkpass/D");
	ins_treearray[i]->Branch("nTrkpass_err", &(fitR.nTrkpass_err), "nTrkpass_err/D");
	ins_treearray[i]->Branch("nTrktotal", &(fitR.nTrktotal), "nTrktotal/D");
	ins_treearray[i]->Branch("nTrktotal_err", &(fitR.nTrktotal_err), "nTrktotal_err/D");
	ins_treearray[i]->Branch("trkeff", &(fitR.trkeff), "trkeff/D");
	ins_treearray[i]->Branch("trkeff_err", &(fitR.trkeff_err), "trkeff_err/D");
}