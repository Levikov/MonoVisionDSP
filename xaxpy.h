/*
 * File: xaxpy.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef XAXPY_H
#define XAXPY_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern void b_xaxpy(int n, double a, const double x[16], int ix0, double y[4],
                    int iy0);
extern void c_xaxpy(int n, double a, const double x[4], int ix0, double y[16],
                    int iy0);
extern void d_xaxpy(int n, double a, int ix0, double y[9], int iy0);
extern void e_xaxpy(int n, double a, const double x[9], int ix0, double y[3],
                    int iy0);
extern void f_xaxpy(int n, double a, const double x[3], int ix0, double y[9],
                    int iy0);
extern void xaxpy(int n, double a, int ix0, double y[16], int iy0);

#endif

/*
 * File trailer for xaxpy.h
 *
 * [EOF]
 */
