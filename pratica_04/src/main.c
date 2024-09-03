/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Deion:
 *
 *        Version:  1.0
 *        Created:  15/05/2018 14:32:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "bbb_regs.h"
#include "hw_types.h"


/**
 * \brief   This macro will check for write POSTED status
 *
 * \param   reg          Register whose status has to be checked
 *
 *    'reg' can take the following values \n
 *    DMTIMER_WRITE_POST_TCLR - Timer Control register \n
 *    DMTIMER_WRITE_POST_TCRR - Timer Counter register \n
 *    DMTIMER_WRITE_POST_TLDR - Timer Load register \n
 *    DMTIMER_WRITE_POST_TTGR - Timer Trigger register \n
 *    DMTIMER_WRITE_POST_TMAR - Timer Match register \n
 *
 **/
#define DMTimerWaitForWrite(reg)   \
            if(HWREG(DMTIMER_TSICR) & 0x4)\
            while((reg & HWREG(DMTIMER_TWPS)));


int flag_timer = false;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;

typedef enum pinNum{
	PIN1=1,
	PIN2,
	PIN3,
    PIN4
}pinNum;

void disableWdt(void){
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));

	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}

void putCh(char c){
	while(!(HWREG(UART0_LSR) & (1<<5)));

	HWREG(UART0_THR) = c;
}

char getCh(){
	while(!(HWREG(UART0_LSR) & (1<<0)));

	return(HWREG(UART0_RHR));
}

int putString(char *str, unsigned int length){
	for(int i = 0; i < length; i++){
    	putCh(str[i]);
	}
	return(length);
}

int getString(char *buf, unsigned int length){
	for(int i = 0; i < length; i ++){
    	buf[i] = getCh();
   	}
	return(length);
}

void timerEnable(){
    /* Espera a conclusão de gravação anterior no Timer control register*/
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) |= 0x1;
}

void timerDisable(){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) &= ~(0x1);
}/* -----  end of function timerEnable  ----- */

void delay(unsigned int mSec,  unsigned int frequency){

    while(mSec != 0){
        /* Wait for previous write to complete */
        DMTimerWaitForWrite(0x2);

        /* Set the counter value. */
        HWREG(DMTIMER_TCRR) = 0x0;

        timerEnable();

        while(HWREG(DMTIMER_TCRR) < (frequency/1000) + 1);

        /* Stop the timer */
        HWREG(DMTIMER_TCLR) &= ~(0x00000001u);

        mSec--;
    }
}

void butConfig ( ){
	HWREG(CM_PER_GPMCA0_REGS) |= 0x2F;
	HWREG(CM_PER_GPMCBEn1_REGS) |= 0x2F;
	HWREG(CM_PER_GPMCAD12_REGS) |= 0x2F;
	HWREG(CM_PER_GPMCAD14_REGS) |= 0x2f;

    HWREG(GPIO1_OE) |= 1<<28;
	HWREG(GPIO1_OE) |= 1<<16;
	HWREG(GPIO1_OE) |= 1<<12;
	HWREG(GPIO1_OE) |= 1<<14;

	
	HWREG(GPIO1_DATAIN) &= (0 << 28);
	HWREG(GPIO1_DATAIN) &= (0 << 16);
	HWREG(GPIO1_DATAIN) &= (0 << 12);
	HWREG(GPIO1_DATAIN) &= (0 << 14);

	
	flag1 = false;
	flag2 = false;
	flag3 = false;
	flag4 = false;

} /* -----  end of function butConfig  ----- */

void timerSetup(void){

    HWREG(CM_PER_TIMER7_CLKCTRL) |= 0x2;


    while((HWREG(CM_PER_TIMER7_CLKCTRL) & 0x3) != 0x2);


}

void gpioSetup(){
	/* set clock for GPIO1, TRM 8.1.12.1.31 */
	HWREG(CM_PER_GPIO1_CLKCTRL) = 0x40002;



	//iniciando os leds
	/*  configure pin mux for output GPIO */
	HWREG(CM_PER_GPMCA5_REGS) |= 0x7;
	HWREG(CM_PER_GPMCA6_REGS) |= 0x7;
    HWREG(CM_PER_GPMCA7_REGS) |= 0x7;
	HWREG(CM_PER_GPMCA8_REGS ) |= 0x7;
    /* clear pin 23 and 24 for output, leds USR3 and USR4, TRM 25.3.4.3 */
	HWREG(GPIO1_OE) &= ~(1<<21);
	HWREG(GPIO1_OE) &= ~(1<<22);
    HWREG(GPIO1_OE) &= ~(1<<23);
	HWREG(GPIO1_OE) &= ~(1<<24);

}
void ledOff(pinNum pin){
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<21);
		break;
		case PIN2:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<22);
		break;
		case PIN3:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<23);
		break;
        case PIN4:
            HWREG(GPIO1_CLEARDATAOUT) |= (1<<24);
        break;
		default:
		break;
	}/* -----  end switch  ----- */
}




void ledOn(pinNum pin){
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_SETDATAOUT) |= (1<<21);
		break;
		case PIN2:
			HWREG(GPIO1_SETDATAOUT) |= (1<<22);
		break;
		case PIN3:
			HWREG(GPIO1_SETDATAOUT) |= (1<<23);
        break;
		case PIN4:
		    HWREG(GPIO1_SETDATAOUT) |= (1 << 24);
		break;
		default:
		break;
	}/* -----  end switch  ----- */
}


void ledsinterns(unsigned int frequency){
    for(int i=1; i < 5; i++){
        ledOn(i);
        delay(1000, frequency);
    }

    for(int i=1;i < 5;i++){
        ledOff(i);
        delay(1000, frequency);
    }
}

int main(void){
	char count=2;

	/* Hardware setup */
	gpioSetup();
	timerSetup();
	disableWdt();
	butConfig();


	putString("Timer: ",9);
	while(count){
		putCh(0x30+count);
		putCh(' ');
		delay(1000, 24000000);
		count--;
	}

	putString("...OK\n\r",9);
    unsigned int frequencia;
	bool flagtrue = true;
	while(flagtrue){

        frequencia = 24000000;
        putString("menu:\n\r", 7);
        putString("1: 30 MHz\n\r", 13);
        putString("2: 5 MHz\n\r", 12);
        putString("3: 10 MHz\n\r", 13);
        putString("4: 50 MHz\n\r", 13);

        if(HWREG(GPIO1_DATAIN) &=(1 << 28)){
			putString("30 MHz\n\r", 10);
            flag1 = true;
			flagtrue = false;
            frequencia = 30000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 16)){
            putString("5 MHz\n\r", 9);
			flag2 = true;
			flagtrue = false;
            frequencia = 5000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 12)){
             putString("10 MHz\n\r", 10);
			 flag3 = true;
			 flagtrue = false;
             frequencia = 10000000;
			 break;


        }else if(HWREG(GPIO1_DATAIN) &= (1 << 14)){
             putString("50 MHz\n\r", 10);
			 flag4 = true;
			 flagtrue = false;
             frequencia = 50000000;
			 break;

        }
	}
        

		while(flag1){
			ledsinterns(frequencia);
			if(HWREG(GPIO1_DATAIN) &=(1 << 28)){
			flagtrue = true;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 16)){
            putString("5 MHz\n\r", 9);
			flag2 = true;
			flagtrue = false;
            frequencia = 5000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 12)){
             putString("10 MHz\n\r", 10);
			 flag3 = true;
			 flagtrue = false;
             frequencia = 10000000;
			 break;


        }else if(HWREG(GPIO1_DATAIN) &= (1 << 14)){
             putString("50 MHz\n\r", 10);
			 flag4 = true;
			 flagtrue = false;
             frequencia = 50000000;
			 break;

        }
			
		}
		 while(flag2){
			ledsinterns(frequencia);
		if(HWREG(GPIO1_DATAIN) &=(1 << 28)){
			putString("30 MHz\n\r", 10);
            flag1 = true;
			flagtrue = false;
            frequencia = 30000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 16)){
            flagtrue = true;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 12)){
             putString("10 MHz\n\r", 10);
			 flag3 = true;
			 flagtrue = false;
             frequencia = 10000000;
			 break;


        }else if(HWREG(GPIO1_DATAIN) &= (1 << 14)){
             putString("50 MHz\n\r", 10);
			 flag4 = true;
			 flagtrue = false;
             frequencia = 50000000;
			 break;

        }
		}
		while(flag3){
			ledsinterns(frequencia);
		if(HWREG(GPIO1_DATAIN) &=(1 << 28)){
			putString("30 MHz\n\r", 10);
            flag1 = true;
			flagtrue = false;
            frequencia = 30000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 16)){
            putString("5 MHz\n\r", 9);
			flag2 = true;
			flagtrue = false;
            frequencia = 5000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 12)){
             flagtrue = true;
			 break;


        }else if(HWREG(GPIO1_DATAIN) &= (1 << 14)){
             putString("50 MHz\n\r", 10);
			 flag4 = true;
			 flagtrue = false;
             frequencia = 50000000;
			 break;

        }
		}
		while(flag4){
			ledsinterns(frequencia);
			if(HWREG(GPIO1_DATAIN) &=(1 << 28)){
			putString("30 MHz\n\r", 10);
            flag1 = true;
			flagtrue = false;
            frequencia = 30000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 16)){
            putString("5 MHz\n\r", 9);
			flag2 = true;
			flagtrue = false;
            frequencia = 5000000;
			break;

        }else if(HWREG(GPIO1_DATAIN) &= (1 << 12)){
             putString("10 MHz\n\r", 10);
			 flag3 = true;
			 flagtrue = false;
             frequencia = 10000000;
			 break;


        }else if(HWREG(GPIO1_DATAIN) &= (1 << 14)){
             flagtrue = true;
			 break;

          }
		}
  
  return(0);
}



