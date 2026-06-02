#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TSystem.h"

using namespace std;

int proj_fluxams(int argc, char **argv){
    string input_file = "datain/hflux_ams.root";
    if(argc >= 2) input_file = argv[1];

    string output_file = "dataout/hflux_ams.root";
    if(argc >= 3) output_file = argv[2];

    gSystem->mkdir(gSystem->DirName(output_file.c_str()), true);

    TFile *file_in = new TFile(input_file.c_str(), "read");
    TH2 *h2_flux = dynamic_cast<TH2*>(file_in->Get("hPosFluxDay"));
    if(h2_flux == nullptr){
        cout<<"Error: hPosFluxDay not found in "<<input_file<<endl;
        return 1;
    }
    int nybin = h2_flux->GetNbinsY();

    TFile *file_out = new TFile(output_file.c_str(), "recreate");

    for(int iy = 1; iy <= nybin; ++iy){
        double ylow = h2_flux->GetYaxis()->GetBinLowEdge(iy);
        double yup = h2_flux->GetYaxis()->GetBinUpEdge(iy);

        ostringstream hname;
        hname<<"hfluxt_ene"<<setw(2)<<setfill('0')<<iy;

        TH1 *h1_proj = h2_flux->ProjectionX(hname.str().c_str(), iy, iy);

        ostringstream htitle;
        htitle<<"hPosFluxDay Y = "<<ylow<<" - "<<yup<<" GeV";
        h1_proj->SetTitle(htitle.str().c_str());
        h1_proj->GetXaxis()->SetTitle("X");
        h1_proj->GetYaxis()->SetTitle("Flux");

        h1_proj->Write();
    }

    file_out->Close();
    file_in->Close();

    cout<<"Write "<<nybin<<" histograms to "<<output_file<<endl;
    return 0;
}

#ifndef __CINT__
int main(int argc, char **argv){
    return proj_fluxams(argc, argv);
}
#endif
