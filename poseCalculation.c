/*
 * File: poseCalculation.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "orthogonalIteration.h"
#include "inv.h"
#include "norm.h"
#include "repmat.h"
#include "sign.h"
#include "pinv.h"

/* Function Definitions */

/*
 * poseCalc - Description
 *
 *  Syntax:[R,t] = poseCalc(X,Y,camera)
 *
 *  Pose Calculation Algorithm
 *  Input:
 *  X - 3 x n world coordinates of n targets
 *  Y - 3 x n focus unified image coordinates of n projections of targets
 *  Output:
 *  R - 3 x 3 rotation matrix
 *  t - 3 x 1 translation vector
 * Arguments    : const double X[32]
 *                double Y[24]
 *                const double M[9]
 *                double epsilon
 *                double maxIteration
 *                double Angle[3]
 *                double t[6]
 * Return Type  : void
 */
void poseCalculation(const double X[32], double Y[24], const double M[9], double
                     epsilon, double maxIteration, double Angle[3], double t[6])
{
  double dv0[24];
  int ix;
  int ixstart;
  double A[16];
  double a[16];
  double b_A[32];
  double I0hap[4];
  int jj;
  double x0;
  double J0hap[4];
  double b_a[32];
  double b_y0;
  double c_a;
  double b;
  double d0;
  double delta;
  double d;
  double e;
  double g;
  double AA;
  double B;
  double AAA;
  double BB;
  double delta2;
  double x;
  double lambda[2];
  double miu[2];
  double err[4];
  double err2[4];
  double y[4];
  double err1[4];
  double minval[2];
  boolean_T exitg1;
  int ii;
  boolean_T guard1 = false;
  int ii_data[4];
  signed char jj_data[4];
  int minR_data[4];
  double R[18];
  signed char minC_data[4];
  double Ip[3];
  double Jp[3];
  static const signed char b_b[3] = { 0, 0, 1 };

  double I[9];
  double b_M[9];
  double c_M[9];
  double b_I[9];
  double b_Ip[3];
  double k[3];
  repmat(*(double (*)[3])&M[6], dv0);
  for (ix = 0; ix < 24; ix++) {
    Y[ix] = (Y[ix] - dv0[ix]) / M[0];
  }

  for (ix = 0; ix < 8; ix++) {
    Y[2 + 3 * ix] = 1.0;
  }

  /*  Numer of targets */
  for (ix = 0; ix < 4; ix++) {
    for (ixstart = 0; ixstart < 8; ixstart++) {
      b_A[ixstart + (ix << 3)] = X[ix + (ixstart << 2)];
    }
  }

  for (ix = 0; ix < 4; ix++) {
    for (ixstart = 0; ixstart < 4; ixstart++) {
      A[ix + (ixstart << 2)] = 0.0;
      for (jj = 0; jj < 8; jj++) {
        A[ix + (ixstart << 2)] += b_A[jj + (ix << 3)] * b_A[jj + (ixstart << 3)];
      }
    }
  }

  pinv(A, a);
  for (ix = 0; ix < 4; ix++) {
    I0hap[ix] = 0.0;
    for (ixstart = 0; ixstart < 8; ixstart++) {
      b_a[ix + (ixstart << 2)] = 0.0;
      for (jj = 0; jj < 4; jj++) {
        b_a[ix + (ixstart << 2)] += a[ix + (jj << 2)] * b_A[ixstart + (jj << 3)];
      }

      I0hap[ix] += b_a[ix + (ixstart << 2)] * Y[3 * ixstart];
    }

    for (ixstart = 0; ixstart < 4; ixstart++) {
      A[ix + (ixstart << 2)] = 0.0;
      for (jj = 0; jj < 8; jj++) {
        A[ix + (ixstart << 2)] += b_A[jj + (ix << 3)] * b_A[jj + (ixstart << 3)];
      }
    }
  }

  pinv(A, a);
  for (ix = 0; ix < 4; ix++) {
    J0hap[ix] = 0.0;
    for (ixstart = 0; ixstart < 8; ixstart++) {
      b_a[ix + (ixstart << 2)] = 0.0;
      for (jj = 0; jj < 4; jj++) {
        b_a[ix + (ixstart << 2)] += a[ix + (jj << 2)] * b_A[ixstart + (jj << 3)];
      }

      J0hap[ix] += b_a[ix + (ixstart << 2)] * Y[1 + 3 * ixstart];
    }
  }

  x0 = I0hap[3];
  b_y0 = J0hap[3];
  c_a = I0hap[3] * J0hap[3] / (1.0 + I0hap[3] * I0hap[3]);
  b = I0hap[3] * J0hap[3] / (1.0 + J0hap[3] * J0hap[3]);
  d0 = 0.0;
  for (ix = 0; ix < 3; ix++) {
    d0 += I0hap[ix] * J0hap[ix];
  }

  delta = norm(*(double (*)[3])&I0hap[0]);
  d = delta * delta;
  delta = norm(*(double (*)[3])&J0hap[0]);
  e = delta * delta;
  g = (1.0 + I0hap[3] * I0hap[3]) / (1.0 + J0hap[3] * J0hap[3]);
  AA = c_a * c_a - g;
  B = ((2.0 * (c_a * c_a) * d - g * d) + e) - 2.0 * c_a * d0;
  AAA = b * b - 1.0 / g;
  BB = ((2.0 * (b * b) * e - e / g) + d) - 2.0 * d * d0;
  delta = B * B - 4.0 * AA * ((c_a * c_a * (d * d) + d0 * d0) - 2.0 * c_a * d0 *
    d);
  delta2 = BB * BB - 4.0 * AAA * ((b * b * (e * e) + d0 * d0) - 2.0 * b * d0 * e);
  x = AA;
  if (AA < 0.0) {
    x = -1.0;
  } else if (AA > 0.0) {
    x = 1.0;
  } else {
    if (AA == 0.0) {
      x = 0.0;
    }
  }

  g = AA;
  b_sign(&g);
  lambda[0] = sqrt((-B + g * sqrt(delta)) / (2.0 * AA));
  lambda[1] = -sqrt((-B + x * sqrt(delta)) / (2.0 * AA));
  x = AAA;
  if (AAA < 0.0) {
    x = -1.0;
  } else if (AAA > 0.0) {
    x = 1.0;
  } else {
    if (AAA == 0.0) {
      x = 0.0;
    }
  }

  AA = AAA;
  if (AAA < 0.0) {
    AA = -1.0;
  } else if (AAA > 0.0) {
    AA = 1.0;
  } else {
    if (AAA == 0.0) {
      AA = 0.0;
    }
  }

  miu[0] = sqrt((-BB + x * sqrt(delta2)) / (2.0 * AAA));
  miu[1] = -sqrt((-BB + AA * sqrt(delta2)) / (2.0 * AAA));
  b_repmat(lambda, err);
  for (jj = 0; jj < 4; jj++) {
    y[jj] = err[jj] * err[jj];
  }

  b_repmat(miu, err2);
  for (ix = 0; ix < 2; ix++) {
    for (ixstart = 0; ixstart < 2; ixstart++) {
      err1[ix + (ixstart << 1)] = (d0 + lambda[ix] * miu[ixstart]) - c_a * (d +
        2.0 * y[ix + (ixstart << 1)]);
      err[ixstart + (ix << 1)] = err2[ix + (ixstart << 1)];
    }
  }

  for (jj = 0; jj < 4; jj++) {
    y[jj] = err[jj] * err[jj];
  }

  for (ix = 0; ix < 2; ix++) {
    for (ixstart = 0; ixstart < 2; ixstart++) {
      err2[ix + (ixstart << 1)] = (d0 + lambda[ix] * miu[ixstart]) - b * (e +
        2.0 * y[ix + (ixstart << 1)]);
    }
  }

  for (jj = 0; jj < 4; jj++) {
    err[jj] = fabs(err1[jj]) + fabs(err2[jj]);
  }

  for (jj = 0; jj < 2; jj++) {
    ix = jj << 1;
    ixstart = (jj << 1) + 1;
    g = err[ix];
    if (rtIsNaN(err[ix])) {
      ii = ixstart;
      exitg1 = false;
      while ((!exitg1) && (ii + 1 <= ix + 2)) {
        ixstart = ii + 1;
        if (!rtIsNaN(err[ii])) {
          g = err[ii];
          exitg1 = true;
        } else {
          ii++;
        }
      }
    }

    if (ixstart < ix + 2) {
      while (ixstart + 1 <= ix + 2) {
        if (err[ixstart] < g) {
          g = err[ixstart];
        }

        ixstart++;
      }
    }

    minval[jj] = g;
  }

  ixstart = 1;
  g = minval[0];
  if (rtIsNaN(minval[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 3)) {
      ixstart = 2;
      if (!rtIsNaN(minval[1])) {
        g = minval[1];
        exitg1 = true;
      } else {
        ix = 3;
      }
    }
  }

  if ((ixstart < 2) && (minval[1] < g)) {
    g = minval[1];
  }

  ixstart = 0;
  ii = 1;
  jj = 1;
  exitg1 = false;
  while ((!exitg1) && (jj <= 2)) {
    guard1 = false;
    if (err[(ii + ((jj - 1) << 1)) - 1] == g) {
      ixstart++;
      ii_data[ixstart - 1] = ii;
      jj_data[ixstart - 1] = (signed char)jj;
      if (ixstart >= 4) {
        exitg1 = true;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1) {
      ii++;
      if (ii > 2) {
        ii = 1;
        jj++;
      }
    }
  }

  if (1 > ixstart) {
    jj = 0;
  } else {
    jj = ixstart;
  }

  for (ix = 0; ix < jj; ix++) {
    minR_data[ix] = ii_data[ix];
  }

  if (1 > ixstart) {
    jj = 0;
  } else {
    jj = ixstart;
  }

  for (ix = 0; ix < jj; ix++) {
    minC_data[ix] = jj_data[ix];
  }

  memset(&R[0], 0, 18U * sizeof(double));
  for (ii = 0; ii < 2; ii++) {
    for (ix = 0; ix < 3; ix++) {
      Ip[ix] = I0hap[ix] + lambda[minR_data[ii] - 1] * (double)b_b[ix];
      Jp[ix] = J0hap[ix] + miu[minC_data[ii] - 1] * (double)b_b[ix];
    }

    g = sqrt(sqrt(1.0 + x0 * x0) / norm(Ip) * sqrt(1.0 + b_y0 * b_y0) / norm(Jp));
    memset(&I[0], 0, 9U * sizeof(double));
    for (jj = 0; jj < 3; jj++) {
      I[jj + 3 * jj] = 1.0;
    }

    /* crossMatrix - Description */
    /*  */
    /*  Syntax: M = crossMatrix(v) */
    /*  */
    /*  Get cross product matrix from vector v */
    /*  Input: */
    /*  v - 3 x 1 vector */
    /*  M - 3 x 1 matrix */
    memset(&b_M[0], 0, 9U * sizeof(double));
    memset(&c_M[0], 0, 9U * sizeof(double));
    b_M[3] = -Ip[2];
    b_M[6] = Ip[1];
    b_M[1] = Ip[2];
    b_M[7] = -Ip[0];
    b_M[2] = -Ip[1];
    b_M[5] = Ip[0];
    c_a = g * b_y0;

    /* crossMatrix - Description */
    /*  */
    /*  Syntax: M = crossMatrix(v) */
    /*  */
    /*  Get cross product matrix from vector v */
    /*  Input: */
    /*  v - 3 x 1 vector */
    /*  M - 3 x 1 matrix */
    c_M[3] = -Jp[2];
    c_M[6] = Jp[1];
    c_M[1] = Jp[2];
    c_M[7] = -Jp[0];
    c_M[2] = -Jp[1];
    c_M[5] = Jp[0];
    delta = g * x0;
    AA = g * g;
    for (ix = 0; ix < 9; ix++) {
      b_I[ix] = (I[ix] - c_a * b_M[ix]) + delta * c_M[ix];
    }

    inv(b_I, I);
    b_Ip[0] = Ip[1] * Jp[2] - Ip[2] * Jp[1];
    b_Ip[1] = Ip[2] * Jp[0] - Ip[0] * Jp[2];
    b_Ip[2] = Ip[0] * Jp[1] - Ip[1] * Jp[0];
    for (ix = 0; ix < 3; ix++) {
      k[ix] = 0.0;
      for (ixstart = 0; ixstart < 3; ixstart++) {
        k[ix] += I[ix + 3 * ixstart] * AA * b_Ip[ixstart];
      }

      R[3 * ix + 9 * ii] = g * Ip[ix] + x0 * k[ix];
      R[1 + (3 * ix + 9 * ii)] = g * Jp[ix] + b_y0 * k[ix];
      R[2 + (3 * ix + 9 * ii)] = k[ix];
    }

    orthogonalIteration(X, Y, *(double (*)[9])&R[9 * ii], epsilon, maxIteration,
                        *(double (*)[3])&t[3 * ii], &d0);
  }

  Angle[0] = -asin(R[7]) / 3.1415926535897931 * 180.0;
  Angle[1] = atan(R[1] / R[4]) / 3.1415926535897931 * 180.0;
  Angle[2] = atan(R[6] / R[8]) / 3.1415926535897931 * 180.0;
}

/*
 * File trailer for poseCalculation.c
 *
 * [EOF]
 */
