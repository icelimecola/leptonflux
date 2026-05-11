_Pragma("once")

#include <iostream>
using namespace std;
#include "ExpsInit.hpp"
#include "ExpsAMSRTI.hpp"
#include "ExpsHist.hpp"
#include "ExpsProcEvent.hpp"

class Exps:public ExpsInit,public ExpsAMSRTI,public ExpsHist,public ExpsProcEvent{
    public:
        Exps():ExpsInit(),ExpsAMSRTI(),ExpsHist(),ExpsProcEvent(){cout<<"Constructor_expsrun"<<endl;}
        Exps(TString fn):ExpsInit(),ExpsAMSRTI(),ExpsHist(),ExpsProcEvent(fn){cout<<"Constructor_expsrun"<<endl;}
        ~Exps(){}
};
