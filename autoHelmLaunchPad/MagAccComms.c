//*****************************************************************************
//
// slave_receive_int.c - Example demonstrating a simple I2C master message
// transmission using a slave interrupt when data is received.
//
// Copyright (c) 2010-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9453 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "MagAccComms.h"
#include "Globals.h"
#include "driverlib/rom.h"
#include "Tester.h"

//*****************************************************************************
//
// Global variable to hold the I2C data that has been received.
//
//*****************************************************************************
//this is for storing data and keeping a count of how much data is stored
unsigned char data[12];
int magAccdataCount;
int dataMax;
//*****************************************************************************
//
// Global variable to handle multiple recieves from slave
//
//*****************************************************************************

char continueReceive;
char Send_I2C_error;
char Receive_I2C_error;
//*****************************************************************************
//
// The interrupt handler for the for I2C1 data slave interrupt.
//
//*****************************************************************************
void I2C1MasterIntHandler(void) {
	unsigned char err;
	//
	// Clear the I2C1 interrupt flag.
	//
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	ROM_I2CMasterIntClearEx(I2C1_MASTER_BASE,I2C_MASTER_INT_DATA | I2C_MASTER_INT_TIMEOUT);
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	err = ROM_I2CMasterErr(I2C1_MASTER_BASE);
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	if (I2C_MASTER_ERR_NONE == err) {
		//
		// Read the data from the slave.
		//
		data[magAccdataCount] = ROM_I2CMasterDataGet(I2C1_MASTER_BASE);
		magAccdataCount++;
		while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
		if (continueReceive) {
			if (dataMax > magAccdataCount) {
				ROM_I2CMasterControl(I2C1_MASTER_BASE,
						I2C_MASTER_CMD_BURST_RECEIVE_CONT);
			} else {
				ROM_I2CMasterControl(I2C1_MASTER_BASE,
						I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
				continueReceive = 0;
			}
		}
	} else if (err & I2C_MASTER_ERR_ARB_LOST) {
		//UARTprintf("Receive: ARB_LOST\n");
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
		Receive_I2C_error = 1;
	} else if (err & I2C_MASTER_ERR_ADDR_ACK || err & I2C_MASTER_ERR_DATA_ACK) {
		//UARTprintf("Receive: NO_ACK\n");
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
		Receive_I2C_error = 1;
	} else if (err & I2C_MASTER_ERR_CLK_TOUT ) {
		//UARTprintf("Receive: CLK_OUT\n");
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
		Receive_I2C_error = 1;
	}

}

//*****************************************************************************
//
// Configure the I2C1 master and slave and connect them using loopback mode.
//
//*****************************************************************************
void initI2Ccomms(void) {
	magAccdataCount = 0;
	dataMax = 0;
	continueReceive = 0;
	Send_I2C_error=0;
	Receive_I2C_error=0;
	ROM_IntMasterDisable();
	//
	// The I2C1 peripheral must be enabled before use.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	ROM_SysCtlDelay(100);
	//
	// For this example I2C1 is used with PORTA[3:2].  The actual port and
	// pins used may be different on your part, consult the data sheet for
	// more information.  GPIO port B needs to be enabled so these pins can
	// be used
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlDelay(100);
	ROM_IntMasterEnable();
	//
	// Configure the pin muxing for I2C1 functions on port B2 and B3.
	// This step is not necessary if your part does not support pin muxing
	//
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);


	ROM_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	ROM_GPIOPinConfigure(GPIO_PA7_I2C1SDA);

	//
	// Select the I2C function for these pins.  This function will also
	// configure the GPIO pins pins for I2C operation, setting them to
	// open-drain operation with weak pull-ups.  Consult the data sheet
	// to see which functions are allocated per pin.
	//
	//GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	//
	// Enable the I2C1 interrupt on the processor (NVIC).
	//
	ROM_IntEnable(INT_I2C1);

	//
	// Configure and turn on the I2C1 master interrupt.  The I2CMasterIntEnableEx()
	// gives you the ability to only enable specific interrupts.  For this case
	// we are only interrupting when the master device receives data.
	//
	I2CMasterTimeoutSet(I2C1_MASTER_BASE, 0x7d);
	I2CMasterIntEnableEx(I2C1_MASTER_BASE, I2C_MASTER_INT_DATA | I2C_MASTER_INT_TIMEOUT);

	//
	// Enable and initialize the I2C1 master module.  Use the system clock for
	// the I2C1 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.  For this example we will use a data rate of 100kbps.
	//
	ROM_I2CMasterInitExpClk(I2C1_MASTER_BASE, ROM_SysCtlClockGet(), true);
	ROM_SysCtlDelay(10000);
	UARTprintf("\n Beginning transmission \n");

	//data[0] = I2CMasterDataGet(I2C1_MASTER_BASE);
	//test for hm6352 i2c chip
//	MyI2C_send(0x21,'g' , FIRST_I2C);
//	MyI2C_send(0x21, 0x74, LAST_I2C);
//	MyI2C_requestFrom(0x21, SINGLE_RECEIVE_I2C, 1);
//	while(magAccdataCount<1);

	UARTprintf("\n Stellaris read on I2c Master: %d\n",data[0]);
	UARTprintf("\n I2C_1 initialized on a6 a7\n");
	ROM_IntMasterDisable();
}

void MyI2C_send(unsigned char address, unsigned char number, char sendType) {
	unsigned char err;
	compassReadTester=51;
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	//
	// Tell the master module what address it will place on the bus when
	// communicating with the slave.  Set the address to SLAVE_ADDRESS
	// (as set in the slave module).  The receive parameter is set to false
	// which indicates the I2C Master is initiating a writes to the slave.  If
	// true, that would indicate that the I2C Master is initiating reads from
	// the slave.
	//
	compassReadTester=52;
	ROM_I2CMasterSlaveAddrSet(I2C1_MASTER_BASE,  address, false);
	compassReadTester=53;
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	//
	// Place the data to be sent in the data register.
	//
	compassReadTester=54;
	ROM_I2CMasterDataPut(I2C1_MASTER_BASE,  number);
	compassReadTester=55;
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	compassReadTester=56;
	switch (sendType) {
	case SINGLE_I2C:
		ROM_I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
		break;
	case FIRST_I2C:
		ROM_I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		break;
	case LAST_I2C:
		ROM_I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
		break;
	}
	compassReadTester=57;
	//wait till master transmit complete
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	compassReadTester=61;
	//check for and handle error
	err = ROM_I2CMasterErr(I2C1_MASTER_BASE);
	compassReadTester=62;
	if (err & I2C_MASTER_ERR_ARB_LOST) {
		compassReadTester=63;
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
		Send_I2C_error = 1;
		UARTprintf("\n Error sending ARB lost reading: %d \n", address);
	} else if (err & I2C_MASTER_ERR_ADDR_ACK || err & I2C_MASTER_ERR_DATA_ACK) {
		compassReadTester=64;
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
		Send_I2C_error = 1;
		UARTprintf("\n Error sending NO ACK reading: %d \n", address);
	} else if (err & I2C_MASTER_ERR_CLK_TOUT) {
		compassReadTester=65;
		ROM_I2CMasterControl(I2C1_MASTER_BASE,
				I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
		Send_I2C_error = 1;
		UARTprintf("\n Error sending Timeout reading: %d \n", address);
	}
	compassReadTester=58;

}

void MyI2C_requestFrom(unsigned char address, char sendType, int count) {
	//restart position of data array for new recieves
	magAccdataCount = 0;
	dataMax = count;
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	//
	// Tell the master module what address it will place on the bus when
	// communicating with the slave.  Set the address to SLAVE_ADDRESS
	// (as set in the slave module).  The receive parameter is set to false
	// which indicates the I2C Master is initiating a writes to the slave.  If
	// true, that would indicate that the I2C Master is initiating reads from
	// the slave.
	//
	ROM_I2CMasterSlaveAddrSet(I2C1_MASTER_BASE,  address, true);
	while(ROM_I2CMasterBusy(I2C1_MASTER_BASE));
	switch (sendType) {
	case MULTI_RECEIVE_I2C:
		continueReceive = 1;
		ROM_I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
		break;
	case SINGLE_RECEIVE_I2C:
		continueReceive = 0;
		ROM_I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
		break;

	}


}
int MyI2C_available() {
	return magAccdataCount;
}
unsigned char MyI2C_receive(int pos) {
	return data[pos];

}
