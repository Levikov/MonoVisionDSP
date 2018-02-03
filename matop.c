#include <matop.h>
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