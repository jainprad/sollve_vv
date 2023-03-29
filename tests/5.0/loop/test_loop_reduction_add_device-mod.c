//===--- test_loop_reduction_add_device-mod.c ------------------------------------------===//
//
// OpenMP API Version 5.0 Nov 2018
//
// This test uses the reduction clause on a loop directive, testing that the
// variables in the reduction clause are properly reduced using the add
// operator. This test checks the above in a target context.
// Test written in reference to test_loop_reduction_add_device.c.
//
//===----------------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

int test_add() {
  int n = -32, errors=0; 
  float f = -32.0;
  double d = -32.0;
  char c = '(';

#pragma omp target map(tofrom:n,f,d,c) 
  {
  #pragma omp loop reduction(+:n,f,d,c) 
    for(int i=0;i<64;i++){
      n = n + 1;
      f = f + 1.0;  
      d = d + 1.0;  
      c = c + 1;
    }
  }

  OMPVV_TEST_AND_SET_VERBOSE(errors, (n != 32) || (f != 32.0) || (d != 32.0) || (c != 'h'));

  OMPVV_ERROR_IF(errors, "\nn|EXPECTED:32, RECEIVED:%d\nf|EXPECTED:32.0, RECEIVED:%.1f\nd|EXPECTED:32.0, RECEIVED:%.1f\nc|EXPECTED:h, RECEIVED:%c\n",n,f,d,c); 

  return errors;
}
int main() {
  OMPVV_TEST_OFFLOADING;

  int total_errors = 0;

  OMPVV_TEST_AND_SET_VERBOSE(total_errors, test_add() != 0);

  OMPVV_REPORT_AND_RETURN(total_errors);
}
