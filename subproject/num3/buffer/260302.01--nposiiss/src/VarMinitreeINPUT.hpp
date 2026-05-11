_Pragma("once")

#include<iostream>
using namespace std;
#include<TBranch.h>

class VarMinitreeINPUT{
    public:
        VarMinitreeINPUT(){cout<<"IN VarMinitreeTNPUT ===== Constructor_varminitreein"<<endl;}
        ~VarMinitreeINPUT(){}
    public:
        UInt_t          run;
        UInt_t          event;
        UInt_t          utime;
        Float_t         lf;////===live time
        Float_t         lfc;////===live time
        Float_t         TTheta;
        Float_t         Phi;
        Float_t         Glong;
        Float_t         Glat;
        Float_t         Mlong;
        Float_t         Mlat;
        Float_t         Gr;
        Float_t         cf[4];
        Float_t         cfi[4];
        Float_t         poscf[4];////===正电子地磁截止刚度,基于STORMER模型
        Float_t         negcf[4];
        Float_t         poscfi[4];////===正电子地磁截止刚度,基于IGRF模型
        Float_t         negcfi[4];
        Float_t         mcp;////===MC模拟的动量
        Float_t         mcq;
        Int_t           isbadrun;
        Int_t           isbad[19];
        //----tsu
        Int_t           isbadrun_trd;
        Int_t           isbadrun_trk;
        Int_t           isbadrun_ecal;
        Int_t           isbadrun_general;
        Int_t           isbadrun_daq;
        Int_t           isbadrun_fov;
        //--------
        Int_t           lvl1_JMembPatt;
        Int_t           lvl1_PhysBPatt;
        Float_t         theta;//重建的径迹和z轴夹角
        Float_t         beta;//重建的相对论速度v/c
        Float_t         tofq;//tof重建的电荷
        Float_t         tofqlow;//lowtof重建的电荷
        Float_t         tofqup;//uptof重建的电荷
        Int_t           nclus_intime;
        Int_t           ntrk;
        Int_t           pat;////===tracker pattren 击中了tracker数量
        Int_t           patx;
        Float_t         rig;////===刚度
        Float_t         qin;////===inner测的的电荷
        Float_t         q1;
        Float_t         q9;
        Float_t         chi2x;////===重建轨迹的优度x
        Float_t         chi2y;////===重建轨迹的优度y
        Float_t         ccbdt0;////===直角坐标系决策树
        Float_t         ccbdt1;
        Float_t         ccbdt2;
        Float_t         ccbdtx2;
        Float_t         bdt_chargepid;////
        Int_t           trdnhits;
        Float_t         trdlkhd;////
        Float_t         trdelprob;////
        Float_t         trdlkhde;
        Float_t         trdlkhde_ehe;
        Int_t           trdnhits2;////
        Float_t         trdlkhde2;////
        Float_t         trdlkhde2_ehe;////
        Float_t         trdlkhdnew;////
        Float_t         trdelprobnew;////
        Float_t         trdlkhdnew_lastbin;
        Float_t         trdelprobnew_lastbin;
        Int_t           ecal_nsh;
        Float_t         ecal_enc;////
        Float_t         ecal_ene17;
        Float_t         ecal_bdt;
        Float_t         ecal_nchi2;
        Int_t           hadflag;
        Int_t           N_Shwr;
        Float_t         Ene;
        Float_t         E0[3];
        Float_t         A0[3];
        Float_t         Lkhd0;////====liklihood: ecal重建的粒子识别估算子,下限为电子
        Float_t         Lkhd1;
        Float_t         EmBDT;
        Float_t         LkhdRatio;
        Int_t           NLatLeakCell;
        Float_t         LatLeakFrac;
        Float_t         RearLeakFrac;
        Float_t         xecal[2];
        Float_t         yecal[2];
        Int_t           trkecalmatch;////
        Int_t           trdecalmatch;
        Int_t           ECAL_IsFocused;
        Int_t           ECAL_IsContained;
        Int_t           inecal;////
        Float_t         trkecaldx;
        Float_t         trkecaldy;
        Float_t         trdecaldx;
        Float_t         trdecaldy;
        Int_t           nvertex_3d;
        Int_t           nvertex_2d_x;
        Int_t           nvertex_2d_y;
    public:
        // List of branches
        TBranch        *b_run;   //!
        TBranch        *b_event;   //!
        TBranch        *b_utime;   //!
        TBranch        *b_lf;   //!
        TBranch        *b_lfc;   //!
        TBranch        *b_TTheta;   //!
        TBranch        *b_Phi;   //!
        TBranch        *b_Glat;   //!
        TBranch        *b_Glong;   //!
        TBranch        *b_Mlat;   //!
        TBranch        *b_Mlong;   //!
        TBranch        *b_Gr;   //!
        TBranch        *b_cf;   //!
        TBranch        *b_cfi;   //!
        TBranch        *b_poscf;   //!
        TBranch        *b_negcf;   //!
        TBranch        *b_poscfi;   //!
        TBranch        *b_negcfi;   //!
        TBranch        *b_mcp;   //!
        TBranch        *b_mcq;   //!
        TBranch        *b_isbadrun;   //!
        TBranch        *b_isbad;   //!
        //----tsu
        TBranch           *b_isbadrun_trd;
        TBranch           *b_isbadrun_trk;
        TBranch           *b_isbadrun_ecal;
        TBranch           *b_isbadrun_general;
        TBranch           *b_isbadrun_daq;
        TBranch           *b_isbadrun_fov;
        //--------
        TBranch        *b_lvl1_JMembPatt;   //!
        TBranch        *b_lvl1_PhysBPatt;   //!
        TBranch        *b_theta;   //!
        TBranch        *b_beta;   //!
        TBranch        *b_tofq;   //!
        TBranch        *b_tofqlow;   //!
        TBranch        *b_tofqup;   //!
        TBranch        *b_nclus_intime;   //!
        TBranch        *b_ntrk;   //!
        TBranch        *b_pat;   //!
        TBranch        *b_patx;   //!
        TBranch        *b_rig;   //!
        TBranch        *b_qin;   //!
        TBranch        *b_q1;   //!
        TBranch        *b_q9;   //!
        TBranch        *b_chi2x;   //!
        TBranch        *b_chi2y;   //!
        TBranch        *b_ccbdt0;   //!
        TBranch        *b_ccbdt1;   //!
        TBranch        *b_ccbdt2;   //!
        TBranch        *b_ccbdtx2;   //!
        TBranch        *b_bdt_chargepid;   //!
        TBranch        *b_trdnhits;   //!
        TBranch        *b_trdklkhd;   //!
        TBranch        *b_trdelprob;   //!
        TBranch        *b_trdklkhde;   //!
        TBranch        *b_trdklkhde_ehe;   //!
        TBranch        *b_trdnhits2;   //!
        TBranch        *b_trdklkhde2;   //!
        TBranch        *b_trdklkhde2_ehe;   //!
        TBranch        *b_trdlkhdnew;   //!
        TBranch        *b_trdelprobnew;   //!
        TBranch        *b_trdlkhdnew_lastbin;   //!
        TBranch        *b_trdelprobnew_lastbin;   //!
        TBranch        *b_ecal_nsh;   //!
        TBranch        *b_ecal_enc;   //!
        TBranch        *b_ecal_ene17;   //!
        TBranch        *b_ecal_bdt;   //!
        TBranch        *b_ecal_nchi2;   //!
        TBranch        *b_hadflag;   //!
        TBranch        *b_N_Shwr;   //!
        TBranch        *b_Ene;   //!
        TBranch        *b_E0;   //!
        TBranch        *b_A0;   //!
        TBranch        *b_Lkhd0;   //!
        TBranch        *b_Lkhd1;   //!
        TBranch        *b_EmBDT;   //!
        TBranch        *b_LkhdRatio;   //!
        TBranch        *b_NLatLeakCell;   //!
        TBranch        *b_LatLeakFrac;   //!
        TBranch        *b_RearLeakFrac;   //!
        TBranch        *b_xecal;   //!
        TBranch        *b_yecal;   //!
        TBranch        *b_trkecalmatch;   //!
        TBranch        *b_trdecalmatch;   //!
        TBranch        *b_ECAL_IsFocused;   //!
        TBranch        *b_ECAL_IsContained;   //!
        TBranch        *b_inecal;   //!
        TBranch        *b_trkecaldx;   //!
        TBranch        *b_trkecaldy;   //!
        TBranch        *b_trdecaldx;   //!
        TBranch        *b_trdecaldy;   //!
        TBranch        *b_nvertex_3d;   //!
        TBranch        *b_nvertex_2d_x;   //!
        TBranch        *b_nvertex_2d_y;   //!
};