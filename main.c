/*
 *  ======== main.c ========
 */
#include "MonoGlobal.h"

#ifdef DEBUG
#include <MonoDebug.h>
#endif


//===========Global Variables===========//
const void * emifRecvAddr = (void *)&debug_img;
const void * emifSendAddr = (void *)EMIF_POSE;

#pragma DATA_ALIGN (image,8)
unsigned char image[IMG_SIZE];
#pragma DATA_ALIGN (threshold,8)
unsigned char threshold[IMG_SIZE];
#pragma DATA_ALIGN (binary,8)
unsigned int binary[IMG_SIZE/32];

/*
 *  ======== main ========
 */
Int main()
{ 
    Error_Block eb;
    System_printf("enter main()\n");
    Error_init(&eb);
    taskProcImage(0);
    BIOS_start();    /* does not return */
    return(0);
}
