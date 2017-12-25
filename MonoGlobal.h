#ifndef MONOGLOBAL_H
#define MONOGLOBAL_H

#include <c6x.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>

#include <ti/sysbios/knl/clock.h>
#include <ti/sysbios/hal/Cache.h>

#include <ti/imglib/imglib.h>
#include <ti/vlib/vlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#define IMG_WIDTH 768
#define IMG_HEIGHT 768
#define IMG_SIZE 589824
#define IMG_THRES 50

#define IMG_BUFFER_SIZE 8

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

//===========Global Variables===========//
extern volatile FrameBuffer inputBuffer;
extern volatile FrameBuffer thresholdBuffer;
extern volatile BinaryBuffer binaryBuffer;

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
#endif
