#ifndef MONOGLOBAL_H
#define MONOGLOBAL_H


#include "config.h"
#include <string.h>
#include <math.h>

//===========Debug Variables==========//
#ifdef DEBUG
extern double debug_imageReceive_time;
extern double debug_imageSegment_time;
#endif

//===========Data Structure===========//
typedef struct
{
    double X;
    double Y;
    double Z;
    double ratio;
}Coord;

//==========Rotation Angle==========//

//1. Rotate roll degrees about Z axis;
//2. Rotate yaw degrees about Y axis;
//3. Rotate pitch degrees about X axis;

//==================================//
typedef struct
{
    double roll;
    double yaw;
    double pitch;
}Angle;


typedef struct
{
    Angle R;
    Coord T;
}Pose;


//===========Global Variables===========//
extern const void * emifRecvAddr;
extern const void * emifSendAddr;
extern unsigned char image[];
extern unsigned char threshold[];
extern unsigned int binary[];
extern double points[3][TARGET_NUM];
extern const double M[3][3];
extern const double P[4][TARGET_NUM];
extern double p[4][TARGET_NUM];

extern unsigned char debug_img[];
extern double debug_pos[3][4];

//===========Function Declaration=======//
extern void generateCoordinates(Pose pose,double (* p)[4][TARGET_NUM]);
extern void poseCalc(const double (* points)[3][TARGET_NUM],Pose *pose);
extern void recvEMIF(const void * address, unsigned char* image);
extern void sendEMIF(const void * address, const Pose * pose);

#endif
