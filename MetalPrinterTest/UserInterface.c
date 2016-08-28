/*
 * UserInterface.c
 *	this is for handling all the buttons and display leds
 *  Created on: Dec 30, 2012
 *      Author: dell
 */
#include "Globals.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "Globals.h"
#include "MotorDrive.h"
#include "HBridgecomms.h"
#define LED_ON 1
#define LED_OFF 0

char buttonCount;

void initUI(void) {
	unsigned long ulPeriod;
	adjustShaft = ADJUST_STOP;
	buttonCount = 0;
	autoHelmState = PAUSE;
	lastautoHelmState = autoHelmState;
	toggleLED = LED_OFF;
	ROM_IntMasterDisable();
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);
	//setup B input for buttons and interupts
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
	ROM_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD_WPU);
	ROM_GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5,
			GPIO_LOW_LEVEL);
	ROM_GPIOPinIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
	ROM_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_OD_WPU);
	ROM_GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);

	ROM_SysCtlDelay(100);
	ROM_IntEnable(INT_GPIOB);
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
	ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);

	//setup E input for button and interupt
//	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
//	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4,
//			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD_WPU);
//	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL);
//	GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);
//	IntEnable(INT_GPIOE);

//setup LED display interupt for 4 times a second
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
	ulPeriod = (ROM_SysCtlClockGet() / 4);
	ROM_TimerLoadSet(TIMER2_BASE, TIMER_A, ulPeriod - 1);
	ROM_IntEnable(INT_TIMER2A);
	ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	ROM_IntMasterEnable();
	//enable led display timer
	ROM_TimerEnable(TIMER2_BASE, TIMER_A);
	ROM_IntMasterDisable();

}

void turnGreenLEDOn(void) {
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 255);
}
void turnGreenLEDOff(void) {
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
}
void turnYellowLEDOn(void) {
	ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 255);
}
void turnYellowLEDOff(void) {
	ROM_GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);

}

void bButtonsPressed(void) {
	//PB0 Pause PB5 middle PB1 oppisite of PAUSE
	ROM_GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
	ROM_IntDisable(INT_GPIOB);
	buttonCount = 1;
	switch (autoHelmState) {
	case PAUSE:
		//if the two direction buttons pressed
		if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5)
				&& !ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) {
			autoHelmState = SELECT_TACK_DIRECTION;
		}
		//if pause button pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) {
			heading.desired=heading.current;
			autoHelmState = AUTOPILOT;
		}
		//if direction toward pause pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5)) {
			adjustShaft = ADJUST_IN;
			buttonCount = 2;
		}
		//if direction away from pause pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) {
			adjustShaft = ADJUST_OUT;
			buttonCount = 2;
		}
		break;
	case AUTOPILOT:
		//if direction toward pause pressed
		if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5)) {
			changeDesired(-1); //change by 1 degree
			turnGreenLEDOff();
		}
		//if pause pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) {
			autoHelmState = PAUSE;
		}
		//if direction away from pause pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) {
			changeDesired(1); //change by one degree
			turnGreenLEDOff();
		}
		break;
	case SELECT_TACK_DIRECTION:
		//if pause and the direction toward pause pressed
		if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5)
				&& !ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) {
			autoHelmState = TACK_PORT;
			buttonCount = 10;
		}
		//if pause and the direction away from pause pressed
		else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)
				&& !ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) {
			autoHelmState = TACK_STARBOARD;
			buttonCount = 10;
		//if pause pressed
		} else if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)) {
			autoHelmState = PAUSE;
		}
		break;
	case TACK_STARBOARD:
	case TACK_PORT:
		//if any button is pressed
		if (!ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0)
				|| !ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_5)
				|| !ROM_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1)) {
			autoHelmState = PAUSE;
		}
		break;
	}
}
//void eButtonsPressed(void) {
//	GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
//	}

void displayLEDTimer(void) {
	ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	//TODO: change this debounce to a faster timer for better human interaction
	if (buttonCount-- < 1) {
		ROM_GPIOPinIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
		ROM_IntEnable(INT_GPIOB);
		buttonCount = 0;
		if(adjustShaft != ADJUST_STOP){
			adjustShaft = ADJUST_STOP;
		}
	}
	if(reverseMotorCount>0){
		reverseMotorCount--;
		if(reverseMotorCount==0){
			setMotorDirection(reverseMotorDir);
			turnOnMotor();
		}
	}
	if(allInOutCount>0){
		allInOutCount--;
		if(allInOutCount==0){
			shaftPosition=MIDDLE;
			currentLimitCount=0;//this is count of current limits hit Before ALLIN or ALLOUT
		}
	}
//	if(--lastMotorDriven<0){
//		lastMotorDriven=0;
//	}
	switch (autoHelmState) {
	case PAUSE:
		turnGreenLEDOff();
		turnYellowLEDOn();
		break;
	case AUTOPILOT:
		turnGreenLEDOn();
		turnYellowLEDOff();
		break;
	case SELECT_TACK_DIRECTION:
		if (toggleLED) {
			turnGreenLEDOff();
			turnYellowLEDOn();
			toggleLED = LED_OFF;
		} else {
			turnGreenLEDOn();
			turnYellowLEDOff();
			toggleLED = LED_ON;
		}
		break;
	case TACK_STARBOARD:
	case TACK_PORT:
		if (toggleLED) {
			turnGreenLEDOn();
			turnYellowLEDOn();
			toggleLED = LED_OFF;
		} else {
			turnGreenLEDOff();
			turnYellowLEDOff();
			toggleLED = LED_ON;
		}
		break;
	}

}
//used by bluetooth android comms to signal desired heading changed
void signalReceiveDesired(void){
	turnGreenLEDOff();
}

