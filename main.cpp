#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "mcmc_span.h"
#include "util.h"
#include "reduce_func.h"
using namespace std;
double min_sup; //probability
int k; //number of pattern to extract
int d; //Dimension of the data (label_num)
int N; //Number of transaction
bool gpu;
int mix_time;
vector<string> filenames;
void parse_parameters(int, char**);

extern float* setdev_data(float* data, int dSize, int numElement);
extern float* setdev_dot(int numElement);
extern float* setdev_out(int numElement);
extern int* setdev_pat(int* pat, int m);
extern float reduce_gpu(int numElement, int numThread, float* dev_data, float* dev_dot, float* dev_out, int* dev_pat);
extern float reduce_cpu(float* data, int* pat);

int main(int argc, char** argv) {
    gpu = false;
    parse_parameters(argc, argv);
    vector<vector<float> > data;
    read_data(filenames.back(), data);
    d = data.back().size();
    N = data.size();
    McmcSpan sampler(min_sup, d);
    sampler.set_data(data);
    ReduceFunc* rfunc;
    if(gpu) {
        rfunc = new GPUReduce();
    } else {
        rfunc = new CPUReduce();
        sampler.init_gpu();
        rfunc->set_gpu_params(sampler.dev_data,
                sampler.dev_dot,
                sampler.dev_out);
    }
    sampler.run_alg(mix_time, k, rfunc);
    sampler.print_max_pat();
    delete rfunc;
}

void parse_parameters(int argc, char** argv) {
    if(argc == 1) {
        printf("More arguments needed\n");
    }
    for(int argnum = 1; argnum < argc; argnum++) {
        if(argv[argnum][0] == '-') {
            if(!strcmp(argv[argnum], "-min_sup")) {
                char* endptr;
                min_sup = strtod(argv[++argnum], &endptr);
            }else if(!strcmp(argv[argnum], "-f")) {
                filenames.push_back(argv[++argnum]);
            }else if(!strcmp(argv[argnum], "-k")) {
                k = atoi(argv[++argnum]);
            }else if(!strcmp(argv[argnum], "-gpu")) {
                gpu = true;
            }else if(!strcmp(argv[argnum], "-mix_time")) {

            }
        }
    }
}


