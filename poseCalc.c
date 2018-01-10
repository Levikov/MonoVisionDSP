#include <MonoGlobal.h>
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
  float x0,y0,a,b,c,d,e,g,A,B,C,delta;
  float err[2];
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
  lambda[0] = a>0?(-b+sqrtsp(delta))/(2*a):(-b-sqrtsp(delta))/(2*a);
  lambda[1] = - lambda[0];

  A = b*b - 1/g;
  B = 2*b*b*e - e/g + d - 2*b*c;
  C = b*b*e*e + c*c - 2*b*c*e;

  delta = B*B - 4*A*C;
  miu[0] = a>0?(-b+sqrtsp(delta))/(2*a):(-b-sqrtsp(delta))/(2*a);
  miu[1] = - miu[0];

  err[0] = abs(c+lambda[0]*miu[0]-a*(d*+2*lambda[0]*lambda[0])) + abs(c+lambda[0]*miu[0]-b*(e*+2*miu[0]*miu[0])); 
  err[1] = abs(c+lambda[0]*miu[1]-a*(d*+2*lambda[0]*lambda[0])) + abs(c+lambda[0]*miu[1]-b*(e*+2*miu[0]*miu[0])); 

  if(err[0]>err[1])
  {
    delta = miu[0];
    miu[0] = miu[1];
    miu[1] = delta;
  }

}

Void taskPoseCalc()
{
  
}
