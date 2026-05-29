_Pragma("once")

#include<iostream>
using namespace std;
#include "TTree.h"
#include "TDatime.h"

class ToolTreeArray{
    public:
        ToolTreeArray():ntree(),ins_treearray(){cout<<"IN ToolTreeArray ===== Constructor_tooltreearray"<<endl;}
        ToolTreeArray(int n):ntree(n),ins_treearray(new TTree*[ntree]){cout<<"IN ToolTreeArray ===== Constructor_tooltreearray"<<endl;}
        ~ToolTreeArray(){}
    public:
        int ntree;
        TTree** ins_treearray;
    public:
        virtual int GetNTree();
        virtual TTree**& GetTreeArray();
        virtual void InitTreeArray(int);
        virtual void InitTreeArray(TString,TString);
        virtual void FillTreeArray();
        virtual void FillTreeArray(int);
        virtual void WriteTreeArray();
        virtual void BuildBranch(){}
};

int ToolTreeArray::GetNTree(){
    return ntree;
}

TTree**& ToolTreeArray::GetTreeArray(){
    return ins_treearray;
}

void ToolTreeArray::InitTreeArray(int n){
    ntree=n;
    ins_treearray=new TTree*[ntree];
    for (int i = 0; i < ntree; i++){
        ins_treearray[i]=new TTree;
    }
}

void ToolTreeArray::InitTreeArray(TString tn,TString tt){
    for (int i = 0; i < ntree; i++){
        ins_treearray[i]=new TTree(tn,tt);
    }
}

void ToolTreeArray::FillTreeArray(){
    for (int i = 0; i < ntree; i++){
        ins_treearray[i]->Fill();
    }
}

void ToolTreeArray::FillTreeArray(int i){
    ins_treearray[i]->Fill();
}

void ToolTreeArray::WriteTreeArray(){
    cout<<"==============================================↓"<<endl;
    for (int i = 0; i < ntree; i++){
        if( ins_treearray[i]->GetEntries() > 0 ){
            TDatime t;
            cout <<Form("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond()) << " Tree["<<i<<"]->GetEntries() = " << ins_treearray[i]->GetEntries() <<" will be writed into the file"<< endl;
            ins_treearray[i]->Write();
        }
    }
    cout<<"==============================================↑"<<endl;
}