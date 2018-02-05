#include <MonoGlobal.h>
#include <ti/csl/csl.h>
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_edma3.h>
#include "KeyStone_EMIF16_Init.h"

typedef struct
{
	unsigned char FarStatus:1;
	unsigned char GlobalStatus:1;
	unsigned char NearLStatus:1;
	unsigned char NearRStatus:1;
	unsigned char Unused:2;
	unsigned char isInRange:1;
}Status;

typedef struct
{
	unsigned short head;
	unsigned short group;
	Status status;
	unsigned char parameter;
	double info;
	unsigned char unused;
	unsigned char checksum;
}PoseInstruction;

#pragma DATA_ALIGN(instructions,8)
PoseInstruction instructions[6];

#pragma DATA_ALIGN(instructions,8)
PoseInstruction instructionsBIG[6];

EMIF16_CE_Config gNandCeCfg;/*the configuration for the CE of NAND FLASH*/
EMIF16_Config gEmif16Cfg;

void EMIF_init()
{
	memset(&gNandCeCfg, 0, sizeof(gNandCeCfg));
	memset(&gEmif16Cfg, 0, sizeof(gEmif16Cfg));
	/*configuraiton for the CE of NAND FLASH*/
	gNandCeCfg.busWidth= EMIF_BUS_16BIT;
	gNandCeCfg.opMode  = NAND_MODE;
	gNandCeCfg.strobeMode = SS_STROBE;
	gNandCeCfg.waitMode = EMIF_WAIT_NONE;
	/*timing configuration for NAND512R3A2DZA6E*/
	gNandCeCfg.wrSetup = 1;//CSL_EMIF16_A0CR_WSETUP_RESETVAL,
	gNandCeCfg.wrStrobe = 2;//CSL_EMIF16_A0CR_WSTROBE_RESETVAL,4
	gNandCeCfg.wrHold= 2;//CSL_EMIF16_A0CR_WHOLD_RESETVAL,    5
	gNandCeCfg.rdSetup= 1;//CSL_EMIF16_A0CR_RSETUP_RESETVAL
	gNandCeCfg.rdStrobe= 2;//CSL_EMIF16_A0CR_RSTROBE_RESETVAL 7
	gNandCeCfg.rdHold= 2;//CSL_EMIF16_A0CR_RHOLD_RESETVAL     2
	gNandCeCfg.turnAroundCycles= CSL_EMIF16_A0CR_TA_RESETVAL;
	gNandCeCfg.nor_pg_Cfg= NULL;

	gEmif16Cfg.ceCfg[1] = &gNandCeCfg;   // ce1,old ce0  niu
	gEmif16Cfg.ceCfg[2] = &gNandCeCfg;   // ce1,old ce0  niu

	gEmif16Cfg.wait0Polarity= EMIF_LOW_WAIT;
	gEmif16Cfg.wait1Polarity= EMIF_LOW_WAIT;
	gEmif16Cfg.maxWait = CSL_EMIF16_AWCCR_MAXEXTWAIT_RESETVAL;

	KeyStone_EMIF16_init(&gEmif16Cfg);
}

/**
 * @brief 
 * Receive image from EMIF
 * @param address[IN/OUT] pointer to EMIF
 * @param image[OUT] pointer 
 */
void recvEMIF(unsigned short * address, unsigned char * image)
{
	int i,j;
	unsigned short temp;
	unsigned char low;
	unsigned char high;
	for(i=0;i<IMG_ACTUAL_HEIGHT;i++)
	for(j=0;j<IMG_ACTUAL_WIDTH/2;j++)
	{
		temp = address[i];
		low = temp&0xff;
		high = temp>>8;
		if(i>=(IMG_ACTUAL_HEIGHT - IMG_HEIGHT)/2&&i<(IMG_ACTUAL_HEIGHT + IMG_HEIGHT)/2
			&&j>=(IMG_ACTUAL_WIDTH - IMG_WIDTH)/4&&j<(IMG_ACTUAL_WIDTH + IMG_WIDTH)/4)
		{
			image[(i - (IMG_ACTUAL_HEIGHT - IMG_HEIGHT)/2)*IMG_WIDTH + 2*(j-(IMG_ACTUAL_WIDTH - IMG_WIDTH)/4)] = low;
			image[(i - (IMG_ACTUAL_HEIGHT - IMG_HEIGHT)/2)*IMG_WIDTH + 2*(j-(IMG_ACTUAL_WIDTH - IMG_WIDTH)/4) +1] = high;
		}
	}
}

void setInstructions(const Pose *pose,const char status)
{
	static unsigned short group = 0;
	int i,j;
	double *info = (double *)pose;
	unsigned char *pChar;

	for(i=0;i<6;i++)
	{
		pChar = (unsigned char *)&instructions[i];
		instructions[i].head = 0x90EB;
		instructions[i].unused = 0;
		instructions[i].group = group;
		instructions[i].status.FarStatus = 1;
		instructions[i].status.NearLStatus = 1;
		instructions[i].status.NearRStatus = 1;
		instructions[i].status.GlobalStatus = 1;
		if(status)
		#ifdef CAMERA_FAR
		instructions[i].status.FarStatus = 0;
		#endif
		#ifdef CAMERA_NEAR_L
		instructions[i].status.NearLStatus = 0;
		#endif
		#ifdef CAMERA_NEAR_R
		instructions[i].status.NearRStatus = 0;
		#endif
		#ifdef CAMERA_GLOBAL
		instructions[i].status.GlobalStatus = 0;
		#endif
		instructions[i].status.Unused = 0;
		#ifdef CAMERA_FAR
		instructions[i].parameter = i+1+0x00;
		#endif
		#ifdef CAMERA_NEAR_L
		instructions[i].parameter = i+1+0x10;
		#endif
		#ifdef CAMERA_NEAR_R
		instructions[i].parameter = i+1+0x16;
		#endif
		#ifdef CAMERA_GLOBAL
		instructions[i].parameter = i+1+0x06;
		#endif
		instructions[i].info = info[i];
		instructions[i].checksum = 0;
		if(pose->T.Z <1500)
			instructions[i].status.isInRange = 1;
		else
			instructions[i].status.isInRange = 0;
			
		for(j=0;j<6;j++)
		{
			instructions[i].checksum += pChar[j];
		}
		for(j=8;j<17;j++)
		{
			instructions[i].checksum += pChar[j];
		}
	}
	DSP_blk_eswap16(instructions,instructionsBIG,72);
	group++;
}

/**
 * @brief 
 * Send pose detection result
 * @param address[IN/OUT] pointer to EMIF.
 * @param pose[IN] pointer to pose result. 
 */
void sendEMIF(unsigned short * address,const Pose * pose,const char status)
{
	int i,j;
	unsigned short *pShort;

	setInstructions(pose,status);

	for(i=0;i<6;i++)
	{
		pShort = (unsigned short *)&instructionsBIG[i];
		for(j=0;j<3;j++)
		{
		address[j] = pShort[j];
		}
		for(j=4;j<9;j++)
		{
				address[j] = pShort[j];
		}
	}
}

