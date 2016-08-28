#include "VariableMotor.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "Globals.h"
#include "PIDController.h"
#include "MotorDrive.h"
#include "Tester.h"


#define PID_TO_MOTOR_DIVISOR 10.0f//20.0f
float pid2MotorDivisor;
char toggleMotorOn;
void initVariableMotor(void) {
	unsigned long ulPeriod;
	pid2MotorDivisor=PID_TO_MOTOR_DIVISOR;
	toggleMotorOn=FALSE;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlDelay(100);
	//E3->PWM
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
	//SET ALL TO ZERO motor off
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);

	ROM_IntMasterDisable();
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlDelay(100);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);

	ulPeriod = (ROM_SysCtlClockGet() / 100);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod - 1);


	ROM_IntEnable(INT_TIMER0A);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	ROM_SysCtlDelay(100);
	ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);

	ulPeriod = (ROM_SysCtlClockGet() / 10000);
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ulPeriod - 1);
	ROM_IntEnable(INT_TIMER1A);
	ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

}
void startMotorToggle() {
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	toggleMotorOn=TRUE;
}
void stopMotorToggle() {
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	toggleMotorOn=FALSE;

}

/*****************************************************************************
 Name:      INTERRUPT HANDLER, rtc_ticker.
 Parameters:    none
 Returns:       none
 Description:   Decrements c_ticks for a count down

 *****************************************************************************/
//void delay_ticker(void) {
//	if (c_ticks)
//		c_ticks--;
//
//}

/*****************************************************************************
 Name:           shaftDelayToggle
 Parameters:     none
 Returns:        none
 Description:    this helps create an intermitent shaft movement by waiting the
 aloted moveDelay time with the shaft moving and then
 spending the same moveDelay time without moving the shaft
 *****************************************************************************/
void motorToggle() {
	unsigned long ulPeriod;
	int pidOut=(abs((int)(PID.out)/(pid2MotorDivisor)));//30.0f//100.0f
	//TODO:not working at 20.0f yet only at 30.0f and up
	// Clear the timer interrupt
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if (pidOut > 9) {
		//turn on motor pwm ->1
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 255);
//		lastMotorDriven=240;
		if (pidOut < 100) {
			ulPeriod = (ROM_SysCtlClockGet() / 10000) * pidOut;
			ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ulPeriod - 1);
			ROM_TimerEnable(TIMER1_BASE, TIMER_A);
		}
	} else {
		//turn off motor pwm ->0
		ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
//		lastMotorDriven=REVERSE_MOTOR_COUNTDOWN;
	}
}
void motorKill(void) {
	// Clear the timer interrupt
	ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	//turn off motor pwm ->0
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
//	lastMotorDriven=REVERSE_MOTOR_COUNTDOWN;
}
void turnOffPWM(){
	//turn off motor pwm ->0
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
//	lastMotorDriven=REVERSE_MOTOR_COUNTDOWN;
}
void turnOnPWM(){
	//turn on motor pwm ->1
	ROM_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 255);
//	lastMotorDriven=240;
}
