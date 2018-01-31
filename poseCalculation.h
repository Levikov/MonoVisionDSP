/*
 * File: poseCalculation.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 31-Jan-2018 16:03:58
 */

#ifndef POSECALCULATION_H
#define POSECALCULATION_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "poseCalculation_types.h"

/* Function Declarations */
extern void poseCalculation(const double X[32], double Y[24], const double M[9],
  double epsilon, double maxIteration, double Angle[3], double t[6]);

#endif

/*
 * File trailer for poseCalculation.h
 *
 * [EOF]
 */
