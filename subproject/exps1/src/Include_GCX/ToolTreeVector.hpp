_Pragma("once")

#include<iostream>
using namespace std;
#include "TTree.h"

class ToolTreeVector{
    public:
        ToolTreeVector():ntree(),ins_treevector(){cout<<"Constructor_ToolTreeVector"<<endl;}
        ToolTreeVector(int n):ntree(n),ins_treevector(new TTree*[ntree]){cout<<"Constructor_ToolTreeVector"<<endl;}
        ~ToolTreeVector(){}
    public:
        int ntree;
        vector<TTree*> ins_treevector;
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

int ToolTreeVector::GetNTree(){
    return ntree;
}

TTree**& ToolTreeVector::GetTreeArray(){
    return ins_treevector;
}

void ToolTreeVector::InitTreeArray(int n){
    ntree=n;
    ins_treevector=new TTree*[ntree];
    for (int i = 0; i < ntree; i++){
        ins_treevector[i]=new TTree;
    }
}

void ToolTreeVector::InitTreeArray(TString tn,TString tt){
    for (int i = 0; i < ntree; i++){
        ins_treevector[i]=new TTree(tn,tt);
    }
}

void ToolTreeVector::FillTreeArray(){
    for (int i = 0; i < ntree; i++){
        ins_treevector[i]->Fill();
    }
}

void ToolTreeVector::FillTreeArray(int i){
    ins_treevector[i]->Fill();
}

void ToolTreeVector::WriteTreeArray(){
    cout<<"==============================================↓"<<endl;
    for (int i = 0; i < ntree; i++){
        if( ins_treevector[i]->GetEntries() > 0 ){
            cout << "Tree["<<i<<"]->GetEntries() = " << ins_treevector[i]->GetEntries() <<" will be writed into the file"<< endl;
            ins_treevector[i]->Write();
        }
    }
    cout<<"==============================================↑"<<endl;
}