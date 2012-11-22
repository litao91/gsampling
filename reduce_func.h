/**
 * Function objects that wrap the pdf functions, including divide and conquer,
 * dynamic programming and GPU accelerated method
 *
 */
#include "util.h"
#ifndef PDF_FUNC_H
#define PDF_FUNC_H
class ReduceFunc{
public:
    virtual float operator()(float* database,
            const vector<int>& pattern,
            int label_num,
            int trans_num);
    virtual void set_gpu_params(float*, float*, float* ) {
        //do nothing
    }
};


class CPUReduce: public ReduceFunc {
public:
    float operator()(float* database,
            const vector<int>& pattern,
            int label_num,
            int trans_num);
};

class GPUReduce: public ReduceFunc {
public:
    float operator()(float* database,
            const vector<int>& pattern,
            int label_num,
            int trans_num);
    virtual void set_gpu_params(float* dev_data, float *dev_dot, float* dev_out);
    float* dev_data, *dev_dot, *dev_out;
};
#endif
