#include <MonoGlobal.h>
#include "KeyStone_UART_Init_drv.h"
#include <gpio.h>

UART_Config    gUARTCfg;
/*UART configure*/
void KeyStone_UART_config(Uint32 baudRate, Bool bLoopBackEnable,
	UART_Tx_Master txMaster)
{
	//clear the configuration data structure
	memset(&gUARTCfg, 0, sizeof(gUARTCfg));

	gUARTCfg.baudRate = baudRate;
	gUARTCfg.DSP_Core_Speed_Hz = gDSP_Core_Speed_Hz;
	gUARTCfg.dataLen = DATA_LEN_8BIT;
	gUARTCfg.parityMode = PARITY_DISABLE;
	gUARTCfg.stopMode = ONE_STOP_BIT;
    gUARTCfg.autoFlow = AUTO_FLOW_DIS;
	gUARTCfg.osmSel = OVER_SAMPLING_16X;
    gUARTCfg.fifoRxTriBytes = TRIGGER_LEVEL_14BYTE;
    gUARTCfg.txMaster = txMaster;
	gUARTCfg.bLoopBackEnable = bLoopBackEnable;

	KeyStone_UART_init(&gUARTCfg, 0);//config uart 115200，8 data bit,1 stop bit,no parity bit,16x sample,fifo mode
	GPIODirModeSet((0x02320000), 14, 0);//14 gpio ctr rs485 ,=1 tx en;=0 rx en
	GPIOPinWrite((0x02320000), 14, 0);//14 gpio reset = 0
}