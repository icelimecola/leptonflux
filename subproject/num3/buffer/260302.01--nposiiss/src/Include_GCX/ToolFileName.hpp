_Pragma("once")

#include<iostream>
using namespace std;
#include <fstream>
#include "TString.h"

class ToolFileName{
    public:
        ToolFileName(){};
        ToolFileName(TString fn):filename(fn){cout<<"Constructor_toolfilename"<<endl;};
        ~ToolFileName(){};
    public:
        TString filename; //input
        vector<TString> fullfilename_v; //output
    public:
        virtual vector<TString> GetFullName();
        virtual void InitFileName();
};

vector<TString> ToolFileName::GetFullName(){
    return fullfilename_v;
}

void ToolFileName::InitFileName(){
    TString fullfilename;
    //===
    TString prefix = "root://castorpublic.cern.ch//";
    TString server = "?svcClass=amsuser";
    // TString eosprefix = "root://eosams.cern.ch//";
    TString eosprefix = "/lustre02/data";
    //===
    if(filename.Contains("root")){
        fullfilename=filename;
        // if( fullfilename.EndsWith("root") ){
        //     if( fullfilename.BeginsWith("/castor") )
        //         fullfilename = prefix + fullfilename + server;
        //     else if( fullfilename.BeginsWith("/eos"))
        //         fullfilename = eosprefix + fullfilename;
        // }
        fullfilename_v.push_back(fullfilename);
        // cout<<"fullfilename: "<<fullfilename<<endl;
    }
    else{
        ifstream tempstream;
        tempstream.open( filename );
        while(1){
            tempstream >> fullfilename;
            if( tempstream.eof() ) break;
            if( fullfilename.BeginsWith("#") ) continue;
            // if( fullfilename.EndsWith("root") ){
            //     if( fullfilename.BeginsWith("/castor") )
            //         fullfilename = prefix + fullfilename + server;
            //     else if( fullfilename.BeginsWith("/eos"))
            //         fullfilename = eosprefix + fullfilename;
            // }
            fullfilename_v.push_back(fullfilename);
            // cout<<"fullfilename: "<<fullfilename<<endl;
        }
    }
}