_Pragma("once")

#include <iostream>
using namespace std;
#include "MergeInit.hpp"

class Merge: public MergeInit{
    public:
        Merge():MergeInit(){cout<<"Constructor_merge"<<endl;}
        ~Merge(){}
};
