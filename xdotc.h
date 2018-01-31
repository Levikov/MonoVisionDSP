/*
 * File: xdotc.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef XDOTC_H
#define XDOTC_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern double b_xdotc(int n, const double x[9], int ix0, const double y[9], int
                      iy0);
extern double xdotc(int n, const double x[16], int ix0, const double y[16], int
                    iy0);

#endif

/*
 * File trailer for xdotc.h
 *
 * [EOF]
 */
