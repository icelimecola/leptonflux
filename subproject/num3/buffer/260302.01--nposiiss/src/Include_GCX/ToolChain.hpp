_Pragma("once")

#include<iostream>
using namespace std;
#include "TString.h"
#include "TChain.h"



class ToolChain{
    public:
        ToolChain(){cout<<"IN ToolChain ===== Constructor_toolchain"<<endl;}
        ToolChain(TString tn):ins_chain(new TChain(tn))
        {
            cout<<"Constructor_toolchain"<<endl;
        }
        ~ToolChain(){}
    public:
        TChain* ins_chain;
    public:
        virtual int GetEntries();
        virtual TChain*& GetChain();
        virtual void InitChain();
        virtual void InitChain(TString);
        virtual void AddTree(vector<TString>,int,int,int);
        virtual void SetBranchAddress(){}
};

int ToolChain::GetEntries(){
    return ins_chain->GetEntries();
}

TChain*& ToolChain::GetChain(){
    return ins_chain;
}

void ToolChain::InitChain(){
    ins_chain=new TChain();
}

void ToolChain::InitChain(TString tn){
    ins_chain=new TChain(tn);
}

void ToolChain::AddTree(vector<TString> fullfilename_vector,int jobid=1,int nfilesperjob=1,int printlv=1){
    if(printlv==1){
    cout<<"==============================================↓"<<endl;
    // cout<<"Select "<<nfilesperjob<<" Runs, From "<<fullfilename_vector.size()<<" Files, JobID: "<< jobid<<endl;
    cout<<"JobID: "<< jobid<<" , Select "<<nfilesperjob<<" FilesPerJob, From "<<fullfilename_vector.size()<<" Files"<<endl;
    cout<<"==================="<<endl;
    }
    if (fullfilename_vector.size()==1){
        jobid=1;
        nfilesperjob=1;
    }
    for(int i=nfilesperjob*(jobid-1);i<nfilesperjob*(jobid);i++){
        if(i<0)break;
        if(i>=fullfilename_vector.size())break;
        if(printlv==1){
        cout<<"Adding File : "<<fullfilename_vector.at(i)<<endl;
        cout<<"==================="<<endl;
        }
        ins_chain->Add( fullfilename_vector.at(i) );
    }
    if(printlv==1){
	cout<<"Totally Add "<<ins_chain->GetEntries()<<" entries in to the chain"<<endl;
    cout<<"==================="<<endl;
    cout<<"==============================================↑"<<endl;
    }
}

