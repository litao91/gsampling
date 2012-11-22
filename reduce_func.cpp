#include "reduce_func.h"
#include <vector>

extern float* setdev_data(float* data, int dSize, int numElement);
extern float* setdev_dot(int numElement);
extern float* setdev_out(int numElement);
extern int* setdev_pat(int* pat, int m);
extern float reduce_gpu(int numElement, int numThread, float* dev_data, float* dev_dot, float* dev_out, int* dev_pat);
extern float reduce_cpu(float* data, int* pat);

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

