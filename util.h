#include <vector>
#include <iostream>
using std::vector;
using std::string;
#ifndef UTIL_H
#define UTIL_H
/**
 * \return  greater than zero if set1 is greater than set2, 0 on equal and
 * less than zero on on set1 less than set2
 * \param vector with size the shame as the label num, 1 if it has the label and zero if not
 */
int compare_set(const vector<int>& set1, const vector<int>& set2);
float* get_pdf(int** database, int N, int label_num, const vector<int>& set);
/**
 * Check whether a set satisfies mind support
 * \param database The data set that needed to do pattern mining
 * \param set the set that needed to be check support
 * \return a boolean value indicating whether it is a frequent pattern
 */
/*
float freq(const vector<vector<float> >& database,
        const vector<int>& set,
        PDFFunc *pdf_func);
        */
void clear_pattern(vector<int>& pat);
/**
 * Read the data, data should be in the format of:
 * row: transaction
 * column: attributes(probability)
 */
void read_data(const string& filename, vector<vector<float> >& r_data);
void to_float(const vector<float>& in, float*& out);
void to_float(const vector<vector<float> >&  in, float*& out);
int conv_pat(const vector<int>& pat, int*& r_val, int N);
void init_gpu(int d,int N, float* data);
#endif
