#include <MonoGlobal.h>
#include <float.h>
#include <orthogonalIteration.h>
#include <orthogonalIteration_initialize.h>

#pragma DATA_ALIGN(zero,8) 
const double zero[4][4] = {0};
double eye3[3][3] = {1,0,0,
                   0,1,0,
                   0,0,1};

//Camera Calibration Matrix
#pragma DATA_ALIGN(M, 16)
double M[3][3] = {2.272727272727273e+03, 0, 640,
                 0, 2.272727272727273e+03, 640,
                 0, 0,  1};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
double P[4][4] = {-72.5, 17.5, 127.5, -72.5,
                 18.75,18.75, 18.75,-56.25,
                 0, 0, 0, 0,
                 1, 1, 1, 1};

//H = pinv(P*P')*P pre-calculated for initialization of R0
#pragma DATA_ALIGN(H, 8)
double H[4][4] = {-0.004817275747508,-3.322259136212628e-04,0.005149501661130,1.301042606982605e-18,
                 0.010653377630122,0.004872646733112,-0.002192691029900,-0.013333333333333,
                 0,0,0,0,
                 0.250000000000000,0.250000000000000,0.250000000000000,0.250000000000000
                };

#pragma DATA_ALIGN(Y, 8)
double Y[4][4] = {0};

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
    double *x2, const int c2, double *restrict y)
{
  int i,j,k;
  for(i=0;i<r1;i++)
  for(j=0;j<c1;j++)
  for(k=0;k<c2;k++)
  y[i*c2+k]+=a*x1[i*c1+j]*x2[j*c2+k];

      }

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
void DSPF_dp_mat_inv(double *x,const int c, double *restrict y)
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
 * Get column of a matrix
 * @param x1[IN]  r x c matrix
 * @param N[IN]   index of column to obtain
 * @param r[IN]   rows of x1
 * @param c[IN]   columns of x1
 * @param y[OUT]  r x 1 vector
 */
void DSPF_dp_mat_col(double *x1, const int N, const int r, const int c, double *restrict y)
{
  int i;
  for(i=0;i<r;i++)
  y[i] = x1[i*c+N];
}

/**
 * @brief 
 * Linear combination of two matrix
 * @param x1[IN]    r x c matrix
 * @param alpha[IN] scale factor of x1
 * @param x2[IN]    r x c matrix
 * @param beta[IN]  scale factor of x2
 * @param r[IN]     rows
 * @param c[IN]     columns
 * @param y[OUT]    r x c matrix
 */
void DSPF_dp_mat_linear_comb(double *x1, double alpha,double *x2, double beta, 
                            const int r, const int c, double *restrict y)
{
  int i,j;
  for(i=0;i<r;i++)
  for(j=0;j<c;j++)
  {
    *(y+i*c+j) = alpha*x1[i*c+j] + beta*x2[i*c+j];
  }
}

/**
 * @brief 
 * Convert image coordinates to focus unified coordinate.
 * @param p[IN/OUT]  3 x TARGET_NUM matrix of image coordinates
 * @param M[IN] 3 x 3 camera calibration matrix
 */
void unifyImageCoord(double (* restrict p)[3][TARGET_NUM],double (* restrict M)[3][3])
{
  int i=0;
  for(i=0;i<TARGET_NUM;i++)
  {
    (*p)[0][i] = ((*p)[0][i] - (*M)[0][2])/(*M)[0][0];
    (*p)[1][i] = ((*p)[1][i] - (*M)[1][2])/(*M)[1][1];
    (*p)[2][i] = 1;
  }
}

/**
 * @brief 
 * Construct cross matrix from vector
 * @param x[IN]  pointer to 3x1 vector
 * @param y[OUT] pointer to 3x3 matrix
 */
void DSPF_dp_crossMat(double *restrict x,double (* restrict y)[3][3])
{
  (*y)[0][0] = 0;
  (*y)[0][1] = -x[2];
  (*y)[0][2] = x[1];
  (*y)[1][0] = x[2];
  (*y)[1][1] = 0;
  (*y)[1][2] = -x[0];
  (*y)[2][0] = -x[1];
  (*y)[2][1] = x[0];
  (*y)[2][2] = 0;
}

/**
 * @brief 
 * Solve λ and μ from given Ip^ and Jp^
 * @param IJphap[IN] 4x2 matrix [Ip^,Jp^]
 * @param lambda[OUT] pointer to 2x1 vector 
 * @param miu[OUT] pointer to 2x1 vector
 */
void solveQuarticEquation(double (* restrict IJphap)[4][2],double *restrict lambda,double *restrict miu)
{
  double x0,y0,a,b,c,d,e,g,A,B,C,delta;
  double err[2];
  x0 = (*IJphap)[3][0];
  y0 = (*IJphap)[3][1];
  a = x0*y0/(1+x0*x0);
  b = x0*y0/(1+y0*y0);
  c = (*IJphap)[0][0]*(*IJphap)[0][1] + (*IJphap)[1][0]*(*IJphap)[1][1] + (*IJphap)[2][0]*(*IJphap)[2][1];
  d = (*IJphap)[0][0]*(*IJphap)[0][0] + (*IJphap)[1][0]*(*IJphap)[1][0] + (*IJphap)[2][0]*(*IJphap)[2][0];
  e = (*IJphap)[0][1]*(*IJphap)[0][1] + (*IJphap)[1][1]*(*IJphap)[1][1] + (*IJphap)[2][1]*(*IJphap)[2][1];
  g = (1 + x0*x0)/(1 + y0*y0);

  A = a*a - g;
  B = 2*a*a*d - g*d + e - 2*a*c;
  C = a*a*d*d + c*c - 2*a*c*d;

  delta = B*B - 4*A*C;
  lambda[0] = A>0?sqrtdp((-B+sqrtdp(delta))/(2*A)):sqrtdp((-B-sqrtdp(delta))/(2*A));
  lambda[1] = - lambda[0];

  A = b*b - 1/g;
  B = 2*b*b*e - e/g + d - 2*b*c;
  C = b*b*e*e + c*c - 2*b*c*e;

  delta = B*B - 4*A*C;
  miu[0] = A>0?sqrtdp((-B+sqrtdp(delta))/(2*A)):sqrtdp((-B-sqrtdp(delta))/(2*A));
  miu[1] = - miu[0];

  err[0] = fabs(c+lambda[0]*miu[0]-a*(d*+2*lambda[0]*lambda[0])) + abs(c+lambda[0]*miu[0]-b*(e*+2*miu[0]*miu[0])); 
  err[1] = fabs(c+lambda[0]*miu[1]-a*(d*+2*lambda[0]*lambda[0])) + abs(c+lambda[0]*miu[1]-b*(e*+2*miu[0]*miu[0])); 

  if(err[0]>err[1])
  {
    delta = lambda[0];
    lambda[0] = lambda[1];
    lambda[1] = delta;
  }

}

/**
 * @brief Initialize rotation matrix R0 for the orthogonal iteration
 * 
 * @param p[IN] - pointer to 3xN focus unified coordinates of images of targets
 * @param P[IN] - pointer to 4xN homogeneous world coordinates of targets
 * @param R0[OUT] -  pointer to 3x3 rotation matrix
 */
void initRotationMatrix(double (*restrict p)[3][TARGET_NUM],double (*restrict P)[4][TARGET_NUM],double (*restrict R0)[3][3])
{
  double IJphap[4][2]={0};
  double pp[TARGET_NUM][2]={0};
  double lambda[2] = {0};
  double miu[2] = {0};
  double Ip[3],Jp[3];
  double crossIp[3][3],crossJp[3][3],temp[3][3],temp2[3][3];
  double tz;
  int i=0,j=0,k=0;
  for(i=0;i<TARGET_NUM;i++)
  for(j=0;j<4;j++)
  for(k=0;k<2;k++)
  {
    IJphap[i][k]+=(*p)[k][j]*H[i][j];
    pp[i][0] = (*p)[0][i];
    pp[i][1] = (*p)[1][i];
  }
  
  solveQuarticEquation(&IJphap,lambda,miu);
  for(i=0;i<3;i++)
  {
    Ip[i] = IJphap[i][0];
    Jp[i] = IJphap[i][1];
  }

  for(i=0;i<2;i++)
  {
    Ip[2] = lambda[i];
    Jp[2] = miu[i];
    DSPF_dp_crossMat(Ip,&crossIp);
    DSPF_dp_crossMat(Jp,&crossJp);
    tz = sqrtdp(sqrtdp(1+IJphap[3][0]*IJphap[3][0])/sqrtdp(Ip[0]*Ip[0]+Ip[1]*Ip[1]+Ip[2]*Ip[2])*sqrtdp(1+IJphap[3][1]*IJphap[3][1])/sqrtdp(Jp[0]*Jp[0]+Jp[1]*Jp[1]+Jp[2]*Jp[2]));
    DSPF_dp_mat_linear_comb(&crossIp,-tz*IJphap[3][1],&crossJp,tz*IJphap[3][0],3,3,&temp);
    DSPF_dp_mat_linear_comb(&eye3,1,&temp,1,3,3,&temp);
    DSPF_dp_mat_inv(&temp,3,&temp2);
    DSPF_dp_mat_linear_comb(&temp2,tz*tz,&zero,0,3,3,temp2);
    DSPF_dp_vec_cross(Ip,Jp,temp[0]);
    DSPF_dp_mat_mul_any(&temp2,1,3,3,&temp[0],1,&R0[i][2]);
    DSPF_dp_mat_linear_comb(&Ip,tz,&R0[i][2],IJphap[3][0],1,3,&R0[i][0]);
    DSPF_dp_mat_linear_comb(&Jp,tz,&R0[i][2],IJphap[3][1],1,3,&R0[i][1]);

  }
}


/**
 * @brief 
 * Calculate pose from given targets coordinates in image
 * @param p[IN]   pointer to 2 x TARGET_NUM coordinates matrix of targets in image 
 * @param pose[OUT]  pointer to Pose structure 
 */
void poseCalc(const float (*p)[2][TARGET_NUM],Pose *restrict pose)
{
  double pp[3][TARGET_NUM] = {1};
  double R[2][3][3] = {0};
  double R0[2][3][3] = {0};
  double t[2][3] = {0};
  double error[2] = {0};
  int i,j;
  for(i=0;i<TARGET_NUM;i++)
  {
    pp[0][i] = (*p)[0][i];
    pp[1][i] = (*p)[1][i];
  }
  unifyImageCoord(&pp,&M);
  initRotationMatrix(&pp,&P,R);
  DSPF_dp_mat_trans_local(R[0],3);
  DSPF_dp_mat_trans_local(R[1],3);
  DSPF_dp_mat_trans_local(P,4);
  double Y[4][3];
  for(i=0;i<4;i++)for(j=0;j<3;j++)
  Y[i][j] = pp[j][i];

  memcpy(R0,R,sizeof(R));
  orthogonalIteration_initialize();
  for(i=0;i<2;i++)
  {
    orthogonalIteration(P,Y,&R0[i],0.0001,&R[i],&t[i],&error[i]);
  }
  i=0;
  DSPF_dp_mat_trans_local(R[0],3);
  DSPF_dp_mat_trans_local(R[1],3);
  DSPF_dp_mat_trans_local(P,4);

  pose->R.roll  =  atandp(R[i][1][0]/R[i][1][1])/PI*180;
  pose->R.pitch = -asindp(R[i][1][2])/PI*180;
  pose->R.yaw   =  atandp(R[i][0][2]/R[i][2][2])/PI*180;
  
  pose->T.X = t[i][0];  
  pose->T.Y = t[i][1];  
  pose->T.Z = t[i][2];  
}
