#ifndef FFT_H
#define FFT_H
#include "header.h"
#include<cstdio>
#include<cstring>
#include<vector>
#include<string>
#include<map>
#include<complex>
#include<set>
#include<queue>
#include<cmath>
#include<algorithm>
using namespace std;
typedef complex <double> Complex;

int bitReverse(int& a, int width);
void initBitReverTab(int idx);
void initTab(int n);
void iterFFT(vector<Complex>& y, vector<double>& a);
void iterFFTInv(vector<Complex>& b, vector<Complex>& y);
void FFT(vector<Complex>& y, vector<double>& a);
void FFTInv(vector<Complex>& b, vector<Complex>& y);
void convol(vector<double>& v, vector<double>& a, vector<double>& b);
void convolWithNegExp(vector<double>& v, vector<double>& a, vector<double>& b);
void deconvol(vector<double>& v, vector<double>& a, vector<double>& b);
void polyDiv(vector<double>& quot, const vector<double>& nn, const vector<double>& dd);

static bool init = 0;

#endif