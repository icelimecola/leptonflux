_Pragma("once")

#include<iostream>
using namespace std;
#include<TBranch.h>
#include "VarMinitreeINPUT.hpp"

class VarBuffertree: public VarMinitreeINPUT{
    public:
        VarBuffertree():VarMinitreeINPUT(){cout<<"IN VarBuffertree ===== Constructor_varbuffertree"<<endl;}
        ~VarBuffertree(){}
    public:
        Int_t       amsday, // day since 2011-05-19
                    day, // day in yyyy-mm-dd
                    dayID,
                    br; // bartels rotation number
		Float_t     EmLkhd;////
		Float_t     ccbdt, ccbdtv;
		Int_t       bin;
		Int_t       datatype;
		Float_t     eop, ecop; // log10(E/|R|)
		Float_t     sf, sf40; // Stormer cutoff safety factor
		Float_t     sfi, sfi40; // IGRF cutoff safety factor
		double      mlat;
		double      mlon;
		double      rr;
        TFile       *file_output;
    public:
        int EventSelectEffType;
        //==== lf
        Float_t lf;
        Float_t lfc;
};