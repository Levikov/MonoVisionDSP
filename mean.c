/*
 * File: mean.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 14:24:47
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "mean.h"

/* Function Definitions */

/*
 * Arguments    : const double x[24]
 *                double y[3]
 * Return Type  : void
 */
void mean(const double x[24], double y[3])
{
  int j;
  double s;
  int k;
  for (j = 0; j < 3; j++) {
    s = x[j];
    for (k = 0; k < 7; k++) {
      s += x[j + (k + 1) * 3];
    }

    y[j] = s / 8.0;
  }
}

/*
 * File trailer for mean.c
 *
 * [EOF]
 */
