#ifndef BINS_H
#define BINS_H

//----gcx
#include <iostream>
using namespace std;
//----gcx

//-----------------
//2017.11.06
//	1) define a functions to retrieve what binning to use according to charge
//
//2019.01.30
//	1) at new parameter on getBins to be able to choose whether use published binning for He, C, O (2017)
//
//2019.02.10
//	1) at new sets of binning which seperated at 2000 GV in order to test MC reweight
//
//2019.02.27
//	1) extend two more bins, [3000,6000] & [6000,8000] for the He, C, O paper for unfolding
//	2) add opt for getNbin to control what kinds of bin to be used
//	3) add opt=2 for getBins to use published binning + [3000,6000] & [6000,8000]
//
//2019.10.31
//	1) update the binning for high Z to Ne, Mg, Si, S binning in Twiki
//
//2020.01.10
//	1) use iBinVersion to return always the High Z binning
//
//2020.02.04
//	1) update getNbin and getBins to be able to deal with F as well
//-----------------
//int iBinVersion=1;
int iBinVersion=0; //2020.01.10: default iBinVersion=0, decide binning according to charge

int getNbin(int q, int opt=iBinVersion);
double* getBins(int q, int opt=iBinVersion); //2019.02.27
/*int getNbin(int q);
double* getBins(int q, int opt=1);*/

const int nuclei_nbin = 74;
double nuclei_bins[nuclei_nbin+1] = {0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						 2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						 7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						 18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						 38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						 80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						 175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						 660.,880.,1300.,2600.,6000.};				//70-74

//----binning for He, C, O in Published paper (2017)
double nuclei_binsP[nuclei_nbin+1] ={0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
                                     2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
                                     7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
                                     18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
                                     38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
                                     80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
                                     175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
                                     643.,822.,1130.,1800.,3000.};				//70-74
//----binning for He, C, O in Published paper (2017) + two more bin: [3000,6000] & [6000,8000]
/*const int nuclei_nbin_unfold = nuclei_nbin+1;
double nuclei_binsP_unfold[nuclei_nbin_unfold+1] = {
						0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
                                    2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
                                    7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
                                    18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
                                    38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
                                    80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
                                    175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
                                    643.,822.,1130.,1800.,3000.,6000.};				//70-75*/
const int nuclei_nbin_unfold = nuclei_nbin+2;
double nuclei_binsP_unfold[nuclei_nbin_unfold+1] = {
						0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
                                    2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
                                    7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
                                    18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
                                    38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
                                    80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
                                    175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
                                    643.,822.,1130.,1800.,3000.,6000.,8000.};			//70-76
//binning for He, C, O in published paper (2017), restrict the last bin to [1800,2000] since the generated rig range for MC l1 is only up to 2000
double nuclei_binsAcc[nuclei_nbin+1] = {	0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
                                    	2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
                                     	7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
                                     	18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
                                     	38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
                                     	80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
                                     	175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
                                     	643.,822.,1130.,1800.,2000.};					//70-75

const int nuclei_nbin_l1puRebin = 70;
double nuclei_bins_l1puRebin[nuclei_nbin+1] = 
						{0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						 2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						 7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						 18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						 38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						 80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						 175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						 3000.};

//--2019.02.10: binning for MC reweight test
const int nuclei_nbin_rewtest = 75;
double nuclei_bins_rewtest[nuclei_nbin_rewtest+1] = {
						0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						660.,880.,1300,2000,4000,6000};				//70-76
						 
//----new binning for Big Z - abandoned
double nuclei_bins_BZ[nuclei_nbin+1] = {0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,		//0-9
						    2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						    7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						    18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						    38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						    80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						    175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						    660.,880.,1300.,2600.,3000.};					//70-75

////----new binning for Li, Be, B and also for Si, which has also different no. of bins
const int nuclei_nbin2 = 73;
double nuclei_bins2[nuclei_nbin2+1] = {0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						  2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						  7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						  18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						  38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						  80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						  175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						  660.,880.,1300.,3300.}; 					//70-73
////----new binning specific for Ne, Mg, Si, S in TWiki //2019.10.31
const int nuclei_nbinBZ = 73;
double nuclei_binsBZ[nuclei_nbinBZ+1] = {0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						     2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						     7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						     18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						     38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						     80.5,86.5,93.0, 100.,108.,116.,125.,135.,147.,160.,	//50-59
						     175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						     660.,860.,1200.,3000.}; 						//70-73
						  
//Carbon bin with (0, 0.8)
const int nuclei_nbin3 = 75;
double nuclei_bins3[nuclei_nbin3+1] = {0, 0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						 2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						 7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						 18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						 38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						 80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						 175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						 660.,880.,1300.,2600.,6000.};				//70-75
						 
////----new binning for Li, Be, B and also for Si, add bin (0, 0.8)
const int nuclei_nbin4 = 74;
double nuclei_bins4[nuclei_nbin4+1] = {0, 0.8,1.00,1.16,1.33,1.51,1.71,1.92,2.15,2.40,2.67,	//0-9
						  2.97,3.29,3.64,4.02,4.43,4.88,5.37,5.90,6.47,7.09,	//10-19
						  7.76,8.48,9.26,10.1,11.0,12.0,13.0,14.1,15.3,16.6,	//20-29
						  18.0,19.5,21.1,22.8,24.7,26.7,28.8,31.1,33.5,36.1,	//30-39
						  38.9,41.9,45.1,48.5,52.2,56.1,60.3,64.8,69.7,74.9,	//40-49
						  80.5,86.5,93.0,100.,108.,116.,125.,135.,147.,160.,	//50-59
						  175.,192.,211.,233.,259.,291.,330.,379.,441.,525.,	//60-69
						  660.,880.,1300.,3300.}; 					//70-74

//2020.01.10: use opt to control whether always using certain binning
//opt=0: binning according to charge
//   =1: He, C, O binning
//   =2: Ne, Mg, Si binning
int getNbin(int q, int opt)
{
	if (opt==0)
	{
		if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_nbin;
		//else if (q>=10) return nuclei_nbinBZ;
		else if (q>=9) return nuclei_nbinBZ; //2020.02.04
	}
	else if (opt==1) return nuclei_nbin;
	else if (opt==2) return nuclei_nbinBZ;
	return 0;
}

double* getBins(int q, int opt)
{
	if (opt==0)
	{
		if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_binsP;
		//else if (q>=10) return nuclei_binsBZ;
		else if (q>=9) return nuclei_binsBZ; //2020.02.04
	}
	else if (opt==1) return nuclei_binsP;
	else if (opt==2) return nuclei_binsBZ;
	return NULL;
}

/*int getNbin(int q, int opt)
{
	if (q==1 || q==2 || (q>=6 && q<=8))
	{
		if (opt<2) return nuclei_nbin;
		else return nuclei_nbin_unfold;
	}
	else if (q>=10) return nuclei_nbinBZ; //2019.10.31
	else return nuclei_nbin2;
}

double* getBins(int q, int opt)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) 
	{
		if (opt==0) return nuclei_bins;
		else if (opt==1) return nuclei_binsP;
		else if (opt==2) return nuclei_binsP_unfold;
		else return NULL;
	}
	else if (q>=10) return nuclei_binsBZ; //2019.10.31
	else return nuclei_bins2;
}*/

/*int getNbin(int q)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_nbin;
	else return nuclei_nbin2;
}

double* getBins(int q, int opt)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) return (opt==0)?nuclei_bins:nuclei_binsP;
	else return nuclei_bins2;
}*/

/*double* getBins(int q)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_bins;
	else return nuclei_bins2;
}*/

int getNbin2(int q)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_nbin3;
	else return nuclei_nbin4;
}

double* getBins2(int q)
{
	if (q==1 || q==2 || (q>=6 && q<=8)) return nuclei_bins3;
	else return nuclei_bins4;
}

//------rebin
const int QYrebin = 26;
double QYrebins[QYrebin+1] = 
{0.8, 1.16, 1.51, 1.92, 2.4, 2.97, 3.64, 4.43, 5.37, 6.47, 7.76, 9.26, 11, 13, 15.3, 18, 22.8, 28.8, 36.1, 48.5, 64.8, 86.5, 125, 192, 379, 1300, 6000};
//double nuclei_rebins[35];
//double nuclei_nrebin=0;
//for (int ibin=0; ibin<nuclei_nbin+1; ibin++) {
//	if (ibin<=56) {
//		if (ibin%2==0) {
//			nuclei_rebins[nuclei_nrebin] = nuclei_bins[ibin];
//			nuclei_nrebin++;
//		}
//	}
//	else {
//		if (ibin%3==0) {
//			nuclei_rebins[nuclei_nrebin] = nuclei_bins[ibin];
//			nuclei_nrebin++;
//		}
//	}
//}

//2019.03.26
//------binning for spectrum index fit
const int nbinSI=10; //same as He, C, O paper
//double binsSI[nbinSI+1] ={8.48,13.0,19.5,28.8,41.9,60.3,86.5,125.,211.,441.,3000.}; //even binning
double binsSI[nbinSI+1] ={8.48,12.0,16.6,22.8,31.1,41.9,56.1,93.0,160.,330.,3000.}; //6*(4 bins) before 60 GV, 3*(7 bins) + 1*(8 bins) after 60 GV

//spectral index binning in HZ paper
constexpr int nbinSIHZ=8;
constexpr double binsSIHZ[nbinSIHZ+1] = {7.09,12.0,16.6,28.8,45.1,86.5,192.0,441.0,3000.0};
#endif                                  
