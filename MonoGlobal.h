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
#include <ti/dsplib/dsplib.h>
#include <ti/mathlib/mathlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#define PI 3.14159265358979323846

#define IMG_WIDTH 1280
#define IMG_HEIGHT 1280
#define IMG_SIZE 1638400
#define IMG_THRES 20

#define IMG_BUFFER_SIZE 8

#define TARGET_NUM 4

#define CORE_NUM 8
#define CLOCK_TICK_TIMEOUT 250000

//===========Debug Variables==========//
#ifdef DEBUG
extern float debug_imageReceive_time;
extern float debug_imageSegment_time;
extern unsigned int debug_buffer_top;
extern unsigned int debug_buffer_tail;
#endif

//===========Data Structure===========//
typedef struct
{
    float X;
    float Y;
    float Z;
    float ratio;
}Coord;

typedef struct
{
    float alpha;
    float beta;
    float gamma;
}Angle;

typedef struct
{
    unsigned char buffer[IMG_SIZE];
    unsigned int FrameId;
}Frame;

typedef struct
{
    unsigned int buffer[IMG_SIZE/32];
    unsigned int FrameId;
}Binary;


typedef struct
{
    Frame buffer[IMG_BUFFER_SIZE];
    unsigned int headId;
    unsigned int tailId;
}FrameBuffer;

typedef struct
{
    Binary buffer[IMG_BUFFER_SIZE];
    unsigned int headId;
    unsigned int tailId;
}BinaryBuffer;

typedef struct
{
    VLIB_CCHandle buffer[IMG_BUFFER_SIZE];
    unsigned int headId;
    unsigned int tailId;
}CCBuffer;

typedef struct
{
    VLIB_CCHandle *buffer[IMG_BUFFER_SIZE];
    unsigned int headId;
    unsigned int tailId;
}CCHandleBuffer;

typedef struct
{
    VLIB_blobList buffer[IMG_BUFFER_SIZE];
    unsigned int headId;
    unsigned int tailId;
}BlobBuffer;

typedef struct
{
    Coord buffer[4];
    unsigned int headId;
    unsigned int tailId;
}PosBuffer;

typedef struct
{
    Angle R;
    Coord T;
}Pose;


//===========Global Variables===========//
extern  FrameBuffer inputBuffer;
extern  FrameBuffer thresholdBuffer;
extern  BinaryBuffer binaryBuffer;
extern  CCBuffer ccBuffer;
extern  BlobBuffer blobBuffer;
extern  PosBuffer posBuffer;
extern void *buffer_CC;
extern Pose pose;

extern unsigned char debug_img[];

extern Clock_Params clockParams[CORE_NUM];
extern Clock_Handle clockHandle[CORE_NUM];

//===========Function Declaration=======//
extern void initInputProc();
extern void initImgSegProc();
extern void initImgJudgeProc();
extern void initPoseCalcProc();
extern void initOutputProc();
extern Void taskReceiveNewImage(UArg a0);
extern Void taskImageSegment(UArg a0);
extern Void taskThreshold();
extern Void taskBinarize();
extern Void taskConnectedComponentAnalysis();
extern Void taskBlobAnalysis();
extern Void taskPoseCalc();
#endif
