_Pragma("once")

#include<iostream>
using namespace std;
#include "TFile.h"

class ToolFileOut{
    public:
        ToolFileOut():ins_file(new TFile){cout<<"IN ToolFileOut ===== Constructor_toolfileout-nopram"<<endl;}
        ToolFileOut(char *fn):ins_file(new TFile(fn,"recreate")){cout<<"IN ToolFileOut ===== Constructor_toolfileout-pram"<<endl;}
        ~ToolFileOut(){}
    public:
        TFile* ins_file;
    public:
        virtual TFile*& GetFileOut();
        virtual void InitFileOut(TString);
        virtual void WriteFileOut();
        void DeleteFileOut();
};

TFile*& ToolFileOut::GetFileOut(){
    return ins_file;
}
void ToolFileOut::InitFileOut(TString fn){
    ins_file=new TFile(fn,"recreate");
}
void ToolFileOut::WriteFileOut(){
    ins_file->cd();
    ins_file->Write();
}
void ToolFileOut::DeleteFileOut(){
    delete ins_file;
}