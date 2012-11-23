#include "reduce_func.h"
#include "gpu.h"
#include <vector>

void ReduceFunc::set_gpu_params(float* a, float* b, float* c) {
    //donothing
}

float CPUReduce::operator()(float* database,
        const vector<int>& pattern, int label_num, int trans_num) {
    int* pat;
    int m = conv_pat(pattern, pat, trans_num);
    reduce_cpu(database, pat);
    delete [] pat;
}

float GPUReduce::operator()(float* database,
        const vector<int>& pattern, int label_num, int trans_num) {
    int* pat;
    conv_pat(pattern, pat, trans_num);
}

void GPUReduce::set_gpu_params(float* _dev_data, float *_dev_dot, float* _dev_out) {
    dev_data = _dev_data;
    dev_out = _dev_out;
    dev_dot = _dev_dot;
}

