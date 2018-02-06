#include <motionPredict.h>
#include <matop.h>
int kalmanFlag = 0;
const double Fk[3][3] = 
{
  1, 250, 31250,
  0,   1,   250,
  0,   0,     1
};

double state[3][3] = {0};
void motionPredictInit(double *x)
{
    register int i;
    for(i=0;i<3;i++)
    state[0][i] = x[i];
}

void motionPredictCorrect(double *x)
{
  register int i;
  for(i=0;i<3;i++)
  {
      state[2][i] = ((x[i] - state[0][i])/250 -state[1][i])/250;
      state[1][i] = (x[i] - state[0][i])/250;
      state[0][i] = x[i];
  }
  
}

void motionPredict(double *restrict y)
{
    register int i=0;
    double yhap[3][3];
    DSPF_dp_mat_mul_any(Fk,1,3,3,state,3,yhap);
    for(i=0;i<3;i++)
    y[i] = yhap[0][i];
}