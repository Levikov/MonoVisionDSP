/*
 * File: diag.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "diag.h"

/* Function Definitions */

/*
 * Arguments    : const double v[3]
 *                double d[9]
 * Return Type  : void
 */
void diag(const double v[3], double d[9])
{
  int j;
  memset(&d[0], 0, 9U * sizeof(double));
  for (j = 0; j < 3; j++) {
    d[j + 3 * j] = v[j];
  }
}

/*
 * File trailer for diag.c
 *
 * [EOF]
 */
