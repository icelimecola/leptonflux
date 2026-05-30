_Pragma("once")

#include <iostream>
using namespace std;
#include "FitInit.hpp"
#include "FitDraw.hpp"

class Fit:public FitInit,public FitDraw{
    public:
        Fit():FitInit(),FitDraw(){cout<<"IN Fit ===== Constructor_fit"<<endl;}
        ~Fit(){}
};