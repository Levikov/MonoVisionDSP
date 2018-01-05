#include <MonoGlobal.h>
//Camera Calibration Matrix
#pragma DATA_ALIGN(M, 8)
float M[4][4] = {2294.35, 0, 1172.41, 0,
                 0, 2310.24, 885.828, 0,
                 0, 0,  1,  0,
                 0, 0,  0,  0};
#pragma DATA_ALIGN(invM, 8)
float invM[4][4] = {4.358532917819862e-04, 0, -0.510998757818118, 0,
                 0, 4.328554609044948e-04, -0.383435487222107, 0,
                 0, 0,  1,  0,
                 0, 0,  0,  0};

//Target Location Matrix
#pragma DATA_ALIGN(X, 8)
float X[4][4] = {0, 90, 200, 0,
                 0, 0, 0, 75,
                 1, 1, 1, 1,
                 0, 0, 0, 0};
#pragma DATA_ALIGN(Y, 8)
float Y[4][4] = {0};

typedef struct
{
  double ratio[9];
  float (*X)[4][4];
  float (*Y)[4][4];
} SimplexParams;

int simon_h(short a[], const void *params)
{
  SimplexParams *p = (SimplexParams*)params;
  int i = 0, j = 0;
  float(*H)[4][4] = Memory_alloc(NULL, sizeof(float) * 16, 8, NULL);
  float(*Yhap)[4][4] = Memory_alloc(NULL, sizeof(float) * 16, 8, NULL);
  float diff[9];
  double error = 0;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      (*H)[i][j] = a[i * 3 + j] * p->ratio[i * 3 + j];
    }

  DSPF_sp_mat_mul((float *)H, 4, 4, (float *)p->X, 4, (float *)Yhap);

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      (*Yhap)[i][j] = (*Yhap)[i][j] / (*Yhap)[2][j];
    };

  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
    {
      diff[3 * i + j] = (*Yhap)[i][j] - (*p->Y)[i][j];
      error += abs(diff[3 * i + j]);
    }

  int result =(int)error*100;

  Memory_free(NULL, H, sizeof(float) * 16);
  Memory_free(NULL, Yhap, sizeof(float) * 16);
  return result;
}

Void taskPoseCalc()
{
  float H[4][4] = {0};

  //Initialize matrix Y
  int i = 0, j = 0;
  for (i = 0; i < 4; i++)
  {
    Y[0][i] = posBuffer.buffer[i].X;
    Y[1][i] = posBuffer.buffer[i].Y;
    Y[2][i] = posBuffer.buffer[i].Z;
  }

  //Allocate memory for SVD
  float invX[4][4] = {0};
  float(*U)[3][3] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);
  float invD[4][4] = {0};
  float(*V)[4][4] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);
  float(*U1)[4][4] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);
  float(*diag)[4] = Memory_alloc(NULL, 4 * sizeof(float), 8, NULL);
  float(*superdiag)[4] = Memory_alloc(NULL, 4 * sizeof(float), 8, NULL);
  float(*transU)[4][4] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);
  float(*invDtransU)[4][4] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);
  float(*transH)[4][4] = Memory_alloc(NULL, 16 * sizeof(float), 8, NULL);

  //Calculate SVD
  DSPF_sp_svd(3, 4, (float *)X, (float *)U, (float *)V, (float *)U1, (float *)diag, (float *)superdiag);
  for (i = 0; i < 3; i++)
    invD[i][i] = 1 / (*diag)[i];

  //Calculate H = VD*U'

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      (*transU)[i][j] = (*U)[j][i];
    }
  DSPF_sp_mat_mul((float *)invD, 4, 4, (float *)transU, 4, (float *)invDtransU);
  DSPF_sp_mat_mul((float *)V, 4, 4, (float *)invDtransU, 4, (float *)invX);
  DSPF_sp_mat_mul((float *)Y, 4, 4, (float *)invX, 4, (float *)V);
  DSPF_sp_mat_mul((float *)invM, 4, 4, (float *)V, 4, (float *)H);
  DSPF_sp_mat_trans((float *)H, 4, 4, (float *)transH);

  float Zc1 = 1/VLIB_L2_norm_f32((*transH)[0],3);
  float Zc2 = 1/VLIB_L2_norm_f32((*transH)[1],3);
  pose.T.Z = sqrtsp(Zc1*Zc2);
  pose.T.X = pose.T.Z*H[0][2];
  pose.T.Y = pose.T.Z*H[1][2];

  //Normalize
  for(i=0;i<3;i++)
  {
    H[i][0] = H[i][0]*Zc1;
    H[i][1] = H[i][1]*Zc2;
  }


  H[0][2] = H[1][0]*H[2][1] - H[1][1]*H[2][0];
  H[1][2] = H[0][0]*H[2][1] - H[0][1]*H[2][0];
  H[2][2] = H[0][0]*H[1][1] - H[1][0]*H[0][1];

  pose.R.yaw = -asinsp(H[2][0]*pose.T.Z)/PI*180;
  pose.R.roll = atansp(H[1][0]/H[0][0])/PI*180;
  pose.R.pitch = atansp(H[2][1]/H[2][2])/PI*180;


  Memory_free(NULL, U, 16 * sizeof(float));
  Memory_free(NULL, V, 16 * sizeof(float));
  Memory_free(NULL, transU, 16 * sizeof(float));
  Memory_free(NULL, invDtransU, 16 * sizeof(float));
  Memory_free(NULL, transH, 16 * sizeof(float));
  Memory_free(NULL, U1, 16 * sizeof(float));
  Memory_free(NULL, diag, 4 * sizeof(float));
  Memory_free(NULL, superdiag, 4 * sizeof(float));
}
