#include "MonoGlobal.h"
#pragma DATA_ALIGN(R,8)
double R[4][4] = {0};
#pragma DATA_ALIGN(T,8)
double T[4][TARGET_NUM] = {0};

void generateCoordinates(Pose pose,double (* p)[4][TARGET_NUM])
{
  int i,j,k;
  memset(R,0,sizeof(R));
  memset(T,0,sizeof(T));
  memset(p,0,sizeof(*p));
  R[0][0] = cos(pose.R.roll/180*PI)*cos(pose.R.yaw/180*PI) + sin(pose.R.pitch/180*PI)*sin(pose.R.roll/180*PI)*sin(pose.R.yaw/180*PI); 
  R[0][1] = cos(pose.R.roll/180*PI)*sin(pose.R.pitch/180*PI)*sin(pose.R.yaw/180*PI) - cos(pose.R.yaw/180*PI)*sin(pose.R.roll/180*PI);
  R[0][2] = cos(pose.R.pitch/180*PI)*sin(pose.R.yaw/180*PI);
  R[1][0] = cos(pose.R.pitch/180*PI)*sin(pose.R.roll/180*PI);
  R[1][1] = cos(pose.R.pitch/180*PI)*cos(pose.R.roll/180*PI);         
  R[1][2] =-sin(pose.R.pitch/180*PI);
  R[2][0] = cos(pose.R.yaw/180*PI)*sin(pose.R.pitch/180*PI)*sin(pose.R.roll/180*PI) - cos(pose.R.roll/180*PI)*sin(pose.R.yaw/180*PI);
  R[2][1] = sin(pose.R.roll/180*PI)*sin(pose.R.yaw/180*PI) + cos(pose.R.roll/180*PI)*cos(pose.R.yaw/180*PI)*sin(pose.R.pitch/180*PI);
  R[2][2] = cos(pose.R.pitch)*cos(pose.R.yaw);

  for(i=0;i<4;i++)
  for(j=0;j<2;j++)
  for(k=0;k<TARGET_NUM;k++)
  T[i][k]+=R[i][j]*P[j][k];

  for(i = 0;i<TARGET_NUM;i++)
  {
    T[0][i] = (T[0][i] + pose.T.X);
    T[1][i] = (T[1][i] + pose.T.Y);
    T[2][i] = (T[2][i] + pose.T.Z);
  }

  for(i=0;i<TARGET_NUM;i++)
  for(j=0;j<3;j++)
  T[j][i] = T[j][i]/T[2][i];
  
  for(i=0;i<4;i++)
  for(j=0;j<4;j++)
  for(k=0;k<TARGET_NUM;k++)
  (*p)[i][k]+=M[i][j]*T[j][k];

  for(i=0;i<TARGET_NUM;i++)
  (*p)[2][i] = 1;
  
}
