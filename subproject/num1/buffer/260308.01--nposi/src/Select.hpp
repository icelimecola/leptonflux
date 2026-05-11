_Pragma("once")

#include <iostream>
using namespace std;
#include "SelectInit.hpp"
#include "SelectProcEvent.hpp"

class Select: public SelectInit,public SelectProcEvent{
    public:
        Select():SelectInit(),SelectProcEvent(){cout<<"Constructor_select"<<endl;}
        ~Select(){}
};
