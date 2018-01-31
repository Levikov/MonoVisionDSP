/*
 * File: kron.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "kron.h"

/* Function Definitions */

/*
 * Arguments    : const double A[3]
 *                const double B[9]
 *                double K[27]
 * Return Type  : void
 */
void kron(const double A[3], const double B[9], double K[27])
{
  int kidx;
  int b_j1;
  int j2;
  int i2;
  kidx = -1;
  for (b_j1 = 0; b_j1 < 3; b_j1++) {
    for (j2 = 0; j2 < 3; j2++) {
      for (i2 = 0; i2 < 3; i2++) {
        kidx++;
        K[kidx] = A[b_j1] * B[i2 + 3 * j2];
      }
    }
  }
}

/*
 * File trailer for kron.c
 *
 * [EOF]
 */
