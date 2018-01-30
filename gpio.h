/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright (C) 2015 Tronlong Electronic Technology Co.,Ltd   */
/*                                                                          */
/****************************************************************************/
/*
 *   - 希望缄默(bin wang)
 *   - bin@tronlong.com
 *   - DSP C665x 项目组
 *
 *   官网 www.tronlong.com
 *   论坛 51dsp.net
 *
 *   2015年05月06日
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
/*              宏定义                                                      */
/*                                                                          */
/****************************************************************************/
/* GPIO 管脚复用 */
// DSP C6678 无管脚复用而且只有 16 个（0 - 15） GPIO 口
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

/* GPIO 管脚功能选择 */
/* 普通 IO 口 */
#define GPIO_NORMAL_ENABLED           1
/* 外设输入输出口 */
#define GPIO_FUNCTION_ENABLED         0

/* GPIO 管脚方向 */
/* 输入 */
#define GPIO_DIR_INPUT                1
/* 输出 */
#define GPIO_DIR_OUTPUT               0

/* 中断触发类型 */
/* 禁用管脚边沿中断触发 */
#define GPIO_INT_TYPE_NOEDGE          0
/* 使能下降沿触发 */
#define GPIO_INT_TYPE_FALLEDGE        1
/* 使能上升沿触发 */
#define GPIO_INT_TYPE_RISEDGE         2
/* 使能上升沿及下降沿触发 */
#define GPIO_INT_TYPE_BOTHEDGE        3

/* 中断等待状态 */
/* 中断清除 */
#define GPIO_INT_NOPEND               0
/* 中断等待 */
#define GPIO_INT_PEND                 1

/* 输出状态 */
/* 逻辑 0 */
#define GPIO_PIN_LOW                  0
/* 逻辑 1 */
#define GPIO_PIN_HIGH                 1

/****************************************************************************/
/*                                                                          */
/*              函数声明                                                    */
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
