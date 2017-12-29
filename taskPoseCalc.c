#include <MonoGlobal.h>
//Camera Calibration Matrix
float M[3][3] = {2294.35, 0, 1172.41,
                 0, 2310.24, 885.828,
                 0, 0, 1};

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
  SimplexParams *p = params;
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

  DSPF_sp_mat_mul(H, 4, 4, p->X, 4, Yhap);

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
  DSPF_sp_svd(3, 4, X, U, V, U1, diag, superdiag);
  for (i = 0; i < 3; i++)
    invD[i][i] = 1 / (*diag)[i];

  //Calculate H = VD*U'

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      transU[i][j] = (*U)[j][i];
    }
  DSPF_sp_mat_mul(invD, 4, 4, transU, 4, invDtransU);
  DSPF_sp_mat_mul(V, 4, 4, invDtransU, 4, invX);
  DSPF_sp_mat_mul(Y, 4, 4, invX, 4, H);

  //Optimiztion

  short(*Hopt)[9] = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  short(*initStep)[9] = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  SimplexParams *simplexParams = Memory_alloc(NULL, sizeof(SimplexParams), 8, NULL);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      if (H[i][j] < 0)
      {
        simplexParams->ratio[3 * i + j] = 1.2 * H[i][j] / 32767;
      }
      else
      {
        simplexParams->ratio[3 * i + j] = -1.2 * H[i][j] / 32767;
      }
      (*Hopt)[3 * i + j] = H[i][j] / simplexParams->ratio[3 * i + j];
      (*initStep)[3*i+j] = 10;
    }
  simplexParams->X = X;
  simplexParams->Y = Y;

  //Allocate memory for simplex
  short *v = Memory_alloc(NULL, sizeof(short) * 90, 8, NULL);
  short *f = Memory_alloc(NULL, sizeof(short) * 10, 8, NULL);
  short *vr = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  short *ve = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  short *vc = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  short *vm = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  int min;
  short(*Hout)[9] = Memory_alloc(NULL, sizeof(short) * 9, 8, NULL);
  VLIB_simplex(simon_h, Hopt, initStep, 3, 1 / 3, 1000, 1000, v, f, vr, ve, vc, vm, simplexParams, &min,&Hout);

  Memory_free(NULL, U, 16 * sizeof(float));
  Memory_free(NULL, V, 16 * sizeof(float));
  Memory_free(NULL, U1, 16 * sizeof(float));
  Memory_free(NULL, diag, 4 * sizeof(float));
  Memory_free(NULL, superdiag, 4 * sizeof(float));
  Memory_free(NULL, v, sizeof(short) * 90);
  Memory_free(NULL, f, sizeof(short) * 10);
  Memory_free(NULL, vr, sizeof(short) * 9);
  Memory_free(NULL, ve, sizeof(short) * 9);
  Memory_free(NULL, vc, sizeof(short) * 9);
  Memory_free(NULL, vm, sizeof(short) * 9);
  Memory_free(NULL, Hout, sizeof(short) * 9);
  Memory_free(NULL, Hopt, sizeof(short) * 9);
  Memory_free(NULL, initStep, sizeof(short) * 9);
  Memory_free(NULL, simplexParams, sizeof(SimplexParams));
}
