/****************************************************************************/
/*                                                                          */
/*              ���ݴ������ӿƼ����޹�˾                                    */
/*                                                                          */
/*              Copyright (C) 2015 Tronlong Electronic Technology Co.,Ltd   */
/*                                                                          */
/****************************************************************************/
/*
 *   - ϣ����Ĭ(bin wang)
 *   - bin@tronlong.com
 *   - DSP C665x ��Ŀ��
 *
 *   ���� www.tronlong.com
 *   ��̳ 51dsp.net
 *
 *   2015��05��06��
 *
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "hw_gpio.h"
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/*                                                                          */
/*              �궨��                                                      */
/*                                                                          */
/****************************************************************************/
/* GPIO �ܽŸ��� */
// DSP C6678 �޹ܽŸ��ö���ֻ�� 16 ����0 - 15�� GPIO ��
#define SOC_GPIO_0_REGS                     (0x02320000)

#define GPIO00_LENDIAN                0
#define GPIO01_BOOTMODE               1
#define GPIO02_BOOTMODE               2
#define GPIO03_BOOTMODE               3
#define GPIO04_BOOTMODE               4
#define GPIO05_BOOTMODE               5
#define GPIO06_BOOTMODE               6
#define GPIO07_BOOTMODE               7
#define GPIO08_BOOTMODE               8
#define GPIO09_BOOTMODE               9
#define GPIO10_BOOTMODE               10
#define GPIO11_BOOTMODE               11
#define GPIO12_BOOTMODE               12
#define GPIO13_BOOTMODE               13
#define GPIO14_PCIESSMODE0            14
#define GPIO15_PCIESSMODE1            15
#define GPIO16_TIMI0_PCIESSEN         16
#define GPIO17_TIMI1                  17
#define GPIO18_TIMO0                  18
#define GPIO19_TIMO1                  19
#define GPIO20_UARTRX0                20
#define GPIO21_UARTTX0                21
#define GPIO22_UARTCTS0               22
#define GPIO23_UARTRTS0               23
#define GPIO24_UARTRX1                24
#define GPIO25_UARTTX1                25
#define GPIO26_UARTCTS1               26
#define GPIO27_UARTRTS1               27
#define GPIO28_SPICS0                 28
#define GPIO29_SPICS1                 29
#define GPIO30_SPIDIN                 30
#define GPIO31_SPIDOUT                31

/* GPIO �ܽŹ���ѡ�� */
/* ��ͨ IO �� */
#define GPIO_NORMAL_ENABLED           1
/* ������������� */
#define GPIO_FUNCTION_ENABLED         0

/* GPIO �ܽŷ��� */
/* ���� */
#define GPIO_DIR_INPUT                1
/* ��� */
#define GPIO_DIR_OUTPUT               0

/* �жϴ������� */
/* ���ùܽű����жϴ��� */
#define GPIO_INT_TYPE_NOEDGE          0
/* ʹ���½��ش��� */
#define GPIO_INT_TYPE_FALLEDGE        1
/* ʹ�������ش��� */
#define GPIO_INT_TYPE_RISEDGE         2
/* ʹ�������ؼ��½��ش��� */
#define GPIO_INT_TYPE_BOTHEDGE        3

/* �жϵȴ�״̬ */
/* �ж���� */
#define GPIO_INT_NOPEND               0
/* �жϵȴ� */
#define GPIO_INT_PEND                 1

/* ���״̬ */
/* �߼� 0 */
#define GPIO_PIN_LOW                  0
/* �߼� 1 */
#define GPIO_PIN_HIGH                 1

/****************************************************************************/
/*                                                                          */
/*              ��������                                                    */
/*                                                                          */
/****************************************************************************/
void GPIODirModeSet(unsigned int baseAdd, unsigned int pinNumber, unsigned int pinDir);
unsigned int GPIODirModeGet(unsigned int baseAdd, unsigned int pinNumber);
void GPIOPinWrite(unsigned int baseAdd, unsigned int pinNumber, unsigned int bitValue);
int GPIOPinRead(unsigned int baseAdd, unsigned int pinNumber);
void GPIOIntTypeSet(unsigned int baseAdd, unsigned int pinNumber, unsigned int intType);
unsigned int GPIOIntTypeGet(unsigned int baseAdd, unsigned int pinNumber);
unsigned int GPIOPinIntStatus(unsigned int baseAdd, unsigned int pinNumber);
void GPIOPinIntClear(unsigned int baseAdd, unsigned int pinNumber);
void GPIOBankIntEnable(unsigned int baseAdd);
void GPIOBankIntDisable(unsigned int baseAdd);
void GPIOPinMuxSet(unsigned int baseAdd, unsigned int pinNumber, unsigned int config);

#ifdef __cplusplus
}
#endif
#endif
