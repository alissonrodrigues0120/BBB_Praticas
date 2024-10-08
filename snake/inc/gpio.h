#ifndef GPIO_H_
#define GPIO_H_

/*
 * =====================================================================================
 *
 *       Filename:  gpio.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/02/2017 10:39:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "control_module.h"
#include "clock_module.h"
#include "bbb_regs.h"
#include "hw_types.h"
#include "pad.h"

#define GPIO_REVISION           0x000
#define GPIO_SYSCONFIG          0x010
#define GPIO_EOI                0x020:
#define GPIO_IRQWAKEN_0         0x044
#define GPIO_IRQWAKEN_1         0x048
#define GPIO_SYSSTATUS          0x114
#define GPIO_CTRL               0x130
#define GPIO_OE                 0x134
#define GPIO_DATAIN             0x138
#define GPIO_DATAOUT            0x13C
#define GPIO_LEVELDETECT0       0x140
#define GPIO_LEVELDETECT1       0x144
#define GPIO_RISINGDETECT       0x148
#define GPIO_FALLINGDETECT      0x14C
#define GPIO_DEBOUNCENABLE      0x150
#define GPIO_DEBOUNCINGTIME     0x154
#define GPIO_CLEARDATAOUT       0x190
#define GPIO_SETDATAOUT         0x194



/*-----------------------------------------------------------------------------
 *  GPIO pin number type (0-31)
 *-----------------------------------------------------------------------------*/
typedef unsigned char ucPinNumber;

/*-----------------------------------------------------------------------------
 *  init ENUMs
 *-----------------------------------------------------------------------------*/
typedef enum _pinLevel{
	LOW,
	HIGH
}pinLevel;				/* ----------  end of enum pinLevel  ---------- */

typedef enum _gpioMod{
	GPIO0,
	GPIO1,
	GPIO2,
	GPIO3
}gpioMod;				/* ----------  end of enum gpioMod  ---------- */

typedef enum _pinDirection{
	OUTPUT,
	INPUT
}pinDirection;				/* ----------  end of enum pinDirection  ---------- */



/*-----------------------------------------------------------------------------
 *  Function prototype
 *-----------------------------------------------------------------------------*/
void gpioInitModule(gpioMod );
void gpioInitPin(gpioMod ,ucPinNumber );
void gpioSetDirection(gpioMod ,ucPinNumber ,pinDirection );	
int  gpioGetDirection(ucPinNumber ,ucPinNumber );
void gpioSetPinValue(gpioMod ,ucPinNumber ,pinLevel );
unsigned int gpioGetPinValue(gpioMod ,ucPinNumber );
	
#endif /*GPIO_H_ */
