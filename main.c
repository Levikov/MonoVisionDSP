/*
 *  ======== main.c ========
 */
#include "MonoGlobal.h"
#include "KeyStone_UART_Init_drv.h"
#include "KeyStone_EMIF16_Init.h"
#include "UART_Interrupt.h"

#ifdef DEBUG
#include <MonoDebug.h>
#endif


//===========Global Variables===========//
const unsigned short * emifRecvAddr = (const unsigned short *)EMIF_IMG;
const unsigned short * emifSendAddr = (const unsigned short *)EMIF_POSE;
const unsigned short * emifFlagAddr = (const unsigned short *)EMIF_FLAG;

unsigned int Parat_Update_Flag = 0;
unsigned int Rec_Img_num = 0;
unsigned int Rec_Img_num_old = 0;

#pragma DATA_ALIGN (image,8)
unsigned char image[IMG_SIZE];
#pragma DATA_ALIGN (threshold,8)
unsigned char threshold[IMG_SIZE];
#pragma DATA_ALIGN (binary,8)
unsigned int binary[IMG_SIZE/32];

#pragma DATA_ALIGN(M,8)
const double M[3][3] = {FOCUS/DX,0,IMG_WIDTH/2,
                      0,FOCUS/DY,IMG_HEIGHT/2,
                      0,0,1};

//Target Location Matrix
#pragma DATA_ALIGN(P, 8)
const double P[4][TARGET_NUM] = 
{76,  -14,  -124,  76,   -14,
 30,    30,    30,   -45,   -45,
 1,   1,    1,  1,   1,
 0,   0,    0,  0,   0};

#pragma DATA_ALIGN(p, 8)
double p[4][TARGET_NUM] = {0};

/*
 *  ======== main ========
 */
Int main()
{ 
    Error_Block eb;
    Error_init(&eb);
   	EMIF_init();
	KeyStone_UART_config(115200, FALSE, UART_USE_CORE_TO_TX);
	KeyStone_UART_Interrupts_Init(TRUE, FALSE);//UART interrupt en,DMA disable
#ifndef POSE_CALC_TEST
#ifdef PRINT_CONSOLE
    printf("Real Image Test\n");
    printf("======================================================================\n");
    printf("case\tpitch\troll\tyaw\tX\tY\tZ\tstatus\n");
    printf("----------------------------------------------------------------------\n");
#endif
#endif
    BIOS_start();
    return(0);
}
