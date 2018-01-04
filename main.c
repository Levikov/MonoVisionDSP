/*
 *  ======== main.c ========
 */
#include "MonoGlobal.h"

#ifdef DEBUG
#include <MonoDebug.h>
#endif


//===========Global Variables===========//

#pragma DATA_ALIGN (inputBuffer,8)
 FrameBuffer inputBuffer;
#pragma DATA_ALIGN (thresholdBuffer,8)
 FrameBuffer thresholdBuffer;
#pragma DATA_ALIGN (binaryBuffer,8)
 BinaryBuffer binaryBuffer;
#pragma DATA_ALIGN (ccBuffer,8)
 CCBuffer ccBuffer;
#pragma DATA_ALIGN(blobBuffer,8)
BlobBuffer blobBuffer;

PosBuffer posBuffer;

Pose pose;

void *buffer_CC;

Clock_Params clockParams[CORE_NUM];
Clock_Handle clockHandle[CORE_NUM];

/*
 *  ======== main ========
 */
Int main()
{ 
    Error_Block eb;
    System_printf("enter main()\n");
    Error_init(&eb);
    switch(DNUM)
    {
        case 0:initInputProc();break;
        case 1:initImgSegProc();break;
        case 2:initImgSegProc();break;
        case 3:initImgJudgeProc();break;
        case 4:initImgJudgeProc();break;
        case 5:initPoseCalcProc();break;
        case 6:initPoseCalcProc();break;
        case 7:initOutputProc();break;
        default:break;
    }
    BIOS_start();    /* does not return */
    return(0);
}
