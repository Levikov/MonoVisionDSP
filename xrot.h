/*
 * File: xrot.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef XROT_H
#define XROT_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern void b_xrot(double x[9], int ix0, int iy0, double c, double s);
extern void xrot(double x[16], int ix0, int iy0, double c, double s);

#endif

/*
 * File trailer for xrot.h
 *
 * [EOF]
 */
