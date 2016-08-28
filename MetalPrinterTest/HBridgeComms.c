#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "Globals.h"
#include "utils/uartstdio.h"
#include "HBridgecomms.h"
#include "MotorDrive.h"


//DEFINES
#ifdef DEBUG
#define DEBUG_PRINT UARTprintf
#endif

#define TIME_TO_DETERMINE_END_REACHED   6           //tenths of a second
#define ALLINOUTCOUNTDOWN 				8

#define DESIRED_BRIDGE_CONFIG			0xE			//0b0000000000001110
#define Open_Load_in_OFF				0x1			//0b0000000000000001
#define Open_Load_in_ON					0x2			//0b0000000000000010
#define Vs_undervoltage					0x4			//0b0000000000000100
#define Vdd_overvoltage					0x8			//0b0000000000001000
#define Current_Limitation_reached		0x10		//0b0000000000010000
#define Temperature_warning				0x20		//0b0000000000100000
#define Over_temperature_Shutdown		0x40		//0b0000000001000000
#define Bridge_enable					0x80		//0b0000000010000000
#define Over_Current_on_Low_Side1		0x100		//0b0000000100000000
#define Over_Current_on_Low_Side2		0x200		//0b0000001000000000
#define Over_Current_on_High_Side1		0x400		//0b0000010000000000
#define Over_Current_on_High_Side2		0x800		//0b0000100000000000
#define Short_to_GND_in_OFF				0x4000		//0b0100000000000000
#define Short_to_Battery_in_OFF			0x8000		//0b1000000000000000

#define Over_Current					0xF00		//0b0000111100000000

//VARIABLES
unsigned long hBridgeRxWord;
unsigned short icIdentifier;

char vddOverVoltage;
char vsUnderVoltage;
char temperatureWarning;
char overTemperatureShutdown;
char currentLimitationReached;
char overCurrentShutdown;
char sucessfulSPIRead;
char currentLimitCount;
//void onSSIReceive(void){
//	unsigned long ulStatus;
//
//    // Values that can be passed to SSIIntEnable, SSIIntDisable, and SSIIntClear
//    // as the ulIntFlags parameter, and returned by SSIIntStatus.
//	ulStatus = ROM_UARTIntStatus(SSI1_BASE, true);
//
//	//
//	// Clear the asserted interrupts.
//	//
//	ROM_UARTIntClear(SSI1_BASE, ulStatus);
//	if(ROM_SSIDataGetNonBlocking(SSI1_BASE, &hBridgeRxWord)){
//		hBridgeStatus=hBridgeRxWord|0;
//		sucessfulSPIRead=TRUE;
//	}
//	else{
//		sucessfulSPIRead=FALSE;
//	}
//}

void initHBridgeComms(void) {
	sucessfulSPIRead=FALSE;
	vddOverVoltage=FALSE;
	vsUnderVoltage=FALSE;
	temperatureWarning=FALSE;
	overTemperatureShutdown=FALSE;
	currentLimitationReached=FALSE;
	overCurrentShutdown=FALSE;
    currentLimitCount=0;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	ROM_SysCtlDelay(100);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlDelay(100);
//	GPIOPinTypeSSI(GPIO_PORTD_BASE,  SSI_CLK | SSI_CS | SSI_RX | SSI_TX );

	ROM_GPIOPinConfigure(GPIO_PD0_SSI1CLK); /* set pin mux */

	ROM_GPIOPinConfigure(GPIO_PD1_SSI1FSS); /* set pin mux */

	ROM_GPIOPinConfigure(GPIO_PD2_SSI1RX); /* set pin mux */

	ROM_GPIOPinConfigure(GPIO_PD3_SSI1TX); /* set pin mux */


	ROM_GPIOPinTypeSSI(GPIO_PORTD_BASE,
			( GPIO_PIN_3 | GPIO_PIN_2  | GPIO_PIN_1 | GPIO_PIN_0 ));

	//this tested correctly at ssi_frf_moto_mode_1
	ROM_SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1,
			SSI_MODE_MASTER, 50000, 16);
	ROM_SSIEnable(SSI1_BASE);
    //SSIIntEnable(SSI1_BASE, SSI_RXFF | SSI_RXTO | SSI_RXOR);
    hBridgeRxWord=0;
    icIdentifier=0;
    while(ROM_SSIDataGetNonBlocking(SSI1_BASE, &hBridgeRxWord))
    {
    }

}
void sendHBridge(){
	unsigned short hBridgeReading=0;
	//slave transmits lsb to msb see datasheet
	ROM_SSIDataPut(SSI1_BASE,swap_uint16((unsigned int)(DESIRED_BRIDGE_CONFIG)));
    while(ROM_SSIBusy(SSI1_BASE))
    {
    }
	//slave transmits lsb to msb see datasheet
    //micro controller receives msb first to lsb
    //this means the binary is the reverse of what it should be look at datasheet
    ROM_SSIDataGet(SSI1_BASE, &hBridgeRxWord);
    hBridgeReading=swap_uint16((unsigned short)(hBridgeRxWord&0xFFFF));
	if(!icIdentifier){
		icIdentifier=hBridgeReading;
	}
	//In case of "DR" set to HIGH, the over-current,
	//Vdd over-voltage and over-temperature
	//diagnostic bits can NOT be reset by SPI read
	//and therefore, the bridge is kept in tri-state
	//until a transition from "Disable" to "Enable"
	//on DI/EN pins or Power-on-Reset condition
	vddOverVoltage=Vdd_overvoltage & hBridgeReading;
	vsUnderVoltage=Vs_undervoltage&hBridgeReading;
	temperatureWarning=Temperature_warning&hBridgeReading;
	overTemperatureShutdown=Over_temperature_Shutdown&hBridgeReading;
	currentLimitationReached=Current_Limitation_reached&hBridgeReading;
	overCurrentShutdown=Over_Current&hBridgeReading;
    if(currentLimitationReached){
        currentLimitCount++;
        if(currentLimitCount>=TIME_TO_DETERMINE_END_REACHED){
		    if (motorState == DRIVING_IN) {
		        shaftPosition = ALLIN;
            }
            else{
                shaftPosition = ALLOUT;
            }
	        allInOutCount=ALLINOUTCOUNTDOWN;
            currentLimitCount=0;
        }
    }
    else{
    	currentLimitCount=0;
    }

	//DEBUG_PRINT("The SPI read: %d\n",icIdentifier);
}
//! Byte swap unsigned short
unsigned short swap_uint16( unsigned short val )
{
    unsigned short temp;
    char i=0;
    for(i=0;i<16;i++){
    	if(val&0x1){
    		temp=temp|0x1;
     	}
    	if(i<15){
			temp=temp<<1;
			val=val>>1;
    	}
    }
	return temp;
}
