#include <MonoGlobal.h>
#ifdef POSE_CALC_METHOD_FAUGERAS


#pragma DATA_ALIGN(Y,8)
double Y[4][4] = {0};

#pragma DATA_ALIGN(X,8)
double X[3][4] = {0};

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

void poseCalc(const double(* points)[3][TARGET_NUM],Pose *pose)
{
  int i,j,k;
  for(i=0;i<TARGET_NUM;i++)
  {
    Y[0][i] = ((*points)[0][i] - IMG_WIDTH/2) * DX;
    Y[1][i] = ((*points)[1][i] - IMG_HEIGHT/2) * DY;
    Y[2][i] = 1;
    X[0][i] = P[0][i];
    X[1][i] = P[1][i];
    X[2][i] = 1;
   }

    double H[4][4] = {0};

  //Allocate memory for SVD
  double invX[4][4] = {0};
  double(*U)[3][3] = Memory_alloc(NULL, 16 * sizeof(double), 8, NULL);
  double invD[4][4] = {0};
  double(*V)[4][4] = Memory_alloc(NULL, 16 * sizeof(double), 8, NULL);
  double(*U1)[4][4] = Memory_alloc(NULL, 16 * sizeof(double), 8, NULL);
  double(*diag)[4] = Memory_alloc(NULL, 4 * sizeof(double), 8, NULL);
  double(*superdiag)[4] = Memory_alloc(NULL, 4 * sizeof(double), 8, NULL);

  double transU[4][4] = {0};
  double invDtransU[4][4] = {0};

  //Calculate SVD
  DSPF_dp_svd(3, 4, X, U, V, U1, diag, superdiag);
  for (i = 0; i < 3; i++)
    invD[i][i] = 1 / (*diag)[i];

  //Calculate H = VD*U'

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      transU[i][j] = (*U)[j][i];
    }
  DSPF_dp_mat_mul_any(invD,1, 4, 4, transU, 4, invDtransU);
  DSPF_dp_mat_mul_any(V,1,4, 4, invDtransU, 4, invX);
  DSPF_dp_mat_mul_any(Y,1,4, 4, invX, 4, H);

  double HH[3][3],t[3],proj[3],R[3][3];
  for(i=0;i<3;i++)for(j=0;j<3;j++)
  HH[i][j] = H[i][j];

  VLIB_homographyDecomposition(HH,R,t,proj);

  pose->R.roll  =  atandp(R[1][0]/R[1][1])/PI*180;
  pose->R.pitch = -asindp(R[1][2])/PI*180;
  pose->R.yaw   =  atandp(R[0][2]/R[2][2])/PI*180;
  
  pose->T.X = t[0];  
  pose->T.Y = t[1];  
  pose->T.Z = t[2];

  Memory_free(NULL,V, 16 * sizeof(double));  
  Memory_free(NULL,U1, 16 * sizeof(double));
  Memory_free(NULL,diag, 4 * sizeof(double));
  Memory_free(NULL,superdiag, 4 * sizeof(double));

}



#endif
