#include "util.h"
#include <iostream>
#include <cstdlib>
#include<fstream>
#include<sstream>

using namespace std;
int compare_set(const vector<int>& set1,
        const vector<int>& set2) {
    if(set1.size()!= set2.size()) {
        return set1.size()-set2.size();
    }
    int size = set1.size();
    for(int i = 0; i < size; i++) {
        if(set1[i]!=set2[i]) {
            return set1[i] - set2[i];
        }
    }
    return 0;
}


double* get_prlist(const vector<vector<double> >& database,
        const vector<int>& set) {
    int N = database.size();
    int label_num = database[0].size();
    double* pdf = new double[N];
    for(int n=0; n< N; n++) {
        double cur_pr = 1;
        for(int i=0; i< label_num; i++) {
            //if the ith label is in the pattern
            if(set[i]) {
                cur_pr*=database[n][i];
            }
        }
        pdf[n] = cur_pr;
    }
    return pdf;
}
/***No longer needed*/
/**
 * Detail:
 * 1. Compute the pdf(use the pdf function object passed in
 * 2. Calculate the mean of the probability, and compare with the min_sup
 */
/*
double freq(const vector<vector<double> >& database,
        const vector<int>& set,
        PDFFunc* pdf_func) {

    int N = database.size();
    int label_num = database[0].size();
    double* prlist = get_prlist(database, set);
    vector<double> prl_vec(prlist,prlist+sizeof(prlist)/sizeof(double));
    delete [] prlist;
    vector<double> pdf;
    (*pdf_func)(pdf, prl_vec);
    double sum = 0;
    for(int i=0; i< N; i++) {
        sum+=pdf[i];
    }
    return sum/N;
}
*/

void clear_pattern(vector<int>& pat) {
    vector<int>::iterator it;
    for(it = pat.begin(); it!=pat.end(); it++) {
        *it = 0;
    }
}

/**
 * \param filename, the file to read
 * \param r_data the returned datastructure
 */
void read_data(const string& filename, vector<vector<float> >& r_data) {
    string line;
    double item;
    ifstream is(filename.c_str());
    r_data.clear();
    while(getline(is, line)){
        stringstream istr((char*) line.c_str());
        r_data.push_back(vector<float>());
        while(istr >> item) {
            r_data.back().push_back(item);
        }
    }
}

void to_float(const vector<float>& in, float*& out) {
    int size = in.size();
    vector<float>::const_iterator iter = in.begin();
    out = new float[size];
    for(int i=0;iter!=in.end();iter++, i++) {
        out[i] = (float)(*iter);
    }
}

void to_float(const vector<vector<float> >& in, float*& out) {
    int row_num = in.size();
    int col_num = in.back().size();
    out = new float[row_num*col_num];
    int i_float = 0;
    for(int j = 0; j<col_num; j++) {  // loop over column
        for(int i=0; i < row_num; i++) { // loop over row
            out[i*row_num+j] = (float) in[i][j];
        }
    }
}

//convert the pattern to the format for reduction function
int conv_pat(const vector<int>& pat, int*& rval, int N) {
    int m=0;
    rval = new int[pat.size()+2];
    for(int i=2; i < pat.size()+2; i++) {
        if(pat[i] == 1) {
            m++;
            rval[m] = i;
        }
    }
    rval[0] = m;
    rval[1] = N;
    return m;
}


