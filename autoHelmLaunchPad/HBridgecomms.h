/*
 * HBridgecomms.h
 *
 *  Created on: Feb 10, 2013
 *      Author: dell
 */

#ifndef HBRIDGECOMMS_H_
#define HBRIDGECOMMS_H_

//variables
extern char sucessfulSPIRead;
extern unsigned long hBridgeRxWord;
extern unsigned short hBridgeStatus;
extern unsigned short icIdentifier;
extern char vddOverVoltage;
extern char vsUnderVoltage;
extern char temperatureWarning;
extern char overTemperatureShutdown;
extern char currentLimitationReached;
extern char overCurrentShutdown;
extern char currentLimitCount;

//methods
void initHBridgeComms(void);
void sendHBridge(void);
unsigned short swap_uint16( unsigned short val );

#endif /* HBRIDGECOMMS_H_ */
