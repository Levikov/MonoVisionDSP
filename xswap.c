/*
 * File: xswap.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "xswap.h"

/* Function Definitions */

/*
 * Arguments    : double x[9]
 *                int ix0
 *                int iy0
 * Return Type  : void
 */
void b_xswap(double x[9], int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  double temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 3; k++) {
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
  }
}

/*
 * Arguments    : double x[16]
 *                int ix0
 *                int iy0
 * Return Type  : void
 */
void xswap(double x[16], int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  double temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 4; k++) {
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix++;
    iy++;
  }
}

/*
 * File trailer for xswap.c
 *
 * [EOF]
 */
