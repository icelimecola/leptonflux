#include <iostream>
using namespace std;

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TKey.h"
#include "TString.h"
#include "TSystem.h"

//====================================================
// static const int NENEBIN = 54;
// static const double ENERGY_BINS[NENEBIN + 1] = {
//     0.80, 1, 1.16, 1.33, 1.51,
//     1.71, 1.92, 2.15, 2.40, 2.67,
//     2.97, 3.29, 3.64, 4.02, 4.43,
//     4.88, 5.37, 5.90, 6.47, 7.09,
//     7.76, 8.48, 9.26, 10.10, 11,
//     12, 13, 14.10, 15.30, 16.60,
//     18, 19.50, 21.10, 22.80, 24.70,
//     26.70, 28.80, 31.10, 33.50, 36.10,
//     38.90, 41.90, 45.10, 48.50, 52.20,
//     56.10, 60.30, 64.80, 69.70, 74.90,
//     80.50, 86.50, 93, 100, 108
// };
static const int NENEBIN = 29;
static const double ENERGY_BINS[NENEBIN + 1] = {
    0.80, 1.00, 1.16, 1.33, 1.51,
    1.71, 1.92, 2.15, 2.40, 2.67,
    2.97, 3.29, 3.64, 4.02, 4.43,
    4.88, 5.37, 5.90, 6.47, 7.09,
    7.76, 8.48, 9.26, 10.10, 11.0,
    13.0, 16.6, 22.8, 41.9, 45.10
};

struct FluxConf{
    TString findir;
    TString foutname;
    TString particle;
    TString exps_geomagmode;
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

struct SeleffItem{
    TString key;
    TString filename;
    TString histname;
    TString outname;
    bool use_in_total;
};

struct FluxBin{
    double num = 0.0;
    double num_err = 0.0;
    double flux = 0.0;
    double flux_err = 0.0;
    double flux_noacc = 0.0;
    double flux_noacc_err = 0.0;
    bool bad_cc = false;
    bool good = false;
};


//==================================================== TOOL_
static const double WT_27D = 60.0 * 60.0 * 24.0 * 27.0;
static const int NT_27D = 225;
static const double TMIN_27D = 1305417600.0;
static const double TMAX_27D = TMIN_27D + NT_27D * WT_27D;

TString TOOL_FormatE(double x){
    return Form("%g", x);
}

TString TOOL_GetNFileName(const TString &datadir, const TString &species, int i_enebin){
    return Form(
        "%s/%s/htime_tfit%02d_%sGeV_%s.root",
        datadir.Data(),
        species.Data(),
        i_enebin,
        TOOL_FormatE(ENERGY_BINS[i_enebin]).Data(),
        species.Data()
    );
}

TString TOOL_GetTRDEffTimeFileName(const TString &datadir, int i_enebin){
    return Form(
        "%s/trdeff/htime_tfit%02d_%sGeV_trdeff.root",
        datadir.Data(),
        i_enebin,
        TOOL_FormatE(ENERGY_BINS[i_enebin]).Data()
    );
}

TString TOOL_GetExpsHistName(const FluxConf &conf){
    return Form(
        "h2/h2exp_%s_TvE_sf%s",
        conf.exps_geomagmode.Data(),
        conf.exps_sf.Data()
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

double TOOL_CalcNCorrPos(double npos_measure, double nele_measure, double cc){
    return ((1.0 - cc) * npos_measure - cc * nele_measure) / (1.0 - 2.0 * cc);
}

double TOOL_CalcNCorrEle(double npos_measure, double nele_measure, double cc){
    return ((1.0 - cc) * nele_measure - cc * npos_measure) / (1.0 - 2.0 * cc);
}

double TOOL_CalcNCorrPosErr(double npos_measure, double npos_err, double nele_measure, double nele_err, double cc, double cc_err){
    double den = 1.0 - 2.0 * cc;
    if(fabs(den) < 1.0e-9) return 0.0;
    double dpos = (1.0 - cc) / den;
    double dele = -cc / den;
    double dcc = (npos_measure - nele_measure) / (den * den);
    return sqrt(
        pow(dpos * npos_err, 2)
        + pow(dele * nele_err, 2)
        + pow(dcc * cc_err, 2)
    );
}

double TOOL_CalcNCorrEleErr(double npos_measure, double npos_err, double nele_measure, double nele_err, double cc, double cc_err){
    double den = 1.0 - 2.0 * cc;
    if(fabs(den) < 1.0e-9) return 0.0;
    double dele = (1.0 - cc) / den;
    double dpos = -cc / den;
    double dcc = (nele_measure - npos_measure) / (den * den);
    return sqrt(
        pow(dele * nele_err, 2)
        + pow(dpos * npos_err, 2)
        + pow(dcc * cc_err, 2)
    );
}

TH1D *TOOL_Build27DayStatHist(TH1 *hin, const TString &hname, const TString &htitle){
    if(hin == nullptr) return nullptr;

    TH1D *hout = new TH1D(hname, htitle, NT_27D, TMIN_27D, TMAX_27D);
    if(hout == nullptr) return nullptr;
    hout->SetDirectory(nullptr);
    hout->Sumw2();

    for(int ibin=1; ibin<=hin->GetNbinsX(); ibin++){
        double x = hin->GetXaxis()->GetBinCenter(ibin);
        double y = hin->GetBinContent(ibin);
        double ey = hin->GetBinError(ibin);
        int jbin = hout->FindBin(x);
        if(jbin < 1 || jbin > hout->GetNbinsX()) continue;

        double yold = hout->GetBinContent(jbin);
        double eold = hout->GetBinError(jbin);
        hout->SetBinContent(jbin, yold + y);
        hout->SetBinError(jbin, sqrt(eold * eold + ey * ey));
    }

    return hout;
}

FluxBin TOOL_CalcFluxBin(
    const FluxConf &conf,
    double npos_measure,
    double npos_measure_err,
    double nele_measure,
    double nele_measure_err,
    double exps,
    double exps_err,
    double trdeff_time,
    double trdeff_time_err,
    const EneValue &cc,
    const EneValue &acc,
    const EneValue &trig,
    const EneValue &eff_total_manual,
    double delta_ene
){
    FluxBin out;
    double den_cc = 1.0 - 2.0 * cc.value;
    if(fabs(den_cc) < 1.0e-9){
        out.bad_cc = true;
        return out;
    }

    if(conf.particle == "npos"){
        out.num = TOOL_CalcNCorrPos(npos_measure, nele_measure, cc.value);
        out.num_err = TOOL_CalcNCorrPosErr(npos_measure, npos_measure_err, nele_measure, nele_measure_err, cc.value, cc.error);
    }
    else{
        out.num = TOOL_CalcNCorrEle(npos_measure, nele_measure, cc.value);
        out.num_err = TOOL_CalcNCorrEleErr(npos_measure, npos_measure_err, nele_measure, nele_measure_err, cc.value, cc.error);
    }

    out.good = TOOL_IsPositive(out.num)
        && TOOL_IsPositive(exps)
        && TOOL_IsPositive(acc.value)
        && TOOL_IsPositive(trig.value)
        && TOOL_IsPositive(eff_total_manual.value)
        && TOOL_IsPositive(trdeff_time)
        && TOOL_IsPositive(delta_ene);
    if(!out.good) return out;

    out.flux = out.num / exps / acc.value / trig.value / eff_total_manual.value / trdeff_time / delta_ene;
    out.flux_err = out.flux * sqrt(
        TOOL_CalcRelErr2(out.num, out.num_err)
        + TOOL_CalcRelErr2(exps, exps_err)
        + TOOL_CalcRelErr2(acc.value, acc.error)
        + TOOL_CalcRelErr2(trig.value, trig.error)
        + TOOL_CalcRelErr2(eff_total_manual.value, eff_total_manual.error)
        + TOOL_CalcRelErr2(trdeff_time, trdeff_time_err)
    );
    out.flux_noacc = out.num / exps / trig.value / eff_total_manual.value / trdeff_time / delta_ene;
    out.flux_noacc_err = out.flux_noacc * sqrt(
        TOOL_CalcRelErr2(out.num, out.num_err)
        + TOOL_CalcRelErr2(exps, exps_err)
        + TOOL_CalcRelErr2(trig.value, trig.error)
        + TOOL_CalcRelErr2(eff_total_manual.value, eff_total_manual.error)
        + TOOL_CalcRelErr2(trdeff_time, trdeff_time_err)
    );
    return out;
}

TString TOOL_GetOutBase(const TString &foutname){
    TString outbase = foutname;
    if(outbase.EndsWith(".root")) outbase.Resize(outbase.Length() - 5);
    return outbase;
}

vector<SeleffItem> TOOL_BuildSeleffItems(){
    vector<SeleffItem> items;
    items.push_back({"tof",   "seleff/hene_tofeff.root",   "hratio", "hseleff_tof_ene",          true });
    items.push_back({"trd",   "seleff/hene_trdeff.root",   "hratio", "hseleff_trd_ene",          false});
    items.push_back({"trk",   "seleff/hene_trkeff.root",   "hratio", "hseleff_trk_ene",          true });
    items.push_back({"ecal",  "seleff/hene_ecaleff.root",  "hratio", "hseleff_ecal_ene",         true });
    items.push_back({"pat",   "seleff/hene_pateff.root",   "hratio", "hseleff_pat_ene",          true });
    items.push_back({"match", "seleff/hene_matcheff.root", "hratio", "hseleff_match_ene",        true });
    items.push_back({"chi2",  "seleff/hene_chi2eff.root",  "hratio", "hseleff_chi2_ene",         true });
    items.push_back({"qin",   "seleff/hene_qineff.root",   "hratio", "hseleff_qin_ene",          true });
    items.push_back({"ntrk",  "seleff/hene_ntrkeff.root",  "hratio", "hseleff_ntrk_ene",         true });
    return items;
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

TH1D *READ_BuildExpsTimeHist(TFile *f, const FluxConf &conf, int i_enebin){
    TString hname = TOOL_GetExpsHistName(conf);
    TH2 *h2 = dynamic_cast<TH2*>(READ_GetHist(f, hname));
    if(h2 == nullptr) return nullptr;

    double elow = ENERGY_BINS[i_enebin];
    double eup = ENERGY_BINS[i_enebin + 1];
    int iy1 = h2->GetYaxis()->FindBin(elow + 1.0e-9);
    int iy2 = h2->GetYaxis()->FindBin(eup - 1.0e-9);
    if(iy1 < 1 || iy2 > h2->GetNbinsY() || iy2 < iy1){
        cerr<<"ERR READ_BuildExpsTimeHist ===== bad energy range"
            <<" i_enebin="<<i_enebin
            <<" elow="<<elow
            <<" eup="<<eup
            <<" iy1="<<iy1
            <<" iy2="<<iy2
            <<endl;
        return nullptr;
    }

    TH1D *hout = h2->ProjectionX(Form("hexps_in_t_ene%02d", i_enebin), iy1, iy2, "e");
    if(hout == nullptr) return nullptr;
    hout->SetDirectory(nullptr);
    hout->SetTitle(Form("exps time, %g to %g GeV", elow, eup));
    return hout;
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

    yaxis->SetNameTitle("flux", Form("Flux (%s)", conf.particle.Data()));
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
    conf.findir = "datain";
    conf.particle = "npos";
    conf.foutname = "hflux_t_igrf.root";
    conf.exps_geomagmode = "igrf";
    conf.exps_sf = "1";
    conf.has_xmin = false;
    conf.has_xmax = false;
    conf.xmin = 1305849600;
    conf.xmax = 1761955200;
    conf.has_ymin = true;
    conf.has_ymax = true;
    conf.ymin = 0.0;
    conf.ymax = 15.0;

    if(argc > 1) conf.foutname = argv[1];
    if(argc > 2) conf.findir = argv[2];
    if(argc > 3) conf.particle = argv[3];
    if(argc > 4) conf.exps_geomagmode = argv[4];
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

    if(!TOOL_IsSupportedSpecies(conf.particle)){
        cerr<<"ERR INIT ===== unsupported species "<<conf.particle
            <<" ; only npos/nele"
            <<endl;
        exit(1);
    }
    if(conf.exps_geomagmode != "igrf" && conf.exps_geomagmode != "st" && conf.exps_geomagmode != "ts05"){
        cerr<<"ERR INIT ===== unsupported exps_mode "<<conf.exps_geomagmode
            <<" ; only igrf/st/ts05"
            <<endl;
        exit(1);
    }

    cout<<"IN INIT ===== species="<<conf.particle
        <<" datadir="<<conf.findir
        <<" foutname="<<conf.foutname
        <<" exps_mode="<<conf.exps_geomagmode
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
    unique_ptr<TFile> f_cc;
    unique_ptr<TFile> f_trig;
    unique_ptr<TFile> f_exps;
    unique_ptr<TFile> f_eff_total_input;
    vector<SeleffItem> seleff_items = TOOL_BuildSeleffItems();
    vector<unique_ptr<TFile>> vfile_eff;
    vector<TH1D*> vh_eff_hist;

    if(!READ_OpenFile(conf.findir + "/mcacc.root", f_acc)) return 1;
    if(!READ_OpenFile(conf.findir + "/cc.root", f_cc)) return 1;
    if(!READ_OpenFile(conf.findir + "/hene_trigeff.root", f_trig)) return 1;
    if(!READ_OpenFile(conf.findir + "/hexps.root", f_exps)) return 1;
    if(!READ_OpenFile(conf.findir + "/seleff/hene_totaleff.root", f_eff_total_input)) return 1;

    vfile_eff.resize(seleff_items.size());
    vh_eff_hist.resize(seleff_items.size(), nullptr);
    for(size_t ieff=0; ieff<seleff_items.size(); ieff++){
        if(!READ_OpenFile(conf.findir + "/" + seleff_items[ieff].filename, vfile_eff[ieff])) return 1;
        TH1 *htmp = READ_GetHist(vfile_eff[ieff].get(), seleff_items[ieff].histname);
        if(htmp == nullptr) return 1;
        vh_eff_hist[ieff] = dynamic_cast<TH1D*>(htmp);
        if(vh_eff_hist[ieff] == nullptr){
            cerr<<"ERR CALC_Flux ===== seleff hist is not TH1D key="<<seleff_items[ieff].key<<endl;
            return 1;
        }
    }

    unique_ptr<TFile> f_out(TFile::Open(conf.foutname, "RECREATE"));
    if(f_out.get() == nullptr || f_out->IsZombie()){
        cerr<<"ERR CALC_Flux ===== failed to create output "<<conf.foutname<<endl;
        return 1;
    }

    if(READ_GetHist(f_acc.get(), "hacc_cut15") == nullptr) return 1;
    if(READ_GetHist(f_cc.get(), "cc_ene") == nullptr) return 1;
    if(READ_GetHist(f_eff_total_input.get(), "hratio") == nullptr) return 1;

    TH1D *hseleff_total_input = dynamic_cast<TH1D*>(READ_GetHist(f_eff_total_input.get(), "hratio")->Clone("hseleff_total_input_ene"));
    TH1D *hseleff_total_manual = new TH1D("hseleff_total_manual_ene", "hseleff_total_manual_ene", NENEBIN, ENERGY_BINS);
    vector<TH1D*> vhseleff_out(seleff_items.size(), nullptr);
    for(size_t ieff=0; ieff<seleff_items.size(); ieff++){
        vhseleff_out[ieff] = new TH1D(seleff_items[ieff].outname, seleff_items[ieff].outname, NENEBIN, ENERGY_BINS);
    }
    hseleff_total_input->SetDirectory(nullptr);
    hseleff_total_manual->SetDirectory(nullptr);
    for(size_t ieff=0; ieff<vhseleff_out.size(); ieff++){
        vhseleff_out[ieff]->SetDirectory(nullptr);
    }

    for(int i_enebin=0; i_enebin<NENEBIN; i_enebin++){
        TString fpath_num_pos = TOOL_GetNFileName(conf.findir, "npos", i_enebin);
        TString fpath_num_ele = TOOL_GetNFileName(conf.findir, "nele", i_enebin);
        TString hname_num = "htime";
        double emid = 0.5 * (ENERGY_BINS[i_enebin] + ENERGY_BINS[i_enebin + 1]);
        double delta_ene = ENERGY_BINS[i_enebin + 1] - ENERGY_BINS[i_enebin];
        EneValue acc{};
        EneValue cc{};
        EneValue trig{};
        EneValue eff_total_input{};
        EneValue eff_total_manual{};
        unique_ptr<TFile> f_num_pos;
        unique_ptr<TFile> f_num_ele;
        unique_ptr<TFile> f_trdeff_time;

        if(!READ_OpenFile(fpath_num_pos, f_num_pos)) return 1;
        if(!READ_OpenFile(fpath_num_ele, f_num_ele)) return 1;
        if(!READ_OpenFile(TOOL_GetTRDEffTimeFileName(conf.findir, i_enebin), f_trdeff_time)) return 1;

        TH1 *hnum_pos_in = READ_GetHist(f_num_pos.get(), hname_num);
        TH1 *hnum_ele_in = READ_GetHist(f_num_ele.get(), hname_num);
        TH1D *hexps_in = READ_BuildExpsTimeHist(f_exps.get(), conf, i_enebin);
        TH1 *htrdeff_time_in = READ_GetHist(f_trdeff_time.get(), "hratio1");
        TH1 *htrdeff_time_27d_in = READ_GetHist(f_trdeff_time.get(), "hratio27");
        if(hnum_pos_in == nullptr || hnum_ele_in == nullptr || hexps_in == nullptr || htrdeff_time_in == nullptr || htrdeff_time_27d_in == nullptr) return 1;
        if(!READ_CheckTimeShape(hnum_pos_in, hexps_in, i_enebin)) return 1;
        if(!READ_CheckTimeShape(hnum_ele_in, hexps_in, i_enebin)) return 1;
        if(!READ_CheckTimeShape(hnum_pos_in, hnum_ele_in, i_enebin)) return 1;
        if(!READ_CheckTimeShape(htrdeff_time_in, hexps_in, i_enebin)) return 1;

        if(!READ_GetEneValue(f_acc.get(), "hacc_cut15", emid, acc)) return 1;
        if(!READ_GetEneValue(f_cc.get(), "cc_ene", emid, cc)) return 1;
        if(!READ_GetEneValue(f_trig.get(), "hene_iss", emid, trig)) return 1;
        if(!READ_GetEneValue(f_eff_total_input.get(), "hratio", emid, eff_total_input)) return 1;

        eff_total_manual.value = 1.0;
        double relerr2_total = 0.0;
        for(size_t ieff=0; ieff<seleff_items.size(); ieff++){
            EneValue eff_now{};
            if(!READ_GetEneValue(vfile_eff[ieff].get(), seleff_items[ieff].histname, emid, eff_now)) return 1;
            vhseleff_out[ieff]->SetBinContent(i_enebin + 1, eff_now.value);
            vhseleff_out[ieff]->SetBinError(i_enebin + 1, eff_now.error);
            if(seleff_items[ieff].use_in_total){
                eff_total_manual.value *= eff_now.value;
                if(eff_now.value > 0.0) relerr2_total += TOOL_CalcRelErr2(eff_now.value, eff_now.error);
            }
        }
        eff_total_manual.error = 0.0;
        if(eff_total_manual.value > 0.0) eff_total_manual.error = eff_total_manual.value * sqrt(relerr2_total);
        hseleff_total_input->SetBinContent(i_enebin + 1, eff_total_input.value);
        hseleff_total_input->SetBinError(i_enebin + 1, eff_total_input.error);
        hseleff_total_manual->SetBinContent(i_enebin + 1, eff_total_manual.value);
        hseleff_total_manual->SetBinError(i_enebin + 1, eff_total_manual.error);

        acc.value /= 1.0e4;
        acc.error /= 1.0e4;

        TH1D *hnum_pos_measure = dynamic_cast<TH1D*>(hnum_pos_in->Clone(Form("hnum_pos_measure_t_ene%02d", i_enebin)));
        TH1D *hnum_ele_measure = dynamic_cast<TH1D*>(hnum_ele_in->Clone(Form("hnum_ele_measure_t_ene%02d", i_enebin)));
        TH1D *hnumcorr = dynamic_cast<TH1D*>(hnum_pos_in->Clone(Form("hnumcorr_t_ene%02d", i_enebin)));
        TH1D *hnum = dynamic_cast<TH1D*>(hnum_pos_in->Clone(Form("hnum_t_ene%02d", i_enebin)));
        TH1D *hexps = dynamic_cast<TH1D*>(hexps_in->Clone(Form("hexps_t_ene%02d", i_enebin)));
        TH1D *htrdeff_time = dynamic_cast<TH1D*>(htrdeff_time_in->Clone(Form("htrdeff_t_ene%02d", i_enebin)));
        TH1D *hflux_noacc = dynamic_cast<TH1D*>(hnum_pos_in->Clone(Form("hflux_noacc_t_ene%02d", i_enebin)));
        TH1D *hflux = dynamic_cast<TH1D*>(hnum_pos_in->Clone(Form("hflux_t_ene%02d", i_enebin)));
        TH1D *hnum_pos_measure_27d = TOOL_Build27DayStatHist(hnum_pos_in, Form("hnum_pos_measure_27d_t_ene%02d", i_enebin), Form("npos measure 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        TH1D *hnum_ele_measure_27d = TOOL_Build27DayStatHist(hnum_ele_in, Form("hnum_ele_measure_27d_t_ene%02d", i_enebin), Form("nele measure 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        TH1D *hexps_27d = TOOL_Build27DayStatHist(hexps_in, Form("hexps_27d_t_ene%02d", i_enebin), Form("exps 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        TH1D *htrdeff_time_27d = dynamic_cast<TH1D*>(htrdeff_time_27d_in->Clone(Form("htrdeff_27d_t_ene%02d", i_enebin)));
        TH1D *hnumcorr_27d = hnum_pos_measure_27d == nullptr ? nullptr : dynamic_cast<TH1D*>(hnum_pos_measure_27d->Clone(Form("hnumcorr_27d_t_ene%02d", i_enebin)));
        TH1D *hnum_27d = hnum_pos_measure_27d == nullptr ? nullptr : dynamic_cast<TH1D*>(hnum_pos_measure_27d->Clone(Form("hnum_27d_t_ene%02d", i_enebin)));
        TH1D *hflux_noacc_27d = hnum_pos_measure_27d == nullptr ? nullptr : dynamic_cast<TH1D*>(hnum_pos_measure_27d->Clone(Form("hflux_noacc_27d_t_ene%02d", i_enebin)));
        TH1D *hflux_27d = hnum_pos_measure_27d == nullptr ? nullptr : dynamic_cast<TH1D*>(hnum_pos_measure_27d->Clone(Form("hflux_27d_t_ene%02d", i_enebin)));
        if(hnum_pos_measure == nullptr || hnum_ele_measure == nullptr || hnumcorr == nullptr || hnum == nullptr || hexps == nullptr || htrdeff_time == nullptr || hflux_noacc == nullptr || hflux == nullptr){
            cerr<<"ERR CALC_Flux ===== failed to clone hist i_enebin="<<i_enebin<<endl;
            return 1;
        }
        if(hnum_pos_measure_27d == nullptr || hnum_ele_measure_27d == nullptr || hexps_27d == nullptr || htrdeff_time_27d == nullptr || hnumcorr_27d == nullptr || hnum_27d == nullptr || hflux_noacc_27d == nullptr || hflux_27d == nullptr){
            cerr<<"ERR CALC_Flux ===== failed to create 27day hist i_enebin="<<i_enebin<<endl;
            return 1;
        }
        if(!READ_CheckTimeShape(htrdeff_time_27d, hexps_27d, i_enebin)) return 1;

        hnum_pos_measure->SetDirectory(nullptr);
        hnum_ele_measure->SetDirectory(nullptr);
        hnumcorr->SetDirectory(nullptr);
        hnum->SetDirectory(nullptr);
        hexps->SetDirectory(nullptr);
        htrdeff_time->SetDirectory(nullptr);
        hflux_noacc->SetDirectory(nullptr);
        hflux->SetDirectory(nullptr);
        htrdeff_time_27d->SetDirectory(nullptr);
        hnumcorr_27d->SetDirectory(nullptr);
        hnum_27d->SetDirectory(nullptr);
        hflux_noacc_27d->SetDirectory(nullptr);
        hflux_27d->SetDirectory(nullptr);

        hnum_pos_measure->SetTitle(Form("npos measure time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnum_ele_measure->SetTitle(Form("nele measure time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnumcorr->SetTitle(Form("ncorr time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnum->SetTitle(Form("num corr time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hexps->SetTitle(Form("exps time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        htrdeff_time->SetTitle(Form("trdeff time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hflux_noacc->SetTitle(Form("flux noacc time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hflux->SetTitle(Form("flux time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnumcorr_27d->SetTitle(Form("ncorr 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnum_27d->SetTitle(Form("num corr 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        htrdeff_time_27d->SetTitle(Form("trdeff 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hflux_noacc_27d->SetTitle(Form("flux noacc 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hflux_27d->SetTitle(Form("flux 27day time, %g to %g GeV", ENERGY_BINS[i_enebin], ENERGY_BINS[i_enebin + 1]));
        hnumcorr_27d->Reset();
        hnum_27d->Reset();
        hflux_noacc_27d->Reset();
        hflux_27d->Reset();
        int n_bad_bin = 0;
        int n_zero_num = 0;
        int n_zero_exps = 0;
        int n_bad_cc = 0;
        int n_bad_bin_27d = 0;
        int n_zero_num_27d = 0;
        int n_zero_exps_27d = 0;
        int n_bad_cc_27d = 0;

        cout<<"IN CALC_Flux ===== i_enebin="<<i_enebin
            <<" elow="<<ENERGY_BINS[i_enebin]
            <<" eup="<<ENERGY_BINS[i_enebin + 1]
            <<" acc="<<acc.value<<" +/- "<<acc.error
            <<" cc="<<cc.value<<" +/- "<<cc.error
            <<" trig="<<trig.value<<" +/- "<<trig.error
            <<" eff_total_input="<<eff_total_input.value<<" +/- "<<eff_total_input.error
            <<" eff_total_manual="<<eff_total_manual.value<<" +/- "<<eff_total_manual.error
            <<" delta_ene="<<delta_ene
            <<endl;

        for(int ibin=1; ibin<=hflux->GetNbinsX(); ibin++){
            double npos_measure = hnum_pos_measure->GetBinContent(ibin);
            double npos_measure_err = hnum_pos_measure->GetBinError(ibin);
            double nele_measure = hnum_ele_measure->GetBinContent(ibin);
            double nele_measure_err = hnum_ele_measure->GetBinError(ibin);
            double exps = hexps->GetBinContent(ibin);
            double exps_err = hexps->GetBinError(ibin);
            double trdeff_time = htrdeff_time->GetBinContent(ibin);
            double trdeff_time_err = htrdeff_time->GetBinError(ibin);
            FluxBin out = TOOL_CalcFluxBin(conf, npos_measure, npos_measure_err, nele_measure, nele_measure_err, exps, exps_err, trdeff_time, trdeff_time_err, cc, acc, trig, eff_total_manual, delta_ene);

            if(out.bad_cc) n_bad_cc++;
            if(!out.good){
                n_bad_bin++;
                if(out.num <= 0.0) n_zero_num++;
                if(exps <= 0.0) n_zero_exps++;
            }

            hnumcorr->SetBinContent(ibin, out.num);
            hnumcorr->SetBinError(ibin, out.num_err);
            hnum->SetBinContent(ibin, out.num);
            hnum->SetBinError(ibin, out.num_err);
            hflux_noacc->SetBinContent(ibin, out.flux_noacc);
            hflux_noacc->SetBinError(ibin, out.flux_noacc_err);
            hflux->SetBinContent(ibin, out.flux);
            hflux->SetBinError(ibin, out.flux_err);
        }

        for(int ibin=1; ibin<=hflux_27d->GetNbinsX(); ibin++){
            double npos_measure = hnum_pos_measure_27d->GetBinContent(ibin);
            double npos_measure_err = hnum_pos_measure_27d->GetBinError(ibin);
            double nele_measure = hnum_ele_measure_27d->GetBinContent(ibin);
            double nele_measure_err = hnum_ele_measure_27d->GetBinError(ibin);
            double exps = hexps_27d->GetBinContent(ibin);
            double exps_err = hexps_27d->GetBinError(ibin);
            double trdeff_time = htrdeff_time_27d->GetBinContent(ibin);
            double trdeff_time_err = htrdeff_time_27d->GetBinError(ibin);
            FluxBin out = TOOL_CalcFluxBin(conf, npos_measure, npos_measure_err, nele_measure, nele_measure_err, exps, exps_err, trdeff_time, trdeff_time_err, cc, acc, trig, eff_total_manual, delta_ene);

            if(out.bad_cc) n_bad_cc_27d++;
            if(!out.good){
                n_bad_bin_27d++;
                if(out.num <= 0.0) n_zero_num_27d++;
                if(exps <= 0.0) n_zero_exps_27d++;
            }

            hnumcorr_27d->SetBinContent(ibin, out.num);
            hnumcorr_27d->SetBinError(ibin, out.num_err);
            hnum_27d->SetBinContent(ibin, out.num);
            hnum_27d->SetBinError(ibin, out.num_err);
            hflux_noacc_27d->SetBinContent(ibin, out.flux_noacc);
            hflux_noacc_27d->SetBinError(ibin, out.flux_noacc_err);
            hflux_27d->SetBinContent(ibin, out.flux);
            hflux_27d->SetBinError(ibin, out.flux_err);
        }

        cout<<"IN CALC_Flux ===== summary"
            <<" i_enebin="<<i_enebin
            <<" n_bad_bin="<<n_bad_bin
            <<" n_zero_num="<<n_zero_num
            <<" n_zero_exps="<<n_zero_exps
            <<" n_bad_cc="<<n_bad_cc
            <<" nbin_1day="<<hflux->GetNbinsX()
            <<" nbin_27day="<<hflux_27d->GetNbinsX()
            <<" n_bad_bin_27d="<<n_bad_bin_27d
            <<" n_zero_num_27d="<<n_zero_num_27d
            <<" n_zero_exps_27d="<<n_zero_exps_27d
            <<" n_bad_cc_27d="<<n_bad_cc_27d
            <<endl;

        f_out->cd();
        // hnum_pos_measure->Write();
        // hnum_ele_measure->Write();
        // hnumcorr->Write();
        // hnum->Write();
        // hexps->Write();
        // htrdeff_time->Write();
        // hflux_noacc->Write();
        hflux->Write();
        // hnum_pos_measure_27d->Write();
        // hnum_ele_measure_27d->Write();
        // hnumcorr_27d->Write();
        // hnum_27d->Write();
        // hexps_27d->Write();
        // htrdeff_time_27d->Write();
        // hflux_noacc_27d->Write();
        // hflux_27d->Write();

        if(!DRAW_FluxTime(conf, i_enebin, hflux, f_out.get())){
            delete hnum_pos_measure;
            delete hnum_ele_measure;
            delete hnumcorr;
            delete hnum;
            delete hexps;
            delete htrdeff_time;
            delete hflux_noacc;
            delete hflux;
            delete hnum_pos_measure_27d;
            delete hnum_ele_measure_27d;
            delete hnumcorr_27d;
            delete hnum_27d;
            delete hexps_27d;
            delete htrdeff_time_27d;
            delete hflux_noacc_27d;
            delete hflux_27d;
            return 1;
        }

        delete hnum_pos_measure;
        delete hnum_ele_measure;
        delete hnumcorr;
        delete hnum;
        delete hexps;
        delete htrdeff_time;
        delete hflux_noacc;
        delete hflux;
        delete hnum_pos_measure_27d;
        delete hnum_ele_measure_27d;
        delete hnumcorr_27d;
        delete hnum_27d;
        delete hexps_27d;
        delete htrdeff_time_27d;
        delete hflux_noacc_27d;
        delete hflux_27d;
    }

    f_out->cd();
    dynamic_cast<TH1*>(READ_GetHist(f_acc.get(), "hacc_cut15"))->Write("hacc_cut15");
    dynamic_cast<TH1*>(READ_GetHist(f_cc.get(), "cc_ene"))->Write("hcc_ene");
    dynamic_cast<TH1*>(READ_GetHist(f_trig.get(), "hene_iss"))->Write("htrig_ene");
    hseleff_total_input->Write();
    hseleff_total_manual->Write();
    for(size_t ieff=0; ieff<vhseleff_out.size(); ieff++){
        vhseleff_out[ieff]->Write();
    }
    f_out->Write();
    f_out->Close();

    delete hseleff_total_input;
    delete hseleff_total_manual;
    for(size_t ieff=0; ieff<vhseleff_out.size(); ieff++){
        delete vhseleff_out[ieff];
    }

    cout<<"IN CALC_Flux ===== output "<<conf.foutname<<endl;
    return 0;
}


//==================================================== main
int main(int argc, char *argv[]){
    FluxConf conf{};
    INIT(argc, argv, conf);
    return CALC_Flux(conf);
}
