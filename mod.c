/*
 * File: mod.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 14:24:47
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "orthogonalIteration.h"
#include "mod.h"

/* Function Definitions */

/*
 * Arguments    : double x
 * Return Type  : double
 */
double b_mod(double x)
{
  double r;
  if ((!rtIsInf(x)) && (!rtIsNaN(x))) {
    if (x == 0.0) {
      r = 0.0;
    } else {
      r = fmod(x, 2.0);
      if (r == 0.0) {
        r = 0.0;
      } else {
        if (x < 0.0) {
          r += 2.0;
        }
      }
    }
  } else {
    r = rtNaN;
  }

  return r;
}

/*
 * Arguments    : double x
 * Return Type  : double
 */
double c_mod(double x)
{
  double r;
  if ((!rtIsInf(x)) && (!rtIsNaN(x))) {
    if (x == 0.0) {
      r = 0.0;
    } else {
      r = fmod(x, 5.0);
      if (r == 0.0) {
        r = 0.0;
      } else {
        if (x < 0.0) {
          r += 5.0;
        }
      }
    }
  } else {
    r = rtNaN;
  }

  return r;
}

/*
 * File trailer for mod.c
 *
 * [EOF]
 */
