/*
 * File: orthogonalIteration.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef ORTHOGONALITERATION_H
#define ORTHOGONALITERATION_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern void orthogonalIteration(const double X[32], const double Y[24], double
  R0[9], double epsilon, double maxIteration, double t[3], double *err2);

#endif

/*
 * File trailer for orthogonalIteration.h
 *
 * [EOF]
 */
