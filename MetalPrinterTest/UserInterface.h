/*
 * UserInterface.h
 *
 *  Created on: Dec 31, 2012
 *      Author: dell
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

extern char buttonCount;

void initUI(void);

void turnGreenLEDOn(void);
void turnGreenLEDOff(void);
void turnYellowLEDOn(void);
void turnYellowLEDOff(void);
void signalReceiveDesired(void);

#endif /* USERINTERFACE_H_ */
