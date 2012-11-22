#include "fft.h"
#ifndef DCPDF_H
#define DCPDF_H


void updatePDF(vector<double>& pdf,  double& pr);
// The dpPDF function calculatethe sum of variable based on dynamic programming
void dpPDF(vector<double>& pdf,  const vector<double> & prList);
bool divAndConq(vector<double>& pdf, int left, int right,  vector<double>& v, int minsup, double minprob);
// calculate sum of variable based on divide-and-conquer
//bool dcPDF(vector<double>& pdf, vector<double>& v, int minsup, double minprob);
void dcPDF1(vector<double>& pdf, const vector<double>&v);

// Then Pr(sup(p) >= tau) >= delta
// pdf3[tau] + ... +pdf3[n] >= delta

#endif
