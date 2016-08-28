/*
 * BlueUART.c
 *
 *  Created on: Dec 24, 2012
 *      Author: dell
 */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "VariableMotor.h"
#include "BlueUI.h"
#include "Globals.h"

#define BLUE_UART_BAUDRATE 115200
void blueUartRecieve(void) {
	unsigned long ulStatus;
	char currCh;
	//
	// Get the interrrupt status.
	//
	ulStatus = ROM_UARTIntStatus(UART3_BASE, true);

	//
	// Clear the asserted interrupts.
	//
	ROM_UARTIntClear(UART3_BASE, ulStatus);

	//
	// Loop while there are characters in the receive FIFO.
	//
	while (ROM_UARTCharsAvail(UART3_BASE)) {
		//
		// Read the next character from the UART and write it back to the UART.
		//
		currCh=ROM_UARTCharGetNonBlocking(UART3_BASE);
		inFromDroid[droidCharCount++]=currCh;
		stopBlueRead=FALSE;
		if(droidCharCount>29){
			droidCharCount=0;
		}
		if(droidCharCount==readDroidPosition){
			readDroidPosition=droidCharCount+1;
		}

	}

}

void initBlueUart(void) {

	char dummy;
	ROM_IntMasterDisable();
	//
	//
	// Enable GPIO port C which is used for UART3 pins.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlDelay(100);
	ROM_IntMasterEnable();

	ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
	ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
	ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	//
	// Configure the UART for 115,200, 8-N-1 operation.
	//
	ROM_UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
//    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
//                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
//                             UART_CONFIG_PAR_NONE));

	//
	// Set interrupt for (UART) function for these pins.
	//
	ROM_IntEnable(INT_UART3);
	ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
	//
	// Initialize the UART for console I/O.
	//
	dummy = UARTCharGetNonBlocking(UART3_BASE);
	ROM_IntMasterDisable();
}

void sendToBlueTooth(char * message) {
	int next = 0;
	char ch = message[next];
	if (ch != '\0') {
		while (ch != '\0') {
			if(UARTCharPutNonBlocking(UART3_BASE, ch)){
				ch = message[++next];
			}
		}
		//UTF-8 linefeed is 0xA --  00001010b
		UARTCharPutNonBlocking(UART3_BASE, 0xA);

	}
}
