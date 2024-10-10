#include "interrupt.h"
#include "clock_module.h"
#include "hw_types.h"
#include "control_module.h"
#include "gpio.h"
#include "pad.h"
#include "config.h"
#include "timer.h"
void ISR_Handler() {
    unsigned int status = HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_RAW_0);

    if (status & (1 << 12)) {
        move_snake(0); // Cima
    }
    if (status & (1 << 13)) {
        move_snake(1); // Baixo
    }
    if (status & (1 << 14)) {
        move_snake(2); // Esquerda
    }
    if (status & (1 << 15)) {
        move_snake(3); // Direita
    }

    // Limpa as interrupções
    HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) |= status;
}


int main() {
    watchdog();
    gpioInitModule(GPIO0);
    gpioInitModule(GPIO1);
    gpioInitModule(GPIO2);
    gpioInitModule(GPIO3);
    timerSetup(TIMER7);
    Interrupt_Setup(98);
    settingButtons();
    settingLeds();

    while (true) {
        delay(200, TIMER7);
    }
}

