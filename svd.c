/*
 * File: svd.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "svd.h"
#include "xaxpy.h"
#include "xdotc.h"
#include "xnrm2.h"
#include "xscal.h"
#include "xrot.h"
#include "xrotg.h"
#include "xswap.h"

/* Function Definitions */

/*
 * Arguments    : const double A[9]
 *                double U[9]
 *                double s[3]
 *                double V[9]
 * Return Type  : void
 */
void c_svd(const double A[9], double U[9], double s[3], double V[9])
{
  double b_A[9];
  int i;
  double b_s[3];
  double e[3];
  int q;
  double work[3];
  int m;
  int qs;
  boolean_T apply_transform;
  double ztest0;
  int ii;
  int iter;
  double snorm;
  double rt;
  double ztest;
  int exitg1;
  boolean_T exitg2;
  double f;
  double varargin_1[5];
  double mtmp;
  double sqds;
  memcpy(&b_A[0], &A[0], 9U * sizeof(double));
  for (i = 0; i < 3; i++) {
    b_s[i] = 0.0;
    e[i] = 0.0;
    work[i] = 0.0;
  }

  memset(&U[0], 0, 9U * sizeof(double));
  memset(&V[0], 0, 9U * sizeof(double));
  for (q = 0; q < 2; q++) {
    qs = q + 3 * q;
    apply_transform = false;
    ztest0 = c_xnrm2(3 - q, b_A, qs + 1);
    if (ztest0 > 0.0) {
      apply_transform = true;
      if (b_A[qs] < 0.0) {
        b_s[q] = -ztest0;
      } else {
        b_s[q] = ztest0;
      }

      if (fabs(b_s[q]) >= 1.0020841800044864E-292) {
        ztest0 = 1.0 / b_s[q];
        i = (qs - q) + 3;
        for (ii = qs; ii + 1 <= i; ii++) {
          b_A[ii] *= ztest0;
        }
      } else {
        i = (qs - q) + 3;
        for (ii = qs; ii + 1 <= i; ii++) {
          b_A[ii] /= b_s[q];
        }
      }

      b_A[qs]++;
      b_s[q] = -b_s[q];
    } else {
      b_s[q] = 0.0;
    }

    for (ii = q + 1; ii + 1 < 4; ii++) {
      i = q + 3 * ii;
      if (apply_transform) {
        d_xaxpy(3 - q, -(b_xdotc(3 - q, b_A, qs + 1, b_A, i + 1) / b_A[q + 3 * q]),
                qs + 1, b_A, i + 1);
      }

      e[ii] = b_A[i];
    }

    for (ii = q; ii + 1 < 4; ii++) {
      U[ii + 3 * q] = b_A[ii + 3 * q];
    }

    if (q + 1 <= 1) {
      ztest0 = d_xnrm2(e, 2);
      if (ztest0 == 0.0) {
        e[0] = 0.0;
      } else {
        if (e[1] < 0.0) {
          ztest0 = -ztest0;
        }

        e[0] = ztest0;
        if (fabs(ztest0) >= 1.0020841800044864E-292) {
          ztest0 = 1.0 / ztest0;
          for (ii = 1; ii + 1 < 4; ii++) {
            e[ii] *= ztest0;
          }
        } else {
          for (ii = 1; ii + 1 < 4; ii++) {
            e[ii] /= ztest0;
          }
        }

        e[1]++;
        e[0] = -e[0];
        for (ii = 2; ii < 4; ii++) {
          work[ii - 1] = 0.0;
        }

        for (ii = 1; ii + 1 < 4; ii++) {
          e_xaxpy(2, e[ii], b_A, 3 * ii + 2, work, 2);
        }

        for (ii = 1; ii + 1 < 4; ii++) {
          f_xaxpy(2, -e[ii] / e[1], work, 2, b_A, 3 * ii + 2);
        }
      }

      for (ii = 1; ii + 1 < 4; ii++) {
        V[ii] = e[ii];
      }
    }
  }

  m = 1;
  b_s[2] = b_A[8];
  e[1] = b_A[7];
  e[2] = 0.0;
  for (ii = 0; ii < 3; ii++) {
    U[6 + ii] = 0.0;
  }

  U[8] = 1.0;
  for (q = 1; q >= 0; q += -1) {
    qs = q + 3 * q;
    if (b_s[q] != 0.0) {
      for (ii = q + 1; ii + 1 < 4; ii++) {
        i = (q + 3 * ii) + 1;
        d_xaxpy(3 - q, -(b_xdotc(3 - q, U, qs + 1, U, i) / U[qs]), qs + 1, U, i);
      }

      for (ii = q; ii + 1 < 4; ii++) {
        U[ii + 3 * q] = -U[ii + 3 * q];
      }

      U[qs]++;
      if (1 <= q) {
        U[3] = 0.0;
      }
    } else {
      for (ii = 0; ii < 3; ii++) {
        U[ii + 3 * q] = 0.0;
      }

      U[qs] = 1.0;
    }
  }

  for (q = 2; q >= 0; q += -1) {
    if ((q + 1 <= 1) && (e[0] != 0.0)) {
      for (ii = 0; ii < 2; ii++) {
        i = 2 + 3 * (ii + 1);
        d_xaxpy(2, -(b_xdotc(2, V, 2, V, i) / V[1]), 2, V, i);
      }
    }

    for (ii = 0; ii < 3; ii++) {
      V[ii + 3 * q] = 0.0;
    }

    V[q + 3 * q] = 1.0;
  }

  for (q = 0; q < 3; q++) {
    ztest0 = e[q];
    if (b_s[q] != 0.0) {
      rt = fabs(b_s[q]);
      ztest = b_s[q] / rt;
      b_s[q] = rt;
      if (q + 1 < 3) {
        ztest0 = e[q] / ztest;
      }

      b_xscal(ztest, U, 1 + 3 * q);
    }

    if ((q + 1 < 3) && (ztest0 != 0.0)) {
      rt = fabs(ztest0);
      ztest = rt / ztest0;
      ztest0 = rt;
      b_s[q + 1] *= ztest;
      b_xscal(ztest, V, 1 + 3 * (q + 1));
    }

    e[q] = ztest0;
  }

  iter = 0;
  snorm = 0.0;
  for (ii = 0; ii < 3; ii++) {
    ztest0 = fabs(b_s[ii]);
    ztest = fabs(e[ii]);
    if ((ztest0 > ztest) || rtIsNaN(ztest)) {
    } else {
      ztest0 = ztest;
    }

    if (!((snorm > ztest0) || rtIsNaN(ztest0))) {
      snorm = ztest0;
    }
  }

  while ((m + 2 > 0) && (!(iter >= 75))) {
    ii = m;
    do {
      exitg1 = 0;
      q = ii + 1;
      if (ii + 1 == 0) {
        exitg1 = 1;
      } else {
        ztest0 = fabs(e[ii]);
        if ((ztest0 <= 2.2204460492503131E-16 * (fabs(b_s[ii]) + fabs(b_s[ii + 1])))
            || (ztest0 <= 1.0020841800044864E-292) || ((iter > 20) && (ztest0 <=
              2.2204460492503131E-16 * snorm))) {
          e[ii] = 0.0;
          exitg1 = 1;
        } else {
          ii--;
        }
      }
    } while (exitg1 == 0);

    if (ii + 1 == m + 1) {
      i = 4;
    } else {
      qs = m + 2;
      i = m + 2;
      exitg2 = false;
      while ((!exitg2) && (i >= ii + 1)) {
        qs = i;
        if (i == ii + 1) {
          exitg2 = true;
        } else {
          ztest0 = 0.0;
          if (i < m + 2) {
            ztest0 = fabs(e[i - 1]);
          }

          if (i > ii + 2) {
            ztest0 += fabs(e[i - 2]);
          }

          ztest = fabs(b_s[i - 1]);
          if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
               1.0020841800044864E-292)) {
            b_s[i - 1] = 0.0;
            exitg2 = true;
          } else {
            i--;
          }
        }
      }

      if (qs == ii + 1) {
        i = 3;
      } else if (qs == m + 2) {
        i = 1;
      } else {
        i = 2;
        q = qs;
      }
    }

    switch (i) {
     case 1:
      f = e[m];
      e[m] = 0.0;
      for (ii = m; ii + 1 >= q + 1; ii--) {
        xrotg(&b_s[ii], &f, &ztest0, &ztest);
        if (ii + 1 > q + 1) {
          f = -ztest * e[0];
          e[0] *= ztest0;
        }

        b_xrot(V, 1 + 3 * ii, 1 + 3 * (m + 1), ztest0, ztest);
      }
      break;

     case 2:
      f = e[q - 1];
      e[q - 1] = 0.0;
      for (ii = q; ii + 1 <= m + 2; ii++) {
        xrotg(&b_s[ii], &f, &ztest0, &ztest);
        f = -ztest * e[ii];
        e[ii] *= ztest0;
        b_xrot(U, 1 + 3 * ii, 1 + 3 * (q - 1), ztest0, ztest);
      }
      break;

     case 3:
      varargin_1[0] = fabs(b_s[m + 1]);
      varargin_1[1] = fabs(b_s[m]);
      varargin_1[2] = fabs(e[m]);
      varargin_1[3] = fabs(b_s[q]);
      varargin_1[4] = fabs(e[q]);
      i = 1;
      mtmp = varargin_1[0];
      if (rtIsNaN(varargin_1[0])) {
        ii = 2;
        exitg2 = false;
        while ((!exitg2) && (ii < 6)) {
          i = ii;
          if (!rtIsNaN(varargin_1[ii - 1])) {
            mtmp = varargin_1[ii - 1];
            exitg2 = true;
          } else {
            ii++;
          }
        }
      }

      if (i < 5) {
        while (i + 1 < 6) {
          if (varargin_1[i] > mtmp) {
            mtmp = varargin_1[i];
          }

          i++;
        }
      }

      f = b_s[m + 1] / mtmp;
      ztest0 = b_s[m] / mtmp;
      ztest = e[m] / mtmp;
      sqds = b_s[q] / mtmp;
      rt = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
      ztest0 = f * ztest;
      ztest0 *= ztest0;
      if ((rt != 0.0) || (ztest0 != 0.0)) {
        ztest = sqrt(rt * rt + ztest0);
        if (rt < 0.0) {
          ztest = -ztest;
        }

        ztest = ztest0 / (rt + ztest);
      } else {
        ztest = 0.0;
      }

      f = (sqds + f) * (sqds - f) + ztest;
      rt = sqds * (e[q] / mtmp);
      for (ii = q + 1; ii <= m + 1; ii++) {
        xrotg(&f, &rt, &ztest0, &ztest);
        if (ii > q + 1) {
          e[0] = f;
        }

        f = ztest0 * b_s[ii - 1] + ztest * e[ii - 1];
        e[ii - 1] = ztest0 * e[ii - 1] - ztest * b_s[ii - 1];
        rt = ztest * b_s[ii];
        b_s[ii] *= ztest0;
        b_xrot(V, 1 + 3 * (ii - 1), 1 + 3 * ii, ztest0, ztest);
        b_s[ii - 1] = f;
        xrotg(&b_s[ii - 1], &rt, &ztest0, &ztest);
        f = ztest0 * e[ii - 1] + ztest * b_s[ii];
        b_s[ii] = -ztest * e[ii - 1] + ztest0 * b_s[ii];
        rt = ztest * e[ii];
        e[ii] *= ztest0;
        b_xrot(U, 1 + 3 * (ii - 1), 1 + 3 * ii, ztest0, ztest);
      }

      e[m] = f;
      iter++;
      break;

     default:
      if (b_s[q] < 0.0) {
        b_s[q] = -b_s[q];
        b_xscal(-1.0, V, 1 + 3 * q);
      }

      i = q + 1;
      while ((q + 1 < 3) && (b_s[q] < b_s[i])) {
        rt = b_s[q];
        b_s[q] = b_s[i];
        b_s[i] = rt;
        b_xswap(V, 1 + 3 * q, 1 + 3 * (q + 1));
        b_xswap(U, 1 + 3 * q, 1 + 3 * (q + 1));
        q = i;
        i++;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (ii = 0; ii < 3; ii++) {
    s[ii] = b_s[ii];
  }
}

/*
 * Arguments    : const double A[16]
 *                double U[16]
 *                double s[4]
 *                double V[16]
 * Return Type  : void
 */
void svd(const double A[16], double U[16], double s[4], double V[16])
{
  double b_A[16];
  int i;
  double b_s[4];
  double Vf[16];
  double e[4];
  int q;
  double work[4];
  int m;
  int iter;
  boolean_T apply_transform;
  double ztest0;
  int qp1jj;
  int qs;
  double snorm;
  double rt;
  double ztest;
  int exitg1;
  boolean_T exitg2;
  double f;
  double varargin_1[5];
  double mtmp;
  double sqds;
  memcpy(&b_A[0], &A[0], sizeof(double) << 4);
  for (i = 0; i < 4; i++) {
    b_s[i] = 0.0;
    e[i] = 0.0;
    work[i] = 0.0;
  }

  memset(&U[0], 0, sizeof(double) << 4);
  memset(&Vf[0], 0, sizeof(double) << 4);
  for (q = 0; q < 3; q++) {
    iter = q + (q << 2);
    apply_transform = false;
    ztest0 = xnrm2(4 - q, b_A, iter + 1);
    if (ztest0 > 0.0) {
      apply_transform = true;
      if (b_A[iter] < 0.0) {
        b_s[q] = -ztest0;
      } else {
        b_s[q] = ztest0;
      }

      if (fabs(b_s[q]) >= 1.0020841800044864E-292) {
        ztest0 = 1.0 / b_s[q];
        i = (iter - q) + 4;
        for (qp1jj = iter; qp1jj + 1 <= i; qp1jj++) {
          b_A[qp1jj] *= ztest0;
        }
      } else {
        i = (iter - q) + 4;
        for (qp1jj = iter; qp1jj + 1 <= i; qp1jj++) {
          b_A[qp1jj] /= b_s[q];
        }
      }

      b_A[iter]++;
      b_s[q] = -b_s[q];
    } else {
      b_s[q] = 0.0;
    }

    for (qs = q + 1; qs + 1 < 5; qs++) {
      i = q + (qs << 2);
      if (apply_transform) {
        xaxpy(4 - q, -(xdotc(4 - q, b_A, iter + 1, b_A, i + 1) / b_A[q + (q << 2)]),
              iter + 1, b_A, i + 1);
      }

      e[qs] = b_A[i];
    }

    for (qp1jj = q; qp1jj + 1 < 5; qp1jj++) {
      U[qp1jj + (q << 2)] = b_A[qp1jj + (q << 2)];
    }

    if (q + 1 <= 2) {
      ztest0 = b_xnrm2(3 - q, e, q + 2);
      if (ztest0 == 0.0) {
        e[q] = 0.0;
      } else {
        if (e[q + 1] < 0.0) {
          e[q] = -ztest0;
        } else {
          e[q] = ztest0;
        }

        ztest0 = e[q];
        if (fabs(e[q]) >= 1.0020841800044864E-292) {
          ztest0 = 1.0 / e[q];
          for (qp1jj = q + 1; qp1jj + 1 < 5; qp1jj++) {
            e[qp1jj] *= ztest0;
          }
        } else {
          for (qp1jj = q + 1; qp1jj + 1 < 5; qp1jj++) {
            e[qp1jj] /= ztest0;
          }
        }

        e[q + 1]++;
        e[q] = -e[q];
        for (qp1jj = q + 1; qp1jj + 1 < 5; qp1jj++) {
          work[qp1jj] = 0.0;
        }

        for (qs = q + 1; qs + 1 < 5; qs++) {
          b_xaxpy(3 - q, e[qs], b_A, (q + (qs << 2)) + 2, work, q + 2);
        }

        for (qs = q + 1; qs + 1 < 5; qs++) {
          c_xaxpy(3 - q, -e[qs] / e[q + 1], work, q + 2, b_A, (q + (qs << 2)) +
                  2);
        }
      }

      for (qp1jj = q + 1; qp1jj + 1 < 5; qp1jj++) {
        Vf[qp1jj + (q << 2)] = e[qp1jj];
      }
    }
  }

  m = 2;
  b_s[3] = b_A[15];
  e[2] = b_A[14];
  e[3] = 0.0;
  for (qp1jj = 0; qp1jj < 4; qp1jj++) {
    U[12 + qp1jj] = 0.0;
  }

  U[15] = 1.0;
  for (q = 2; q >= 0; q += -1) {
    iter = q + (q << 2);
    if (b_s[q] != 0.0) {
      for (qs = q + 1; qs + 1 < 5; qs++) {
        i = (q + (qs << 2)) + 1;
        xaxpy(4 - q, -(xdotc(4 - q, U, iter + 1, U, i) / U[iter]), iter + 1, U,
              i);
      }

      for (qp1jj = q; qp1jj + 1 < 5; qp1jj++) {
        U[qp1jj + (q << 2)] = -U[qp1jj + (q << 2)];
      }

      U[iter]++;
      for (qp1jj = 1; qp1jj <= q; qp1jj++) {
        U[(qp1jj + (q << 2)) - 1] = 0.0;
      }
    } else {
      for (qp1jj = 0; qp1jj < 4; qp1jj++) {
        U[qp1jj + (q << 2)] = 0.0;
      }

      U[iter] = 1.0;
    }
  }

  for (q = 3; q >= 0; q += -1) {
    if ((q + 1 <= 2) && (e[q] != 0.0)) {
      i = (q + (q << 2)) + 2;
      for (qs = q + 1; qs + 1 < 5; qs++) {
        qp1jj = (q + (qs << 2)) + 2;
        xaxpy(3 - q, -(xdotc(3 - q, Vf, i, Vf, qp1jj) / Vf[i - 1]), i, Vf, qp1jj);
      }
    }

    for (qp1jj = 0; qp1jj < 4; qp1jj++) {
      Vf[qp1jj + (q << 2)] = 0.0;
    }

    Vf[q + (q << 2)] = 1.0;
  }

  for (q = 0; q < 4; q++) {
    ztest0 = e[q];
    if (b_s[q] != 0.0) {
      rt = fabs(b_s[q]);
      ztest = b_s[q] / rt;
      b_s[q] = rt;
      if (q + 1 < 4) {
        ztest0 = e[q] / ztest;
      }

      xscal(ztest, U, 1 + (q << 2));
    }

    if ((q + 1 < 4) && (ztest0 != 0.0)) {
      rt = fabs(ztest0);
      ztest = rt / ztest0;
      ztest0 = rt;
      b_s[q + 1] *= ztest;
      xscal(ztest, Vf, 1 + ((q + 1) << 2));
    }

    e[q] = ztest0;
  }

  iter = 0;
  snorm = 0.0;
  for (qp1jj = 0; qp1jj < 4; qp1jj++) {
    ztest0 = fabs(b_s[qp1jj]);
    ztest = fabs(e[qp1jj]);
    if ((ztest0 > ztest) || rtIsNaN(ztest)) {
    } else {
      ztest0 = ztest;
    }

    if (!((snorm > ztest0) || rtIsNaN(ztest0))) {
      snorm = ztest0;
    }
  }

  while ((m + 2 > 0) && (!(iter >= 75))) {
    qp1jj = m;
    do {
      exitg1 = 0;
      q = qp1jj + 1;
      if (qp1jj + 1 == 0) {
        exitg1 = 1;
      } else {
        ztest0 = fabs(e[qp1jj]);
        if ((ztest0 <= 2.2204460492503131E-16 * (fabs(b_s[qp1jj]) + fabs
              (b_s[qp1jj + 1]))) || (ztest0 <= 1.0020841800044864E-292) ||
            ((iter > 20) && (ztest0 <= 2.2204460492503131E-16 * snorm))) {
          e[qp1jj] = 0.0;
          exitg1 = 1;
        } else {
          qp1jj--;
        }
      }
    } while (exitg1 == 0);

    if (qp1jj + 1 == m + 1) {
      i = 4;
    } else {
      qs = m + 2;
      i = m + 2;
      exitg2 = false;
      while ((!exitg2) && (i >= qp1jj + 1)) {
        qs = i;
        if (i == qp1jj + 1) {
          exitg2 = true;
        } else {
          ztest0 = 0.0;
          if (i < m + 2) {
            ztest0 = fabs(e[i - 1]);
          }

          if (i > qp1jj + 2) {
            ztest0 += fabs(e[i - 2]);
          }

          ztest = fabs(b_s[i - 1]);
          if ((ztest <= 2.2204460492503131E-16 * ztest0) || (ztest <=
               1.0020841800044864E-292)) {
            b_s[i - 1] = 0.0;
            exitg2 = true;
          } else {
            i--;
          }
        }
      }

      if (qs == qp1jj + 1) {
        i = 3;
      } else if (qs == m + 2) {
        i = 1;
      } else {
        i = 2;
        q = qs;
      }
    }

    switch (i) {
     case 1:
      f = e[m];
      e[m] = 0.0;
      for (qp1jj = m; qp1jj + 1 >= q + 1; qp1jj--) {
        xrotg(&b_s[qp1jj], &f, &ztest0, &ztest);
        if (qp1jj + 1 > q + 1) {
          f = -ztest * e[qp1jj - 1];
          e[qp1jj - 1] *= ztest0;
        }

        xrot(Vf, 1 + (qp1jj << 2), 1 + ((m + 1) << 2), ztest0, ztest);
      }
      break;

     case 2:
      f = e[q - 1];
      e[q - 1] = 0.0;
      for (qp1jj = q; qp1jj + 1 <= m + 2; qp1jj++) {
        xrotg(&b_s[qp1jj], &f, &ztest0, &ztest);
        f = -ztest * e[qp1jj];
        e[qp1jj] *= ztest0;
        xrot(U, 1 + (qp1jj << 2), 1 + ((q - 1) << 2), ztest0, ztest);
      }
      break;

     case 3:
      varargin_1[0] = fabs(b_s[m + 1]);
      varargin_1[1] = fabs(b_s[m]);
      varargin_1[2] = fabs(e[m]);
      varargin_1[3] = fabs(b_s[q]);
      varargin_1[4] = fabs(e[q]);
      i = 1;
      mtmp = varargin_1[0];
      if (rtIsNaN(varargin_1[0])) {
        qp1jj = 2;
        exitg2 = false;
        while ((!exitg2) && (qp1jj < 6)) {
          i = qp1jj;
          if (!rtIsNaN(varargin_1[qp1jj - 1])) {
            mtmp = varargin_1[qp1jj - 1];
            exitg2 = true;
          } else {
            qp1jj++;
          }
        }
      }

      if (i < 5) {
        while (i + 1 < 6) {
          if (varargin_1[i] > mtmp) {
            mtmp = varargin_1[i];
          }

          i++;
        }
      }

      f = b_s[m + 1] / mtmp;
      ztest0 = b_s[m] / mtmp;
      ztest = e[m] / mtmp;
      sqds = b_s[q] / mtmp;
      rt = ((ztest0 + f) * (ztest0 - f) + ztest * ztest) / 2.0;
      ztest0 = f * ztest;
      ztest0 *= ztest0;
      if ((rt != 0.0) || (ztest0 != 0.0)) {
        ztest = sqrt(rt * rt + ztest0);
        if (rt < 0.0) {
          ztest = -ztest;
        }

        ztest = ztest0 / (rt + ztest);
      } else {
        ztest = 0.0;
      }

      f = (sqds + f) * (sqds - f) + ztest;
      rt = sqds * (e[q] / mtmp);
      for (qp1jj = q + 1; qp1jj <= m + 1; qp1jj++) {
        xrotg(&f, &rt, &ztest0, &ztest);
        if (qp1jj > q + 1) {
          e[qp1jj - 2] = f;
        }

        f = ztest0 * b_s[qp1jj - 1] + ztest * e[qp1jj - 1];
        e[qp1jj - 1] = ztest0 * e[qp1jj - 1] - ztest * b_s[qp1jj - 1];
        rt = ztest * b_s[qp1jj];
        b_s[qp1jj] *= ztest0;
        xrot(Vf, 1 + ((qp1jj - 1) << 2), 1 + (qp1jj << 2), ztest0, ztest);
        b_s[qp1jj - 1] = f;
        xrotg(&b_s[qp1jj - 1], &rt, &ztest0, &ztest);
        f = ztest0 * e[qp1jj - 1] + ztest * b_s[qp1jj];
        b_s[qp1jj] = -ztest * e[qp1jj - 1] + ztest0 * b_s[qp1jj];
        rt = ztest * e[qp1jj];
        e[qp1jj] *= ztest0;
        xrot(U, 1 + ((qp1jj - 1) << 2), 1 + (qp1jj << 2), ztest0, ztest);
      }

      e[m] = f;
      iter++;
      break;

     default:
      if (b_s[q] < 0.0) {
        b_s[q] = -b_s[q];
        xscal(-1.0, Vf, 1 + (q << 2));
      }

      i = q + 1;
      while ((q + 1 < 4) && (b_s[q] < b_s[i])) {
        rt = b_s[q];
        b_s[q] = b_s[i];
        b_s[i] = rt;
        xswap(Vf, 1 + (q << 2), 1 + ((q + 1) << 2));
        xswap(U, 1 + (q << 2), 1 + ((q + 1) << 2));
        q = i;
        i++;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (qp1jj = 0; qp1jj < 4; qp1jj++) {
    s[qp1jj] = b_s[qp1jj];
    for (i = 0; i < 4; i++) {
      V[i + (qp1jj << 2)] = Vf[i + (qp1jj << 2)];
    }
  }
}

/*
 * File trailer for svd.c
 *
 * [EOF]
 */
