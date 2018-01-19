/*
 * File: svd.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 19-Jan-2018 19:38:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "svd.h"
#include "svd1.h"

/* Function Definitions */

/*
 * Arguments    : const double A[9]
 *                double U[9]
 *                double S[9]
 *                double V[9]
 * Return Type  : void
 */
void svd(const double A[9], double U[9], double S[9], double V[9])
{
  boolean_T p;
  int i;
  double dv4[9];
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
    b_svd(A, U, s, V);
  } else {
    memset(&dv4[0], 0, 9U * sizeof(double));
    b_svd(dv4, U1, s, V1);
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
 * File trailer for svd.c
 *
 * [EOF]
 */
