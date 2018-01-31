/*
 * File: svd1.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "svd1.h"
#include "svd.h"

/* Function Definitions */

/*
 * Arguments    : const double A[9]
 *                double U[9]
 *                double S[9]
 *                double V[9]
 * Return Type  : void
 */
void b_svd(const double A[9], double U[9], double S[9], double V[9])
{
  boolean_T p;
  int i;
  double dv1[9];
  double s[3];
  double U1[9];
  double V1[9];
  p = false;
  for (i = 0; i < 9; i++) {
    if (p || rtIsInf(A[i]) || rtIsNaN(A[i])) {
      p = true;
    } else {
      p = false;
    }
  }

  if (!p) {
    c_svd(A, U, s, V);
  } else {
    memset(&dv1[0], 0, 9U * sizeof(double));
    c_svd(dv1, U1, s, V1);
    for (i = 0; i < 9; i++) {
      U[i] = rtNaN;
    }

    for (i = 0; i < 3; i++) {
      s[i] = rtNaN;
    }

    for (i = 0; i < 9; i++) {
      V[i] = rtNaN;
    }
  }

  memset(&S[0], 0, 9U * sizeof(double));
  for (i = 0; i < 3; i++) {
    S[i + 3 * i] = s[i];
  }
}

/*
 * File trailer for svd1.c
 *
 * [EOF]
 */
