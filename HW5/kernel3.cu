#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
/*
__global__ static inline int mandel(float c_re, float c_im, int count)
{
  float z_re = c_re, z_im = c_im;
  int i;
  for (i = 0; i < count; ++i)
  {

    if (z_re * z_re + z_im * z_im > 4.f)
      break;

    float new_re = z_re * z_re - z_im * z_im;
    float new_im = 2.f * z_re * z_im;
    z_re = c_re + new_re;
    z_im = c_im + new_im;
  }

  return i;
}
*/
__global__ void mandelKernel( int* d_data,float stepX, float stepY,float lowerX,float lowerY ,int width, int maxIterations, size_t pitch) {
    // To avoid error caused by the floating number, use the following pseudo code
    //
	int thisX= blockIdx.x * blockDim.x + threadIdx.x;
	int thisY= blockIdx.y * blockDim.y + threadIdx.y;
	if( (thisX % 16 == 0) && (thisY % 16 == 0)  ){
		int j = thisX , k = thisY;
		for(j=thisX;j < thisX+16;j++){
			for( k=thisY ; k < thisY+16 ; k++){
				float x = lowerX + j * stepX;
				float y = lowerY + k * stepY;
				  
				float c_re = x, c_im = y;
				float z_re = c_re, z_im = c_im;

				int i;
				for (i = 0; i < maxIterations; ++i)
				{

						if (z_re * z_re + z_im * z_im > 4.f)
						break;

						float new_re = z_re * z_re - z_im * z_im;
						float new_im = 2.f * z_re * z_im;
						z_re = c_re + new_re;
						z_im = c_im + new_im;
				}
				int *ptr = (int *)((char*)d_data+k*pitch);
				ptr[j] = i;
				//d_data[ thisX + thisY * width ] = i;
			}
		}
	}
}

// Host front-end function that allocates the memory and launches the GPU kernel
void hostFE (float upperX, float upperY, float lowerX, float lowerY, int* img, int resX, int resY, int maxIterations)
{
    float stepX = (upperX - lowerX) / resX;
    float stepY = (upperY - lowerY) / resY;
	int *data, *d_data;
	dim3 threadPerBlock(25,25);
	dim3 numBlocks(resX/threadPerBlock.x,resY/threadPerBlock.y);
	size_t pitch;
	//data = (int*)malloc( sizeof(int)*resX*resY );
	cudaHostAlloc(&data, sizeof(int) * resX*resY, cudaHostAllocMapped);

	cudaMallocPitch((void **)&d_data, &pitch, sizeof(int)*resX, resY);
//	cudaMalloc((void**)&d_data, sizeof(int)*resX*resY );

	mandelKernel<<<numBlocks,threadPerBlock>>>(d_data,stepX,stepY,lowerX,lowerY,resX,maxIterations,pitch);

 	cudaMemcpy2D(data, sizeof(int)*resX, d_data, pitch, sizeof(int)*resX, resY, cudaMemcpyDeviceToHost);

	memcpy(img,data,sizeof(int)*resX*resY);
	cudaFreeHost(data);
	cudaFree(d_data);
	
}
