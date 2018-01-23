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

#include <ti/sysbios/knl/clock.h>
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
extern float debug_imageReceive_time;
extern float debug_imageSegment_time;
#endif

//===========Data Structure===========//
typedef struct
{
    float X;
    float Y;
    float Z;
    float ratio;
}Coord;

//==========Rotation Angle==========//

//1. Rotate roll degrees about Z axis;
//2. Rotate yaw degrees about Y axis;
//3. Rotate pitch degrees about X axis;

//==================================//
typedef struct
{
    float roll;
    float yaw;
    float pitch;
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
extern Coord points[];

extern unsigned char debug_img[];
extern float debug_pos[3][4];

//===========Function Declaration=======//
extern void taskProcImage(UArg a0);
extern void binarize(const unsigned char *p,unsigned int *q);
extern void connectedComponent(unsigned int * binary, VLIB_CCHandle *ccHandle,unsigned char ** bufferCC,int * size);
extern void blob(VLIB_CCHandle *ccHandle,Coord *points);
extern void poseCalc(const Coord * points,Pose *pose);
extern void recvEMIF(const void * address, unsigned char* image);
extern void sendEMIF(const void * address, const Pose * pose);

#endif
