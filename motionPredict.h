#ifndef MOTIONPREDICT_H
#define MOTIONPREDICT_H
extern int kalmanFlag;
extern const double Fk[3][3];
extern double state[3][3];
extern void motionPredictInit(double *x);
extern void motionPredictCorrect(double *x);
extern void motionPredict(double *restrict y);
#endif