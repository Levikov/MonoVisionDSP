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
{0, -90, -200,  0, -90,
 0,   0,    0,-75, -75,
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
    System_printf("enter main()\n");
    Error_init(&eb);
   	EMIF_init();//use nand cfg
	//configure for non-loopback test, and use CPU for TX
	KeyStone_UART_config(115200, FALSE, UART_USE_CORE_TO_TX);
	KeyStone_UART_Interrupts_Init(TRUE, FALSE);//UART interrupt en,DMA disable

	while(TRUE)
    {
        if(*emifFlagAddr==1)
        {
            taskProcImage(0);
        }
    }
    BIOS_start();
    return(0);
}
