/*
 * File: pinv.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "poseCalculation.h"
#include "pinv.h"
#include "svd.h"

/* Function Definitions */

/*
 * Arguments    : const double A[16]
 *                double X[16]
 * Return Type  : void
 */
void pinv(const double A[16], double X[16])
{
  boolean_T p;
  int ar;
  double U[16];
  double s[4];
  double V[16];
  int i0;
  double absxk;
  int vcol;
  int r;
  int br;
  int ic;
  int ib;
  int ia;
  p = false;
  for (ar = 0; ar < 16; ar++) {
    X[ar] = 0.0;
    if (p || rtIsInf(A[ar]) || rtIsNaN(A[ar])) {
      p = true;
    } else {
      p = false;
    }
  }

  if (p) {
    for (i0 = 0; i0 < 16; i0++) {
      X[i0] = rtNaN;
    }
  } else {
    svd(A, U, s, V);
    absxk = fabs(s[0]);
    if ((!rtIsInf(absxk)) && (!rtIsNaN(absxk))) {
      if (absxk <= 2.2250738585072014E-308) {
        absxk = 4.94065645841247E-324;
      } else {
        frexp(absxk, &vcol);
        absxk = ldexp(1.0, vcol - 53);
      }
    } else {
      absxk = rtNaN;
    }

    absxk *= 4.0;
    r = 0;
    ar = 1;
    while ((ar < 5) && (s[ar - 1] > absxk)) {
      r++;
      ar++;
    }

    if (r > 0) {
      vcol = 0;
      for (br = 1; br <= r; br++) {
        absxk = 1.0 / s[br - 1];
        for (ar = vcol; ar + 1 <= vcol + 4; ar++) {
          V[ar] *= absxk;
        }

        vcol += 4;
      }

      for (vcol = 0; vcol <= 13; vcol += 4) {
        for (ic = vcol; ic + 1 <= vcol + 4; ic++) {
          X[ic] = 0.0;
        }
      }

      br = -1;
      for (vcol = 0; vcol <= 13; vcol += 4) {
        ar = -1;
        br++;
        i0 = (br + ((r - 1) << 2)) + 1;
        for (ib = br; ib + 1 <= i0; ib += 4) {
          if (U[ib] != 0.0) {
            ia = ar;
            for (ic = vcol; ic + 1 <= vcol + 4; ic++) {
              ia++;
              X[ic] += U[ib] * V[ia];
            }
          }

          ar += 4;
        }
      }
    }
  }
}

/*
 * File trailer for pinv.c
 *
 * [EOF]
 */
