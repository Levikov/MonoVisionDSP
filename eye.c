/*
 * File: eye.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "eye.h"

/* Function Definitions */

/*
 * Arguments    : double I[9]
 * Return Type  : void
 */
void eye(double I[9])
{
  int k;
  memset(&I[0], 0, 9U * sizeof(double));
  for (k = 0; k < 3; k++) {
    I[k + 3 * k] = 1.0;
  }
}

/*
 * File trailer for eye.c
 *
 * [EOF]
 */
