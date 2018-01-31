/*
 * File: repmat.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "repmat.h"

/* Function Definitions */

/*
 * Arguments    : const double a[2]
 *                double b[4]
 * Return Type  : void
 */
void b_repmat(const double a[2], double b[4])
{
  int jtilecol;
  int ibtile;
  int k;
  for (jtilecol = 0; jtilecol < 2; jtilecol++) {
    ibtile = jtilecol << 1;
    for (k = 0; k < 2; k++) {
      b[ibtile + k] = a[k];
    }
  }
}

/*
 * Arguments    : const double a[3]
 *                double b[24]
 * Return Type  : void
 */
void repmat(const double a[3], double b[24])
{
  int jtilecol;
  int ibtile;
  int k;
  for (jtilecol = 0; jtilecol < 8; jtilecol++) {
    ibtile = jtilecol * 3;
    for (k = 0; k < 3; k++) {
      b[ibtile + k] = a[k];
    }
  }
}

/*
 * File trailer for repmat.c
 *
 * [EOF]
 */
