		/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													1000000
#define TOGGLE          										(0x01u)

#define CM_PER_GPIO2                                            0XB0
#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE   				(0x2u)
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK   			(0x00040000u)

#define CM_conf_gpmc_ben1      	 								0x0878
#define CM_conf_gpmc_a5         								0x0854
#define CM_conf_gpmc_a6         								0x0858
#define CM_conf_gpmc_a7         								0x081C
#define CM_conf_gpmc_a8         								0x0820
#define CM_conf_gpmc_ad12         								0x0830
#define CM_conf_gpmc_a0                                         0x0840
#define CM_conf_gpmc_clk                                        0x088C





#define GPIO_DATAIN                                             0x138
#define GPIO_OE                 								0x134
#define GPIO_CLEARDATAOUT       								0x190
#define GPIO_SETDATAOUT         								0x194


#define  WDT_BASE 								0x44E35000
#define  WDT_WSPR 								0x44E35048
#define  WDT_WWPS 								0x44E35034

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 *
 *
 *
 */






void disableWdt(void){
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));

	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}



void Delai(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}

void buttonleds2(){
        HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<28;
        Delai();
        HWREG(SOC_GPIO_2_REGS+GPIO_SETDATAOUT) = 1<<1;
        Delai();
        HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = 1<<28;
        Delai();
        HWREG(SOC_GPIO_2_REGS+GPIO_CLEARDATAOUT) = 1<<1;
    }


void buttonleds1(){
        HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<21;
        Delai();
        HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<22;
        Delai();
        HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<23;
        Delai();
        HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<24;
        Delai();
        Delai();
        HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = 1<<21;
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = 1<<22;
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = 1<<23;
		HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = 1<<24;
    }
int _main(void){
	// ini flag for blink LED

  	unsigned int val_temp;
    unsigned int val_temptwo;


	// Configure the green LED control pin
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
    HWREG(SOC_CM_PER_REGS+CM_PER_GPIO2) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
 	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a6) |= 7;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a5) |= 7;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a7) |= 7;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a8) |= 7;
    HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_ad12) |= 7;
    HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a0) |= 7;
    HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_clk) |= 7;
    HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_ben1) |= 7;






	/*-----------------------------------------------------------------------------
	 *  set pin direction
	 *-----------------------------------------------------------------------------*/
	val_temp = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
	val_temp &= ~(1<<21);
	val_temp &= ~(1<<22);
	val_temp &= ~(1<<23);
	val_temp &= ~(1<<24);
	val_temp &= ~(1<<28);
    val_temp |= (1 << 12);
    val_temp |= (1 << 16);


    val_temptwo = HWREG(SOC_GPIO_2_REGS+GPIO_OE);
    val_temptwo &= ~(1<<1);

    HWREG(SOC_GPIO_2_REGS+GPIO_OE) = val_temptwo;



    bool flagzin = true;





	HWREG(SOC_GPIO_1_REGS+GPIO_OE) = val_temp;

    int state;

  	while (1){
    	// Change the state of the green LED.
        disableWdt();

        while(flagzin){
            HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<21;
			HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<22;
			HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<23;
			HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = 1<<24;


            HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<12);
            HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<16);



            if(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<12)){
                flagzin = false;
                state = 0;
                break;
            }

            if(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<16)){
                flagzin = false;
                state = 1;
                break;
            }
       }

        if(!flagzin){
            if(state){
                HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<12);
                HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<16);
                buttonleds1();
                if((HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<16))){
                    flagzin = true;
                }

                if(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<12)){
                    state = 0;
                }
            }else{
                HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<12);
                HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (0<<16);
                buttonleds2();
                if((HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<12))){
                    flagzin = true;
                }

                if(HWREG(SOC_GPIO_1_REGS + GPIO_DATAIN) &= (1<<16)){
                    state = 0;
                }


            }
        }

	}

	return(0);
} /* ----------  end of function main  ---------- */

