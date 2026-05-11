#include "MyBDT_ChargePID.h"

const int MyBDT_ChargePID::nbin = 68; // Positron Fraction / Lepton Bin ?
const double MyBDT_ChargePID::rigBins[69] = {
    0.50, 0.65, 0.81, 1.00, 1.21, 1.45, 1.70, 1.97, 2.28, 2.60,
    2.94, 3.30, 3.70, 4.11, 4.54, 5.00, 5.50, 6.00, 6.56, 7.16,
    7.80, 8.50, 9.21, 9.95, 10.73, 11.54, 12.39, 13.27, 14.19, 15.15,
    16.15, 17.18, 18.25, 19.37, 20.54, 21.76, 23.07, 24.45, 25.87,27.34,
    28.87, 30.45, 32.10, 33.80, 35.57, 37.40, 40.00, 43.39, 47.01,50.87,
    54.98, 59.36, 64.03, 69.00, 74.30, 80.00, 86.00, 92.50, 100.00,115.10,
    132.10, 151.50, 173.50, 206.00, 260.00, 350.00,500.00,700.00,1000.00
};


using namespace std;


int MyBDT_ChargePID::FindBin(float x){
    static int i=0;
    for(i=0;i<nbin;i++){
        if(fabs(x) >= rigBins[i] && fabs(x)<rigBins[i+1]) return i;
    }
    return i;
}


MyBDT_ChargePID::MyBDT_ChargePID()
{
    t_input=0;
    reader=0;
    factory=0;

    //    Use["Cut"]       =1 ;
    Use["BDTG"]            = 1; // uses Gradient Boost
    Use["BDT"]            = 1; // uses Adaptive Boost

    std::cout << std::endl;
    std::cout << "==> Start TMVAClassification" << std::endl;


    memset(var,0,100*sizeof(float));
    memset(spec,0,100*sizeof(float));



}


void MyBDT_ChargePID::Config(TString s_config){

    fconf=new ConfigParser(s_config.Data());


}

void MyBDT_ChargePID::DefineVariables(){

    if(t_input){
        for(int i=0;i<100;i++){
            string svar=fconf->Value("UseVariables",Form("usevar%i",i));
            if(svar.size()){
                cout<<"Enable Variable:"<<svar<<endl;
                t_input->SetBranchStatus(svar.data(),1);
            }
        }
    }

    for(int i=0;i<100;i++){
        string svar=fconf->Value("Variables",Form("var%i",i));
        if(svar.size())AddVariable(TString(svar),&(var[i]));
    }

    for(int i=0;i<100;i++){
        string svar=fconf->Value("Spectators",Form("spec%i",i));
        if(svar.size())AddSpectator(TString(svar),&(spec[i]));
    }

    return;
}



void MyBDT_ChargePID::AssignValues(){




    return;
}


int MyBDT_ChargePID::PassPreselection(){
    //    if(TrTrack_Rigidity_MaxSpan==0)return 0;
    //    if(trd_nhit<12)return 0;
    //    if(betah<0.5)return 0;
    //    if((pattern&257)!=257) return 0;
    //    if(tofhq>1.5 || tofhq==0)return 0;
    //    if(bdt>-0.8)return 0;
    //    if(trd<0.8)return 0;
    return 1;
}

void MyBDT_ChargePID::PrepareForTraining_SelectFromTree(){


    std::string s_signal;
    std::string s_background;
    std::string s_common;

    s_common=fconf->Value("Selection","Common");
    s_signal=fconf->Value("Selection","Signal");
    s_background=fconf->Value("Selection","Background");

    TCut cut_common=s_common.data();
    TCut cut_signal=s_signal.data();
    TCut cut_background=s_background.data();

    cout<<"Common Cut:  " <<cut_common<<endl;
    cout<<"Cut for signal :  "<<cut_signal<<endl;
    cout<<"Cut for background :  "<<cut_background<<endl;



    string s_sig=fconf->Value("Selection","NSig");
    string s_bkg=fconf->Value("Selection","NBkg");

    double nsig=atoi(s_sig.data());
    double nbkg=atoi(s_bkg.data());




    cout<<"Select Signal tree..........................."<<endl;

    t_signal=t_input->CopyTree(cut_common+cut_signal,"",nsig);

    cout<<"Select Background tree..........................."<<endl;

    t_background=t_input->CopyTree(cut_common+cut_background,"",nbkg);

    cout<<"Nentries: " <<t_signal->GetEntries()<<","<<t_background->GetEntries()<<endl;



    cout<<"Signal Tree: " <<endl;
    t_signal->Print("*");

    // global event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight     = 1.0;
    Double_t backgroundWeight = 1.0;
    factory->AddSignalTree    ( t_signal,     signalWeight     );
    factory->AddBackgroundTree( t_background, backgroundWeight );

    cout<<" Select Tree Finished............."<<endl;
}


void MyBDT_ChargePID::BookMethods(){
    std::cout << "--- TMVAClassification     Book Methods:"<< std::endl;
    // Cut optimisation
    if (Use["Cuts"])
        factory->BookMethod( TMVA::Types::kCuts, "Cuts",
                             "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

    // Likelihood ("naive Bayes estimator")
    if (Use["Likelihood"])
        factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood",
                             "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );


    // Boosted Decision Trees
    if (Use["BDTG"]) // Gradient Boost
        factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                             "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:GradBaggingFraction=0.5:nCuts=20:MaxDepth=3:IgnoreNegWeightsInTraining");

    if (Use["BDT"])  // Adaptive Boost
        factory->BookMethod( TMVA::Types::kBDT, "BDT",
                             "!H:!V:NTrees=1000:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );


}

void MyBDT_ChargePID::PrepareForTraining(){

    SetInputTree();

    SetFactory();


    DefineVariables();
    PrepareForTraining_SelectFromTree();


    TCut mycuts = "";
    TCut mycutb = "";

    std::cout << "--- TMVAClassification     Prepare Tree:"<< std::endl;
    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                         "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );


    BookMethods();

}

void MyBDT_ChargePID::SetInputTree()
{
    t_input=new TChain("t_add","t_add");
    string s_file1=fconf->Value("Input","File1");
    cout<<"Add File:   "<<s_file1<<endl;
    t_input->Add(TString(s_file1));

    string s_file2=fconf->Value("Input","File2");
    cout<<"Add File:   "<<s_file2<<endl;
    t_input->Add(TString(s_file2));


    t_input->SetBranchStatus("*",0);

}

void MyBDT_ChargePID::SetFactory()
{
    string svar=fconf->Value("General","Version");
    TString prefix = svar;
    Option=prefix;
    TString outfileName( Form("TMVA_%s.root",Option.Data()) );
    outputFile = TFile::Open( outfileName, "RECREATE" );
    factory = new TMVA::Factory( Option, outputFile,
                                 "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
}


void MyBDT_ChargePID::DoTraning(){

    std::cout << "---------------------------------------------------- TMVAClassification     Train Methods: ----------------------------------------------------"<< std::endl;
    factory->TrainAllMethods();
    std::cout << "---------------------------------------------------- TMVAClassification     Test  Methods: ----------------------------------------------------"<< std::endl;
    factory->TestAllMethods();
    std::cout << "---------------------------------------------------- TMVAClassification     Evaluate Methods: ----------------------------------------------------"<< std::endl;
    factory->EvaluateAllMethods();

    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

}

MyBDT_ChargePID::~MyBDT_ChargePID()
{
    if(factory)delete factory;
    if(reader)delete reader;
}


float MyBDT_ChargePID::Eval_BDTG(){
    return  reader->EvaluateMVA("BDTG method");
}

float MyBDT_ChargePID::Eval_BDT(){
    return  reader->EvaluateMVA("BDT method");
}


void MyBDT_ChargePID::PrepareForReading(){

    reader = new TMVA::Reader( "!Color:!Silent" );

    DefineVariables();


    string svar=fconf->Value("General","Version");
    TString prefix = svar;



    // TString dir    = "/afs/cern.ch/work/t/tsu/BDT_ChargePID/weights/";
    TString dir    = "/afs/cern.ch/work/c/chguan/public/BDT_ChargePID/weights/";
//TString dir = "/afs/cern.ch/work/w/wxu/work/ams/analysis/positron/Positron2017/WXU/data/";
    // Book method(s)
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
        if (it->second) {
            TString methodName = TString(it->first) + TString(" method");
            TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
            reader->BookMVA( methodName, weightfile );
        }
    }

}



void MyBDT_ChargePID::DoTesting(bool IsSignal){

    TFile* finput;
    TFile *fout;

    if(IsSignal){
        finput=new TFile("sample_signal.root");
    }else{
        finput=new TFile("sample_background.root");
    }

    TTree* tin=(TTree*)finput->Get("t_add");

    double bdtg_Chargepid;
    double bdta_Chargepid;

    if(IsSignal){
        fout=new TFile("sample_signal_BDT.root","RECREATE");
    }else{
        fout=new TFile("sample_background_BDT.root","RECREATE");
    }

    TTree *tout=(TTree*)tin->CloneTree(0);

    tout->Branch("bdtg_Chargepid",&bdtg_Chargepid,"bdtg_Chargepid/D");
    tout->Branch("bdta_Chargepid",&bdta_Chargepid,"bdta_Chargepid/D");

    Init(tin);

    cout<<"Entries: " <<fChain->GetEntries()<<endl;
    double nentries=fChain->GetEntries();
    for(int i=0;i<nentries;i++){


        if(i%10000==0){
            cout<<"Processed: " <<i<<"/"<<nentries<<endl;
        }
        fChain->GetEntry(i);


        var[0]=tof_qup;
        var[1]=tof_qlow;
        var[2]=trd_elprob[0];
        var[3]=trd_prprob[0];
        var[4]=trd_heprob[0];
        var[5]=trk_qin;
        var[6]=trk_qinrms;
        var[7]=trk_qinup;
        var[8]=trk_qinlow;



        bdtg_Chargepid=Eval_BDTG();
        bdta_Chargepid=Eval_BDT();

        tout->Fill();
    }


    fout->cd();
    tout->Write();
    fout->Close();

}






