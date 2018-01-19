/*
 * File: det.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 19-Jan-2018 19:38:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "det.h"

/* Function Definitions */

/*
 * Arguments    : const double x[9]
 * Return Type  : double
 */
double det(const double x[9])
{
  double y;
  double b_x[9];
  int i0;
  int j;
  signed char ipiv[3];
  int c;
  boolean_T isodd;
  int iy;
  int jy;
  int ix;
  double smax;
  double s;
  int b_j;
  int ijA;
  memcpy(&b_x[0], &x[0], 9U * sizeof(double));
  for (i0 = 0; i0 < 3; i0++) {
    ipiv[i0] = (signed char)(1 + i0);
  }

  for (j = 0; j < 2; j++) {
    c = j << 2;
    iy = 0;
    ix = c;
    smax = fabs(b_x[c]);
    for (jy = 2; jy <= 3 - j; jy++) {
      ix++;
      s = fabs(b_x[ix]);
      if (s > smax) {
        iy = jy - 1;
        smax = s;
      }
    }

    if (b_x[c + iy] != 0.0) {
      if (iy != 0) {
        ipiv[j] = (signed char)((j + iy) + 1);
        ix = j;
        iy += j;
        for (jy = 0; jy < 3; jy++) {
          smax = b_x[ix];
          b_x[ix] = b_x[iy];
          b_x[iy] = smax;
          ix += 3;
          iy += 3;
        }
      }

      i0 = (c - j) + 3;
      for (iy = c + 1; iy + 1 <= i0; iy++) {
        b_x[iy] /= b_x[c];
      }
    }

    iy = c;
    jy = c + 3;
    for (b_j = 1; b_j <= 2 - j; b_j++) {
      smax = b_x[jy];
      if (b_x[jy] != 0.0) {
        ix = c + 1;
        i0 = (iy - j) + 6;
        for (ijA = 4 + iy; ijA + 1 <= i0; ijA++) {
          b_x[ijA] += b_x[ix] * -smax;
          ix++;
        }
      }

      jy += 3;
      iy += 3;
    }
  }

  y = b_x[0];
  isodd = false;
  for (jy = 0; jy < 2; jy++) {
    y *= b_x[(jy + 3 * (jy + 1)) + 1];
    if (ipiv[jy] > 1 + jy) {
      isodd = !isodd;
    }
  }

  if (isodd) {
    y = -y;
  }

  return y;
}

/*
 * File trailer for det.c
 *
 * [EOF]
 */
