#ifndef MONOGLOBAL_H
#define MONOGLOBAL_H

#include <c6x.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/gates/GateAll.h>
#include <ti/sysbios/hal/Cache.h>

#include <ti/imglib/imglib.h>
#include <ti/vlib/vlib.h>
#include <ti/vlib/src/common/c66/VLIB_types.h>
#include <ti/dsplib/dsplib.h>
#include <ti/mathlib/mathlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <config.h>

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
    double area;
}Circle;

typedef struct
{
    double X;
    double Y;
    double Z;
}Coord;

//==========Rotation Angle==========//

//1. Rotate roll degrees about Z axis;
//2. Rotate yaw degrees about Y axis;
//3. Rotate pitch degrees about X axis;

//==================================//
typedef struct
{
    double pitch;
    double roll;
    double yaw;
}Angle;


typedef struct
{
    Coord T;
    Angle R;
}Pose;


//===========Global Variables===========//
extern const unsigned short * emifRecvAddr;
extern const unsigned short * emifSendAddr;
extern const unsigned short * emifFlagAddr;
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
extern void taskProcImage(UArg a0);
extern void binarize(const unsigned char *p,unsigned int *q);
extern void connectedComponent(unsigned int * binary, VLIB_CCHandle *ccHandle,unsigned char ** bufferCC,int * size);
extern char blob(VLIB_CCHandle *ccHandle,double (*points)[3][TARGET_NUM]);
extern void generateCoordinates(Pose pose,double (*restrict p)[4][TARGET_NUM]);
extern void poseCalc(const double (* points)[3][TARGET_NUM],Pose *pose);
extern void recvEMIF(unsigned short * address, unsigned char* image);
extern void sendEMIF(unsigned short * address, const Pose * pose);
extern void EMIF_init();


#endif
