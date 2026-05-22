#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"

using namespace std;

bool CMP_FILE_INDEX(const pair<int, string> &left, const pair<int, string> &right){
    return left.first < right.first;
}

struct VarFitR{
    int Eid;
    double Elow, Eup;
    int tid;
    unsigned int tlow, tmid, tup;
    double npos, npos_err;
    double npro, npro_err;
    double nele, nele_err;
    double nccpro, nccpro_err;
    double chi2pos, ndfpos;
    double chi2neg, ndfneg;
    double nelephy, nelephy_err;
    double neleunphy, neleunphy_err;
    double trigeff, trigeff_err;
    float lf;
    double n_NegaSample, n_NegaSample_err;
    double n_PosiSample, n_PosiSample_err;
    double sf;
    double cc, cc_err;
    double posfrac, posfrac_err;
    double nTOFpass, nTOFpass_err;
    double nTOFtotal, nTOFtotal_err;
    double tofeff, tofeff_err;
    double nTRDpass, nTRDpass_err;
    double nTRDtotal, nTRDtotal_err;
    double trdeff, trdeff_err;
    double nECALpass, nECALpass_err;
    double nECALtotal, nECALtotal_err;
    double ecaleff, ecaleff_err;
    double nTrkpass, nTrkpass_err;
    double nTrktotal, nTrktotal_err;
    double trkeff, trkeff_err;
    double nPATpass, nPATpass_err;
    double nPATtotal, nPATtotal_err;
    double pateff, pateff_err;
    double nMATCHpass, nMATCHpass_err;
    double nMATCHtotal, nMATCHtotal_err;
    double matcheff, matcheff_err;
    double nCHI2pass, nCHI2pass_err;
    double nCHI2total, nCHI2total_err;
    double chi2eff, chi2eff_err;
    double nQINpass, nQINpass_err;
    double nQINtotal, nQINtotal_err;
    double qineff, qineff_err;
    double nNTRKpass, nNTRKpass_err;
    double nNTRKtotal, nNTRKtotal_err;
    double ntrkeff, ntrkeff_err;
    double totaleff, totaleff_err;
};

int merge_tfit(int argc, char **argv){
    if(argc < 3){
        cout<<"Usage: "<<argv[0]<<" begin_index end_index [input_dir] [output_file]"<<endl;
        return 1;
    }

    int begin_index = atoi(argv[1]);
    int end_index = atoi(argv[2]);
    if(begin_index > end_index){
        cout<<"Error: begin_index > end_index"<<endl;
        return 1;
    }

    string input_dir = "datain/dataiss/tfit";
    if(argc >= 4) input_dir = argv[3];

    ostringstream output_name_builder;
    output_name_builder
        <<"fitresult_enebin"
        <<setw(2)<<setfill('0')<<begin_index
        <<"to"
        <<setw(2)<<setfill('0')<<end_index
        <<"_merge.root";
    string output_file = output_name_builder.str();
    if(argc >= 5) output_file = argv[4];

    vector<pair<int, string>> selected_files;
    void *input_dir_ptr = gSystem->OpenDirectory(input_dir.c_str());
    if(input_dir_ptr == 0){
        cout<<"Error: failed to open input directory "<<input_dir<<endl;
        return 1;
    }

    const char *entry_name = 0;
    while((entry_name = gSystem->GetDirEntry(input_dir_ptr)) != 0){
        TString file_name = entry_name;
        if(file_name.BeginsWith(".")) continue;
        int file_index = -1;
        if(sscanf(file_name.Data(), "fitresult_enebin%d_", &file_index) != 1) continue;
        if(file_index < begin_index || file_index > end_index) continue;
        selected_files.push_back(make_pair(file_index, input_dir + "/" + string(file_name.Data())));
    }

    sort(selected_files.begin(), selected_files.end(), CMP_FILE_INDEX);

    if(selected_files.empty()){
        cout<<"Error: no input ROOT files found in "<<input_dir<<" for enebin "<<begin_index<<" to "<<end_index<<endl;
        return 1;
    }

    if(static_cast<int>(selected_files.size()) != end_index - begin_index + 1){
        cout<<"Error: input ROOT files are incomplete for enebin "<<begin_index<<" to "<<end_index<<endl;
        for(const auto &item : selected_files){
            cout<<"  found enebin "<<item.first<<" : "<<item.second<<endl;
        }
        return 1;
    }

    vector<TFile*> input_files;
    vector<TTree*> input_trees;
    input_files.reserve(selected_files.size());
    input_trees.reserve(selected_files.size());

    Long64_t nentries = -1;
    for(size_t ifile = 0; ifile < selected_files.size(); ++ifile){
        TFile *input_file = TFile::Open(selected_files[ifile].second.c_str(), "READ");
        if(input_file == nullptr || input_file->IsZombie()){
            cout<<"Error: failed to open "<<selected_files[ifile].second<<endl;
            return 1;
        }

        TTree *tree_in = dynamic_cast<TTree*>(input_file->Get("tfit"));
        if(tree_in == nullptr){
            cout<<"Error: tfit tree not found in "<<selected_files[ifile].second<<endl;
            input_file->Close();
            delete input_file;
            return 1;
        }
        if(tree_in->GetEntries() < 1){
            cout<<"Error: tfit tree is empty in "<<selected_files[ifile].second<<endl;
            input_file->Close();
            delete input_file;
            return 1;
        }

        if(nentries < 0) nentries = tree_in->GetEntries();
        if(tree_in->GetEntries() != nentries){
            cout<<"Error: tfit entry number mismatch in "<<selected_files[ifile].second<<endl;
            cout<<"  expected entries : "<<nentries<<endl;
            cout<<"  current entries  : "<<tree_in->GetEntries()<<endl;
            input_file->Close();
            delete input_file;
            return 1;
        }

        input_files.push_back(input_file);
        input_trees.push_back(tree_in);
    }

    VarFitR fitR_in = {};
    for(size_t ifile = 0; ifile < input_trees.size(); ++ifile){
        input_trees[ifile]->SetBranchAddress("Eid", &(fitR_in.Eid));
        input_trees[ifile]->SetBranchAddress("Elow", &(fitR_in.Elow));
        input_trees[ifile]->SetBranchAddress("Eup", &(fitR_in.Eup));
        input_trees[ifile]->SetBranchAddress("tid", &(fitR_in.tid));
        input_trees[ifile]->SetBranchAddress("tlow", &(fitR_in.tlow));
        input_trees[ifile]->SetBranchAddress("tmid", &(fitR_in.tmid));
        input_trees[ifile]->SetBranchAddress("tup", &(fitR_in.tup));
        input_trees[ifile]->SetBranchAddress("npos", &(fitR_in.npos));
        input_trees[ifile]->SetBranchAddress("npos_err", &(fitR_in.npos_err));
        input_trees[ifile]->SetBranchAddress("npro", &(fitR_in.npro));
        input_trees[ifile]->SetBranchAddress("npro_err", &(fitR_in.npro_err));
        input_trees[ifile]->SetBranchAddress("nele", &(fitR_in.nele));
        input_trees[ifile]->SetBranchAddress("nele_err", &(fitR_in.nele_err));
        input_trees[ifile]->SetBranchAddress("nccpro", &(fitR_in.nccpro));
        input_trees[ifile]->SetBranchAddress("nccpro_err", &(fitR_in.nccpro_err));
        input_trees[ifile]->SetBranchAddress("chi2pos", &(fitR_in.chi2pos));
        input_trees[ifile]->SetBranchAddress("ndfpos", &(fitR_in.ndfpos));
        input_trees[ifile]->SetBranchAddress("chi2neg", &(fitR_in.chi2neg));
        input_trees[ifile]->SetBranchAddress("ndfneg", &(fitR_in.ndfneg));
        input_trees[ifile]->SetBranchAddress("nelephy", &(fitR_in.nelephy));
        input_trees[ifile]->SetBranchAddress("nelephy_err", &(fitR_in.nelephy_err));
        input_trees[ifile]->SetBranchAddress("neleunphy", &(fitR_in.neleunphy));
        input_trees[ifile]->SetBranchAddress("neleunphy_err", &(fitR_in.neleunphy_err));
        input_trees[ifile]->SetBranchAddress("trigeff", &(fitR_in.trigeff));
        input_trees[ifile]->SetBranchAddress("trigeff_err", &(fitR_in.trigeff_err));
        input_trees[ifile]->SetBranchAddress("lf", &(fitR_in.lf));
        input_trees[ifile]->SetBranchAddress("n_NegaSample", &(fitR_in.n_NegaSample));
        input_trees[ifile]->SetBranchAddress("n_NegaSample_err", &(fitR_in.n_NegaSample_err));
        input_trees[ifile]->SetBranchAddress("n_PosiSample", &(fitR_in.n_PosiSample));
        input_trees[ifile]->SetBranchAddress("n_PosiSample_err", &(fitR_in.n_PosiSample_err));
        input_trees[ifile]->SetBranchAddress("sf", &(fitR_in.sf));
        input_trees[ifile]->SetBranchAddress("cc", &(fitR_in.cc));
        input_trees[ifile]->SetBranchAddress("cc_err", &(fitR_in.cc_err));
        input_trees[ifile]->SetBranchAddress("posfrac", &(fitR_in.posfrac));
        input_trees[ifile]->SetBranchAddress("posfrac_err", &(fitR_in.posfrac_err));
        input_trees[ifile]->SetBranchAddress("nTOFpass", &(fitR_in.nTOFpass));
        input_trees[ifile]->SetBranchAddress("nTOFpass_err", &(fitR_in.nTOFpass_err));
        input_trees[ifile]->SetBranchAddress("nTOFtotal", &(fitR_in.nTOFtotal));
        input_trees[ifile]->SetBranchAddress("nTOFtotal_err", &(fitR_in.nTOFtotal_err));
        input_trees[ifile]->SetBranchAddress("tofeff", &(fitR_in.tofeff));
        input_trees[ifile]->SetBranchAddress("tofeff_err", &(fitR_in.tofeff_err));
        input_trees[ifile]->SetBranchAddress("nTRDpass", &(fitR_in.nTRDpass));
        input_trees[ifile]->SetBranchAddress("nTRDpass_err", &(fitR_in.nTRDpass_err));
        input_trees[ifile]->SetBranchAddress("nTRDtotal", &(fitR_in.nTRDtotal));
        input_trees[ifile]->SetBranchAddress("nTRDtotal_err", &(fitR_in.nTRDtotal_err));
        input_trees[ifile]->SetBranchAddress("trdeff", &(fitR_in.trdeff));
        input_trees[ifile]->SetBranchAddress("trdeff_err", &(fitR_in.trdeff_err));
        input_trees[ifile]->SetBranchAddress("nECALpass", &(fitR_in.nECALpass));
        input_trees[ifile]->SetBranchAddress("nECALpass_err", &(fitR_in.nECALpass_err));
        input_trees[ifile]->SetBranchAddress("nECALtotal", &(fitR_in.nECALtotal));
        input_trees[ifile]->SetBranchAddress("nECALtotal_err", &(fitR_in.nECALtotal_err));
        input_trees[ifile]->SetBranchAddress("ecaleff", &(fitR_in.ecaleff));
        input_trees[ifile]->SetBranchAddress("ecaleff_err", &(fitR_in.ecaleff_err));
        input_trees[ifile]->SetBranchAddress("nTrkpass", &(fitR_in.nTrkpass));
        input_trees[ifile]->SetBranchAddress("nTrkpass_err", &(fitR_in.nTrkpass_err));
        input_trees[ifile]->SetBranchAddress("nTrktotal", &(fitR_in.nTrktotal));
        input_trees[ifile]->SetBranchAddress("nTrktotal_err", &(fitR_in.nTrktotal_err));
        input_trees[ifile]->SetBranchAddress("trkeff", &(fitR_in.trkeff));
        input_trees[ifile]->SetBranchAddress("trkeff_err", &(fitR_in.trkeff_err));
        input_trees[ifile]->SetBranchAddress("nPATpass", &(fitR_in.nPATpass));
        input_trees[ifile]->SetBranchAddress("nPATpass_err", &(fitR_in.nPATpass_err));
        input_trees[ifile]->SetBranchAddress("nPATtotal", &(fitR_in.nPATtotal));
        input_trees[ifile]->SetBranchAddress("nPATtotal_err", &(fitR_in.nPATtotal_err));
        input_trees[ifile]->SetBranchAddress("pateff", &(fitR_in.pateff));
        input_trees[ifile]->SetBranchAddress("pateff_err", &(fitR_in.pateff_err));
        input_trees[ifile]->SetBranchAddress("nMATCHpass", &(fitR_in.nMATCHpass));
        input_trees[ifile]->SetBranchAddress("nMATCHpass_err", &(fitR_in.nMATCHpass_err));
        input_trees[ifile]->SetBranchAddress("nMATCHtotal", &(fitR_in.nMATCHtotal));
        input_trees[ifile]->SetBranchAddress("nMATCHtotal_err", &(fitR_in.nMATCHtotal_err));
        input_trees[ifile]->SetBranchAddress("matcheff", &(fitR_in.matcheff));
        input_trees[ifile]->SetBranchAddress("matcheff_err", &(fitR_in.matcheff_err));
        input_trees[ifile]->SetBranchAddress("nCHI2pass", &(fitR_in.nCHI2pass));
        input_trees[ifile]->SetBranchAddress("nCHI2pass_err", &(fitR_in.nCHI2pass_err));
        input_trees[ifile]->SetBranchAddress("nCHI2total", &(fitR_in.nCHI2total));
        input_trees[ifile]->SetBranchAddress("nCHI2total_err", &(fitR_in.nCHI2total_err));
        input_trees[ifile]->SetBranchAddress("chi2eff", &(fitR_in.chi2eff));
        input_trees[ifile]->SetBranchAddress("chi2eff_err", &(fitR_in.chi2eff_err));
        input_trees[ifile]->SetBranchAddress("nQINpass", &(fitR_in.nQINpass));
        input_trees[ifile]->SetBranchAddress("nQINpass_err", &(fitR_in.nQINpass_err));
        input_trees[ifile]->SetBranchAddress("nQINtotal", &(fitR_in.nQINtotal));
        input_trees[ifile]->SetBranchAddress("nQINtotal_err", &(fitR_in.nQINtotal_err));
        input_trees[ifile]->SetBranchAddress("qineff", &(fitR_in.qineff));
        input_trees[ifile]->SetBranchAddress("qineff_err", &(fitR_in.qineff_err));
        input_trees[ifile]->SetBranchAddress("nNTRKpass", &(fitR_in.nNTRKpass));
        input_trees[ifile]->SetBranchAddress("nNTRKpass_err", &(fitR_in.nNTRKpass_err));
        input_trees[ifile]->SetBranchAddress("nNTRKtotal", &(fitR_in.nNTRKtotal));
        input_trees[ifile]->SetBranchAddress("nNTRKtotal_err", &(fitR_in.nNTRKtotal_err));
        input_trees[ifile]->SetBranchAddress("ntrkeff", &(fitR_in.ntrkeff));
        input_trees[ifile]->SetBranchAddress("ntrkeff_err", &(fitR_in.ntrkeff_err));
        input_trees[ifile]->SetBranchAddress("totaleff", &(fitR_in.totaleff));
        input_trees[ifile]->SetBranchAddress("totaleff_err", &(fitR_in.totaleff_err));
    }

    vector<VarFitR> fitR_outs(static_cast<size_t>(nentries));
    for(Long64_t ientry = 0; ientry < nentries; ++ientry){
        VarFitR &fitR_out = fitR_outs[static_cast<size_t>(ientry)];

        for(size_t ifile = 0; ifile < input_trees.size(); ++ifile){
            input_trees[ifile]->GetEntry(ientry);

            if(ifile == 0){
                fitR_out.Eid = fitR_in.Eid;
                fitR_out.Elow = fitR_in.Elow;
                fitR_out.tid = fitR_in.tid;
                fitR_out.tlow = fitR_in.tlow;
                fitR_out.tmid = fitR_in.tmid;
                fitR_out.tup = fitR_in.tup;
            }
            if(ifile == input_trees.size() - 1){
                fitR_out.Eup = fitR_in.Eup;
            }

            fitR_out.npos += fitR_in.npos;
            fitR_out.npro += fitR_in.npro;
            fitR_out.nele += fitR_in.nele;
            fitR_out.nccpro += fitR_in.nccpro;
            fitR_out.nelephy += fitR_in.nelephy;
            fitR_out.neleunphy += fitR_in.neleunphy;
            fitR_out.n_NegaSample += fitR_in.n_NegaSample;
            fitR_out.n_PosiSample += fitR_in.n_PosiSample;
            fitR_out.nTOFpass += fitR_in.nTOFpass;
            fitR_out.nTOFtotal += fitR_in.nTOFtotal;
            fitR_out.nTRDpass += fitR_in.nTRDpass;
            fitR_out.nTRDtotal += fitR_in.nTRDtotal;
            fitR_out.nECALpass += fitR_in.nECALpass;
            fitR_out.nECALtotal += fitR_in.nECALtotal;
            fitR_out.nTrkpass += fitR_in.nTrkpass;
            fitR_out.nTrktotal += fitR_in.nTrktotal;
            fitR_out.nPATpass += fitR_in.nPATpass;
            fitR_out.nPATtotal += fitR_in.nPATtotal;
            fitR_out.nMATCHpass += fitR_in.nMATCHpass;
            fitR_out.nMATCHtotal += fitR_in.nMATCHtotal;
            fitR_out.nCHI2pass += fitR_in.nCHI2pass;
            fitR_out.nCHI2total += fitR_in.nCHI2total;
            fitR_out.nQINpass += fitR_in.nQINpass;
            fitR_out.nQINtotal += fitR_in.nQINtotal;
            fitR_out.nNTRKpass += fitR_in.nNTRKpass;
            fitR_out.nNTRKtotal += fitR_in.nNTRKtotal;
        }

        fitR_out.npos_err = sqrt(fitR_out.npos);
        fitR_out.npro_err = sqrt(fitR_out.npro);
        fitR_out.nele_err = sqrt(fitR_out.nele);
        fitR_out.nccpro_err = sqrt(fitR_out.nccpro);
        fitR_out.nelephy_err = sqrt(fitR_out.nelephy);
        fitR_out.neleunphy_err = sqrt(fitR_out.neleunphy);
        fitR_out.n_NegaSample_err = sqrt(fitR_out.n_NegaSample);
        fitR_out.n_PosiSample_err = sqrt(fitR_out.n_PosiSample);
        fitR_out.nTOFpass_err = sqrt(fitR_out.nTOFpass);
        fitR_out.nTOFtotal_err = sqrt(fitR_out.nTOFtotal);
        fitR_out.nTRDpass_err = sqrt(fitR_out.nTRDpass);
        fitR_out.nTRDtotal_err = sqrt(fitR_out.nTRDtotal);
        fitR_out.nECALpass_err = sqrt(fitR_out.nECALpass);
        fitR_out.nECALtotal_err = sqrt(fitR_out.nECALtotal);
        fitR_out.nTrkpass_err = sqrt(fitR_out.nTrkpass);
        fitR_out.nTrktotal_err = sqrt(fitR_out.nTrktotal);
        fitR_out.nPATpass_err = sqrt(fitR_out.nPATpass);
        fitR_out.nPATtotal_err = sqrt(fitR_out.nPATtotal);
        fitR_out.nMATCHpass_err = sqrt(fitR_out.nMATCHpass);
        fitR_out.nMATCHtotal_err = sqrt(fitR_out.nMATCHtotal);
        fitR_out.nCHI2pass_err = sqrt(fitR_out.nCHI2pass);
        fitR_out.nCHI2total_err = sqrt(fitR_out.nCHI2total);
        fitR_out.nQINpass_err = sqrt(fitR_out.nQINpass);
        fitR_out.nQINtotal_err = sqrt(fitR_out.nQINtotal);
        fitR_out.nNTRKpass_err = sqrt(fitR_out.nNTRKpass);
        fitR_out.nNTRKtotal_err = sqrt(fitR_out.nNTRKtotal);

        fitR_out.chi2pos = 0.0;
        fitR_out.ndfpos = 0.0;
        fitR_out.chi2neg = 0.0;
        fitR_out.ndfneg = 0.0;
        fitR_out.lf = 0.0f;
        fitR_out.sf = 0.0;
        fitR_out.cc = 0.0;
        fitR_out.cc_err = 0.0;
        fitR_out.posfrac = 0.0;
        fitR_out.posfrac_err = 0.0;

        fitR_out.trigeff = 1.0;
        fitR_out.trigeff_err = 0.0;
        if(fitR_out.neleunphy >= 1.0 && fitR_out.nelephy > 0.0){
            fitR_out.trigeff = fitR_out.nelephy / (fitR_out.nelephy + fitR_out.neleunphy);
            fitR_out.trigeff_err =
                sqrt(
                    pow(fitR_out.neleunphy_err / fitR_out.neleunphy, 2) +
                    pow(fitR_out.nelephy_err / fitR_out.nelephy, 2)
                ) * fitR_out.trigeff * (1.0 - fitR_out.trigeff);
        }

        fitR_out.tofeff = 0.0;
        fitR_out.tofeff_err = 0.0;
        if(fitR_out.nTOFtotal > 0.0){
            fitR_out.tofeff = fitR_out.nTOFpass / fitR_out.nTOFtotal;
            fitR_out.tofeff_err = sqrt(fitR_out.tofeff * (1.0 - fitR_out.tofeff) / fitR_out.nTOFtotal);
        }

        fitR_out.trdeff = 0.0;
        fitR_out.trdeff_err = 0.0;
        if(fitR_out.nTRDtotal > 0.0){
            fitR_out.trdeff = fitR_out.nTRDpass / fitR_out.nTRDtotal;
            fitR_out.trdeff_err = sqrt(fitR_out.trdeff * (1.0 - fitR_out.trdeff) / fitR_out.nTRDtotal);
        }

        fitR_out.ecaleff = 0.0;
        fitR_out.ecaleff_err = 0.0;
        if(fitR_out.nECALtotal > 0.0){
            fitR_out.ecaleff = fitR_out.nECALpass / fitR_out.nECALtotal;
            fitR_out.ecaleff_err = sqrt(fitR_out.ecaleff * (1.0 - fitR_out.ecaleff) / fitR_out.nECALtotal);
        }

        fitR_out.trkeff = 0.0;
        fitR_out.trkeff_err = 0.0;
        if(fitR_out.nTrktotal > 0.0){
            fitR_out.trkeff = fitR_out.nTrkpass / fitR_out.nTrktotal;
            fitR_out.trkeff_err = sqrt(fitR_out.trkeff * (1.0 - fitR_out.trkeff) / fitR_out.nTrktotal);
        }

        fitR_out.pateff = 0.0;
        fitR_out.pateff_err = 0.0;
        if(fitR_out.nPATtotal > 0.0){
            fitR_out.pateff = fitR_out.nPATpass / fitR_out.nPATtotal;
            fitR_out.pateff_err = sqrt(fitR_out.pateff * (1.0 - fitR_out.pateff) / fitR_out.nPATtotal);
        }

        fitR_out.matcheff = 0.0;
        fitR_out.matcheff_err = 0.0;
        if(fitR_out.nMATCHtotal > 0.0){
            fitR_out.matcheff = fitR_out.nMATCHpass / fitR_out.nMATCHtotal;
            fitR_out.matcheff_err = sqrt(fitR_out.matcheff * (1.0 - fitR_out.matcheff) / fitR_out.nMATCHtotal);
        }

        fitR_out.chi2eff = 0.0;
        fitR_out.chi2eff_err = 0.0;
        if(fitR_out.nCHI2total > 0.0){
            fitR_out.chi2eff = fitR_out.nCHI2pass / fitR_out.nCHI2total;
            fitR_out.chi2eff_err = sqrt(fitR_out.chi2eff * (1.0 - fitR_out.chi2eff) / fitR_out.nCHI2total);
        }

        fitR_out.qineff = 0.0;
        fitR_out.qineff_err = 0.0;
        if(fitR_out.nQINtotal > 0.0){
            fitR_out.qineff = fitR_out.nQINpass / fitR_out.nQINtotal;
            fitR_out.qineff_err = sqrt(fitR_out.qineff * (1.0 - fitR_out.qineff) / fitR_out.nQINtotal);
        }

        fitR_out.ntrkeff = 0.0;
        fitR_out.ntrkeff_err = 0.0;
        if(fitR_out.nNTRKtotal > 0.0){
            fitR_out.ntrkeff = fitR_out.nNTRKpass / fitR_out.nNTRKtotal;
            fitR_out.ntrkeff_err = sqrt(fitR_out.ntrkeff * (1.0 - fitR_out.ntrkeff) / fitR_out.nNTRKtotal);
        }

        fitR_out.totaleff =
            fitR_out.tofeff *
            fitR_out.trdeff *
            fitR_out.ecaleff *
            fitR_out.pateff *
            fitR_out.matcheff *
            fitR_out.chi2eff *
            fitR_out.qineff *
            fitR_out.ntrkeff;
        fitR_out.totaleff_err = 0.0;
        if(fitR_out.totaleff > 0.0){
            double totaleff_err2 = 0.0;
            if(fitR_out.tofeff > 0.0) totaleff_err2 += pow(fitR_out.tofeff_err / fitR_out.tofeff, 2);
            if(fitR_out.trdeff > 0.0) totaleff_err2 += pow(fitR_out.trdeff_err / fitR_out.trdeff, 2);
            if(fitR_out.ecaleff > 0.0) totaleff_err2 += pow(fitR_out.ecaleff_err / fitR_out.ecaleff, 2);
            if(fitR_out.pateff > 0.0) totaleff_err2 += pow(fitR_out.pateff_err / fitR_out.pateff, 2);
            if(fitR_out.matcheff > 0.0) totaleff_err2 += pow(fitR_out.matcheff_err / fitR_out.matcheff, 2);
            if(fitR_out.chi2eff > 0.0) totaleff_err2 += pow(fitR_out.chi2eff_err / fitR_out.chi2eff, 2);
            if(fitR_out.qineff > 0.0) totaleff_err2 += pow(fitR_out.qineff_err / fitR_out.qineff, 2);
            if(fitR_out.ntrkeff > 0.0) totaleff_err2 += pow(fitR_out.ntrkeff_err / fitR_out.ntrkeff, 2);
            fitR_out.totaleff_err = fitR_out.totaleff * sqrt(totaleff_err2);
        }
    }

    TFile *output_root = TFile::Open(output_file.c_str(), "RECREATE");
    if(output_root == nullptr || output_root->IsZombie()){
        cout<<"Error: failed to create output ROOT file "<<output_file<<endl;
        return 1;
    }

    VarFitR fitR_out = {};
    TTree *tree_out = new TTree("tfit", "fit result");
    tree_out->Branch("Eid", &(fitR_out.Eid), "Eid/I");
    tree_out->Branch("Elow", &(fitR_out.Elow), "Elow/D");
    tree_out->Branch("Eup", &(fitR_out.Eup), "Eup/D");
    tree_out->Branch("tid", &(fitR_out.tid), "tid/I");
    tree_out->Branch("tlow", &(fitR_out.tlow), "tlow/i");
    tree_out->Branch("tmid", &(fitR_out.tmid), "tmid/i");
    tree_out->Branch("tup", &(fitR_out.tup), "tup/i");
    tree_out->Branch("npos", &(fitR_out.npos), "npos/D");
    tree_out->Branch("npos_err", &(fitR_out.npos_err), "npos_err/D");
    tree_out->Branch("npro", &(fitR_out.npro), "npro/D");
    tree_out->Branch("npro_err", &(fitR_out.npro_err), "npro_err/D");
    tree_out->Branch("nele", &(fitR_out.nele), "nele/D");
    tree_out->Branch("nele_err", &(fitR_out.nele_err), "nele_err/D");
    tree_out->Branch("nccpro", &(fitR_out.nccpro), "nccpro/D");
    tree_out->Branch("nccpro_err", &(fitR_out.nccpro_err), "nccpro_err/D");
    tree_out->Branch("chi2pos", &(fitR_out.chi2pos), "chi2pos/D");
    tree_out->Branch("ndfpos", &(fitR_out.ndfpos), "ndfpos/D");
    tree_out->Branch("chi2neg", &(fitR_out.chi2neg), "chi2neg/D");
    tree_out->Branch("ndfneg", &(fitR_out.ndfneg), "ndfneg/D");
    tree_out->Branch("nelephy", &(fitR_out.nelephy), "nelephy/D");
    tree_out->Branch("nelephy_err", &(fitR_out.nelephy_err), "nelephy_err/D");
    tree_out->Branch("neleunphy", &(fitR_out.neleunphy), "neleunphy/D");
    tree_out->Branch("neleunphy_err", &(fitR_out.neleunphy_err), "neleunphy_err/D");
    tree_out->Branch("trigeff", &(fitR_out.trigeff), "trigeff/D");
    tree_out->Branch("trigeff_err", &(fitR_out.trigeff_err), "trigeff_err/D");
    tree_out->Branch("lf", &(fitR_out.lf), "lf/F");
    tree_out->Branch("n_NegaSample", &(fitR_out.n_NegaSample), "n_NegaSample/D");
    tree_out->Branch("n_NegaSample_err", &(fitR_out.n_NegaSample_err), "n_NegaSample_err/D");
    tree_out->Branch("n_PosiSample", &(fitR_out.n_PosiSample), "n_PosiSample/D");
    tree_out->Branch("n_PosiSample_err", &(fitR_out.n_PosiSample_err), "n_PosiSample_err/D");
    tree_out->Branch("sf", &(fitR_out.sf), "sf/D");
    tree_out->Branch("cc", &(fitR_out.cc), "cc/D");
    tree_out->Branch("cc_err", &(fitR_out.cc_err), "cc_err/D");
    tree_out->Branch("posfrac", &(fitR_out.posfrac), "posfrac/D");
    tree_out->Branch("posfrac_err", &(fitR_out.posfrac_err), "posfrac_err/D");
    tree_out->Branch("nTOFpass", &(fitR_out.nTOFpass), "nTOFpass/D");
    tree_out->Branch("nTOFpass_err", &(fitR_out.nTOFpass_err), "nTOFpass_err/D");
    tree_out->Branch("nTOFtotal", &(fitR_out.nTOFtotal), "nTOFtotal/D");
    tree_out->Branch("nTOFtotal_err", &(fitR_out.nTOFtotal_err), "nTOFtotal_err/D");
    tree_out->Branch("tofeff", &(fitR_out.tofeff), "tofeff/D");
    tree_out->Branch("tofeff_err", &(fitR_out.tofeff_err), "tofeff_err/D");
    tree_out->Branch("nTRDpass", &(fitR_out.nTRDpass), "nTRDpass/D");
    tree_out->Branch("nTRDpass_err", &(fitR_out.nTRDpass_err), "nTRDpass_err/D");
    tree_out->Branch("nTRDtotal", &(fitR_out.nTRDtotal), "nTRDtotal/D");
    tree_out->Branch("nTRDtotal_err", &(fitR_out.nTRDtotal_err), "nTRDtotal_err/D");
    tree_out->Branch("trdeff", &(fitR_out.trdeff), "trdeff/D");
    tree_out->Branch("trdeff_err", &(fitR_out.trdeff_err), "trdeff_err/D");
    tree_out->Branch("nECALpass", &(fitR_out.nECALpass), "nECALpass/D");
    tree_out->Branch("nECALpass_err", &(fitR_out.nECALpass_err), "nECALpass_err/D");
    tree_out->Branch("nECALtotal", &(fitR_out.nECALtotal), "nECALtotal/D");
    tree_out->Branch("nECALtotal_err", &(fitR_out.nECALtotal_err), "nECALtotal_err/D");
    tree_out->Branch("ecaleff", &(fitR_out.ecaleff), "ecaleff/D");
    tree_out->Branch("ecaleff_err", &(fitR_out.ecaleff_err), "ecaleff_err/D");
    tree_out->Branch("nTrkpass", &(fitR_out.nTrkpass), "nTrkpass/D");
    tree_out->Branch("nTrkpass_err", &(fitR_out.nTrkpass_err), "nTrkpass_err/D");
    tree_out->Branch("nTrktotal", &(fitR_out.nTrktotal), "nTrktotal/D");
    tree_out->Branch("nTrktotal_err", &(fitR_out.nTrktotal_err), "nTrktotal_err/D");
    tree_out->Branch("trkeff", &(fitR_out.trkeff), "trkeff/D");
    tree_out->Branch("trkeff_err", &(fitR_out.trkeff_err), "trkeff_err/D");
    tree_out->Branch("nPATpass", &(fitR_out.nPATpass), "nPATpass/D");
    tree_out->Branch("nPATpass_err", &(fitR_out.nPATpass_err), "nPATpass_err/D");
    tree_out->Branch("nPATtotal", &(fitR_out.nPATtotal), "nPATtotal/D");
    tree_out->Branch("nPATtotal_err", &(fitR_out.nPATtotal_err), "nPATtotal_err/D");
    tree_out->Branch("pateff", &(fitR_out.pateff), "pateff/D");
    tree_out->Branch("pateff_err", &(fitR_out.pateff_err), "pateff_err/D");
    tree_out->Branch("nMATCHpass", &(fitR_out.nMATCHpass), "nMATCHpass/D");
    tree_out->Branch("nMATCHpass_err", &(fitR_out.nMATCHpass_err), "nMATCHpass_err/D");
    tree_out->Branch("nMATCHtotal", &(fitR_out.nMATCHtotal), "nMATCHtotal/D");
    tree_out->Branch("nMATCHtotal_err", &(fitR_out.nMATCHtotal_err), "nMATCHtotal_err/D");
    tree_out->Branch("matcheff", &(fitR_out.matcheff), "matcheff/D");
    tree_out->Branch("matcheff_err", &(fitR_out.matcheff_err), "matcheff_err/D");
    tree_out->Branch("nCHI2pass", &(fitR_out.nCHI2pass), "nCHI2pass/D");
    tree_out->Branch("nCHI2pass_err", &(fitR_out.nCHI2pass_err), "nCHI2pass_err/D");
    tree_out->Branch("nCHI2total", &(fitR_out.nCHI2total), "nCHI2total/D");
    tree_out->Branch("nCHI2total_err", &(fitR_out.nCHI2total_err), "nCHI2total_err/D");
    tree_out->Branch("chi2eff", &(fitR_out.chi2eff), "chi2eff/D");
    tree_out->Branch("chi2eff_err", &(fitR_out.chi2eff_err), "chi2eff_err/D");
    tree_out->Branch("nQINpass", &(fitR_out.nQINpass), "nQINpass/D");
    tree_out->Branch("nQINpass_err", &(fitR_out.nQINpass_err), "nQINpass_err/D");
    tree_out->Branch("nQINtotal", &(fitR_out.nQINtotal), "nQINtotal/D");
    tree_out->Branch("nQINtotal_err", &(fitR_out.nQINtotal_err), "nQINtotal_err/D");
    tree_out->Branch("qineff", &(fitR_out.qineff), "qineff/D");
    tree_out->Branch("qineff_err", &(fitR_out.qineff_err), "qineff_err/D");
    tree_out->Branch("nNTRKpass", &(fitR_out.nNTRKpass), "nNTRKpass/D");
    tree_out->Branch("nNTRKpass_err", &(fitR_out.nNTRKpass_err), "nNTRKpass_err/D");
    tree_out->Branch("nNTRKtotal", &(fitR_out.nNTRKtotal), "nNTRKtotal/D");
    tree_out->Branch("nNTRKtotal_err", &(fitR_out.nNTRKtotal_err), "nNTRKtotal_err/D");
    tree_out->Branch("ntrkeff", &(fitR_out.ntrkeff), "ntrkeff/D");
    tree_out->Branch("ntrkeff_err", &(fitR_out.ntrkeff_err), "ntrkeff_err/D");
    tree_out->Branch("totaleff", &(fitR_out.totaleff), "totaleff/D");
    tree_out->Branch("totaleff_err", &(fitR_out.totaleff_err), "totaleff_err/D");

    for(Long64_t ientry = 0; ientry < nentries; ++ientry){
        fitR_out = fitR_outs[static_cast<size_t>(ientry)];
        tree_out->Fill();
    }

    output_root->cd();
    tree_out->Write();
    output_root->Close();
    delete output_root;

    for(size_t ifile = 0; ifile < input_files.size(); ++ifile){
        input_files[ifile]->Close();
        delete input_files[ifile];
    }

    cout<<"Output ROOT file : "<<output_file<<endl;
    cout<<"Merged enebin     : "<<begin_index<<" to "<<end_index<<endl;
    cout<<"Input ROOT files  : "<<selected_files.size()<<endl;
    cout<<"Output entries    : "<<nentries<<endl;
    if(nentries > 0){
        cout<<"Energy range      : "<<fitR_outs[0].Elow<<" -> "<<fitR_outs[0].Eup<<" GeV"<<endl;
        cout<<"entry0 trigeff    : "<<fitR_outs[0].trigeff<<" +/- "<<fitR_outs[0].trigeff_err<<endl;
        cout<<"entry0 matcheff   : "<<fitR_outs[0].matcheff<<" +/- "<<fitR_outs[0].matcheff_err<<endl;
        cout<<"entry0 totaleff   : "<<fitR_outs[0].totaleff<<" +/- "<<fitR_outs[0].totaleff_err<<endl;
    }

    return 0;
}

int main(int argc, char **argv){
    return merge_tfit(argc, argv);
}
