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

  float transU[4][4] = {0};
  float invDtransU[4][4] = {0};

  //Calculate SVD
  DSPF_sp_svd(3, 4, (float *)X, (float *)U, (float *)V, (float *)U1, (float *)diag, (float *)superdiag);
  for (i = 0; i < 3; i++)
    invD[i][i] = 1 / (*diag)[i];

  //Calculate H = VD*U'

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      transU[i][j] = (*U)[j][i];
    }
  DSPF_sp_mat_mul((float *)invD, 4, 4, (float *)transU, 4, (float *)invDtransU);
  DSPF_sp_mat_mul((float *)V, 4, 4, (float *)invDtransU, 4, (float *)invX);
  DSPF_sp_mat_mul((float *)Y, 4, 4, (float *)invX, 4, (float *)H);

  Memory_free(NULL, U, 16 * sizeof(float));
  Memory_free(NULL, V, 16 * sizeof(float));
  Memory_free(NULL, U1, 16 * sizeof(float));
  Memory_free(NULL, diag, 4 * sizeof(float));
  Memory_free(NULL, superdiag, 4 * sizeof(float));
}
