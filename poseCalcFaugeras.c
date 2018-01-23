#include <MonoGlobal.h>
#ifdef POSE_CALC_METHOD_FAUGERAS

//Camera Calibration Matrix
#pragma DATA_ALIGN(M, 8)
double M[3][3] = {2.272727272727273e+03, 0, 640,
                 0, 2.272727272727273e+03, 640,
                 0, 0,  1};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
double P[4][4] = {-72.5, 17.5, 127.5, -72.5,
                 18.75,18.75, 18.75,-56.25,
                 0, 0, 0, 0,
                 1, 1, 1, 1};

#pragma DATA_AlIGN(x,8)
double x[4][4] = {0};

void poseCalc(const Coord * points,Pose *pose)
{
  int i,j,k;
  for(i=0;i<TARGET_NUM;i++)
  {
    x[0][i] = points[i].X * FOCUS;
    x[1][i] = points[i].Y * FOCUS;
    x[2][i] = 1; 
  }

  

}



#endif