/*
 * File: xscal.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 19-Jan-2018 19:38:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "xscal.h"

/* Function Definitions */

/*
 * Arguments    : double a
 *                double x[9]
 *                int ix0
 * Return Type  : void
 */
void xscal(double a, double x[9], int ix0)
{
  int k;
  for (k = ix0; k <= ix0 + 2; k++) {
    x[k - 1] *= a;
  }
}

/*
 * File trailer for xscal.c
 *
 * [EOF]
 */
