/*
 * BlueUI.h
 *
 *  Created on: Jan 4, 2013
 *      Author: dell
 */

#ifndef BLUEUI_H_
#define BLUEUI_H_

extern char droidCharCount;
extern char stopBlueRead;
extern char readDroidPosition;
extern char inFromDroid[];
extern char desiredHeadingStr[];
extern char blueStr[];
extern char sendOnceShaftAtEnd;
extern char sendInitBluart;
extern char sendAutoTuneData;
extern char sendPopUp;

void initDroidComms(void);
void statusToBlueTooth_WhileForever();
void handleCommsFromDroid();
int isDigit(char ch);
int getCharLeftCount(char currPosition);
int getPositiveDigitNumber(int currPosition, int length);
char handleLoopAt29(char num);
int getSetting(void);
void sendPopUpMessage(char * message,int value);

#endif /* BLUEUI_H_ */
