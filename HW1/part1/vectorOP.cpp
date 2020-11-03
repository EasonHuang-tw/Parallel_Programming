#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{

  
  int mod = N%VECTOR_WIDTH;
  if(mod!=0){
  	for(int i =0;i<VECTOR_WIDTH-mod;i++){
		values[N+i]=0;
		exponents[N+i]=1;
	}
  }
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  __pp_vec_float x;
  __pp_vec_int y,Count;
  __pp_vec_float result;
  __pp_vec_int zero = _pp_vset_int(0);
  __pp_vec_int Const_1 = _pp_vset_int(1);
  __pp_vec_float Const_9_9 = _pp_vset_float(9.999999f);
  __pp_mask maskAll, maskIsEqual, maskIsNotEqual, maskCount,maskGreater;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsEqual = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];
    _pp_vload_int(y, exponents + i, maskAll); // y = exponents[i];

    // Set mask according to predicate
    _pp_veq_int(maskIsEqual, y, zero, maskAll); //if(y==0)
    _pp_vset_float(result, 1.0f , maskIsEqual);//output[i] = 1.f
     

    // Inverse maskIsEqual to generate "else" mask
    maskIsNotEqual = _pp_init_ones(0);
    maskIsNotEqual = _pp_mask_not(maskIsEqual); // } else {

    // Execute instruction ("else" clause)
    _pp_vmove_float(result, x , maskIsNotEqual); // result = x

    _pp_vsub_int(Count, y, Const_1, maskIsNotEqual);    //int count = y - 1;
    maskCount = _pp_init_ones(0);
    _pp_vgt_int(maskCount, Count , zero , maskIsNotEqual);
    
    	while(_pp_cntbits(maskCount)>0){
		_pp_vmult_float(result, result, x, maskCount);
   		_pp_vsub_int(Count, Count, Const_1, maskCount);    //count --;
   		_pp_vgt_int(maskCount, Count , zero , maskCount);
	}
    
    maskGreater = _pp_init_ones(0);
    _pp_vgt_float(maskGreater, result , Const_9_9 , maskIsNotEqual);
    _pp_vmove_float(result, Const_9_9,  maskGreater);

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  int mod = N%VECTOR_WIDTH;
  if(mod!=0){
  	for(int i =0;i<VECTOR_WIDTH-mod;i++){
		values[N+i]=0;
	}
  }
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_int zero = _pp_vset_int(0);
  __pp_mask maskAll;

  double Sum = 0;
  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();
    
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    _pp_hadd_float(result, x);
    _pp_interleave_float(result, result);

    for(int j = 0;j<VECTOR_WIDTH/2; j++){
    	Sum+=result.value[j];
    }

  }

  return Sum;
}
