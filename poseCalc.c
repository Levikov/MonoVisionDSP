#include <MonoGlobal.h>
float zero[3][3] = {0};
float eye[3][3] = {1,0,0,
                   0,1,0,
                   0,0,1};

//Camera Calibration Matrix
#pragma DATA_ALIGN(M, 8)
float M[3][3] = {2.272727272727273e+03, 0, 640,
                 0, 2.272727272727273e+03, 640,
                 0, 0,  1};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
float P[4][4] = {-72.5, 17.5, 127.5, -72.5,
                 18.75,18.75, 18.75,-56.25,
                 0, 0, 0, 0,
                 1, 1, 1, 1};

//H = pinv(P*P')*P pre-calculated for initialization of R0
#pragma DATA_ALIGN(H, 8)
float H[4][4] = {-0.004817275747508,-3.322259136212628e-04,0.005149501661130,1.301042606982605e-18,
                 0.010653377630122,0.004872646733112,-0.002192691029900,-0.013333333333333,
                 0,0,0,0,
                 0.250000000000000,0.250000000000000,0.250000000000000,0.250000000000000
                };

#pragma DATA_ALIGN(Y, 8)
float Y[4][4] = {0};

/**
 * @brief 
 * Local transpose of matrix
 * @param x[IN/OUT] N x N matrix
 * @param N[IN]     dimension of matrix
 */
void DSPF_sp_mat_trans_local(float *restrict x, const int N)
{
  int i,j = 0;
  float temp;
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
void DSPF_sp_vec_cross(float *x1, float *x2, float *restrict y)
{
  y[0] = x1[1]*x2[2] - x1[2]*x2[1]; 
  y[1] = x1[2]*x2[0] - x1[0]*x2[2];
  y[2] = x1[0]*x2[1] - x1[1]*x2[0];
}

/**
 * @brief 
 * Invert matrix calculation of 3 x 3 matrix
 * @param x[IN]   3 x 3 matrix 
 * @param y[OUT]  3 x 3 matrix
 */
void DSPF_sp_mat_inv(float (*x)[3][3], float (*restrict y)[3][3])
{
  float det = ((*x)[0][0]*(*x)[1][1]*(*x)[2][2] - (*x)[0][0]*(*x)[1][2]*(*x)[2][1] - (*x)[0][1]*(*x)[1][0]*(*x)[2][2] + (*x)[0][1]*(*x)[1][2]*(*x)[2][0] + (*x)[0][2]*(*x)[1][0]*(*x)[2][1] - (*x)[0][2]*(*x)[1][1]*(*x)[2][0]);
  (*y)[0][0] = ((*x)[1][1]*(*x)[2][2] - (*x)[1][2]*(*x)[2][1])/det;
  (*y)[0][1] =-((*x)[0][1]*(*x)[2][2] - (*x)[0][2]*(*x)[2][1])/det;
  (*y)[0][2] = ((*x)[0][1]*(*x)[1][2] - (*x)[0][2]*(*x)[1][1])/det;
  (*y)[1][0] =-((*x)[1][0]*(*x)[2][2] - (*x)[1][2]*(*x)[2][0])/det;
  (*y)[1][1] = ((*x)[0][0]*(*x)[2][2] - (*x)[0][2]*(*x)[2][0])/det;
  (*y)[1][2] =-((*x)[0][0]*(*x)[1][2] - (*x)[0][2]*(*x)[1][0])/det;
  (*y)[2][0] = ((*x)[1][0]*(*x)[2][1] - (*x)[1][1]*(*x)[2][0])/det;
  (*y)[2][1] =-((*x)[0][0]*(*x)[2][1] - (*x)[0][1]*(*x)[2][0])/det;
  (*y)[2][2] = ((*x)[0][0]*(*x)[1][1] - (*x)[0][1]*(*x)[1][0])/det;
}

/**
 * @brief 
 * Get column of a matrix
 * @param x1[IN]  r x ? matrix
 * @param r[IN]   rows of x1
 * @param c[IN]   column to obatin
 * @param y[OUT]  r x 1 vector
 */
void DSPF_sp_mat_col(float *x1, const int r, const int c, float *restrict y)
{
  int i;
  for(i=0;i<r;i++)
  y[i] = x1[i*r+c];
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
void DSPF_sp_mat_linear_comb(float *x1, float alpha,float *x2, float beta, 
                            const int r, const int c, float *restrict y)
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
void unifyImageCoord(float (* restrict p)[3][TARGET_NUM],float (* restrict M)[3][3])
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
void DSPF_sp_crossMat(float *restrict x,float (* restrict y)[3][3])
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
void solveQuarticEquation(float (* restrict IJphap)[4][2],float *restrict lambda,float *restrict miu)
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
    delta = miu[0];
    miu[0] = miu[1];
    miu[1] = delta;
  }

}

/**
 * @brief Initialize rotation matrix R0 for the orthogonal iteration
 * 
 * @param p[IN] - pointer to 3xN focus unified coordinates of images of targets
 * @param P[IN] - pointer to 4xN homogeneous world coordinates of targets
 * @param R0[OUT] -  pointer to 3x3 rotation matrix
 */
void initRotationMatrix(float (*restrict p)[3][TARGET_NUM],float (*restrict P)[4][TARGET_NUM],float (*restrict R0)[3][3])
{
  float IJphap[4][2]={0};
  float pp[TARGET_NUM][2]={0};
  float lambda[2] = {0};
  float miu[2] = {0};
  float Ip[3],Jp[3];
  float crossIp[3][3],crossJp[3][3];
  float tz;
  int i=0;
  for(i=0;i<TARGET_NUM;i++)
  {
    pp[i][0] = (*p)[0][i];
    pp[i][1] = (*p)[1][i];
  }
  
  DSPF_sp_mat_mul(&H,4,TARGET_NUM,&pp,2,&IJphap);
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
    DSPF_sp_crossMat(Ip,&crossIp);
    DSPF_sp_crossMat(Jp,&crossJp);
    tz = sqrtdp(sqrtdp(1+IJphap[3][0]*IJphap[3][0])/sqrtdp(Ip[0]*Ip[0]+Ip[1]*Ip[1]+Ip[2]*Ip[2])*sqrtdp(1+IJphap[3][1]*IJphap[3][1])/sqrtdp(Jp[0]*Jp[0]+Jp[1]*Jp[1]+Jp[2]*Jp[2]));

  }
}

Void taskPoseCalc()
{
  float p[3][TARGET_NUM] = {1};
  float R[2][3][3] = {0};
  int i=0;
  for(i=0;i<TARGET_NUM;i++)
  {
    p[0][i] = posBuffer.buffer[i].X;
    p[1][i] = posBuffer.buffer[i].Y;
  }
  unifyImageCoord(&p,&M);
  initRotationMatrix(&p,&P,R);

  
}
