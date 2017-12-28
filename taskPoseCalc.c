#include <MonoGlobal.h>

float M[3][3] = { 2294.35,      0,  1172.41,
                        0,2310.24,  885.828,
                        0,      0,        1};
float X[4][3] = {       0,      0,        1,
                       90,      0,        1,
                      200,      0,        1,
                        0,     75,        1};

Void taskPoseCalc()
{
  float Y[4][3];
  int i=0,j=0;
  for(i=0;i<4;i++)
  {
    Y[i][0] = posBuffer.buffer[i].X;
    Y[i][1] = posBuffer.buffer[i].Y;
    Y[i][2] = posBuffer.buffer[i].Z;
  }
}