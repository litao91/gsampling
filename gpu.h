#include <cstdlib>
#ifndef GPU_H
#define GPU_H

extern float* setdev_data(float* data, int dSize, int numElement);
extern float* setdev_dot(int numElement);
extern float* setdev_out(int numElement);
extern int* setdev_pat(int* pat, int m);
extern float reduce_gpu(int numElement, int numThread, float* dev_data, float* dev_dot, float* dev_out, int* dev_pat);
extern float reduce_cpu(float* data, int* pat);
//float* data;
extern float* dev_data;
extern float* dev_dot;
extern float* dev_out;
/*
int* pat;
int* dev_pat;
*/
/*
const int numElement = 512*500;
const int numAttribute = 100;
const int dSize = numElement*numAttribute;
const int numThread = 512;
float result = 0;
*/

/*
void Init()
{	
	data = (float*)malloc(sizeof(float)*dSize);
	for(int i = 0; i < dSize; i++) 
	{
		//data[i] = (float)rand()/RAND_MAX;
		data[i] = 0.5;
	}
	
	dev_data = setdev_data(data, dSize, numElement);
	dev_dot = setdev_dot(numElement);
	dev_out = setdev_out(numElement);
}
*/
#endif
