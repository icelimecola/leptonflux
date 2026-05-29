_Pragma("once")

#include <iostream>
using namespace std;
#include <TString.h>
#include <TFile.h>
#include <TDatime.h>
//====
#include "Exps.hpp"
//====
#include "../include/rootclass_gcx/ToolFileName.hpp"
#include "../include/rootclass_gcx/ToolChain.hpp"
#include "../include/rootclass_gcx/ToolTree.hpp"
#include "../include/rootclass_gcx/ToolFileOut.hpp"
#include "../include/general/ConsoleDisplay.h"

class ExpsRun{
    public:
        ExpsRun(){cout<<"ExpsRun-cont"<<endl;}
        ExpsRun(TString fn,int jid,int nfpj,TString tn):
        filename(fn),jobid(jid),nfilesperjob(nfpj),
        ins_filename(fn),ins_chain(tn),ins_tree(),ins_fileout(),
        ins_exps(fn)
        {cout<<"ExpsRun-cont"<<endl;}
        ~ExpsRun(){}
    public:
        TString filename;
        int jobid;
        int nfilesperjob;
        vector<TString> fullfilename;
    public:
        ToolFileName ins_filename;
        ToolChain ins_chain;
        ToolTree ins_tree;
        ToolFileOut ins_fileout;
    public:
        Exps ins_exps;
    public:
        void EXPS_RUN_02();
};


void ExpsRun::EXPS_RUN_02(){
    //============ init ============
    //==== filename
    ins_filename.InitFileName();
    fullfilename=ins_filename.GetFullName();
    //==== chain
    ins_chain.AddTree(fullfilename,jobid,nfilesperjob);
    ins_exps.INIT_CHAIN_SetBranchAddress(ins_chain.GetChain());
    //==== tree 
    // ins_exps.INIT_TREE_BuildBrand(ins_tree.GetTree());
    //==== fileout 
    // ins_chain.GetChain()->GetEntry(0);
    // ins_fileout.InitFileOut(Form("minitree_%010i_%010i.root", ins_exps.info_run, ins_exps.info_event));
    
    //============ run ============
    long nentries = ins_chain.GetChain()->GetEntries();
    ConsoleDisplay mydisplay(nentries);
    UInt_t previous_time = 0;
    //====traversal
    cout<<"================ loop ================↓"<<endl;
    for(long entry=0; entry<nentries; entry++){
        //======== output
        mydisplay.Update(entry);
        //======== getvalue
        ins_chain.GetChain()->GetEntry(entry);
        //======== cut--time
        //----260406--prl122
        // if(ins_exps.info_utime<1305763200) continue;
        // if(ins_exps.info_utime>1510444800) continue;
        //======== reset
        ins_exps.INIT_SetZero();
        ins_exps.INIT_SetDatatype();
        //======== process--all
        ins_exps.PROCESS_AllData();
        //======== process--mc
        if(ins_exps.PROCESS_IsMC()) ins_exps.PROCESS_MCdata();
        //======== process--iss
        if(ins_exps.PROCESS_IsISS()){
            //==== runtri
            ins_exps.AMSRTI_RunRTI();
            //==== cut--good daq
            if(!ins_exps.HIST_IsGoodDAQ()) continue;
            //==== cut--badrun
            //----original
            // if(ins_exps.isbadrun) continue;
            //----260509
            if(ins_exps.isbadrun_daq || ins_exps.isbadrun_fov || ins_exps.isbadrun_ecal || ins_exps.isbadrun_trk) continue;
            if(ins_exps.info_utime>=1305417600 && ins_exps.info_utime<=1307750400 && ins_exps.isbadrun_general) continue;
            //==== cut--unique utime
            if(ins_exps.info_utime == previous_time) continue;  //----每个utime只填充一次exps直方图
            previous_time=ins_exps.info_utime;
            //==== fillhist & cut--geomag
            ins_exps.HIST_FillExps_TvE();
        }
        //======== filltree
        // ins_tree.FillTree();
    }
    mydisplay.Finish();
    cout<<"================ loop ================↑"<<endl;
    
    //============ save ============
    //======== savetree
    // ins_fileout.GetFileOut()->cd();
    // ins_tree.WriteTree();
    // ins_fileout.GetFileOut()->Close();
    //======== savehist
    //====init fileout
    ins_chain.GetChain()->GetEntry(0);
    // TFile ins_fo(Form("expshist_%010i_%010i.root", ins_exps.info_run, ins_exps.info_event), "recreate");
    TFile ins_fo(Form("expshist_%05d_%010i.root", jobid, ins_exps.info_run), "recreate");
    //====savehist--aacgm
    ins_fo.cd();
    // ins_exps.HIST_WriteHist();
    // ins_exps.HIST_DrawHist();
    //====savehist--tene
    ins_exps.HIST_WriteHist_TvE();
    ins_fo.Close();
}