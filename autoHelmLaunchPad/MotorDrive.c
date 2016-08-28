/*
 * MotorDrive.c
 *
 *  Created on: Dec 22, 2012
 *      Author: Andrew McKnight
 *
 *
 *
 */
#include "Globals.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "MotorDrive.h"
#include "driverlib/rom.h"
#include "BlueUART.h"
#include "PIDController.h"
#include "VariableMotor.h"
#include "HBridgecomms.h"
#include "Tester.h"


char reverseMotorCount;
//short int lastMotorDriven;
char reverseMotorDir;
char allInOutCount;
void initMotor(void) {
	reverseMotorCount=0;
	allInOutCount=0;
//	lastMotorDriven=0;
	reverseMotorDir=FALSE;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlDelay(100);
	//E1->DIR->DIRECTION,E2->DI->DISABLE,E3->PWM
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2);


	//initialize all pins to zero
	//SET ENABLE->E2 TO 0 FOR NOT ENABLE AND E1 DIRECTION TO ZERO FOR DRIVE OUT
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 , 0);

	//SET ENABLE->E2 TO 1 FOR ON/ENABLE
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2 , 255);


	motorState = DRIVING_OUT;
}
//drive the motor desired direction
//dir is the direction should be desired direction
//checkPosition is the position to be sure doesn't exsist before going desired direction
//so for example dir driveOut checkPos AllOUT make sure not all out before driving out
char driveMotor(char dir,char checkPosition){
	//make sure not all out before driving out or all in before drive in
	if(shaftPosition != checkPosition){
		//check to be sure that we havn't switched directions
		if (motorState != dir){// && lastMotorDriven!=0) {
			turnOffMotor();
			reverseMotorDir=dir;
			if(reverseMotorCount<=0){
				reverseMotorCount=REVERSE_MOTOR_COUNTDOWN;
			}
			return 0;
		}
		setMotorDirection(dir);
		turnOnMotor();
	}
	else{
		turnOffMotor();
	}
	return 0;
}

void setMotorDirection(char dir){
	motorTester=4;
	if(dir==DRIVING_IN){
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 255);
		motorState = DRIVING_IN;
	}
	else{
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
		motorState = DRIVING_OUT;
	}
	if(shaftPosition!=MIDDLE){
		shaftPosition=MIDDLE;
		currentLimitCount=0;//this is count of current limits hit Before ALLIN or ALLOUT
	}
}
void turnOffMotor(void) {
	motorTester=5;
	//this will stop the timer that turns PWM on and off
	stopMotorToggle();
	//this will turn off the pwm if its high setting it to low
	turnOffPWM();
}
void turnOnMotor(void) {
	motorTester=6;
	startMotorToggle();
}
void check4ShaftNOut() {
	motorTester=7;
	if (shaftPosition == ALLIN) {
		if (motorState == DRIVING_IN) {
			turnOffMotor();
		}
	} else if (shaftPosition == ALLOUT) {
		if (motorState == DRIVING_OUT) {
			turnOffMotor();
		}
	}
}

void motorDrive_WhileForever(void) {
	motorTester=9;
	switch (autoHelmState) {
	case PAUSE:
		runPID=FALSE;
		if (adjustShaft == ADJUST_IN) {
			driveMotor(DRIVING_IN,ALLIN);
			PID.out=50*110;
		} else if (adjustShaft == ADJUST_OUT) {
			driveMotor(DRIVING_OUT,ALLOUT);
			PID.out=50*110;
		} else {
			if(toggleMotorOn){
				turnOffMotor();
			}
		}
		break;
	case AUTOPILOT:
		runPID=TRUE;
		if(PID.out<0){
			driveMotor(DRIVING_IN,ALLIN);
		}
		else if(PID.out>0){
			driveMotor(DRIVING_OUT,ALLOUT);
		}
		break;
	case SELECT_TACK_DIRECTION:
		runPID=FALSE;
		turnOffMotor();
		break;
	case TACK_STARBOARD:
		runPID=FALSE;
		motorTester=8;
		turnOffMotor();
		driveMotor(DRIVING_OUT,ALLOUT);
		if(!tacking){
			changeDesired(90); //tack to other side of the wind
		}
		tacking=TRUE;
		PID.out = 130*50;
		if(degreeDifferancef(1, heading.current, (float)heading.desired) < 35) {
			autoHelmState = AUTOPILOT;
			tacking=FALSE;
			runPID=TRUE;
		}
		break;
	case TACK_PORT:
		runPID=FALSE;
		tacking=TRUE;
		turnOffMotor();
		driveMotor(DRIVING_IN,ALLIN);
		if(!tacking){
			changeDesired(-90); //tack to the other side of the wind
		}
		tacking=TRUE;
		if(degreeDifferancef(1, heading.current, (float)heading.desired) < 35) {
			autoHelmState = AUTOPILOT;
			tacking=FALSE;
			runPID=TRUE;
		}
		break;
	}
	sendHBridge();
	if(	vddOverVoltage||vsUnderVoltage
			||overTemperatureShutdown||overCurrentShutdown){
		autoHelmState=PAUSE;
	}
	check4ShaftNOut();
}
void resetFailedHBridge(){
	//SET ENABLE->E2 TO 0 FOR off/DISABLE
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2 , 0);
	ROM_SysCtlDelay(100);
	//SET ENABLE->E2 TO 1 FOR ON/ENABLE
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2 , 255);

}

