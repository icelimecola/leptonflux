_Pragma("once")

#include <iostream>
using namespace std;
#include "Include_GCX/ToolFileName.hpp"
#include "ExpsHist.hpp"

class ExpsHistRun{
    public:
        ExpsHistRun(){cout<<"Constructor_AddRun"<<endl;}
        ExpsHistRun(TString fn,int jid,int nfpj):
        jid(jid),nfpj(nfpj),ins_filename(fn),ins_expshist()
        {cout<<"Constructor_AddRun"<<endl;}
        ~ExpsHistRun(){}
    public:
        int jid,nfpj;
        ToolFileName ins_filename;
        ExpsHist ins_expshist;
    public:
        void EXPSHIST_RUN();
};

void ExpsHistRun::EXPSHIST_RUN(){
    //==================================================== EXPSHIST_AddHist
    //-- 累加hist
    ins_filename.InitFileName();
    // ins_expshist.EXPSHIST_AddHist(ins_filename.GetFullName(),jid,nfpj);
    ins_expshist.EXPSHIST_AddHist_dayVSene(ins_filename.GetFullName(),jid,nfpj);
    //==================================================== SaveHist
    //-- 初始化hist输出文件,写入hist
    TFile ins_fileout("hexps.root", "recreate");
    ins_fileout.cd();
    // ins_expshist.EXPSHIST_WriteHist();
    ins_expshist.EXPSHIST_WriteHist_dayVSene();
    ins_fileout.Close();
    //==================================================== EXPSHIST_DrawHist
    //-- 初始化canvas输出文件,写入canvas
    TFile ins_fo("cexps.root", "recreate");
    ins_fo.cd();
    // ins_expshist.EXPSHIST_DrawHist();
    ins_expshist.EXPSHIST_DrawHist_dayVSene();
    ins_fo.Close();
}