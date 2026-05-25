#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TSystem.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
using namespace std;

#include "binning.h"

void BuildProcessedRunMap(TString fname, int JobID=0, int NFilesPerJob=0){
	UInt_t info_run;
	//========read
	TChain *fChain = new TChain("t_add");
	// TChain *fChain = new TChain("runlist");
	TString eosprefix = "root://eosams.cern.ch//";
	if(fname.Contains("root")){
		TString fnm=fname;
		if( fnm.EndsWith("root") ){
			if( fnm.BeginsWith("/eos"))
				fnm = eosprefix + fnm;
		}
		cout<<"Add Single Root File: " <<fnm<<endl;
		fChain->Add( fnm );
	}else{
		ifstream fin( fname.Data() );
		TString fnm;
		vector<TString> v_fname;
		while(1){
			fin >> fnm;
			if( fin.eof() ) break;
			if( fnm.BeginsWith("#") ) continue;
			if( fnm.EndsWith("root") ){
				if( fnm.BeginsWith("/eos") )
					fnm = eosprefix + fnm;
				v_fname.push_back(fnm);
			}
		}
		cout<<"=============================================="<<endl;
		cout<<"Select "<<NFilesPerJob<<" Runs, From "<<v_fname.size()<<" Files, JobID: "<< JobID<<endl;
		cout<<"======================="<<endl;
		if( JobID<=0 || NFilesPerJob<=0 ){
			for(int i=0;i<v_fname.size();i++){
				cout<<"Adding File : "<<v_fname.at(i)<<endl;
				fChain->Add( v_fname.at(i) );
			}
		}else{
			for(int i=NFilesPerJob*(JobID-1);i<NFilesPerJob*(JobID);i++){
				if(i<0)break;
				if(i>=v_fname.size())break;
				cout<<"Adding File : "<<v_fname.at(i)<<endl;
				fChain->Add( v_fname.at(i) );
			}
		}
		cout<<"======================="<<endl;
	}
	//========init chain
	fChain->SetBranchStatus("*", 0);
	fChain->SetBranchStatus("info.run", 1);
	fChain->SetBranchAddress("info.run", &info_run);
	// fChain->SetBranchStatus("info_run", 1);
	// fChain->SetBranchAddress("info_run", &info_run);
	//========get value
	set<int> run_list;
	set<int>::iterator run_list_iter;
	for(long entry=0; entry<fChain->GetEntries(); entry++){
		fChain->GetEntry( entry );
		run_list.insert( info_run );
	}
	cout<<"fchain entries = "<<fChain->GetEntries()<<endl;
	cout << "totally " << run_list.size() << " runs processed" << endl;
	//========save
	// TFile *fout = new TFile( Form("%s_%03d.root", gSystem->BaseName( fname ), JobID), "recreate");
	TFile *fout = new TFile( Form("trunlist_%03d.root",JobID), "recreate");
	TTree *tree = new TTree("runlist", "run list");
	tree->Branch("info_run", &info_run, "info_run/i");
	for(run_list_iter = run_list.begin(); run_list_iter != run_list.end(); ++run_list_iter){
		info_run = *run_list_iter;
		tree->Fill();
		// cout << "run = " << *run_list_iter << endl;
	}
	fout->cd();
	tree->Write();
	fout->Close();
	if( fChain ) delete fChain;
}

void ReadMCInfo(TString dir, TString sample, TString stream){
	//========runlist
	cout << "--> load run list ..." << endl;
	set<int> run_list;
	TChain *fChain = new TChain("runlist");
	// fChain->Add( Form("%s.%s.list.root", sample.Data(), stream.Data()) );
	// fChain->Add("MCinfo_new.root");
	// fChain->Add("tskimMC.root");
	// fChain->Add("../1-ACC2-1/data/tskimMC.root");
	// fChain->Add("/afs/cern.ch/work/c/chguan/public/01--251112/04.211--ACC2/tskimMC.root");
	fChain->Add("../1-ACC2-1/data/trunlist.root");
	UInt_t info_run;
	fChain->SetBranchAddress("info_run", &info_run);
	for(long entry=0; entry<fChain->GetEntries(); entry++){
		fChain->GetEntry( entry );
		run_list.insert( info_run );
	}
	cout << "--> load run list OK" << endl;
	//========writeinit
	//====variable
	TString fnm;
	int runid, nfile,nevents,ntrig;
	float emin_input,emax_input,ntrig_input,emin,emax;
	TString s_findex;
	//====file
	// TFile* fout=new TFile("MCjobinfo."+sample+"_"+stream+".root","RECREATE");
	TFile* fout=new TFile("mchgen.root","RECREATE");
	//====tree
	TTree* tree=new TTree("MCjobinfo","MCjobinfo");
	tree->Branch("runid",&runid,"runid/I");
	tree->Branch("emin_input",&emin_input,"emin_input/F");
	tree->Branch("emax_input",&emax_input,"emax_input/F");
	tree->Branch("ntrig_input",&ntrig_input,"ntrig_input/F");
	tree->Branch("emin",&emin,"emin/F");
	tree->Branch("emax",&emax,"emax/F");
	tree->Branch("ntrig",&ntrig,"ntrig/I");
	tree->Branch("nevents",&nevents,"nevents/I");
	tree->Branch("nfile",&nfile,"nfile/I");
	//====hist
	// TH1F *hmcspectra = new TH1F("hmcspectra", "MC spectra", nbin, energy_bins);
	TH1F *hgen = new TH1F("hgen", "hgen", nbin, energy_bins);
	// TH1F *hmcspectra_finebinning = new TH1F("hmcspectra_finebinning", "MC spectra", nbin_fine, energy_bins_fine);
	TH1F *hgen_finebin = new TH1F("hgen_finebin", "hgen_finebin", nbin_fine, energy_bins_fine);
	//========readinit
	//====file
	TString fname = Form("%s/%s/%s.info", dir.Data(), sample.Data(), stream.Data());
	cout << "processing " << fname << endl;
	ifstream fin(fname);
	//========loopreadfile
	int ncount=0;
	double ntrig_tot=0;
	std::string line;
	TString srunid, semin_input, semax_input, sntrig_input, semin, semax, sntrig, snevents, snfile;
	while(std::getline(fin, line)){
		//====getvalue
		if(TString(line).Contains("#"))continue;
		std::istringstream iss(line);
		//iss>>runid>>emin_input>>emax_input>>ntrig_input>>emin>>emax>>ntrig>>nevents>>nfile>>s_findex;
		iss>>srunid>>semin_input>>semax_input>>sntrig_input>>semin>>semax>>sntrig>>snevents>>snfile>>s_findex;
		if( s_findex.Sizeof()<2 ){
			std::istringstream iss_v2(line);
			// emin_input and emax_input columns are not filled for some info files
			iss_v2>>runid>>ntrig_input>>emin>>emax>>ntrig>>nevents>>nfile>>s_findex;
			cout << "!!!! info file format error" << endl;
			exit(-1);
		}
		// if( s_findex.Sizeof()<2 ){ 
		// 	cout << "!!!! info file format error" << endl;
		// 	exit(-1);
		// }
		runid = srunid.Atoi();
		emin = semin.Atof();
		emax = semax.Atof();
		ntrig = sntrig.Atoi();
		nfile = snfile.Atoi();
		if( run_list.count( runid ) < 1 ) continue; // not processed yet
		ncount++;
		// if( ncount>100 ) break; // for debug
		ntrig_tot += ntrig;
		//====fillhist
		// mc spectra, index = -1 
		for(int ix=0; ix<hgen->GetNbinsX(); ix++){
			double xmin = hgen->GetBinLowEdge(ix+1);
			double xmax = hgen->GetBinLowEdge(ix+2);
			if( xmax < emin || xmin > emax ) continue;
			if( xmin >= emin && xmax <= emax ){
				hgen->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(xmax/xmin)/TMath::Log(emax/emin));
			}else if( xmin >= emin && xmin < emax && xmax > emax ){
				hgen->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(emax/xmin)/TMath::Log(emax/emin));
			}else if( xmin < emin && xmax > emin && xmax < emax ){
				hgen->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(xmax/emin)/TMath::Log(emax/emin));
			}
		}
		for(int ix=0; ix<hgen_finebin->GetNbinsX(); ix++){
			double xmin = hgen_finebin->GetBinLowEdge(ix+1);
			double xmax = hgen_finebin->GetBinLowEdge(ix+2);
			if( xmax < emin || xmin > emax ) continue;
			if( xmin >= emin && xmax <= emax ){
				hgen_finebin->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(xmax/xmin)/TMath::Log(emax/emin));
			}else if( xmin >= emin && xmin < emax && xmax > emax ){
				hgen_finebin->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(emax/xmin)/TMath::Log(emax/emin));
			}else if( xmin < emin && xmax > emin && xmax < emax ){
				hgen_finebin->Fill(0.5*(xmin+xmax), ntrig*TMath::Log(xmax/emin)/TMath::Log(emax/emin));
			}
		}
		tree->Fill();
	}
	std::cout << ntrig_tot << std::endl;
	//========save
	tree->Write();
	hgen->Write();
	hgen_finebin->Write();
	fout->Close();
	fin.close();
}

int main(int argc, char *argv[]){
	if( argc < 3 ){
		cout << "./readmcinfo 0 flistname JobID NFilesPerJob" << endl; // build processed run map
		cout << "./readmcinfo 1 sample[like el.B1091] stream[like el.pl1.2002000]" << endl; // read database
		exit(-1);
	}
	int step=atoi(argv[1]);
	//==== first step
	if(step==0){ 
		TString flistname = argv[2];
		int JobID = 0, NFilesPerJob = 0;
		if( argc > 3 ) JobID = atoi(argv[3]);
		if( argc > 4 ) NFilesPerJob = atoi(argv[4]);
		BuildProcessedRunMap( flistname, JobID, NFilesPerJob );
		return 0;
	}
	//==== second step
	else{
		//---- index 验证成功
		TString dir="/afs/cern.ch/ams/Offline/AMSDataDirRW/DataManagement/DataSetsDesc/";
		//---- 
		TString sample = argv[2];
		//---- 
		TString stream = argv[3];
		ReadMCInfo(dir, sample, stream);
	}
	return 0;
}
