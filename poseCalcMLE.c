#include "MonoGlobal.h"
#include "getH.h"
#include "getH_initialize.h"
#include <math.h>
#include <float.h>
#include "levmar.h"

#ifdef POSE_CALC_METHOD_MLE


#pragma DATA_ALIGN(Y,8)
double Y[4][3] = {0};

#pragma DATA_ALIGN(X,8)
double X[4][3] = {0};


/**
 * @brief 
 * Local trandpose of matrix
 * @param x[IN/OUT] N x N matrix
 * @param N[IN]     dimension of matrix
 */
void DSPF_dp_mat_trans_local(double *restrict x, const int N)
{
  int i,j = 0;
  double temp;
  for(i = 0;i < N;i++)
   for(j = i+1;j < N;j++)
   {
     temp = x[i*N+j];
     x[i*N+j] = x[j*N+i];
     x[j*N+i] = temp;
   }
}

/**
 * @brief 
 * Cross product of two vectors
 * @param x1[IN] pointer to 3 x 1 vector
 * @param x2[IN] pointer to 3 x 1 vector
 * @param y[OUT] pointer to 3 x 1 vector
 */
void DSPF_dp_vec_cross(double *x1, double *x2, double *restrict y)
{
  y[0] = x1[1]*x2[2] - x1[2]*x2[1]; 
  y[1] = x1[2]*x2[0] - x1[0]*x2[2];
  y[2] = x1[0]*x2[1] - x1[1]*x2[0];
}





/**
 * @brief 
 * Invert matrix calculation of 3 x 3 matrix
 * @param x[IN]   3 x 3 matrix 
 * @param c[IN]  number ofcolumns of a row
 * @return det of matrix
 */
double DSPF_dp_mat_det(double *x,const int c)
{
  double Y[3][3] = {0};
  double det = (x[0*c+0]*x[1*c+1]*x[2*c+2] - x[0*c+0]*x[1*c+2]*x[2*c+1] - x[0*c+1]*x[1*c+0]*x[2*c+2] + x[0*c+1]*x[1*c+2]*x[2*c+0] + x[0*c+2]*x[1*c+0]*x[2*c+1] - x[0*c+2]*x[1*c+1]*x[2*c+0]);
  return det;
}

/**
 * @brief 
 * Invert matrix calculation of 3 x 3 matrix
 * @param x[IN]   3 x 3 matrix 
 * @param c[IN]   number ofcolumns of a row
 * @param y[OUT]  3 x 3 matrix
 */
void DSPF_dp_mat_inv(double *x,const int c, double * y)
{
  double Y[3][3] = {0};
  double det = (x[0*c+0]*x[1*c+1]*x[2*c+2] - x[0*c+0]*x[1*c+2]*x[2*c+1] - x[0*c+1]*x[1*c+0]*x[2*c+2] + x[0*c+1]*x[1*c+2]*x[2*c+0] + x[0*c+2]*x[1*c+0]*x[2*c+1] - x[0*c+2]*x[1*c+1]*x[2*c+0]);
  int i,j;
  if(fabs(det)>1e-9)
  {
    Y[0][0] = (x[1*c+1]*x[2*c+2] - x[1*c+2]*x[2*c+1])/det;
    Y[0][1] =-(x[0*c+1]*x[2*c+2] - x[0*c+2]*x[2*c+1])/det;
    Y[0][2] = (x[0*c+1]*x[1*c+2] - x[0*c+2]*x[1*c+1])/det;
    Y[1][0] =-(x[1*c+0]*x[2*c+2] - x[1*c+2]*x[2*c+0])/det;
    Y[1][1] = (x[0*c+0]*x[2*c+2] - x[0*c+2]*x[2*c+0])/det;
    Y[1][2] =-(x[0*c+0]*x[1*c+2] - x[0*c+2]*x[1*c+0])/det;
    Y[2][0] = (x[1*c+0]*x[2*c+1] - x[1*c+1]*x[2*c+0])/det;
    Y[2][1] =-(x[0*c+0]*x[2*c+1] - x[0*c+1]*x[2*c+0])/det;
    Y[2][2] = (x[0*c+0]*x[1*c+1] - x[0*c+1]*x[1*c+0])/det;
  }
  for(i=0;i<3;i++)
  {
    for(j=0;j<3;j++)
    y[i*c+j] = Y[i][j]; 
  }
}

/**
 * @brief 
 * Matrix multiply (max dimension should be equal to or lower than 4)
 * @param x1[IN]  pointer to r1 x c1 matrix
 * @param r1[IN]  number of rows of x1
 * @param c1[IN]  number of columns of x1
 * @param x2[IN]  pointer to c1 x c2 matrix
 * @param c2[IN]  number of columns of x2
 * @param y[OUT]  pointer to r1 x c2 matrix
 */
void DSPF_dp_mat_mul_any(double *x1,const double a, const int r1, const int c1,
    double *x2, const int c2, double * y)
{
  int i,j,k;
  for(i=0;i<r1;i++)
  for(j=0;j<c1;j++)
  for(k=0;k<c2;k++)
  y[i*c2+k]+=a*x1[i*c1+j]*x2[j*c2+k];

      }


void simon_h(double *p, double *hx, int m, int n,void *adata)
{
  double **A = (double **)adata;
  double (*H)[3][3] = p;
  double (*Y)[3][4] = hx;
  memset(hx,0,sizeof(double)*12);
  int i,j;
  double Yhap[2][4] = {0};
  for (i = 0; i < 3; i++)
  for (j = 0; j < 3; j++)
  {
    (*H)[i][j] = (*H)[j][i];
  }

  DSPF_dp_mat_mul_any(H,1,3,3,A[0], 4,Y);

}

void poseCalc(const double(* points)[3][TARGET_NUM],Pose *pose)
{
  int i,j,k;
  double H[3][3];
  double invM[3][3];
  double invMH[3][3] = {0};
  double *A[2];
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
  getH_initialize();
  getH(Y,X,H);
  dlevmar_dif(simon_h,H,points,9,12,1000,NULL,info,NULL,NULL,A);
  DSPF_dp_mat_trans_local(H,3);
  DSPF_dp_mat_inv(M,3,invM);
  DSPF_dp_mat_mul_any(invM,1,3,3,H,3,invMH);


  double Zc1 = 1/sqrt(invMH[0][0]*invMH[0][0]+invMH[1][0]*invMH[1][0]+invMH[2][0]*invMH[2][0]);
  double Zc2 = 1/sqrt(invMH[0][1]*invMH[0][1]+invMH[1][1]*invMH[1][1]+invMH[2][1]*invMH[2][1]);

  pose->T.Z = sqrt(Zc1*Zc2);
  pose->T.X = pose->T.Z*invMH[0][2];
  pose->T.Y = pose->T.Z*invMH[1][2];

  //Normalize
  for(i=0;i<3;i++)
  {
    invMH[i][0] = invMH[i][0]*Zc1;
    invMH[i][1] = invMH[i][1]*Zc2;
  }

  invMH[0][2] = invMH[1][0]*invMH[2][1] - invMH[1][1]*invMH[2][0];
  invMH[1][2] = invMH[0][0]*invMH[2][1] - invMH[0][1]*invMH[2][0];
  invMH[2][2] = invMH[0][0]*invMH[1][1] - invMH[1][0]*invMH[0][1];

  pose->R.roll  =  atan(invMH[1][0]/invMH[1][1])/PI*180;
  pose->R.pitch = -asin(invMH[1][2])/PI*180;
  pose->R.yaw   =  atan(invMH[0][2]/invMH[2][2])/PI*180;

}



#endif
