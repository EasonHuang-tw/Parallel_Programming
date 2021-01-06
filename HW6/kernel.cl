__kernel void convolution(	
							const int filterWidth,
							__constant float* filter,
							const int imageHeight,
							const int imageWidth,
							__global float* inputImage,
							__global float* outputImage
						){	

	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int halfWidth = filterWidth/2;
	float sum = 0;
	int k =0;
	int l = 0;
	const int idxTmp = j * get_global_size(0) +i;
	const int x = idxTmp % imageWidth;
	const int y = idxTmp / imageWidth;

	for( k = -halfWidth ; k <= halfWidth ; k++  ){
		for ( l = -halfWidth ; l <= halfWidth ; l++  ){
			if( y + k >= 0 && y + k < imageHeight && x + l >=0 && x + l < imageWidth){
				sum  += inputImage[(idxTmp + k*imageWidth + l)] * filter[(k+halfWidth)*filterWidth + l + halfWidth];
			}

		}
	}

	outputImage[idxTmp] = sum;

}
