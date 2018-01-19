/*
 * File: repmat.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 19-Jan-2018 19:38:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "repmat.h"

/* Function Definitions */

/*
 * Arguments    : const double a[3]
 *                double b[12]
 * Return Type  : void
 */
void repmat(const double a[3], double b[12])
{
  int jtilecol;
  int ibtile;
  int k;
  for (jtilecol = 0; jtilecol < 4; jtilecol++) {
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
