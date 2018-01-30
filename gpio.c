/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright (C) 2015 Tronlong Electronic Technology Co.,Ltd   */
/*                                                                          */
/****************************************************************************/
/*
 *   移植自 C6748 StarterWare
 *   DSP C665x GPIO 口设备抽象层应用程序接口函数
 *
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

/* TI 版权声明 */
/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/* Driver APIs */
#include "gpio.h"
#include "hw_types.h"

/***********************************************************************/
/*              API FUNCTION DEFINITIONS.                              */ 
/***********************************************************************/

/**
 * \brief    This function configures the direction of a pin as input or 
 *           output.
 *
 * \param    baseAdd     The memory address of the GPIO instance being used.
 * \param    pinNumber   The serial number of the GPIO pin.
 *                       The 32 GPIO pins have serial numbers from 1 to 32.
 *                       
 * \param    pinDir      The direction to be set for the pin.
 *                       This can take the values:
 *                       1> GPIO_DIR_INPUT, for configuring the pin as input.
 *                       2> GPIO_DIR_OUTPUT, for configuring the pin as output.
 * 
 * \return   None.
 *
 * \note     Here we write to the DIRn register. Writing a logic 1 configures 
 *           the pin as input and writing logic 0 as output. By default, all
 *           the pins are set as input pins.
 */
void GPIODirModeSet(unsigned int baseAdd, unsigned int pinNumber, 
                    unsigned int pinDir)

{
    if(GPIO_DIR_OUTPUT == pinDir)
    {
        HWREG(baseAdd + GPIO_DIR) &= ~(1 << pinNumber);
    }
    else
    {
        HWREG(baseAdd + GPIO_DIR) |= (1 << pinNumber);
    }
}

/**
 * \brief  This function gets the direction of a pin which has been configured
 *         as an input or an output pin.
 * 
 * \param   baseAdd    The memory address of the GPIO instance being used.
 * \param   pinNumber  The serial number of the GPIO pin.
 *                     The 32 GPIO pins have serial numbers from 1 to 32.
 *
 * \return  This returns one of the following two values:
 *          1> GPIO_DIR_INPUT, if the pin is configured as an input pin.
 *          2> GPIO_DIR_OUTPUT, if the pin is configured as an output pin.
 *
 */
unsigned int GPIODirModeGet(unsigned int baseAdd, unsigned int pinNumber)
{
    unsigned int dir = GPIO_DIR_INPUT;

    dir = (HWREG(baseAdd + GPIO_DIR) & (1 << pinNumber));

    return (dir >> pinNumber);
}


/**
 * \brief   This function writes a logic 1 or a logic 0 to the specified pin.
 *
 * \param   baseAdd    The memory address of the GPIO instance being used.
 * \param   pinNumber  The serial number of the GPIO pin.
 *                     The 32 GPIO pins have serial numbers from 1 to 32.
 *
 * \param   bitValue   This signifies whether to write a logic 0 or logic 1 
 *                     to the specified pin.This variable can take any of the 
 *                     following two values:
 *                     1> GPIO_PIN_LOW, which indicates to clear(logic 0) the bit.
 *                     2> GPIO_PIN_HIGH, which indicates to set(logic 1) the bit.
 *
 * \return  None.
 *
 * \note    The pre-requisite to write to any pin is that the pin has to
 *          be configured as an output pin.
 */
void GPIOPinWrite(unsigned int baseAdd, unsigned int pinNumber, 
                  unsigned int bitValue)
{
    if(GPIO_PIN_LOW == bitValue)
    {
        HWREG(baseAdd + GPIO_CLR_DATA) = (1 << pinNumber);
    }
    else if(GPIO_PIN_HIGH == bitValue)
    {
        HWREG(baseAdd + GPIO_SET_DATA) = (1 << pinNumber);
    }
}

/**
 * \brief    This function reads the value(logic level) of an input or an
 *           output pin.
 * 
 * \param    baseAdd     The memory address of the GPIO instance being used.
 * \param    pinNumber   The serial number of the GPIO pin.
 *                       The 32 GPIO pins have serial numbers from 1 to 32.
 *
 * \return   This returns the value present on the specified pin. This returns
 *           one of the following values:
 *           1> GPIO_PIN_LOW, if the value on the pin is logic 0.
 *           2> GPIO_PIN_HIGH, if the value on the pin is logic 1.
 *
 * \note     Using this function, we can read the values of both input and 
 *           output pins.
 */
int GPIOPinRead(unsigned int baseAdd, unsigned int pinNumber)
{
    unsigned int val = 0;

    val = HWREG(baseAdd + GPIO_IN_DATA) & (1 << pinNumber);

    return (val >> pinNumber);
}

/**
 * \brief   This function configures the trigger level type for which an 
 *          interrupt is required to occur.
 * 
 * \param   baseAdd    The memory address of the GPIO instance being used.
 *
 * \param   pinNumber  The serial number of the GPIO pin.
 *                     The 32 GPIO pins have serial numbers from 1 to 32.
 *
 * \param   intType    This specifies the trigger level type. This can take 
 *                     one of the following four values:
 *                     1> GPIO_INT_TYPE_NOEDGE, to not generate any interrupts.
 *                     2> GPIO_INT_TYPE_FALLEDGE, to generate an interrupt on 
 *                        the falling edge of a signal on that pin. 
 *                     3> GPIO_INT_TYPE_RISEDGE, to generate an interrupt on the 
 *                        rising edge of a signal on that pin.
 *                     4> GPIO_INT_TYPE_BOTHEDGE, to generate interrupts on both
 *                        rising and falling edges of a signal on that pin.
 *
 * \return   None.
 *
 * \note     Configuring the trigger level type for generating interrupts is not 
 *           enough for the GPIO module to generate interrupts. The user should 
 *           also enable the interrupt generation capability for the bank to which
 *           the pin belongs to. Use the function GPIOBankIntEnable() to do the same.             
 */
 

void GPIOIntTypeSet(unsigned int baseAdd, unsigned int pinNumber, 
                    unsigned int intType)
{
    switch (intType)
    {
        case GPIO_INT_TYPE_RISEDGE:
            /* Setting Rising edge and clearing Falling edge trigger levels.*/
            HWREG(baseAdd + GPIO_SET_RIS_TRIG) = (1 << pinNumber);
            HWREG(baseAdd + GPIO_CLR_FAL_TRIG) = (1 << pinNumber);
            break;

        case GPIO_INT_TYPE_FALLEDGE:
            /* Setting Falling edge and clearing Rising edge trigger levels.*/ 
            HWREG(baseAdd + GPIO_SET_FAL_TRIG) = (1 << pinNumber);
            HWREG(baseAdd + GPIO_CLR_RIS_TRIG) = (1 << pinNumber);
            break;

        case GPIO_INT_TYPE_BOTHEDGE:
            /* Setting both Rising and Falling edge trigger levels.*/
            HWREG(baseAdd + GPIO_SET_RIS_TRIG) = (1 << pinNumber);
            HWREG(baseAdd + GPIO_SET_FAL_TRIG) = (1 << pinNumber);
            break;

        case GPIO_INT_TYPE_NOEDGE:
            /* Clearing both Rising and Falling edge trigger levels. */
            HWREG(baseAdd + GPIO_CLR_RIS_TRIG) = (1 << pinNumber);
            HWREG(baseAdd + GPIO_CLR_FAL_TRIG) = (1 << pinNumber);
            break;

        default:
            break;
    }
}

/**
 * \brief   This function reads the trigger level type being set for interrupts
 *          to be generated.
 * 
 * \param   baseAdd    The memory address of the GPIO instance being used.
 *
 * \param   pinNumber  The serial number of the GPIO pin to be accessed.
 *                     The 32 GPIO pins have serial numbers from 1 to 32.
 *
 * \return  This returns a value which indicates the type of trigger level 
 *          type being set. One of the following values is returned.
 *          1> GPIO_INT_TYPE_NOEDGE, indicating no interrupts will be 
 *             generated over the corresponding pin.
 *          2> GPIO_INT_TYPE_FALLEDGE, indicating a falling edge on the 
 *             corresponding pin signifies an interrupt generation.
 *          3> GPIO_INT_TYPE_RISEDGE, indicating a rising edge on the 
 *             corresponding pin signifies an interrupt generation.
 *          4> GPIO_INT_TYPE_BOTHEDGE, indicating both edges on the
 *             corresponding pin signifies an interrupt each being generated.
 *
 */
unsigned int GPIOIntTypeGet(unsigned int baseAdd, unsigned int pinNumber)
{
    unsigned int intType = GPIO_INT_TYPE_NOEDGE;

    if ((HWREG(baseAdd + GPIO_SET_FAL_TRIG)) & (1 << pinNumber))
    {
        intType = GPIO_INT_TYPE_FALLEDGE;
    }

    if ((HWREG(baseAdd + GPIO_SET_RIS_TRIG)) & (1 << pinNumber))
    {
        intType |= GPIO_INT_TYPE_RISEDGE;
    }
  
    return intType;    
}

/**
 * \brief    This function determines the status of interrupt on a specified
 *           pin.  
 * 
 * \param    baseAdd    The memory address of the GPIO instance being used.

 * \param    pinNumber  The serial number of the GPIO pin to be accessed.
 *                      The 32 GPIO pins have serial numbers from 1 to 32.

 * \return   This returns a value which expresses the status of an interrupt
 *           raised over the specified pin.
 *           1> GPIO_INT_NOPEND, if no interrupts are left to be serviced.
 *           2> GPIO_INT_PEND, if the interrupt raised over that pin is yet
 *              to be cleared and serviced.
 *              
 * \note     If an interrupt over a pin is found to be pending, then the 
 *           application can call GPIOPinIntClear() to clear the interrupt
 *           status.  
 *           
 *
 */
unsigned int GPIOPinIntStatus(unsigned int baseAdd, unsigned int pinNumber)
{
    unsigned int intStatus = GPIO_INT_NOPEND;

    if(HWREG(baseAdd + GPIO_INTSTAT) & (1 << pinNumber))
    {
        intStatus = GPIO_INT_PEND;
    }
        
    return intStatus;
}

/**
 * \brief     This function clears the interrupt status of the pin being 
 *            accessed.
 *
 * \param     baseAdd    The memory address of the GPIO instance being used.
 * \param     pinNumber  The serial number of the GPIO pin to be accessed.
 *                       The 32 GPIO pins have serial numbers from 1 to 32.
 * \return    None.
 *
 */

void GPIOPinIntClear(unsigned int baseAdd, unsigned int pinNumber)
{
    HWREG(baseAdd + GPIO_INTSTAT) = (1 << pinNumber);
}

/**
 * \brief   This function enables the interrupt generation capability for the
 *          bank of GPIO pins specified.
 *
 * \param   baseAdd     The memory address of the GPIO instance being used.
 * \param   bankNumber  This is the bank for whose pins interrupt generation 
 *                      capabiility needs to be enabled.
 *                      bankNumber is 0 for bank 0, 1 for bank 1 and so on.
 * \return  None. 
 *
 */

void GPIOBankIntEnable(unsigned int baseAdd)
{
    HWREG(baseAdd + GPIO_BINTEN) |= (1 << 0);
} 

/**
 * \brief   This function disables the interrupt generation capability for the
 *          bank of GPIO pins specified.
 *
 * \param   baseAdd     The memory address of the GPIO instance being used.
 * \param   bankNumber  This is the bank for whose pins interrupt generation
 *                      capability needs to be disabled.
 *                      bankNumber is 0 for bank 0, 1 for bank 1 and so on.
 * \return  None.
 *
 */

void GPIOBankIntDisable(unsigned int baseAdd)
{
    HWREG(baseAdd + GPIO_BINTEN) &= ~(1 << 0);
}

/**
 * \brief   This function config the gpio pin multiplex
 *
 * \param   baseAdd     The memory address of the Pin control of Device State Control Registers.
 * \param   pinNumber   The serial number of the GPIO pin to be accessed.
 *                      The 32 GPIO pins have serial numbers from 1 to 32.
 * \param   config      The pin's function is a gpio or a peripheral.
 * \return  None.
 *
 */

void GPIOPinMuxSet(unsigned int baseAdd, unsigned int pinNumber,
		            unsigned int config)
{
    if(GPIO_FUNCTION_ENABLED == config)
    {
    	HWREG(baseAdd) &= ~(1 << pinNumber);
    }
    else if(GPIO_NORMAL_ENABLED == config)
    {
    	HWREG(baseAdd) |=  (1 << pinNumber);
    }
}

/*****************************END OF FILE*************************************/ 
