#include <iostream>
using namespace std;
#include <algorithm>
#include "TString.h"
#include "TROOT.h"
#include "FitRun.hpp"


int main(int argc, char *argv[]){
    gROOT->SetBatch(kTRUE);
    TH1::AddDirectory(kFALSE);
    //==================================================== init
    //---- 声明
    TString filename="bin02.root";
    int i_enebin=0;
    //---- 传递
    if( argc > 1 ) filename = argv[1];
    if( argc > 2 ) i_enebin = atoi(argv[2]);
    //==================================================== init--bindivision
    //-- 1. nolatbin--1
    //-- 2. NoPeriod--1
    //-- 3. DayPeriod--1
    int bin_division_value=10;
    if( argc > 3 ) bin_division_value = atoi(argv[3]);
    cout<<"IN main ===== filename "+filename<<" i_enebin "<<i_enebin<<" bin_division_value "<<bin_division_value<<endl;
    vector<int>bin_division;
    while(bin_division_value>0){
        bin_division.push_back(bin_division_value%10);
        bin_division_value/=10;
    }
    reverse(bin_division.begin(),bin_division.end());
        std::cout <<"====bin_division========";
        for (int i = 0; i < bin_division.size(); ++i) {
            std::cout<<"["<<i<<"]: "<< bin_division[i] << " ;";
        }
        std::cout<<endl;
    //==================================================== run
    FitRun ins_fit(filename);
    ins_fit.FIT_RUN_iene(i_enebin,bin_division,0);
    //==================================================== return
    return 0;
}

