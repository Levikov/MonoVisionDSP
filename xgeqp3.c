/*
 * xgeqp3.c
 *
 * Code generation for function 'xgeqp3'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "getH.h"
#include "xgeqp3.h"
#include "xnrm2.h"
#include "xswap.h"
#include "ixamax.h"

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

void xgeqp3(double A[12], double tau[3], int jpvt[3])
{
  int k;
  int j;
  int i;
  double work[3];
  int i_i;
  double xnorm;
  int pvt;
  double vn1[3];
  double vn2[3];
  double t;
  double absxk;
  double beta1;
  int i0;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  int ix;
  int ijA;
  k = 1;
  for (j = 0; j < 3; j++) {
    jpvt[j] = 1 + j;
    work[j] = 0.0;
    xnorm = xnrm2(A, k);
    vn2[j] = xnorm;
    k += 4;
    vn1[j] = xnorm;
  }

  for (i = 0; i < 3; i++) {
    i_i = i + (i << 2);
    pvt = (i + ixamax(3 - i, vn1, i + 1)) - 1;
    if (pvt + 1 != i + 1) {
      xswap(A, 1 + (pvt << 2), 1 + (i << 2));
      k = jpvt[pvt];
      jpvt[pvt] = jpvt[i];
      jpvt[i] = k;
      vn1[pvt] = vn1[i];
      vn2[pvt] = vn2[i];
    }

    t = A[i_i];
    absxk = 0.0;
    xnorm = b_xnrm2(3 - i, A, i_i + 2);
    if (xnorm != 0.0) {
      beta1 = rt_hypotd_snf(A[i_i], xnorm);
      if (A[i_i] >= 0.0) {
        beta1 = -beta1;
      }

      if (fabs(beta1) < 1.0020841800044864E-292) {
        pvt = 0;
        i0 = (i_i - i) + 4;
        do {
          pvt++;
          for (k = i_i + 1; k + 1 <= i0; k++) {
            A[k] *= 9.9792015476736E+291;
          }

          beta1 *= 9.9792015476736E+291;
          t *= 9.9792015476736E+291;
        } while (!(fabs(beta1) >= 1.0020841800044864E-292));

        beta1 = rt_hypotd_snf(t, b_xnrm2(3 - i, A, i_i + 2));
        if (t >= 0.0) {
          beta1 = -beta1;
        }

        absxk = (beta1 - t) / beta1;
        t = 1.0 / (t - beta1);
        i0 = (i_i - i) + 4;
        for (k = i_i + 1; k + 1 <= i0; k++) {
          A[k] *= t;
        }

        for (k = 1; k <= pvt; k++) {
          beta1 *= 1.0020841800044864E-292;
        }

        t = beta1;
      } else {
        absxk = (beta1 - A[i_i]) / beta1;
        t = 1.0 / (A[i_i] - beta1);
        i0 = (i_i - i) + 4;
        for (k = i_i + 1; k + 1 <= i0; k++) {
          A[k] *= t;
        }

        t = beta1;
      }
    }

    tau[i] = absxk;
    A[i_i] = t;
    if (i + 1 < 3) {
      t = A[i_i];
      A[i_i] = 1.0;
      i_ip1 = (i + ((i + 1) << 2)) + 1;
      if (tau[i] != 0.0) {
        lastv = 4 - i;
        pvt = (i_i - i) + 3;
        while ((lastv > 0) && (A[pvt] == 0.0)) {
          lastv--;
          pvt--;
        }

        lastc = 2 - i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          pvt = i_ip1 + ((lastc - 1) << 2);
          j = pvt;
          do {
            exitg1 = 0;
            if (j <= (pvt + lastv) - 1) {
              if (A[j - 1] != 0.0) {
                exitg1 = 1;
              } else {
                j++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          for (pvt = 1; pvt <= lastc; pvt++) {
            work[pvt - 1] = 0.0;
          }

          pvt = 0;
          i0 = i_ip1 + ((lastc - 1) << 2);
          for (k = i_ip1; k <= i0; k += 4) {
            ix = i_i;
            xnorm = 0.0;
            ijA = (k + lastv) - 1;
            for (j = k; j <= ijA; j++) {
              xnorm += A[j - 1] * A[ix];
              ix++;
            }

            work[pvt] += xnorm;
            pvt++;
          }
        }

        if (!(-tau[i] == 0.0)) {
          pvt = i_ip1 - 1;
          k = 0;
          for (j = 1; j <= lastc; j++) {
            if (work[k] != 0.0) {
              xnorm = work[k] * -tau[i];
              ix = i_i;
              i0 = lastv + pvt;
              for (ijA = pvt; ijA + 1 <= i0; ijA++) {
                A[ijA] += A[ix] * xnorm;
                ix++;
              }
            }

            k++;
            pvt += 4;
          }
        }
      }

      A[i_i] = t;
    }

    for (j = i + 1; j + 1 < 4; j++) {
      pvt = (i + (j << 2)) + 1;
      if (vn1[j] != 0.0) {
        xnorm = fabs(A[i + (j << 2)]) / vn1[j];
        xnorm = 1.0 - xnorm * xnorm;
        if (xnorm < 0.0) {
          xnorm = 0.0;
        }

        beta1 = vn1[j] / vn2[j];
        beta1 = xnorm * (beta1 * beta1);
        if (beta1 <= 1.4901161193847656E-8) {
          xnorm = 0.0;
          if (3 - i == 1) {
            xnorm = fabs(A[pvt]);
          } else {
            beta1 = 2.2250738585072014E-308;
            k = (pvt - i) + 3;
            while (pvt + 1 <= k) {
              absxk = fabs(A[pvt]);
              if (absxk > beta1) {
                t = beta1 / absxk;
                xnorm = 1.0 + xnorm * t * t;
                beta1 = absxk;
              } else {
                t = absxk / beta1;
                xnorm += t * t;
              }

              pvt++;
            }

            xnorm = beta1 * sqrt(xnorm);
          }

          vn1[j] = xnorm;
          vn2[j] = vn1[j];
        } else {
          vn1[j] *= sqrt(xnorm);
        }
      }
    }
  }
}

/* End of code generation (xgeqp3.c) */
