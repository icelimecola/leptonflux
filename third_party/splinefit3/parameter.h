#ifndef PARAMETER_H
#define PARAMETER_H
//------track
const int NTKL=9;
const float tk_pz[NTKL]={159.04,53.05,29.22,25.24,1.706,-2.292,-25.26,-29.25,-136.03};
const int icffv=1;
const int NSPAN=3;

//------fiducial constant
const double cirr[]={62,62,46,46,46,46,46,46,43};
const double ciry[]={47,40,44,44,36,36,44,44,29};

//------Ecal
const double ecal_pz=-143;
const double ecal_thickness=1.85;
const double ecalxy[]={32,32};
//2019.05.28: detail Ecal coordinate, from Mercedes (see file ~/public/ntple_prod/utils/ecal_gendim.C)   -----------
static float ecal_plane_top_z[18]={-142.827, -143.747, -144.677, -145.597, -146.527, -147.447, -148.377, -149.297, -150.227, -151.147, -152.077, -152.997, -153.927, -154.847, -155.777, -156.697, -157.627, -158.547};/// ECAL first fiber layer center z coordinate for each plane in AMS ref system (cm)
static float ecal_plane_center_z[18]={-143.195, -144.115, -145.045, -145.965, -146.895, -147.815, -148.745, -149.665, -150.595, -151.515, -152.445, -153.365, -154.295, -155.215, -156.145, -157.065, -157.995, -158.915};
static float ecal_plane_bottom_z[18]={-143.563, -144.483, -145.413, -146.333, -147.263, -148.183, -149.113, -150.033, -150.963, -151.883, -152.813, -153.733, -154.663, -155.583, -156.513, -157.433, -158.363, -159.283};/// ECAL last fiber layer center z coordinate for each plane in AMS ref system (cm)
const float ecal_fl_pitchz[2]={0.184, 0.194};//ECAL distance between adjacent fiber layers 0: belonging to same SL, 1: belonging to different Sls

static float ecal_min[2]={-32.27, -32.47};// ECAL x,y minimum (1st cell left border) coordinate value in AMS ref system (cm)
static float ecal_max[2]={32.53, 32.33};// ECAL x,y maximum  (last cell right border) coordinate value in AMS ref system (cm)

static float ecal_cell_dim=0.9; ////ECAL pixel (is a cube) dimension in cm

static float ecal_cell_x[72]={	-31.82,-30.92,-30.02,-29.12,-28.22,-27.32,-26.42,-25.52,-24.62,-23.72,
						-22.82,-21.92,-21.02,-20.12,-19.22,-18.32,-17.42,-16.52,-15.62,-14.72,
						-13.82,-12.92,-12.02,-11.12,-10.22,-9.32,-8.42,-7.52,-6.62,-5.72,
						-4.82,-3.92,-3.02,-2.12,-1.22,-0.32,0.58,1.48,2.38,3.28,
						4.18,5.08,5.98,6.88,7.78,8.68,9.58,10.48,11.38,12.28,
						13.18,14.08,14.98,15.88,16.78,17.68,18.58,19.48,20.38,21.28,
						22.18,23.08,23.98,24.88,25.78,26.68,27.58,28.48,29.38,30.28,
						31.18,32.08};// ECAL cells center x coordinate in AMS ref. system (cm)

static float ecal_cell_y[72]={	-32.02,-31.12,-30.22,-29.32,-28.42,-27.52,-26.62,-25.72,-24.82,-23.92,
						-23.02,-22.12,-21.22,-20.32,-19.42,-18.52,-17.62,-16.72,-15.82,-14.92,
						-14.02,-13.12,-12.22,-11.32,-10.42,-9.52,-8.62,-7.72,-6.82,-5.92,
						-5.02,-4.12,-3.22,-2.32,-1.42,-0.52,0.38,1.28,2.18,3.08,
						3.98,4.88,5.78,6.68,7.58,8.48,9.38,10.28,11.18,12.08,
						12.98,13.88,14.78,15.68,16.58,17.48,18.38,19.28,20.18,21.08,
						21.98,22.88,23.78,24.68,25.58,26.48,27.38,28.28,29.18,30.08,
						30.98,31.88};// ECAL cells center x coordinate in AMS ref. system (cm)


//------no. of cuts
const int BETA_SEL_NO = 2;
const int TKGEOM_SEL_NO = 3;
const int TRACK_SEL_NO = 6;
const int CHARGE_SEL_NO = 7;

//------limit for unbiased Tof Layer difference
const double ISSTofLQ[30] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.5, 0., 0.5, 0., 0.5, 0., 0.6, 0., 0., 0., 0.8, 0., 0., 0., 0., 0., 0.8, 0., 0., 0., 0.};

//------element name
const char *Element[26] = {"", "", "", "", "B10", "C12", "N14", "O16", "F", "Ne20", "Na", "Mg24", "Al27", "Si28", "", "S32", "", "Ar36", "", "Ca40", "", "", "", "", "", "Fe56"};
const char *ElementFull[26] = {"", "", "", "", "Boron", "Carbon", "Nitrogen", "Oxygen", "Fluorine", "Neon", "Sodium", "Magnesium", "Aluminium", "Silicon", "", "Sulfur", "", "Argon", "", "Calcium", "", "", "", "", "", "Iron"};
const string sElement[26] = {"", "", "", "", "B10", "C12", "N14", "O16", "F", "Ne20", "Na", "Mg24", "Al27", "Si28", "", "S32", "", "Ar36", "", "Ca40", "", "", "", "", "", "Fe56"};
const string sElement2[26] = {"", "", "", "", "B10", "C12", "N14", "O16", "F18", "Ne20", "Na22", "Mg24", "Al26", "Si28", "P30", "S32", "Cl34", "Ar36", "K38", "Ca40", "", "", "", "", "", "Fe56"};

//const string sElementShort[26] = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "", "", "", "", "", "Fe"};
const string sElementShort[28] = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "", "", "", "", "Mn", "Fe", "Co", "Ni"};
const string sElementFull[26] = {"", "Helium", "", "", "Boron", "Carbon", "Nitrogen", "Oxygen", "Fluorine", "Neon", "Sodium", "Magnesium", "Aluminium", "Silicon", "", "Sulfur", "", "Argon", "", "Calcium", "", "", "", "", "", "Iron"};
const string sElementFull2[26] = {"", "", "", "", "boron", "carbon", "nitrogen", "oxygen", "fluorine", "neon", "sodium", "magnesium", "aluminium", "silicon", "", "sulfur", "", "argon", "", "calcium", "", "", "", "", "", "iron"}; //2020.01.11

//------mass of Elements (in units of proton mass
const double dMass[] = {0., 0., 0., 0., 0., 10, 12, 14, 16, 0., 20., 0., 24., 0., 28., 0., 32., 0., 36., 0., 40., 0., 0., 0., 0., 0., 56.};
constexpr double MPROTON{0.938};

//------name
//----tracker pattern
const char cSpan[NSPAN][1000] = {"Inner", "L1Inner", "FullSpan"};
//const string sSpan[NSPAN] = {"Inner", "L1Inner", "FullSpan"};
const string sSpan[4] = {"Inner", "L1Inner", "FullSpan", "InnerL9"};

//----Tof
constexpr int nSuccTofQ[3]{1, 1, 2}; //without tof path length cut
//const int nSuccTofQ[3]={2, 2, 4}; //with tof path length cut
//const char cQEff[3] = {'U', 'L', 'T'};
const char cTofEff[3] = {'U', 'L', 'T'};
const string sQEff[3] = {"UTofQ", "LTofQ", "UTofQ&LTofQ"};
const string sQEff2[3] = {"UTof", "LTof", "UTof&LTof"}; //2019.09.23

//----L1 efficiency
const int nSuccL1BZ=3;
//const int nSuccL1PU=4;
const int nSuccL1PU[2]={5,4}; //2019.09.09
//const int nSuccL1PUT=nSuccL1PU[0];
const int nSuccL1PUT=5; //2019.10.10

//----L9 efficiency
const int nSuccL9=3;

//----Tracking efficiecny
const char cRigMethod[3][1000] = {"Beta", "Cutoff", "Ecal"};

static const int nSuccTk=9;
//static const int nSuccTk=10;
static const int nSuccTkSam=20;
static const int nSuccTkSamPresel=5;
static const int nSuccTkSamR1=nSuccTkSam-3;
static const int nSuccTkSamVal=12;
static const int nSuccTkSamValR1=nSuccTkSamVal-4;
static const int nSuccEx=5;
//static const int nSuccEx=4;
static const int nSuccEx_l9=3;

const char cSuccTk[nSuccTk][1000] = {"Tk Particle", "#beta>0.4", "nhit>=5", "hit pattern", "#chi^{2}_{Y, Inn}<10", "InnerQ cut", "Qrms<0.55", "fiducial volume1", "fiducial volume2"};
//const char cSuccTk[nSuccTk][1000] = {"Tk Particle", "#beta>0.4", "nhit>=5", "hit pattern", "#chi^{2}_{Y, Inn}<10", "fiducial volume1", "fiducial volume2", "InnerQ cut", "Qrms<0.55"};
//const char cSuccTk[nSuccTk][1000] = {"Tk Particle", "#beta>0.4", "nhitY>=5", "hit pattern", "#chi^{2}_{Y, Inn}<10", "fiducial volume1", "fiducial volume2", "InnerQ cut", "Qrms<0.55", "L1Z=InnerZ"};

//const char cSuccEx[3][nSuccEx][1000] = {"XY hit on L1", "#chi^{2}_{Y,L1Inn}<10", "L1Q cut", "L1Q_Status",  
//						    "XY hit on L1", "#chi^{2}_{Y,FS}<10", "L1Q cut", "L1Q_Status", 
//						    "XY hit on L9", "#chi^{2}_{Y,FS}<10", "L9Q cut", ""};
const char cSuccEx[NSPAN][nSuccEx][1000] = {{"XY hit on L1", "#chi^{2}_{Y,L1Inn}<10", "#chi^{2}_{Y,L1}<10", "L1Q cut", "L1Q_Status"},  
						    {"XY hit on L1", "#chi^{2}_{Y,FS}<10", "#chi^{2}_{Y,L1}<10", "L1Q cut", "L1Q_Status"}, 
						    {"XY hit on L9", "#chi^{2}_{Y,FS}<10", "L9Q cut", "", ""}};

//----charge calibration
/*const int NQDIS=5;
const char cQDis[NQDIS][1000] = {"InnerQ", "UTofQ", "LTofQ", "L1Q", "L9Q"};
const string sQDis[NQDIS] = {"InnerQ", "UTofQ", "LTofQ", "L1Q", "L9Q"};*/
const int NQDIS=6;
const char cQDis[NQDIS][1000] = {"InnerQ", "UTofQ", "LTofQ", "InnInnerQ", "L1Q", "L9Q"};
const string sQDis[NQDIS] = {"InnerQ", "UTofQ", "LTofQ", "InnInnerQ", "L1Q", "L9Q"};
const string sQDis2[NQDIS] = {"Inner Silicon Tracker Charge", "Upper TOF Charge", "Lower TOF Charge", "InnInnerQ", "Tracker Layer 1 Charge", "Tracker Layer 9 Charge"}; //2019.10.23

/*//------draw
Option_t *DrawOp = "PE1";
Option_t *DrawOpS = "PE1SAME";

Option_t *DrawOp1 = "P";
Option_t *DrawOp1S = "P SAME";

const double dLedgeX = 2.;
const double dUedgeX = 6000.;*/

//------MC version
std::string cMCVer[26] = {"", "", "", "", "", "", "", "", "", "B1116", "", "B1116", "B1116", "B1117", "", "B1116", "", "B1116", "", "B1116", "", "", "", "", "", "B1116"};

//2019.06.02: rich cut from JiaHui
const float kCutRichProb                  = 0.01;          //  Kolmogorov test probability
const float kCutRichPmt                   = 3;             //  number of pmts
const float kCutRichCollOverTotal         = 0.4;           //  ring photoelctrons / total photoelectrons in the event
const float kCutRichChargeConsistency     = 15;            //  hit/PMT charge consistency test
const float kCutRichBetaConsistency[2]    = {0.01,0.005};  //  beta_lip vs beta_ciemat consistency ([0]=NaF, [1]=aerogel)
const float kCutRichExpPhe[2]             = {1,2};         //  expected number of photoelectrons   ([0]=NaF, [1]=aerogel)
const float kCutRichAerogelExternalBorder = 3500.;         //  aerogel external border (r**2)
const float kCutRichAerogelNafBorder[2]   = {17.,19.};     //  aerogel/NaF border                  ([0]=NaF, [1]=aerogel)
const int kNBadTiles = 5;                            
int kBadTile[kNBadTiles] = {
    3,
    7,
    87,
    100,
    108
};   //  tiles with bad beta recosntruction

//------L8L9 material missing //2020.04.03
const double dL8L9MissingMat=0.035;
#endif //PARAMETER_H
