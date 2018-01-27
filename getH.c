/*
 * getH.c
 *
 * Code generation for function 'getH'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "getH.h"
#include "xgeqp3.h"

/* Function Definitions */
void getH(const double Y[12], const double X[12], double H[9])
{
  int rankR;
  double A[12];
  double tau[3];
  int jpvt[3];
  int j;
  double tol;
  double B[12];
  double b_Y[9];
  int k;
  int i;
  for (rankR = 0; rankR < 3; rankR++) {
    for (j = 0; j < 4; j++) {
      B[j + (rankR << 2)] = Y[rankR + 3 * j];
      A[j + (rankR << 2)] = X[rankR + 3 * j];
    }
  }

  xgeqp3(A, tau, jpvt);
  rankR = 0;
  tol = 4.0 * fabs(A[0]) * 2.2204460492503131E-16;
  while ((rankR < 3) && (fabs(A[rankR + (rankR << 2)]) >= tol)) {
    rankR++;
  }

  memset(&b_Y[0], 0, 9U * sizeof(double));
  for (j = 0; j < 3; j++) {
    if (tau[j] != 0.0) {
      for (k = 0; k < 3; k++) {
        tol = B[j + (k << 2)];
        for (i = j + 1; i + 1 < 5; i++) {
          tol += A[i + (j << 2)] * B[i + (k << 2)];
        }

        tol *= tau[j];
        if (tol != 0.0) {
          B[j + (k << 2)] -= tol;
          for (i = j + 1; i + 1 < 5; i++) {
            B[i + (k << 2)] -= A[i + (j << 2)] * tol;
          }
        }
      }
    }
  }

  for (k = 0; k < 3; k++) {
    for (i = 0; i + 1 <= rankR; i++) {
      b_Y[(jpvt[i] + 3 * k) - 1] = B[i + (k << 2)];
    }

    for (j = rankR - 1; j + 1 > 0; j--) {
      b_Y[(jpvt[j] + 3 * k) - 1] /= A[j + (j << 2)];
      for (i = 0; i + 1 <= j; i++) {
        b_Y[(jpvt[i] + 3 * k) - 1] -= b_Y[(jpvt[j] + 3 * k) - 1] * A[i + (j << 2)];
      }
    }
  }

  for (rankR = 0; rankR < 3; rankR++) {
    for (j = 0; j < 3; j++) {
      H[j + 3 * rankR] = b_Y[rankR + 3 * j];
    }
  }
}

/* End of code generation (getH.c) */
