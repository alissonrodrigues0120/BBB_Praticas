#ifndef CONFIG_H
#define CONFIG_H

#define MATRIX_SIZE 3
#define WDT_WSPR 0x48
#define WDT_WWPS 0x34
#define W_PEND_WSPR (1 << 0x4u)
#include "interrupt.h"
#include "clock_module.h"
#include "hw_types.h"
#include "control_module.h"
#include "gpio.h"
#include "pad.h"

void watchdog(void);
void delay_ms(unsigned int ms);
void update_leds(void);
void move_snake(int direction);
void ISR_Handler(void);
void settingButtons(void);
void settingLeds(void);

#endif /* CONFIG_H */

