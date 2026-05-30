#include <iostream>
using namespace std;
#include "TString.h"
#include "SelectRun.hpp"

int main(int argc, char *argv[]){
    //==================================================== INIT
    //-- 声明
    TString treename;
    TString filename;
    int jobid,nfilesperjob;
    //-- 初始化
    treename="mt";
    filename="minitree_hist.list";
    jobid=1;
    nfilesperjob=10;
    //-- 传参
    if( argc > 1 ) filename = argv[1];
    if( argc > 2 ) jobid = atoi(argv[2]);
    if( argc > 3 ) nfilesperjob = atoi(argv[3]);
    //==================================================== INIT -- eventselecteff_type
    //-- 0-all 1-tof 2-trk 3-TRD
    int eventselecteff_type;
    eventselecteff_type=0;
    if( argc > 4 ) eventselecteff_type = atoi(argv[4]);
    //==================================================== RUN
    SelectRun ins_select(filename,jobid,nfilesperjob,treename);
    ins_select.SECLECT_RUN();
    //==================================================== RETURN
    return 0;
}

