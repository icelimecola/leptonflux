_Pragma("once")

#include <iostream>
using namespace std;
#include "TDatime.h"

#include "Include_GCX/ToolFileName.hpp"
#include "Include_GCX/ToolChain.hpp"
#include "Include_GCX/ToolFileOut.hpp"
#include "Include_GCX/ToolTreeArray.hpp"
#include "VarEneBin.hpp"
#include "Merge.hpp"

class MergeRun{
    public:
        MergeRun(){};
        //-- 使用run内部默认treename
        MergeRun(TString fn,int jid,int nfpj):
            jid(jid),nfpj(nfpj),
            ins_filename(fn),ins_chain(),ins_treearray(),ins_fileout(),
            ins_me()
            {cout<<"Constructor_mergerun"<<endl;}
        //-- 使用外部treename
        // MergeRun(TString fn,int jid,int nfpj,TString tn):
        //     jid(jid),nfpj(nfpj),
        //     ins_filename(fn),ins_chain(tn)
        //     {cout<<"Constructor_mergerun"<<endl;}
        ~MergeRun(){};
    public:
        int jid,nfpj;
    public:
        ToolFileName ins_filename;
        ToolChain ins_chain;
        ToolFileOut ins_fileout;
        ToolTreeArray ins_treearray;
        VarEneBin ins_varenebin;
    public:
        Merge ins_me;
    public:
        void MERGE_RUN_i(int);
};

void MergeRun::MERGE_RUN_i(int i_enebin){
    //======== init
    //-- 将list压入vector
    ins_filename.InitFileName();
    TString treename;
    //-- 初始化ttree名
    treename=Form("tbin%02d", i_enebin);
    //-- 初始化tchain
    ins_chain.InitChain(treename);
    ins_chain.GetChain()->SetCacheSize(200*1024*1024);
    ins_chain.GetChain()->AddBranchToCache("*", kTRUE);
    //-- 从vector中选取对应的文件添加同名ttree
    ins_chain.AddTree(ins_filename.GetFullName(),jid,nfpj);
    //-- 将tchain中的branch和select中变量链接
    ins_me.INIT_CHAIN_SetBranchAddress(ins_chain.GetChain());
    //-- 初始化输出文件
    //* 这里是一个enebin一个文件,等于依次循环一个新文件
    ins_fileout.InitFileOut(Form("bin%02d.root", i_enebin));
    //-- 初始化输出ttree(bintree)
    ins_treearray.InitTreeArray(1);
    //-- 初始化bintree的branch
    ins_me.INIT_TREEARRAY_BuildBrand(ins_treearray.GetNTree(),ins_treearray.GetTreeArray(),i_enebin);
    //======== traverse
    long nentries = ins_chain.GetChain()->GetEntries();
    long print_step = nentries/100;
    if(print_step<1) print_step=1;
    for(long entry=0; entry<nentries; entry++){
        TDatime t;
        if(entry%print_step==0 || entry==nentries-1){
            double progress = 100.0*(entry+1)/nentries;
            cout<<Form("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond())
                <<" Processing entry in merge "<<entry<<" / "<<nentries
                <<" ("<<Form("%.2f", progress)<<"%)"<<endl;
        }
        ins_chain.GetChain()->GetEntry(entry);
        ins_treearray.FillTreeArray(0);
    }
    //======== save
    ins_fileout.GetFileOut()->cd();
    ins_treearray.WriteTreeArray();
    ins_fileout.GetFileOut()->Close();
}
