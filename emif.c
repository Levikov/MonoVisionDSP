#include <MonoGlobal.h>
#include <ti/csl/csl.h>
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_edma3.h>
#include "KeyStone_EMIF16_Init.h"

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
void recvEMIF(const void * address, unsigned char * image)
{
	int i,j;
	unsigned short *p = address;
	unsigned short temp;
	unsigned char low;
	unsigned char high;
	for(i=0;i<IMG_SIZE/2;i++)
	{
		temp = p[i];
		low = temp&0xff;
		high = temp>>8;
		image[2*i] = low;
		image[2*i+1] = high;
	}
}

/**
 * @brief 
 * Send pose detection result
 * @param address[IN/OUT] pointer to EMIF.
 * @param pose[IN] pointer to pose result. 
 */
void sendEMIF(const void * address,const Pose * pose)
{
  memcpy(address,pose,sizeof(pose));
}

