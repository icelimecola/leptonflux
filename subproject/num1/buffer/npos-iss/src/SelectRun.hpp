_Pragma("once")

#include <iostream>
using namespace std;
#include <TString.h>
#include <TDatime.h>
#include <TTimeStamp.h>
//====
#include "Include_GCX/ToolFileName.hpp"
#include "Include_GCX/ToolChain.hpp"
#include "Include_GCX/ToolTreeArray.hpp"
#include "Include_GCX/ToolFileOut.hpp"
//====
#include "Select.hpp"
//====
#include "root_RVSP.h"
#include "../third_party/c_aacgm_v2.6/aacgmlib_v2.h"
#include "../third_party/c_aacgm_v2.6/mlt_v2.h"
//====


class SelectRun{
    public:
        SelectRun(){cout<<"Constructor-selectrun"<<endl;}
        SelectRun(TString fn,int jid,int nfpj,TString tn):
        filename(fn),jobid(jid),nfilesperjob(nfpj),
        ins_filename(fn),ins_chain(tn),ins_treearray(),ins_fileout(),
        ins_select()
        {cout<<"Constructor-selectrun"<<endl;}
        ~SelectRun(){}
    public:
        TString filename;
        int jobid;
        int nfilesperjob;
        vector<TString> fullfilename;
    public:
        ToolFileName ins_filename;
        ToolChain ins_chain;
        ToolTreeArray ins_treearray;
        ToolFileOut ins_fileout;
    public:
        Select ins_select;
    public:
        void SECLECT_RUN();
};

void SelectRun::SECLECT_RUN(){
    //============ init ============
    //====filename
    ins_filename.InitFileName();
    fullfilename=ins_filename.GetFullName();
    //====chain
    ins_chain.AddTree(fullfilename,jobid,nfilesperjob);
    ins_select.INIT_CHAIN_SetBranchAddress(ins_chain.GetChain());
    //====fout&tree
    ins_fileout.InitFileOut(Form("buffer_%03d.root", jobid));
    ins_treearray.InitTreeArray(ins_select.ENEBIN_GetNEneBin());
    ins_select.INIT_TREEARRAY_BuildBrand(ins_treearray.GetNTree(),ins_treearray.GetTreeArray());
    
    //============ select ============
    //====init
    long nentries = ins_chain.GetChain()->GetEntries();
    long print_step = nentries/100;
    if(print_step<1) print_step=1;
    //====traversal
    for(long entry=0; entry<nentries; entry++){
        //====print
        TDatime t;
        if(entry%print_step==0 || entry==nentries-1){
            double progress = 100.0*(entry+1)/nentries;
            cout<<Form("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond())
                <<" Processing entry "<<entry<<" / "<<nentries
                <<" ("<<Form("%.2f", progress)<<"%)"<<endl;
        }
        //====getentry
        ins_chain.GetChain()->GetEntry(entry);
        //====ene
        ins_select.PROCESS_CalcEnebinIndex();
            if(ins_select.PROCESS_SEL_IsWrongEnebin()) continue;
        //====cut--badlf  (same as tmini)
        if(ins_select.PROCESS_SELECT_IsISS() && ins_select.lf<0.05) continue;  
        //====cut--badrun
        // if(ins_select.PROCESS_SELECT_IsISS() && ins_select.isbadrun) continue;
        // if(ins_select.PROCESS_SELECT_IsISS() && (ins_select.isbadrun_daq || ins_select.isbadrun_fov || ins_select.isbadrun_ecal || ins_select.isbadrun_trk)) continue;
        //====cut--time
        // if(ins_select.utime<1305763200) continue;
        // if(ins_select.utime>1385510400) continue;
        // if(ins_select.utime>1510444800) continue;
        //----260419
        // if(ins_select.utime<1305849600) continue;   //----2011.05.20
        // if(ins_select.utime>1635811200) continue;   //----2021.11.02
        //====cut--geomag
        //----stormer
        // if(ins_select.energy_bins[ins_select.PROCESS_GetEnebinIndex()]<1.2*ins_select.cf[0]) continue;
        //----igrf
        // if(ins_select.energy_bins[ins_select.PROCESS_GetEnebinIndex()]<1.0*ins_select.cfi[0]) continue;
        // if(ins_select.energy_bins[ins_select.PROCESS_GetEnebinIndex()]<1.1*ins_select.cfi[0]) continue;
        // if(ins_select.energy_bins[ins_select.PROCESS_GetEnebinIndex()]<1.2*ins_select.cfi[0]) continue;
        //====cut--rig
        if(ins_select.rig>=0) continue;     //----260516
        //==== reset
        ins_select.PROCESS_ResetVar();
        //====process
        ins_select.PROCESS_InitVar();   //-- 利用传入的变量给输出的变量赋值
        ins_select.PROCESS_Classify();  //-- 设置datatype
        //====cut--presel
        if(!ins_select.PROCESS_SELECT_Preselect()) continue;
        //============ rti ============
        //========init rti
        AMSSetupR::RTI rti;
        AMSSetupR::RTI::UseLatest(8);
        UInt_t utime = ins_select.utime;
        int ret = AMSEventR::GetRTI( rti, utime );
        if( ret > 1 ){
            cout << "no official RTI found, ret = " << ret << endl;
        }else{
            //========mlat
            //==== rti value
            double info_theta, info_phi, info_r;
            info_theta = rti.theta*180.0/3.14159;
            info_phi = rti.phi*180.0/3.14159;
                if(info_phi>180.0) {info_phi=info_phi-360.0;}
            info_r =rti.r;
            //==== position--GTOD
            double LAT=info_theta;
            double LON=info_phi;
            double ALT=(info_r/100.0-6371393)/1000.00;
            // cout<<"GTOD_POSITION= "<<LAT<<","<<LON<<","<<ALT<<endl;
            //==== time
            TTimeStamp TT(ins_select.utime);
            int date=TT.GetDate();
                int year=date/10000;
                int mon=TT.GetMonth();
                int day=date%100;
            int time=TT.GetTime();
                int hr=time/10000;
                int min=(time%10000)/100;
                int sec=time%100; 
            // cout<<"TIME= "<<date<<","<<year<<","<<mon<<","<<day<<","<<hr<<","<<min<<","<<sec<<endl;
            //==== AACGM
            double mlat,mlon,rr;
            AACGM_v2_SetDateTime(year,mon,day,hr,min,sec);
            AACGM_v2_Convert(LAT,LON,ALT,&mlat,&mlon,&rr,G2A);
            // cout<<"GTOD_POSITION"<<LAT<<","<<LON<<","<<ALT<<endl;
            // cout<<"AACGM_POSITION"<<mlat<<","<<mlon<<","<<rr<<endl;
            //==== position--AACGM
            ins_select.mlat = mlat;
            ins_select.mlon = mlon;
            ins_select.rr = rr;
        }
        //==== fill
        if(ins_select.PROCESS_SELECT_ShouldFill()) ins_treearray.FillTreeArray(ins_select.PROCESS_GetEnebinIndex());
    }
    
    //============ save ============
    ins_fileout.GetFileOut()->cd();
    ins_treearray.WriteTreeArray();
}