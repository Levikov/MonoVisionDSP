/*
 * File: orthogonalIteration.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "orthogonalIteration.h"
#include "mod.h"
#include "eye.h"
#include "kron.h"
#include "norm.h"
#include "diag.h"
#include "det.h"
#include "svd1.h"
#include "repmat.h"
#include "mean.h"
#include "inv.h"

/* Function Definitions */

/*
 * orthogonalIteration - Description
 *
 *  Syntax: [R,t] = orthognalIteration(X,Y,R0,epsilon)
 *
 *  Orthogonal Iteration Algorithm for Pose Estimation
 *  Input:
 *  X - 3 x n world coordinates of n targets
 *  Y - 3 x n focus unified image coordinates of n projections of targets
 *  R0 - 3 x 3 initial rotation matrix
 *  epsilon - min error
 *  Output:
 *  R - 3 x 3 rotation matrix
 *  t - 3 x 1 translation vector
 *  err - indicate the transform error
 * Arguments    : const double X[32]
 *                const double Y[24]
 *                double R0[9]
 *                double epsilon
 *                double maxIteration
 *                double t[3]
 *                double *err2
 * Return Type  : void
 */
void orthogonalIteration(const double X[32], const double Y[24], double R0[9],
  double epsilon, double maxIteration, double t[3], double *err2)
{
  double G[27];
  double F[9];
  int i;
  double s[9];
  double mtmp;
  int ix;
  double b_i;
  int b_ix;
  double err[10];
  int exitg1;
  double V[72];
  double b_X[3];
  static const double dv5[9] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };

  double b[27];
  int ixstart;
  double Q[24];
  double dv6[3];
  double dv7[24];
  double b_s[9];
  double c_X[24];
  double dv8[3];
  double dv9[24];
  double b_Q[24];
  double u[9];
  double v[9];
  double minval[5];
  double c_s[3];
  boolean_T exitg2;
  boolean_T guard1 = false;
  double r;
  double b_mtmp;
  memset(&G[0], 0, 27U * sizeof(double));
  memset(&F[0], 0, 9U * sizeof(double));
  for (i = 0; i < 8; i++) {
    mtmp = 0.0;
    for (ix = 0; ix < 3; ix++) {
      mtmp += Y[ix + 3 * i] * Y[ix + 3 * i];
    }

    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        V[(ix + 3 * b_ix) + 9 * i] = Y[ix + 3 * i] * Y[b_ix + 3 * i] / mtmp;
      }
    }

    eye(s);
    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        F[b_ix + 3 * ix] += s[b_ix + 3 * ix] - V[(b_ix + 3 * ix) + 9 * i];
      }

      b_X[ix] = X[ix + (i << 2)];
    }

    kron(b_X, dv5, b);
    eye(s);
    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        b_s[b_ix + 3 * ix] = s[b_ix + 3 * ix] - V[(b_ix + 3 * ix) + 9 * i];
      }
    }

    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 9; b_ix++) {
        mtmp = 0.0;
        for (ixstart = 0; ixstart < 3; ixstart++) {
          mtmp += b_s[ix + 3 * ixstart] * b[ixstart + 3 * b_ix];
        }

        G[ix + 3 * b_ix] += mtmp;
      }
    }
  }

  inv(F, s);
  for (ix = 0; ix < 9; ix++) {
    F[ix] = -s[ix];
  }

  b_i = 0.0;
  for (ix = 0; ix < 10; ix++) {
    err[ix] = rtInf;
  }

  do {
    exitg1 = 0;
    b_i++;
    for (ix = 0; ix < 3; ix++) {
      t[ix] = 0.0;
      for (b_ix = 0; b_ix < 9; b_ix++) {
        b[ix + 3 * b_ix] = 0.0;
        for (ixstart = 0; ixstart < 3; ixstart++) {
          b[ix + 3 * b_ix] += F[ix + 3 * ixstart] * G[ixstart + 3 * b_ix];
        }

        t[ix] += b[ix + 3 * b_ix] * R0[b_ix];
      }
    }

    for (ixstart = 0; ixstart < 8; ixstart++) {
      for (ix = 0; ix < 3; ix++) {
        Q[ix + 3 * ixstart] = 0.0;
        mtmp = 0.0;
        for (b_ix = 0; b_ix < 3; b_ix++) {
          mtmp += R0[ix + 3 * b_ix] * X[b_ix + (ixstart << 2)];
        }

        b_X[ix] = mtmp + t[ix];
      }

      for (ix = 0; ix < 3; ix++) {
        Q[ix + 3 * ixstart] = 0.0;
        for (b_ix = 0; b_ix < 3; b_ix++) {
          Q[ix + 3 * ixstart] += V[(ix + 3 * b_ix) + 9 * ixstart] * b_X[b_ix];
        }
      }
    }

    mean(Q, dv6);
    repmat(dv6, dv7);
    for (ix = 0; ix < 8; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        c_X[b_ix + 3 * ix] = X[b_ix + (ix << 2)];
      }
    }

    mean(c_X, dv8);
    repmat(dv8, dv9);
    for (ix = 0; ix < 8; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        b_Q[b_ix + 3 * ix] = Q[b_ix + 3 * ix] - dv7[b_ix + 3 * ix];
      }
    }

    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 8; b_ix++) {
        c_X[b_ix + (ix << 3)] = X[ix + (b_ix << 2)] - dv9[ix + 3 * b_ix];
      }
    }

    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        b_s[ix + 3 * b_ix] = 0.0;
        for (ixstart = 0; ixstart < 8; ixstart++) {
          b_s[ix + 3 * b_ix] += b_Q[ix + 3 * ixstart] * c_X[ixstart + (b_ix << 3)];
        }
      }
    }

    b_svd(b_s, u, s, v);
    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        b_s[ix + 3 * b_ix] = 0.0;
        for (ixstart = 0; ixstart < 3; ixstart++) {
          b_s[ix + 3 * b_ix] += v[ix + 3 * ixstart] * u[b_ix + 3 * ixstart];
        }
      }
    }

    b_X[0] = 1.0;
    b_X[1] = 1.0;
    b_X[2] = det(b_s);
    diag(b_X, s);
    for (ix = 0; ix < 3; ix++) {
      for (b_ix = 0; b_ix < 3; b_ix++) {
        b_s[ix + 3 * b_ix] = 0.0;
        for (ixstart = 0; ixstart < 3; ixstart++) {
          b_s[ix + 3 * b_ix] += v[ix + 3 * ixstart] * s[ixstart + 3 * b_ix];
        }
      }

      for (b_ix = 0; b_ix < 3; b_ix++) {
        R0[ix + 3 * b_ix] = 0.0;
        for (ixstart = 0; ixstart < 3; ixstart++) {
          R0[ix + 3 * b_ix] += b_s[ix + 3 * ixstart] * u[b_ix + 3 * ixstart];
        }
      }
    }

    err[((int)(b_mod(b_i) + 1.0) + (((int)(c_mod((b_i - b_mod(b_i)) / 2.0) + 1.0)
           - 1) << 1)) - 1] = 0.0;
    eye(s);
    for (ixstart = 0; ixstart < 8; ixstart++) {
      for (ix = 0; ix < 3; ix++) {
        mtmp = 0.0;
        for (b_ix = 0; b_ix < 3; b_ix++) {
          b_s[b_ix + 3 * ix] = s[b_ix + 3 * ix] - V[(b_ix + 3 * ix) + 9 *
            ixstart];
          mtmp += R0[ix + 3 * b_ix] * X[b_ix + (ixstart << 2)];
        }

        b_X[ix] = mtmp + t[ix];
      }

      for (ix = 0; ix < 3; ix++) {
        c_s[ix] = 0.0;
        for (b_ix = 0; b_ix < 3; b_ix++) {
          c_s[ix] += b_s[ix + 3 * b_ix] * b_X[b_ix];
        }
      }

      err[((int)(b_mod(b_i) + 1.0) + (((int)(c_mod((b_i - b_mod(b_i)) / 2.0) +
              1.0) - 1) << 1)) - 1] = err[((int)(b_mod(b_i) + 1.0) + (((int)
        (c_mod((b_i - b_mod(b_i)) / 2.0) + 1.0) - 1) << 1)) - 1] + norm(c_s);
    }

    err[((int)(b_mod(b_i) + 1.0) + (((int)(c_mod((b_i - b_mod(b_i)) / 2.0) + 1.0)
           - 1) << 1)) - 1] = err[((int)(b_mod(b_i) + 1.0) + (((int)(c_mod((b_i
      - b_mod(b_i)) / 2.0) + 1.0) - 1) << 1)) - 1] / 8.0;
    for (i = 0; i < 5; i++) {
      ix = i << 1;
      ixstart = (i << 1) + 1;
      mtmp = err[ix];
      if (rtIsNaN(err[ix])) {
        b_ix = ixstart + 1;
        exitg2 = false;
        while ((!exitg2) && (b_ix <= ix + 2)) {
          ixstart = b_ix;
          if (!rtIsNaN(err[b_ix - 1])) {
            mtmp = err[b_ix - 1];
            exitg2 = true;
          } else {
            b_ix++;
          }
        }
      }

      if (ixstart < ix + 2) {
        while (ixstart + 1 <= ix + 2) {
          if (err[ixstart] < mtmp) {
            mtmp = err[ixstart];
          }

          ixstart++;
        }
      }

      minval[i] = mtmp;
    }

    ixstart = 1;
    mtmp = minval[0];
    if (rtIsNaN(minval[0])) {
      ix = 2;
      exitg2 = false;
      while ((!exitg2) && (ix < 6)) {
        ixstart = ix;
        if (!rtIsNaN(minval[ix - 1])) {
          mtmp = minval[ix - 1];
          exitg2 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 5) {
      while (ixstart + 1 < 6) {
        if (minval[ixstart] < mtmp) {
          mtmp = minval[ixstart];
        }

        ixstart++;
      }
    }

    guard1 = false;
    if (err[((int)(b_mod(b_i) + 1.0) + (((int)(c_mod((b_i - b_mod(b_i)) / 2.0) +
            1.0) - 1) << 1)) - 1] == mtmp) {
      if (!rtIsInf(b_i)) {
        r = fmod(b_i, 2.0);
      } else {
        r = rtNaN;
      }

      ixstart = 1;
      mtmp = err[(int)(r + 1.0) - 1];
      if (rtIsNaN(err[(int)(r + 1.0) - 1])) {
        ix = 2;
        exitg2 = false;
        while ((!exitg2) && (ix < 6)) {
          ixstart = ix;
          if (!rtIsNaN(err[((int)(r + 1.0) + ((ix - 1) << 1)) - 1])) {
            mtmp = err[((int)(r + 1.0) + ((ix - 1) << 1)) - 1];
            exitg2 = true;
          } else {
            ix++;
          }
        }
      }

      if (ixstart < 5) {
        while (ixstart + 1 < 6) {
          if (err[((int)(r + 1.0) + (ixstart << 1)) - 1] > mtmp) {
            mtmp = err[((int)(r + 1.0) + (ixstart << 1)) - 1];
          }

          ixstart++;
        }
      }

      if (!rtIsInf(b_i)) {
        r = fmod(b_i, 2.0);
      } else {
        r = rtNaN;
      }

      ixstart = 1;
      b_mtmp = err[(int)(r + 1.0) - 1];
      if (rtIsNaN(err[(int)(r + 1.0) - 1])) {
        ix = 2;
        exitg2 = false;
        while ((!exitg2) && (ix < 6)) {
          ixstart = ix;
          if (!rtIsNaN(err[((int)(r + 1.0) + ((ix - 1) << 1)) - 1])) {
            b_mtmp = err[((int)(r + 1.0) + ((ix - 1) << 1)) - 1];
            exitg2 = true;
          } else {
            ix++;
          }
        }
      }

      if (ixstart < 5) {
        while (ixstart + 1 < 6) {
          if (err[((int)(r + 1.0) + (ixstart << 1)) - 1] < b_mtmp) {
            b_mtmp = err[((int)(r + 1.0) + (ixstart << 1)) - 1];
          }

          ixstart++;
        }
      }

      if (fabs(mtmp - b_mtmp) < epsilon) {
        for (i = 0; i < 5; i++) {
          ix = i << 1;
          ixstart = (i << 1) + 1;
          mtmp = err[ix];
          if (rtIsNaN(err[ix])) {
            b_ix = ixstart + 1;
            exitg2 = false;
            while ((!exitg2) && (b_ix <= ix + 2)) {
              ixstart = b_ix;
              if (!rtIsNaN(err[b_ix - 1])) {
                mtmp = err[b_ix - 1];
                exitg2 = true;
              } else {
                b_ix++;
              }
            }
          }

          if (ixstart < ix + 2) {
            while (ixstart + 1 <= ix + 2) {
              if (err[ixstart] < mtmp) {
                mtmp = err[ixstart];
              }

              ixstart++;
            }
          }

          minval[i] = mtmp;
        }

        ixstart = 1;
        *err2 = minval[0];
        if (rtIsNaN(minval[0])) {
          ix = 2;
          exitg2 = false;
          while ((!exitg2) && (ix < 6)) {
            ixstart = ix;
            if (!rtIsNaN(minval[ix - 1])) {
              *err2 = minval[ix - 1];
              exitg2 = true;
            } else {
              ix++;
            }
          }
        }

        if (ixstart < 5) {
          while (ixstart + 1 < 6) {
            if (minval[ixstart] < *err2) {
              *err2 = minval[ixstart];
            }

            ixstart++;
          }
        }

        exitg1 = 1;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }

    if (guard1 && (b_i > maxIteration)) {
      for (i = 0; i < 5; i++) {
        ix = i << 1;
        ixstart = (i << 1) + 1;
        mtmp = err[ix];
        if (rtIsNaN(err[ix])) {
          b_ix = ixstart + 1;
          exitg2 = false;
          while ((!exitg2) && (b_ix <= ix + 2)) {
            ixstart = b_ix;
            if (!rtIsNaN(err[b_ix - 1])) {
              mtmp = err[b_ix - 1];
              exitg2 = true;
            } else {
              b_ix++;
            }
          }
        }

        if (ixstart < ix + 2) {
          while (ixstart + 1 <= ix + 2) {
            if (err[ixstart] < mtmp) {
              mtmp = err[ixstart];
            }

            ixstart++;
          }
        }

        minval[i] = mtmp;
      }

      ixstart = 1;
      *err2 = minval[0];
      if (rtIsNaN(minval[0])) {
        ix = 2;
        exitg2 = false;
        while ((!exitg2) && (ix < 6)) {
          ixstart = ix;
          if (!rtIsNaN(minval[ix - 1])) {
            *err2 = minval[ix - 1];
            exitg2 = true;
          } else {
            ix++;
          }
        }
      }

      if (ixstart < 5) {
        while (ixstart + 1 < 6) {
          if (minval[ixstart] < *err2) {
            *err2 = minval[ixstart];
          }

          ixstart++;
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

/*
 * File trailer for orthogonalIteration.c
 *
 * [EOF]
 */
