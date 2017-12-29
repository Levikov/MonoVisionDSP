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

#define IMG_WIDTH 768
#define IMG_HEIGHT 768
#define IMG_SIZE 589824
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
}Coord;

typedef struct
{
    float alpha;
    float beta;
    float gamma;
}Angle;

typedef struct
{
    unsigned int FrameId;
    unsigned char buffer[IMG_SIZE];
}Frame;

typedef struct
{
    unsigned int FrameId;
    unsigned int buffer[IMG_SIZE/32];
}Binary;


typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    Frame buffer[IMG_BUFFER_SIZE];
}FrameBuffer;

typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    Binary buffer[IMG_BUFFER_SIZE];
}BinaryBuffer;

typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    VLIB_CCHandle buffer[IMG_BUFFER_SIZE];
}CCBuffer;

typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    VLIB_CCHandle *buffer[IMG_BUFFER_SIZE];
}CCHandleBuffer;

typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    VLIB_blobList buffer[IMG_BUFFER_SIZE];
}BlobBuffer;

typedef struct
{
    unsigned int headId;
    unsigned int tailId;
    Coord buffer[4];
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
