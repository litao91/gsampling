#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

float reduce_cpu(float* data, int* pat){
	
	float sum = 0;
	int m = pat[0];
	int numElement = pat[1];
		
	for(int i = 0; i < numElement; i++) 
	{
		float prod = 1;
		for(int j = 2; j < m+2; j++)
			prod *= data[pat[j]*numElement+i];
		sum += prod;
	}
	
	return sum;
}


__global__
void reduce_kernel1(float* d_out, float* d_in, const int size) {
	extern __shared__ float s_data[];
	const int tid = threadIdx.x;
	const int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (i >= size)
		s_data[tid] = 0;
	else	
		s_data[tid] = d_in[i];
	
	__syncthreads();

	for(unsigned int s = blockDim.x/2; s > 0; s >>= 1) 
	{
		if(tid < s) 
		{
			s_data[tid] += s_data[tid + s];
		}
		__syncthreads();
	}

	if(tid == 0) 
	{
		d_out[blockIdx.x] = s_data[0];
	}
}	

inline void swap(float* &p1, float* &p2) {

	float* tmp = p1;
	p1 = p2;
	p2 = tmp;

}


__global__
void reduce_kernel4(float* d_out, float* d_in, const int size) {

	extern __shared__ float s_data[];
	unsigned int tid = threadIdx.x;
	unsigned int i = blockIdx.x*(blockDim.x*2) + threadIdx.x;
	s_data[tid] = 0;
	__syncthreads();
	if (i + blockDim.x < size )
		s_data[tid] = d_in[i] + d_in[i + blockDim.x];
	else if (i < size)
		s_data[tid] = d_in[i];
	__syncthreads();
	for(unsigned int s = blockDim.x/2; s > 0; s >>= 1) {
		if(tid < s) {
			s_data[tid] += s_data[tid + s];
		}
		__syncthreads();
	}
	if(tid == 0) {d_out[blockIdx.x] = s_data[0];}
}	

float reduction(float* d_data, float* d_buf, const int numElement, int numThread, 
		void (*reduce_kernel)(float* d_out, float* d_data, const int size), 
		bool isKernel4 = false) {

	float* d_src = NULL;	//always store input data
	float* d_dst = NULL;	//always store output data

	//for the first level of reduction
	int n = numElement;
	d_src = d_data;
	d_dst = d_buf;

	int numBlock = numElement/numThread + (numElement%numThread? 1 : 0);
	int sharedMemSize = sizeof(float)*numThread;
	while(n > 1) {
		
		if(isKernel4) 
			numBlock = numBlock/2 + (numBlock%2 ? 1 : 0);
		reduce_kernel<<<numBlock, numThread, sharedMemSize>>>(d_dst, d_src, n);
		cudaThreadSynchronize();
		//for the next level		
		n = numBlock;
		numBlock = n/numThread+ (n%numThread ? 1 : 0);
		swap(d_dst, d_src);
	}

	cudaDeviceSynchronize();
	swap(d_dst, d_src);
	float result = 0;
	cudaMemcpy(&result, d_dst, sizeof(float), cudaMemcpyDeviceToHost);
	return result;
}



__global__
void dot_kernel(float* dev_data, float* dev_dot, int* dev_pat) {
	const int index = blockIdx.x*blockDim.x + threadIdx.x;
	int m = dev_pat[0];
	int numElement = dev_pat[1];
	if (index < numElement)
	{
		float prod = 1;
		for(int j = 2; j < m+2; j++)
			prod = prod*dev_data[dev_pat[j]*numElement+index];
	
		dev_dot[index] = prod;
		__syncthreads();
	}
}


/*
*******************************************************************************************											
********************************************************************************************
*/

float* setdev_data(float* data, int dSize, int numElement)
{
	float* dev_data; //data在device的拷贝
	cudaMalloc((void**)&dev_data, sizeof(float)*dSize);
	cudaMemcpy(dev_data, data, sizeof(float)*dSize, cudaMemcpyHostToDevice);
	return dev_data;
}

float* setdev_dot(int numElement)
{
	float* dev_dot;	//dev_dot计算attribute之间的dot-product
	cudaMalloc((void**)&dev_dot, sizeof(float)*numElement);
	return dev_dot;
}

float* setdev_out(int numElement)
{
	float* dev_out;
	cudaMalloc((void**)&dev_out, sizeof(float)*numElement);
	return dev_out;
}

int* setdev_pat(int* pat, int m)
{
	int* dev_pat;//pat在device的拷贝
	cudaMalloc((void**)& dev_pat, sizeof(int)*(m+2));
	cudaMemcpy(dev_pat, pat, sizeof(int)*(m+2), cudaMemcpyHostToDevice);
	return dev_pat;
}

float reduce_gpu(int numElement, int numThread, float* dev_data, float* dev_dot, float* dev_out, int* dev_pat)
{
	dot_kernel<<<numElement/numThread+(numElement%numThread ? 1 : 0),numThread>>>(dev_data, dev_dot, dev_pat);
	return reduction(dev_dot, dev_out, numElement, numThread, reduce_kernel4);	
}


void test_reduction() {

	////////////////////////////////////////////////////////////////////////改动/////////////////////////////////////////////

	const int numElement = 512*500;
	const int numAttribute = 100;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//const int numElement = 1024*1024;
	//const int numAttribute = 100;
	const int dSize = numElement*numAttribute;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
	////////////////////////////////////////////////////////////////////////改动/////////////////////////////////////////////
	//Data Generator for the Table
	float* data = (float*)malloc(sizeof(float)*dSize);
	for(int i = 0; i < dSize; i++) 
	{
		//data[i] = (float)rand()/RAND_MAX;
		data[i] = 0.5;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	const int numThread = 512;
	float elapsedTime = 0.0f;
	float result = 0;
	float* dev_data; //data在device的拷贝
	cudaMalloc((void**)&dev_data, sizeof(float)*dSize);
	cudaMemcpy(dev_data, data, sizeof(float)*dSize, cudaMemcpyHostToDevice);
	float* dev_dot;	//dev_dot计算attribute之间的dot-product
	cudaMalloc((void**)&dev_dot, sizeof(float)*numElement);
	float* dev_out;
	cudaMalloc((void**)&dev_out, sizeof(float)*numElement);

	////////////////////////////////////////////////////////////////////////改动/////////////////////////////////////////////
	int m = 3; //有3个attribute的pattern
	int* pat = (int *)malloc(sizeof(int)*(m+2));
	pat[0] = m; pat[1] = numElement; pat[2] = 1; pat[3] = 2; pat[4] = 3;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	int* dev_pat;//pat在device的拷贝
	cudaMalloc((void**)& dev_pat, sizeof(int)*(m+2));
	cudaMemcpy(dev_pat, pat, sizeof(int)*(m+2), cudaMemcpyHostToDevice);
	
	//**************************************CPU*******************************************
	cudaEventRecord(start, 0);
	printf("numElement = %d\n", numElement);
	printf("reduce_cpu result: %f\n", reduce_cpu(data, pat));
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("cpu elapsed time: %.3f ms\n", elapsedTime);
	printf("---------------------------------\n\n");
	
	//************************************GPU******************************************
	elapsedTime = 0.0f;	
	cudaEventRecord(start, 0);
	dot_kernel<<<numElement/numThread+(numElement%numThread ? 1 : 0),numThread>>>(dev_data, dev_dot, dev_pat);
	
//	float* dot = (float*)malloc(sizeof(float)*numElement);
//	cudaMemcpy(dot, dev_dot, sizeof(float)*numElement, cudaMemcpyDeviceToHost);
//	float testsum = 0;
//	for (int i = 0; i < numElement; i++)
//		testsum += dot[i];
		
//	cout<<"dot result = "<<testsum<<endl;
	
	result = reduction(dev_dot, dev_out, numElement, numThread, reduce_kernel4);
	
	
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("kernel1 elapsed time: %.3f ms\n", elapsedTime);
	printf("kernel1 result: %f\n\n", result);

	cudaFree(dev_data);
	cudaFree(dev_out);
	cudaFree(dev_dot);
	free(data);

}


