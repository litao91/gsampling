#include "util.h"
#include "reduce_func.h"
#include <iostream>
using namespace std;
//#include "hash.h"
//#include <ext/hash_set>
#include <vector>
#include <set>
#ifndef _MCMC_SPAN_H_
#define _MCMC_SPAN_H_

struct CompareSet;

using std::set;
using std::vector;
/**
 * Type of a pattern, currently implemented as array with the same
 * size as the label number, if the set contain a certain label,
 * the corresponding value in the array is 1 and 0- otherwise.
 */
typedef vector<int> pattern_t;
/**
 * Use the set to store the mined pattern to make finding faster
 */
typedef set<pattern_t, CompareSet>  pattern_db_t;

struct CompareSet{
    bool operator()(const vector<int>& set1, const vector<int>& set2);
};

class McmcSpan{
public:
    McmcSpan(float min_sup, int label_num):
        _m_min_sup(min_sup),
        _m_label_num(label_num){}
    ~McmcSpan();
    //the type for a single pattern

    //Data format:
    //columns corresponding to labels and rows as sets
    void set_data(const vector<vector<float> >& database);
    void run_alg(int mix_time, int k, ReduceFunc * reduce_func);
    void init_gpu();
    void print_max_pat();
    float* dev_data;
    float* dev_dot;
    float* dev_out;

private:
    int _m_N; //database size
    vector<vector<float> > _m_database;
    float* _m_raw_database;
    int _m_min_sup;
    int _m_label_num;
    pattern_db_t _m_max_pattern;


};

#endif
