_Pragma("once")

#include<iostream>
using namespace std;
#include<vector>
#include "TTree.h"
#include "TDatime.h"

class ToolTreeArray{
    public:
        ToolTreeArray():ntree(0),ins_treearray(nullptr),lengths(),dist_nextbin(){cout<<"IN ToolTreeArray ===== Constructor_tooltreearray"<<endl;}
        ToolTreeArray(int n):ntree(0),ins_treearray(nullptr),lengths(),dist_nextbin(){
            cout<<"IN ToolTreeArray ===== Constructor_tooltreearray"<<endl;
            InitTreeArray(n);
        }
        ~ToolTreeArray(){}
    public:
        int ntree;
        TTree** ins_treearray;
        vector<int> lengths;
        vector<int> dist_nextbin;
    public:
        virtual int GetNTree();
        virtual int GetNDim();
        virtual vector<int> GetShape();
        virtual TTree**& GetTreeArray();
        virtual void InitTreeArray(int);
        virtual void InitTreeArray(const vector<int>&);
        virtual void InitTreeArray(TString,TString);
        virtual void FillTreeArray();
        virtual void FillTreeArray(int);
        virtual void FillTreeArray(const vector<int>&);
        virtual void WriteTreeArray();
        virtual void BuildBranch(){}
    private:
        virtual void CalcDistNextbin();
        virtual int CalcDistTotal(const vector<int>&);
        virtual void ClearTreeArray();
};

int ToolTreeArray::GetNTree(){
    return ntree;
}

int ToolTreeArray::GetNDim(){
    return lengths.size();
}

vector<int> ToolTreeArray::GetShape(){
    return lengths;
}

TTree**& ToolTreeArray::GetTreeArray(){
    return ins_treearray;
}


void ToolTreeArray::ClearTreeArray(){
    if(!ins_treearray) return;
    for(int i = 0; i < ntree; i++){
        if(ins_treearray[i]) delete ins_treearray[i];
    }
    delete[] ins_treearray;
    ins_treearray = nullptr;
}

void ToolTreeArray::InitTreeArray(int n){
    // ClearTreeArray();
    ntree=n;
    // shape.clear();
    // dist_nextbin.clear();
    // if(n>0) shape.push_back(n);
    // CalcDistNextbin();
    // if(ntree<=0) return;
    ins_treearray=new TTree*[ntree];
    for (int i = 0; i < ntree; i++) ins_treearray[i]=new TTree;
}

void ToolTreeArray::InitTreeArray(const vector<int>& len){
    // ClearTreeArray();
    ntree = 1;
    lengths = len;
    // CalcDistNextbin();
    for(int idim = 0; idim < lengths.size(); idim++){
        // if(lengths[d] <= 0){
        //     ntree = 0;
        //     lengths.clear();
        //     dist_nextbin.clear();
        //     return;
        // }
        ntree *= lengths[idim];
    }
    // if(ntree<=0) return;
    ins_treearray=new TTree*[ntree];
    for (int i = 0; i < ntree; i++) ins_treearray[i]=new TTree;
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
    if(i<0 || i>=ntree) return;
    ins_treearray[i]->Fill();
}


void ToolTreeArray::CalcDistNextbin(){
    dist_nextbin.clear();
    if(lengths.empty()) return;
    dist_nextbin.resize(lengths.size(),1);
    for(int i = (int)lengths.size() - 2; i >= 0; i--){
        dist_nextbin[i] = dist_nextbin[i+1] * lengths[i+1];
    }
}
int ToolTreeArray::CalcDistTotal(const vector<int>& index){
    if(index.size() != lengths.size()) return -1;
    int dist_total = 0;
    for(int idim = 0; idim < index.size(); idim++){
        if(index[idim] < 0 || index[idim] >= lengths[idim]) return -1;
        dist_total += index[idim] * dist_nextbin[idim];
    }
    return dist_total;
}
void ToolTreeArray::FillTreeArray(const vector<int>& index){
    CalcDistNextbin();
    int i = CalcDistTotal(index);
    if(i<0) return;
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