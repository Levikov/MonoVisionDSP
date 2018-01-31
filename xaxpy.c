/*
 * File: xaxpy.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "xaxpy.h"

/* Function Definitions */

/*
 * Arguments    : int n
 *                double a
 *                const double x[16]
 *                int ix0
 *                double y[4]
 *                int iy0
 * Return Type  : void
 */
void b_xaxpy(int n, double a, const double x[16], int ix0, double y[4], int iy0)
{
  int ix;
  int iy;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                const double x[4]
 *                int ix0
 *                double y[16]
 *                int iy0
 * Return Type  : void
 */
void c_xaxpy(int n, double a, const double x[4], int ix0, double y[16], int iy0)
{
  int ix;
  int iy;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                int ix0
 *                double y[9]
 *                int iy0
 * Return Type  : void
 */
void d_xaxpy(int n, double a, int ix0, double y[9], int iy0)
{
  int ix;
  int iy;
  int k;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                const double x[9]
 *                int ix0
 *                double y[3]
 *                int iy0
 * Return Type  : void
 */
void e_xaxpy(int n, double a, const double x[9], int ix0, double y[3], int iy0)
{
  int ix;
  int iy;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                const double x[3]
 *                int ix0
 *                double y[9]
 *                int iy0
 * Return Type  : void
 */
void f_xaxpy(int n, double a, const double x[3], int ix0, double y[9], int iy0)
{
  int ix;
  int iy;
  int k;
  if (!(a == 0.0)) {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * x[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                int ix0
 *                double y[16]
 *                int iy0
 * Return Type  : void
 */
void xaxpy(int n, double a, int ix0, double y[16], int iy0)
{
  int ix;
  int iy;
  int k;
  if ((n < 1) || (a == 0.0)) {
  } else {
    ix = ix0 - 1;
    iy = iy0 - 1;
    for (k = 0; k < n; k++) {
      y[iy] += a * y[ix];
      ix++;
      iy++;
    }
  }
}

/*
 * File trailer for xaxpy.c
 *
 * [EOF]
 */
