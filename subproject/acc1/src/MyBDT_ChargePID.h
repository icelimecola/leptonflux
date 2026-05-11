#ifndef MyBDT_ChargePID_H
#define MyBDT_ChargePID_H
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include "math.h"
#include "t_add.h"

#include "ConfigParser.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMath.h"
#include "TProofOutputFile.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

class MyBDT_ChargePID : public t_add
{

public:
    MyBDT_ChargePID();
    ~MyBDT_ChargePID();

    void Config(TString s_config);
    static const double rigBins[69];
    static const int nbin ;

    TMVA::Factory *factory;
    TMVA::Reader *reader;
    TFile* outputFile;

    ConfigParser*  fconf;


    float var[100];
    float spec[100];


    TTree* ts;

    TTree *t_signal     ;
    TTree *t_background ;

    int ibin;
    TString Option;
    std::map<std::string,int> Use;

    void DoTesting(bool IsMC);

    void DoTraning();
    void PrepareForTraining();

    void SetInputTree();
    void SetFactory();

    void DefineVariables();

    void PrepareForTraining_SelectFromTree();

    TChain* t_input;



    void PrepareForReading();


    float Eval_BDTG();
    float Eval_BDT();


    void AddVariable(TString name){
        std::cout<<"Add Variable: "<< name<<std::endl;
        if(factory)factory->AddVariable(name);
    }


    template<typename T>  void AddVariable(TString name , T* _ptr){
        std::cout<<"Add Variable: "<< name<<std::endl;
        if(factory)factory->AddVariable(name);
        if(reader)reader->AddVariable(name,_ptr);
    }

    void AddSpectator(TString name){
        std::cout<<"Add Spectator: "<< name<<std::endl;
        if(factory)factory->AddSpectator(name);
    }



    template<typename T> void AddSpectator(TString name , T* _ptr){
        std::cout<<"Add Spectator: "<< name<<std::endl;
        if(factory)factory->AddSpectator(name);
        if(reader)reader->AddSpectator(name,_ptr);
    }

    //========Variable



    void BookMethods();
    int PassPreselection();
    void AssignValues();
    int FindBin(float x);


};

#endif // MyBDT_ChargePID_H
