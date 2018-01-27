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

#pragma DATA_ALIGN(M,8)
const double M[4][4] = {FOCUS/DX,0,IMG_WIDTH/2,0,
                      0,FOCUS/DY,IMG_HEIGHT/2,0,
                      0,0,1,0,
                      0,0,0,0};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
const double P[4][4] = {-72.5, 17.5, 127.5, -72.5,
                 18.75,18.75, 18.75,-56.25,
                 0, 0, 0, 0,
                 0,0,0,0};

#pragma DATA_ALIGN(p, 8)
double p[4][4] = {0};

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
