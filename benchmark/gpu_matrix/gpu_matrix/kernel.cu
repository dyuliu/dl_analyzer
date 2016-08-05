
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <random>
#include <iostream>
#include <time.h>


#define CUDA_CALL(x) { const cudaError_t cudaError_code = (x) ; \
	if ( cudaError_code != cudaSuccess) { \
		printf("\nCUDA Error: %s (err_num=%d)\n", cudaGetErrorString(cudaError_code), cudaError_code); \
		cudaDeviceReset(); assert(0); } }

#define CUDA_NUM_THREADS 1024
#define CUDA_NUM_BLOCKS 65535

#define CUDA_GET_BLOCKS(n) (((n)+CUDA_NUM_THREADS-1)/CUDA_NUM_THREADS)

#define KERNEL_CALL(x, n) x <<< CUDA_GET_BLOCKS(n), CUDA_NUM_THREADS >>>

#define CUDA_IDX (threadIdx.x + blockIdx.x * blockDim.x)

/*---------------------------------------------------------*/
//  SETTING
//
/*---------------------------------------------------------*/

#define data_type float
#define LENGTH (100)

/*---------------------------------------------------------*/
//  TEST GPU ADD
//  RESULT:
//
/*---------------------------------------------------------*/
__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

void benchmark() {

	auto rand_clock_t = clock();

	// generate
	auto host_a = new data_type[LENGTH];
	auto host_b = new data_type[LENGTH];
	auto host_c = new data_type[LENGTH];

	// random
	srand(clock() - rand_clock_t);
	for (int i = 0; i < LENGTH; i++) {
		host_a[i] = data_type(rand()) / RAND_MAX;
		host_b[i] = data_type(rand()) / RAND_MAX;
		host_c[i] = data_type(rand()) / RAND_MAX;
	}

	// destory
	delete[] host_a;
	delete[] host_b;
	delete[] host_c;
}


// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}
