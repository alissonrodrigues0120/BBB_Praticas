#include "config.h"

void watchdog() {
    HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0xAAAA;
    while ((HWREG(SOC_WDT_1_REGS + WDT_WWPS) & W_PEND_WSPR) != 0x0) {};
    HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0x5555;
    while ((HWREG(SOC_WDT_1_REGS + WDT_WWPS) & W_PEND_WSPR) != 0x0) {};
}

void delay_ms(unsigned int ms) {
    volatile unsigned int count;
    while (ms > 0) {
        count = 1000;
        while (count > 0) { count--; }
        ms--;
    }
}

int led_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 0},
    {0, 1, 0}, // Cobra começa no centro
    {0, 0, 0}
};

int head_x = 1;
int head_y = 1;

void update_leds() {
    // Apagar todos os LEDs
    gpioSetPinValue(GPIO1, 29, LOW);
    gpioSetPinValue(GPIO0, 27, LOW);
    gpioSetPinValue(GPIO0, 26, LOW);
    gpioSetPinValue(GPIO2, 1, LOW);
    gpioSetPinValue(GPIO1, 16, LOW);
    gpioSetPinValue(GPIO1, 28, LOW);
    gpioSetPinValue(GPIO1, 17, LOW);
    gpioSetPinValue(GPIO3, 21, LOW);
    gpioSetPinValue(GPIO3, 19, LOW);

    // Acender LEDs de acordo com a matriz
    if (led_matrix[0][0]) gpioSetPinValue(GPIO1, 29, HIGH);
    else gpioSetPinValue(GPIO1, 29, LOW);


    if (led_matrix[0][1]) gpioSetPinValue(GPIO0, 27, HIGH);
    else gpioSetPinValue(GPIO0, 27, LOW);

    if (led_matrix[0][2]) gpioSetPinValue(GPIO0, 26, HIGH);
    else gpioSetPinValue(GPIO0, 26, LOW);

    if (led_matrix[1][0]) gpioSetPinValue(GPIO2, 1, HIGH);
    else gpioSetPinValue(GPIO2, 1, LOW);

    if (led_matrix[1][1]) gpioSetPinValue(GPIO1, 16, HIGH);
    else  gpioSetPinValue(GPIO1, 16, LOW);

    if (led_matrix[1][2]) gpioSetPinValue(GPIO1, 28, HIGH);
    else gpioSetPinValue(GPIO1, 28, LOW);

    if (led_matrix[2][0]) gpioSetPinValue(GPIO1, 17, HIGH);
    else gpioSetPinValue(GPIO1, 17, LOW);

    if (led_matrix[2][1]) gpioSetPinValue(GPIO3, 21, HIGH);
    else  gpioSetPinValue(GPIO3, 21, LOW);


    if (led_matrix[2][2]) gpioSetPinValue(GPIO3, 19, HIGH);
    else gpioSetPinValue(GPIO3, 19, LOW);
}

void move_snake(int direction) {
    // Apaga a posição atual
    led_matrix[head_x][head_y] = 0;

    // Movimenta a cobra baseado na direção
    switch (direction) {
        case 0: head_x = (head_x > 0) ? head_x - 1 : 2; break;  // Cima
        case 1: head_x = (head_x < 2) ? head_x + 1 : 0; break;  // Baixo
        case 2: head_y = (head_y > 0) ? head_y - 1 : 2; break;  // Esquerda
        case 3: head_y = (head_y < 2) ? head_y + 1 : 0; break;  // Direita
    }

    // Limpa a matriz de LEDs
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int y = 0; y < MATRIX_SIZE; y++) {
            led_matrix[i][y] = 0;
        }
    }

    // Atualiza a nova posição da cobra
    led_matrix[head_x][head_y] = 1;
    update_leds();
}


void settingButtons() {
    gpioInitPin(GPIO1, 12);
    gpioInitPin(GPIO1, 13);
    gpioInitPin(GPIO1, 14);
    gpioInitPin(GPIO1, 15);
    gpioSetDirection(GPIO1, 12, INPUT);
    gpioSetDirection(GPIO1, 13, INPUT);
    gpioSetDirection(GPIO1, 14, INPUT);
    gpioSetDirection(GPIO1, 15, INPUT);

    pinInterrup(0, GPIO1, 12);
    pinInterrup(0, GPIO1, 13);
    pinInterrup(0, GPIO1, 14);
    pinInterrup(0, GPIO1, 15);
}

void settingLeds() {
    gpioInitPin(GPIO1, 29);
    gpioInitPin(GPIO0, 27);
    gpioInitPin(GPIO0, 26);
    gpioInitPin(GPIO2, 1);
    gpioInitPin(GPIO1, 16);
    gpioInitPin(GPIO1, 28);
    gpioInitPin(GPIO1, 17);
    gpioInitPin(GPIO3, 21);
    gpioInitPin(GPIO3, 19);

    gpioSetDirection(GPIO1, 29, OUTPUT);
    gpioSetDirection(GPIO0, 27, OUTPUT);
    gpioSetDirection(GPIO0, 26, OUTPUT);
    gpioSetDirection(GPIO2, 1, OUTPUT);
    gpioSetDirection(GPIO1, 16, OUTPUT);
    gpioSetDirection(GPIO1, 28, OUTPUT);
    gpioSetDirection(GPIO1, 17, OUTPUT);
    gpioSetDirection(GPIO3, 21, OUTPUT);
    gpioSetDirection(GPIO3, 19, OUTPUT);

    // Apagar todos os LEDs no início
    gpioSetPinValue(GPIO1, 29, LOW);
    gpioSetPinValue(GPIO0, 27, LOW);
    gpioSetPinValue(GPIO0, 26, LOW);
    gpioSetPinValue(GPIO2, 1, LOW);
    gpioSetPinValue(GPIO1, 16, LOW);
    gpioSetPinValue(GPIO1, 28, LOW);
    gpioSetPinValue(GPIO1, 17, LOW);
    gpioSetPinValue(GPIO3, 21, LOW);
    gpioSetPinValue(GPIO3, 19, LOW);
}
