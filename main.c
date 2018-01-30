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
const void * emifRecvAddr = (void *)EMIF_IMG;
const void * emifSendAddr = (void *)EMIF_POSE;

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
   	EMIF_init();//use nand cfg
	//configure for non-loopback test, and use CPU for TX
	KeyStone_UART_config(115200, FALSE, UART_USE_CORE_TO_TX);
	KeyStone_UART_Interrupts_Init(TRUE, FALSE);//UART interrupt en,DMA disable
    while(1)
	{
		if(Parat_Update_Flag == 1)
		{
			Parat_Update_Flag = 0;
		}
		else if(Rec_Img_num != Rec_Img_num_old)
		{
			Rec_Img_num_old = Rec_Img_num;
			//taskProcImage(0);
			if(Rec_Img_num > 10000)
			{
				Rec_Img_num = 0;
				Rec_Img_num_old = 0;
			}
		}
	}
    BIOS_start();    
    return(0);
}
