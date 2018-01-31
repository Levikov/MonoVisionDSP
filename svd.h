/*
 * File: svd.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef SVD_H
#define SVD_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern void c_svd(const double A[9], double U[9], double s[3], double V[9]);
extern void svd(const double A[16], double U[16], double s[4], double V[16]);

#endif

/*
 * File trailer for svd.h
 *
 * [EOF]
 */
