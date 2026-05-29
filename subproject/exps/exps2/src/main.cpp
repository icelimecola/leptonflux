#include <iostream>
using namespace std;
#include "ExpsHistRun.hpp"

int main(int argc, char *argv[]){
    //==================================================== initialization
    //-- 声明
    TString filename;
    int jobid;
	int nfilesperjob;
    //-- 初始化
    filename="expshist.list";
    jobid=1;
	nfilesperjob=10;
    //-- 传递
	if( argc > 1 ) filename = argv[1];
	if( argc > 2 ) jobid = atoi(argv[2]);
	if( argc > 3 ) nfilesperjob = atoi(argv[3]);
    //==================================================== mergerun
    ExpsHistRun ins_expshist(filename,jobid,nfilesperjob);
    ins_expshist.EXPSHIST_RUN();
    //==================================================== return
    return 0;
}