#include <iostream>
using namespace std;

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TKey.h"
#include "TString.h"
#include "TSystem.h"

//====================================================
static const int NENEBIN = 54;
static const double ENERGY_BINS[NENEBIN + 1] = {
    0.80, 1, 1.16, 1.33, 1.51,
    1.71, 1.92, 2.15, 2.40, 2.67,
    2.97, 3.29, 3.64, 4.02, 4.43,
    4.88, 5.37, 5.90, 6.47, 7.09,
    7.76, 8.48, 9.26, 10.10, 11,
    12, 13, 14.10, 15.30, 16.60,
    18, 19.50, 21.10, 22.80, 24.70,
    26.70, 28.80, 31.10, 33.50, 36.10,
    38.90, 41.90, 45.10, 48.50, 52.20,
    56.10, 60.30, 64.80, 69.70, 74.90,
    80.50, 86.50, 93, 100, 108
};

struct FluxConf{
    TString datadir;
    TString species;
    TString foutname;
    TString exps_mode;
    TString exps_sf;
    bool has_xmin;
    bool has_xmax;
    double xmin;
    double xmax;
    bool has_ymin;
    bool has_ymax;
    double ymin;
    double ymax;
};

struct EneValue{
    double value;
    double error;
};


//==================================================== TOOL_
TString TOOL_FormatE(double x){
    return Form("%g", x);
}

TString TOOL_GetNFileName(const FluxConf &conf, int i_enebin){
    return Form(
        "%s/%s/htime_ene%02d_%sGeV_%s.root",
        conf.datadir.Data(),
        conf.species.Data(),
        i_enebin,
        TOOL_FormatE(ENERGY_BINS[i_enebin]).Data(),
        conf.species.Data()
    );
}

TString TOOL_GetExpsHistName(const FluxConf &conf, int i_enebin){
    return Form(
        "h1exp_%s_T_fov25_sf%s_ene%sto%sGeV",
        conf.exps_mode.Data(),
        conf.exps_sf.Data(),
        TOOL_FormatE(ENERGY_BINS[i_enebin]).Data(),
        TOOL_FormatE(ENERGY_BINS[i_enebin + 1]).Data()
    );
}

bool TOOL_IsSupportedSpecies(const TString &species){
    return species == "npos" || species == "nele";
}

bool TOOL_IsPositive(double value){
    return value > 0.0;
}

double TOOL_CalcRelErr2(double value, double error){
    if(value <= 0.0) return 0.0;
    return pow(error / value, 2);
}

TString TOOL_GetOutBase(const TString &foutname){
    TString outbase = foutname;
    if(outbase.EndsWith(".root")) outbase.Resize(outbase.Length() - 5);
    return outbase;
}


//==================================================== READ_
bool READ_OpenFile(const TString &fpathname, unique_ptr<TFile> &fptr){
    fptr.reset(TFile::Open(fpathname, "READ"));
    if(fptr.get() == nullptr || fptr->IsZombie()){
        cerr<<"ERR READ_OpenFile ===== failed to open "<<fpathname<<endl;
        return false;
    }
    cout<<"IN READ_OpenFile ===== "<<fpathname<<endl;
    return true;
}

TH1 *READ_GetHist(TFile *f, const TString &hname){
    if(f == nullptr) return nullptr;
    TH1 *h = dynamic_cast<TH1*>(f->Get(hname));
    if(h == nullptr){
        cerr<<"ERR READ_GetHist ===== missing hist "<<hname
            <<" in "<<f->GetName()
            <<endl;
        return nullptr;
    }
    return h;
}

bool READ_GetEneValue(TFile *f, const TString &hname, double energy, EneValue &out){
    TH1 *h = READ_GetHist(f, hname);
    if(h == nullptr) return false;
    int ibin = h->FindBin(energy);
    out.value = h->GetBinContent(ibin);
    out.error = h->GetBinError(ibin);
    return true;
}

bool READ_CheckTimeShape(TH1 *h_num, TH1 *h_exps, int i_enebin){
    if(h_num == nullptr || h_exps == nullptr) return false;
    if(h_num->GetNbinsX() != h_exps->GetNbinsX()){
        cerr<<"ERR READ_CheckTimeShape ===== nbin mismatch"
            <<" i_enebin="<<i_enebin
            <<" n_num="<<h_num->GetNbinsX()
            <<" n_exps="<<h_exps->GetNbinsX()
            <<endl;
        return false;
    }
    double eps = 1.0e-6;
    if(fabs(h_num->GetXaxis()->GetXmin() - h_exps->GetXaxis()->GetXmin()) > eps
    || fabs(h_num->GetXaxis()->GetXmax() - h_exps->GetXaxis()->GetXmax()) > eps){
        cerr<<"ERR READ_CheckTimeShape ===== x-range mismatch"
            <<" i_enebin="<<i_enebin
            <<" num=("<<h_num->GetXaxis()->GetXmin()<<","<<h_num->GetXaxis()->GetXmax()<<")"
            <<" exps=("<<h_exps->GetXaxis()->GetXmin()<<","<<h_exps->GetXaxis()->GetXmax()<<")"
            <<endl;
        return false;
    }
    return true;
}


//==================================================== DRAW_
bool DRAW_FluxTime(const FluxConf &conf, int i_enebin, TH1D *hflux, TFile *fout){
    if(hflux == nullptr) return false;
    if(fout == nullptr) return false;

    TString outbase = TOOL_GetOutBase(conf.foutname);
    TString outdir = outbase + "_pdf";
    TString fout_pdf = Form("%s/hflux_t_ene%02d.pdf", outdir.Data(), i_enebin);
    TString canvas_name = Form("cflux_t_ene%02d", i_enebin);
    double elow = ENERGY_BINS[i_enebin];
    double eup = ENERGY_BINS[i_enebin + 1];

    gSystem->mkdir(outdir, true);

    TCanvas *c = new TCanvas(canvas_name, canvas_name, 1000, 400);
    TAxis *xaxis = hflux->GetXaxis();
    TAxis *yaxis = hflux->GetYaxis();

    hflux->SetStats(0);
    hflux->SetNameTitle("", "");
    c->SetTopMargin(0.13);
    c->SetBottomMargin(0.15);
    c->SetLeftMargin(0.13);
    c->SetRightMargin(0.08);
    c->cd();
    gPad->SetGridx();
    gPad->SetGridy();

    xaxis->SetNameTitle("Date", "Date");
    xaxis->CenterTitle();
    xaxis->SetTitleFont(62);
    xaxis->SetTitleSize(0.05);
    xaxis->SetTitleOffset(1.2);
    xaxis->SetLabelOffset(0.025);
    gStyle->SetTimeOffset(0);
    xaxis->SetTimeDisplay(1);
    xaxis->SetTimeFormat("%b/%d/%Y");
    xaxis->SetNdivisions(-505);
    if(conf.has_xmin && conf.has_xmax) xaxis->SetRangeUser(conf.xmin, conf.xmax);
    if(!conf.has_xmin && !conf.has_xmax) xaxis->SetRangeUser(conf.xmin, conf.xmax);

    yaxis->SetNameTitle("flux", Form("Flux (%s)", conf.species.Data()));
    yaxis->CenterTitle();
    yaxis->SetTitleFont(62);
    yaxis->SetTitleSize(0.05);
    yaxis->SetTitleOffset(0.9);
    yaxis->SetLabelOffset(0.012);

    hflux->SetMarkerStyle(20);
    hflux->SetMarkerSize(0.9);
    hflux->SetMarkerColor(kBlue);
    hflux->SetLineColor(kBlue);
    hflux->SetLineWidth(2);
    if(conf.has_ymin) hflux->SetMinimum(conf.ymin);
    if(conf.has_ymax) hflux->SetMaximum(conf.ymax);

    gStyle->SetEndErrorSize(0);
    TGaxis::SetMaxDigits(3);
    hflux->Draw("E1X0P");

    c->cd(0);
    TLatex latex;
    latex.SetNDC();
    latex.SetTextFont(62);
    latex.SetTextSize(0.033);
    latex.SetTextAlign(22);
    latex.DrawLatex(0.5, 0.97, Form("Energy %g to %g GeV", elow, eup));

    c->SaveAs(fout_pdf);
    fout->cd();
    c->Write();
    delete c;

    cout<<"IN DRAW_FluxTime ===== output pdf : "<<fout_pdf<<endl;
    cout<<"IN DRAW_FluxTime ===== output canvas : "<<canvas_name<<endl;
    return true;
}


//==================================================== INIT_
void INIT(int argc, char *argv[], FluxConf &conf){
    conf.datadir = "datain";
    conf.species = "npos";
    conf.foutname = "hflux_t_igrf.root";
    conf.exps_mode = "igrf";
    conf.exps_sf = "1";
    conf.has_xmin = false;
    conf.has_xmax = false;
    conf.xmin = 1305849600;
    conf.xmax = 1761955200;
    conf.has_ymin = true;
    conf.has_ymax = true;
    conf.ymin = 0.0;
    conf.ymax = 15.0;

    if(argc > 1) conf.species = argv[1];
    if(argc > 2) conf.datadir = argv[2];
    if(argc > 3) conf.foutname = argv[3];
    if(argc > 4) conf.exps_mode = argv[4];
    if(argc > 5) conf.exps_sf = argv[5];
    if(argc > 6 && argv[6][0] != '\0'){
        conf.xmin = atof(argv[6]);
        conf.has_xmin = true;
    }
    if(argc > 7 && argv[7][0] != '\0'){
        conf.xmax = atof(argv[7]);
        conf.has_xmax = true;
    }
    if(argc > 8 && argv[8][0] != '\0'){
        conf.ymin = atof(argv[8]);
        conf.has_ymin = true;
    }
    if(argc > 9 && argv[9][0] != '\0'){
        conf.ymax = atof(argv[9]);
        conf.has_ymax = true;
    }

    if(!TOOL_IsSupportedSpecies(conf.species)){
        cerr<<"ERR INIT ===== unsupported species "<<conf.species
            <<" ; only npos/nele"
            <<endl;
        exit(1);
    }
    if(conf.exps_mode != "igrf" && conf.exps_mode != "st"){
        cerr<<"ERR INIT ===== unsupported exps_mode "<<conf.exps_mode
            <<" ; only igrf/st"
            <<endl;
        exit(1);
    }

    cout<<"IN INIT ===== species="<<conf.species
        <<" datadir="<<conf.datadir
        <<" foutname="<<conf.foutname
        <<" exps_mode="<<conf.exps_mode
        <<" exps_sf="<<conf.exps_sf
        <<" xmin="<<(conf.has_xmin ? Form("%g", conf.xmin) : TString("N/A"))
        <<" xmax="<<(conf.has_xmax ? Form("%g", conf.xmax) : TString("N/A"))
        <<" ymin="<<(conf.has_ymin ? Form("%g", conf.ymin) : TString("N/A"))
        <<" ymax="<<(conf.has_ymax ? Form("%g", conf.ymax) : TString("N/A"))
        <<endl;
    cout<<"IN INIT ===== time-bin default = 1day"<<endl;
    //---- backup option, follow num3 27day setting
    // const double wt = 60*60*24*27;
    // static const int nt = 225;
}


//==================================================== CALC_
int CALC_Flux(const FluxConf &conf){
    unique_ptr<TFile> f_acc;
    unique_ptr<TFile> f_trig;
    unique_ptr<TFile> f_eff;
    unique_ptr<TFile> f_exps;

    if(!READ_OpenFile(conf.datadir + "/mcacc.root", f_acc)) return 1;
    if(!READ_OpenFile(conf.datadir + "/hene_trigeff.root", f_trig)) return 1;
    if(!READ_OpenFile(conf.datadir + "/hene_totaleff.root", f_eff)) return 1;
    if(!READ_OpenFile(conf.datadir + "/hexps.root", f_exps)) return 1;

    unique_ptr<TFile> f_out(TFile::Open(conf.foutname, "RECREATE"));
    if(f_out.get() == nullptr || f_out->IsZombie()){
        cerr<<"ERR CALC_Flux ===== failed to create output "<<conf.foutname<<endl;
        return 1;
    }

    if(READ_GetHist(f_acc.get(), "hacc_cut15") == nullptr) return 1;

    for(int i_enebin=0; i_enebin<NENEBIN; i_enebin++){
        TString fpath_num = TOOL_GetNFileName(conf, i_enebin);
        TString hname_exps = TOOL_GetExpsHistName(conf, i_enebin);
        TString hname_num = "htime";
        double emid = 0.5 * (ENERGY_BINS[i_enebin] + ENERGY_BINS[i_enebin + 1]);
        double delta_ene = ENERGY_BINS[i_enebin + 1] - ENERGY_BINS[i_enebin];
        EneValue acc{};
        EneValue trig{};
        EneValue eff{};
        unique_ptr<TFile> f_num;

        if(!READ_OpenFile(fpath_num, f_num)) return 1;

        TH1 *hnum_in = READ_GetHist(f_num.get(), hname_num);
        TH1 *hexps_in = READ_GetHist(f_exps.get(), hname_exps);
        if(hnum_in == nullptr || hexps_in == nullptr) return 1;
        if(!READ_CheckTimeShape(hnum_in, hexps_in, i_enebin)) return 1;

        if(!READ_GetEneValue(f_acc.get(), "hacc_cut15", emid, acc)) return 1;
        if(!READ_GetEneValue(f_trig.get(), "hene_iss", emid, trig)) return 1;
        if(!READ_GetEneValue(f_eff.get(), "hratio", emid, eff)) return 1;

        acc.value /= 1.0e4;
        acc.error /= 1.0e4;

        TH1D *hnum = dynamic_cast<TH1D*>(hnum_in->Clone(Form("hnum_t_ene%02d", i_enebin)));
        TH1D *hexps = dynamic_cast<TH1D*>(hexps_in->Clone(Form("hexps_t_ene%02d", i_enebin)));
        TH1D *hflux = dynamic_cast<TH1D*>(hnum_in->Clone(Form("hflux_t_ene%02d", i_enebin)));
        if(hnum == nullptr || hexps == nullptr || hflux == nullptr){
            cerr<<"ERR CALC_Flux ===== failed to clone hist i_enebin="<<i_enebin<<endl;
            return 1;
        }

        hnum->SetDirectory(nullptr);
        hexps->SetDirectory(nullptr);
        hflux->SetDirectory(nullptr);

        hnum->SetTitle(Form("num time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hexps->SetTitle(Form("exps time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hflux->SetTitle(Form("flux time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        int n_bad_bin = 0;
        int n_zero_num = 0;
        int n_zero_exps = 0;

        cout<<"IN CALC_Flux ===== i_enebin="<<i_enebin
            <<" elow="<<ENERGY_BINS[i_enebin]
            <<" eup="<<ENERGY_BINS[i_enebin + 1]
            <<" acc="<<acc.value<<" +/- "<<acc.error
            <<" trig="<<trig.value<<" +/- "<<trig.error
            <<" eff="<<eff.value<<" +/- "<<eff.error
            <<" delta_ene="<<delta_ene
            <<endl;

        for(int ibin=1; ibin<=hflux->GetNbinsX(); ibin++){
            double num = hnum->GetBinContent(ibin);
            double num_err = hnum->GetBinError(ibin);
            double exps = hexps->GetBinContent(ibin);
            double exps_err = hexps->GetBinError(ibin);
            double flux = 0.0;
            double flux_err = 0.0;

            if(TOOL_IsPositive(num)
            && TOOL_IsPositive(exps)
            && TOOL_IsPositive(acc.value)
            && TOOL_IsPositive(trig.value)
            && TOOL_IsPositive(eff.value)
            && TOOL_IsPositive(delta_ene)){
                flux = num / exps / acc.value / trig.value / eff.value / delta_ene;
                flux_err = flux * sqrt(
                    TOOL_CalcRelErr2(num, num_err)
                    + TOOL_CalcRelErr2(exps, exps_err)
                    + TOOL_CalcRelErr2(acc.value, acc.error)
                    + TOOL_CalcRelErr2(trig.value, trig.error)
                    + TOOL_CalcRelErr2(eff.value, eff.error)
                );
            }
            else{
                n_bad_bin++;
                if(num <= 0.0) n_zero_num++;
                if(exps <= 0.0) n_zero_exps++;
            }

            hflux->SetBinContent(ibin, flux);
            hflux->SetBinError(ibin, flux_err);
        }

        cout<<"IN CALC_Flux ===== summary"
            <<" i_enebin="<<i_enebin
            <<" n_bad_bin="<<n_bad_bin
            <<" n_zero_num="<<n_zero_num
            <<" n_zero_exps="<<n_zero_exps
            <<endl;

        f_out->cd();
        hnum->Write();
        hexps->Write();
        hflux->Write();

        if(!DRAW_FluxTime(conf, i_enebin, hflux, f_out.get())){
            delete hnum;
            delete hexps;
            delete hflux;
            return 1;
        }

        delete hnum;
        delete hexps;
        delete hflux;
    }

    f_out->cd();
    dynamic_cast<TH1*>(READ_GetHist(f_acc.get(), "hacc_cut15"))->Write("hacc_cut15");
    dynamic_cast<TH1*>(READ_GetHist(f_trig.get(), "hene_iss"))->Write("htrig_ene");
    dynamic_cast<TH1*>(READ_GetHist(f_eff.get(), "hratio"))->Write("hseleff_ene");
    f_out->Write();
    f_out->Close();

    cout<<"IN CALC_Flux ===== output "<<conf.foutname<<endl;
    return 0;
}


//==================================================== main
int main(int argc, char *argv[]){
    FluxConf conf{};
    INIT(argc, argv, conf);
    return CALC_Flux(conf);
}
