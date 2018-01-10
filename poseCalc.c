#include <MonoGlobal.h>
//Camera Calibration Matrix
#pragma DATA_ALIGN(M, 8)
float M[4][4] = {2294.35, 0, 1172.41, 0,
                 0, 2310.24, 885.828, 0,
                 0, 0,  1,  0,
                 0, 0,  0,  0};
#pragma DATA_ALIGN(invM, 8)
float invM[4][4] = {4.358532917819862e-04, 0, -0.510998757818118, 0,
                 0, 4.328554609044948e-04, -0.383435487222107, 0,
                 0, 0,  1,  0,
                 0, 0,  0,  0};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
float P[4][4] = {-72.5, 17.5, 127.5, -72.5,
                 18.75,18.75, 18.75,-56.25,
                 0, 0, 0, 0,
                 1, 1, 1, 1};

#pragma DATA_ALIGN(Y, 8)
float Y[4][4] = {0};

typedef struct
{
  double ratio[9];
  float (*X)[4][4];
  float (*Y)[4][4];
} SimplexParams;

Void taskPoseCalc()
{
  
}
