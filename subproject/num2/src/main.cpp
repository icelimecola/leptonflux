#include <iostream>
using namespace std;
#include "TROOT.h"
#include "TH1.h"
#include "MergeRun.hpp"

int main(int argc, char *argv[]){
    gROOT->SetBatch(kTRUE);
    TH1::AddDirectory(kFALSE);
    TDirectory::TContext context(0);
    //==================================================== init
    //-- 声明 & 初始化
    TString filename="buffertree.list";
    int jobid=1;
    int nfilesperjob=10;
    int i_enebin=0;
    //-- 传参
    if( argc > 1 ) filename = argv[1];
	if( argc > 2 ) jobid = atoi(argv[2]);
	if( argc > 3 ) nfilesperjob = atoi(argv[3]);
	if( argc > 4 ) i_enebin = atoi(argv[4]);
    //==================================================== mergerun
    MergeRun ins_merge(filename,jobid,nfilesperjob);
    ins_merge.MERGE_RUN_i(i_enebin);
    //==================================================== return
    return 0;
}