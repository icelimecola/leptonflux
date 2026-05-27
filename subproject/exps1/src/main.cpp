#include <iostream>
using namespace std;
#include "TString.h"
#include "TROOT.h"
#include "ExpsRun.hpp"

int main(int argc, char *argv[]){
    gROOT->SetBatch(kTRUE);
    TH1::AddDirectory(kFALSE);
    //==== init
    TString filename;
    int jobid=1;
	int nfilesperjob=10;
	if( argc > 1 ) filename = argv[1];
	if( argc > 2 ) jobid = atoi(argv[2]);
	if( argc > 3 ) nfilesperjob = atoi(argv[3]);
    TString treename="t_add";
    cout<<"========filename: "<<filename<<" treename: "<<treename<<endl;
    //==== run
    ExpsRun ins_exps(filename,jobid,nfilesperjob,treename);
    ins_exps.EXPS_RUN_02();
    //==== return
    return 0;
} 