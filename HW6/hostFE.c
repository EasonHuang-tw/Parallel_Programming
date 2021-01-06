#include <stdio.h>
#include <stdlib.h>
#include "hostFE.h"
#include "helper.h"


void hostFE(int filterWidth, float *filter, int imageHeight, int imageWidth,
            float *inputImage, float *outputImage, cl_device_id *device,
            cl_context *context, cl_program *program)
{

//	cl memory	
    cl_int status;
	cl_kernel kernel;
    cl_command_queue queue;

	cl_mem input_buffer;
	cl_mem output_buffer;
	cl_mem filter_buffer;

	
	const size_t global = imageHeight * imageWidth;
	const size_t local = 1000;

	int filterSize = filterWidth * filterWidth;
	int i , j;
	/*	kernal	*/
	
	kernel = clCreateKernel(*program, "convolution", &status);
	
	queue = clCreateCommandQueue(*context, *device, NULL, &status);

	/*	create buffers	*/	

// input
	input_buffer = clCreateBuffer(	*context,CL_MEM_READ_ONLY,
									sizeof(cl_float) * imageHeight * imageWidth,
									NULL, 
									&status);
// filter
	filter_buffer = clCreateBuffer(	*context,CL_MEM_READ_ONLY,
									sizeof(cl_float) * filterSize ,
									NULL , 
									&status);
// output
	output_buffer = clCreateBuffer(	*context,CL_MEM_WRITE_ONLY,
									sizeof(cl_float) * imageHeight * imageWidth,
									NULL , 
									&status);
	/*	write buffers*/
	 clEnqueueWriteBuffer ( queue , input_buffer, CL_TRUE, 0 , sizeof(cl_float) * imageHeight * imageWidth, (void *)inputImage, 0, NULL, NULL);
	//if(err){printf("%d, writebuffer Error",err);}
	 clEnqueueWriteBuffer ( queue , filter_buffer, CL_TRUE, 0 , sizeof(cl_float) * filterSize,  (void *)filter, 0, NULL, NULL);

	
	clSetKernelArg(kernel, 0, sizeof(int), &filterWidth);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &filter_buffer);
	clSetKernelArg(kernel, 2, sizeof(int), &imageHeight);
	clSetKernelArg(kernel, 3, sizeof(int), &imageWidth);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), &input_buffer);
	clSetKernelArg(kernel, 5, sizeof(cl_mem), &output_buffer);
/*
   if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }
*/
	/* execute */
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global , &local, 0, NULL, NULL);
/*
	if (err != CL_SUCCESS)
    {
        printf("Error: Failed to execute kernel!,err num = %d\n",err);
//        return EXIT_FAILURE;
    }
*/
	/*	load the result		*/
	clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, sizeof(cl_float) * imageHeight * imageWidth, outputImage, 0, NULL, NULL);
/*
	if(err){
		printf("NO!!!!");
	}
*/
/*
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(filter_buffer);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
*/
/*
	for( i = 0 ; i<imageHeight ;i++){
		for( j = 0 ; j<imageWidth ; j++ ){
			outputImage[i*imageWidth+j] = 20.0f;
		}
	}

	for( i = 0 ; i<imageHeight ;i++){
		for( j = 0 ; j<imageWidth ; j++ ){
			printf("idx = %d , num = %.1f \n", i*imageWidth+j,outputImage[i*imageWidth+j]);
		}
		
	}
*/
}
