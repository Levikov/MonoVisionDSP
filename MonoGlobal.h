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

#include <ti/imglib/imglib.h>
#include <ti/vlib/vlib.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#define IMG_WIDTH 768
#define IMG_HEIGHT 768
#define IMG_SIZE 589824
#define IMG_BUFFER_SIZE 8

#define CORE_NUM 8
#define CLOCK_TICK_TIMEOUT 250000

//===========Debug Variables==========//
#ifdef DEBUG
extern float debug_imageReceive_time;
extern float debug_imageSegment_time;
extern long long debug_buffer_top;
extern long long debug_buffer_tail;
#endif

//===========Data Structure===========//
typedef struct
{
    unsigned long long FrameId;
    unsigned char buffer[IMG_SIZE];
}Frame;

typedef struct
{
    unsigned long long headId;
    unsigned long long tailId;
    Frame inputFrames[IMG_BUFFER_SIZE];
}InputBuffer;

//===========Global Variables===========//
extern InputBuffer inputBuffer;
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

#endif
