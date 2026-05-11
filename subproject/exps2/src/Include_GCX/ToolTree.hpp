_Pragma("once")

#include<iostream>
using namespace std;
#include "TTree.h"

class ToolTree{
    public:
        ToolTree():ins_tree(new TTree){cout<<"IN ToolTree ===== Constructor_tooltree"<<endl;}
        ~ToolTree(){}
    public:
        TTree* ins_tree;
    public:
        virtual TTree*& GetTree();
        virtual void InitTree(TString,TString);
        virtual void FillTree();
        virtual void WriteTree();
        virtual void BuildBranch(){}
};

TTree*& ToolTree::GetTree(){
    return ins_tree;
}

void ToolTree::InitTree(TString tn,TString tt){
    ins_tree=new TTree(tn,tt);
}


void ToolTree::FillTree(){
    ins_tree->Fill();
}

void ToolTree::WriteTree(){
    cout<<"==============================================↓"<<endl;
	cout << "Tree->GetEntries() = " << ins_tree->GetEntries() <<" will be writed into the file"<< endl;
    ins_tree->Write();
    cout<<"==============================================↑"<<endl;
}