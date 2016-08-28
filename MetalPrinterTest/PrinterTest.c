/*
 * PrinterTest.c
 *
 *  Created on: Sep 6, 2013
 *      Author: dell
 */
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "MagAccComms.h"
#include "MagAcc.h"
#include "VariableMotor.h"
#include "BlueUART.h"
#include "Globals.h"
#include "MotorDrive.h"
#include "UserInterface.h"
#include "BlueUI.h"
#include "PIDController.h"
#include "HBridgeComms.h"
#include "AutoTunePID.h"
#include "PrinterTest.h"

#ifdef DEBUG
#define DEBUG_PRINT UARTprintf

#endif


char readCompass;
char centiTimerCounter;
char handleBlueTooth;
char desiredToggle;
char sendDesired;
char printerChange;



void initPrinterTest(){

	unsigned long ulPeriod=0;
	centiTimerCounter=0;
	handleBlueTooth=FALSE;
	desiredToggle=0;
	sendDesired=FALSE;
	readCompass=FALSE;
	//setup 100 times a second interupt on timer 4
		ROM_IntMasterDisable();
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
		ROM_SysCtlDelay(100);
		ROM_TimerConfigure(TIMER4_BASE, TIMER_CFG_32_BIT_PER);
		//set timer for 10 times a second
		ROM_SysCtlDelay(100);
		ulPeriod = (ROM_SysCtlClockGet() / 100);
		ROM_SysCtlDelay(100);
		ROM_TimerLoadSet(TIMER4_BASE, TIMER_A, ulPeriod - 1);
		ROM_SysCtlDelay(100);
		ROM_IntEnable(INT_TIMER4A);
		ROM_SysCtlDelay(100);
		ROM_TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
		ROM_SysCtlDelay(100);
		ROM_IntMasterEnable();
		ROM_TimerEnable(TIMER4_BASE, TIMER_A);
		ROM_SysCtlDelay(100);
		ROM_IntMasterDisable();

}

//100 times a second timer on
//Timer 4 A
void taskTimerCenti(void) {
	ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	if(++centiTimerCounter>99){
		centiTimerCounter=0;
	}
	if(centiTimerCounter%10==0){
		printerChange=TRUE;
	}

	//startCalcTspec=TRUE;

}
void runPrinterTest(){
	char countDownRow=10;
	char wireFeedRun=TRUE;
	int wireFeedSpeed=10;
	char printerRight=0;
	int i,j,k;
	initDroidComms();
	initBlueUart();
	initI2Ccomms();
	initMagAcc();
	initUI();
	initHBridgeComms();
	initAutoTune();
	initPID();
	initPrinterTest();
	initVariableMotor();
	initMotor();
	//start interupts
	ROM_IntMasterEnable();
	setCompass(getCompassReading(0));
	//sendHBridge();
	while (1) {
		if(printerChange){
			if(countDownRow--<0){
				countDownRow=10;
				//change direction
				if(printerRight){
					printerDir=RIGHT;
				}
				else{
					printerRight=LEFT;
				}
				wireFeedRun;
			}
			wireFeedSpeed+=4;
			if(wireFeedRun){
				wireFeedRun=FALSE;
			}
			else{
				wireFeedRun=TRUE;
			}
			motorDrive_WhileForever();
		}
		printerChange=FALSE;
	}
}

