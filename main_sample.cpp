#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

extern void test_reduction();
extern float* setdev_data(float* data, int dSize, int numElement);
extern float* setdev_dot(int numElement);
extern float* setdev_out(int numElement);
extern int* setdev_pat(int* pat, int m);
extern float reduce_gpu(int numElement, int numThread, float* dev_data, float* dev_dot, float* dev_out, int* dev_pat);
extern float reduce_cpu(float* data, int* pat);



int main0()
{
    test_reduction();
    return 0;
}


int main()
{

    //////////////////////////////////////////////initialization/////////////////////////////////////////////
    const int numElement = 512*500;
    const int numAttribute = 100;
    const int dSize = numElement*numAttribute;
    const int numThread = 512;
    float result = 0;

    float* data = (float*)malloc(sizeof(float)*dSize);
    for(int i = 0; i < dSize; i++)
    {
        //data[i] = (float)rand()/RAND_MAX;
        data[i] = 0.5;
    }

    float* dev_data = setdev_data(data, dSize, numElement);
    float* dev_dot = setdev_dot(numElement);
    float* dev_out = setdev_out(numElement);

    ////////////////////////////////compute expected support of pattern//////////////////////////

    int m = 3; //pattern with 3 attribute
    int* pat = (int *)malloc(sizeof(int)*(m+2));
    pat[0] = m; pat[1] = numElement; pat[2] = 1; pat[3] = 2; pat[4] = 3;

    // compute by cpu and gpu, respectively
    cout<<"result of CPU = "<<reduce_cpu(data, pat)<<endl;
    int* dev_pat = setdev_pat(pat, m);
    cout<<"result of GPU = "<<reduce_gpu(numElement, numThread, dev_data, dev_dot, dev_out, dev_pat)<<endl;
}
