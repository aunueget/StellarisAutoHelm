#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "Globals.h"
#include "Tester.h"
#include "AutoHelm.h"

#ifdef DEBUG
#define DEBUG_PRINT UARTprintf
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
int PinData=2;
void initDebugUart(void);

int main(void)
{
    ROM_FPULazyStackingEnable();
    ROM_FPUEnable();
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//address fault if not enabled
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	initDebugUart();
	ROM_IntMasterEnable();
	DEBUG_PRINT("ClockSpeed %i\n",ROM_SysCtlClockGet());
	//bluetooth uart and compass i2c
//	initBlueUart();
//	initI2Ccomms();
//	initVariableMotor(); //for motor driving
//	initMagAcc();
//	initMotor();
//	initUI();
	initGlobal();
//	ROM_IntMasterEnable();
	DEBUG_PRINT("ClockSpeed %i\n",ROM_SysCtlClockGet());
	ROM_IntMasterDisable();
	//testFloat2String();
	//testHbridgeComms();
	//testBitOper();
	//testRandom();
	//getCompassForCalibrate();
	//testLEDDisplay();
	//testI2Ccompass();
	//testI2CBlueUart();
	//testI2CBlueUartWithUI();
	//testAllAndroidCommsWithUI();
	//testAutoTuneClocks();
	//testDriveMotorOneWay();
	//testDriveMotor();
	//testVariableMotor();
//	ROM_IntMasterDisable();
//	testVariableMotorWithAllComms();
//	ROM_IntMasterDisable();
//	testBigDigits2String();
//	testAndroidcommsWithWarningsNKeytrys();
//	testAllInAllOutWithAllComms();
//	runAutoHelm();
//	testGetMedian();
//	getCompassForCalibrate();
//	testAddKpid();
//	testFindInsert();
//	testInListAlreadyAt();
//	testSelectNextKpid();
//	testCurrentTestKpidFAILED();
//	testModPID_Task();
	runPrintTest();
}

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void initDebugUart(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    //
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Select the alternate (UART) function for these pins.
    //
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
	UARTStdioInit(0);
}
