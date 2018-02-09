#include "MonoGlobal.h"
#include <getHomographyMatrix.h>
#include <getHomographyMatrix_initialize.h>
#include <math.h>
#include <matop.h>
#include <float.h>
#include "levmar.h"

#ifdef POSE_CALC_METHOD_MLE


#pragma DATA_ALIGN(Y,8)
double Y[TARGET_NUM][3] = {0};

#pragma DATA_ALIGN(X,8)
double X[TARGET_NUM][3] = {0};


void simon_h(double *p, double *hx, int m, int n,void *adata)
{
  double **A = (double **)adata;
  double H[3][3];
  double (*Y)[3][TARGET_NUM] = A[1];
  memset(hx,0,sizeof(double)*2*TARGET_NUM);
  register int i,j;
  double Yhap[3][TARGET_NUM] = {0};
  for (i = 0; i < 3; i++)
  for (j = 0; j < 3; j++)
  {
    H[i][j] = p[j*3+i];
  }

  DSPF_dp_mat_mul_any(H,1,3,3,A[0], TARGET_NUM,Yhap);

  for (i = 0; i < 3; i++)
    for (j = 0; j < TARGET_NUM; j++)
    {
      Yhap[i][j] = Yhap[i][j] / Yhap[2][j];
    };

  for (i = 0; i < 2; i++)
    for (j = 0; j < TARGET_NUM; j++)
    {
      hx[TARGET_NUM * i + j] =   Yhap[i][j] - (*Y)[i][j];
    }

}

unsigned char poseCalc(const double(* points)[3][TARGET_NUM],Pose *pose)
{
  register int i,j,k;
  unsigned char status = 0;
  double H[3][3];
  double invM[3][3];
  double invMH[3][3] = {0};
  double *A[2];
  double opts[5] = {1e-6,1e-9,1e-9,1e-9,1e-6};
  A[0]  = &P;
  A[1]  = points;
  double info[LM_INFO_SZ];

  for(i=0;i<TARGET_NUM;i++)
  {
    Y[i][0] = (*points)[0][i]; 
    Y[i][1] = (*points)[1][i];  
    Y[i][2] = 1;  
    X[i][0] = P[0][i]; 
    X[i][1] = P[1][i];  
    X[i][2] = 1;  
  }
  getHomographyMatrix_initialize();
  getHomographyMatrix(X,Y,H);
  dlevmar_dif(simon_h,H,NULL,9,2*TARGET_NUM,1000,opts,info,NULL,NULL,A);
  if(info[1] > POSE_CALC_METHOD_MLE_EPSILON)
  {
    status = 4;
    goto end;
  }
  DSPF_dp_mat_trans_local(H,3);
  DSPF_dp_mat_inv(M,3,invM);
  DSPF_dp_mat_mul_any(invM,1,3,3,H,3,invMH);


  double Zc1 = 1/sqrt(invMH[0][0]*invMH[0][0]+invMH[1][0]*invMH[1][0]+invMH[2][0]*invMH[2][0]);
  double Zc2 = 1/sqrt(invMH[0][1]*invMH[0][1]+invMH[1][1]*invMH[1][1]+invMH[2][1]*invMH[2][1]);
  double Zc = sqrt(Zc1*Zc2);
  
  DSPF_dp_mat_linear_comb(invMH,Zc,invMH,0,3,3,invMH);

  
  pose->T.X = invMH[0][2];
  pose->T.Y = invMH[1][2];
  pose->T.Z = invMH[2][2];

  invMH[0][2] = invMH[1][0]*invMH[2][1] - invMH[1][1]*invMH[2][0];
  invMH[1][2] = invMH[0][0]*invMH[2][1] - invMH[0][1]*invMH[2][0];
  invMH[2][2] = invMH[0][0]*invMH[1][1] - invMH[1][0]*invMH[0][1];

  pose->R.roll  =  atan(invMH[1][0]/invMH[1][1])/PI*180;
  pose->R.pitch =  asin(invMH[1][2])/PI*180;
  pose->R.yaw   =  atan(invMH[0][2]/invMH[2][2])/PI*180;
end:
  return status;
}



#endif
