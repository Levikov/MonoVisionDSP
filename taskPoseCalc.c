#include <MonoGlobal.h>
//Camera Calibration Matrix
float M[3][3] = {2294.35, 0, 1172.41,
                 0, 2310.24, 885.828,
                 0, 0, 1};

//Target Location Matrix
float X[3][4] = {0, 90, 200, 0,
                 0, 0, 0, 75,
                 1, 1, 1, 1};

Void taskPoseCalc()
{
  float H[4][4] = {0};

  //Initialize matrix Y
  float Y[4][4] = {0};
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

  Memory_free(NULL,U, 16 * sizeof(float));
  Memory_free(NULL,V,16 * sizeof(float));
  Memory_free(NULL,U1,16 * sizeof(float));
  Memory_free(NULL, diag,4 * sizeof(float));
  Memory_free(NULL,superdiag, 4 * sizeof(float));
  //Optimiztion

}