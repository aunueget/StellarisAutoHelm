/*
 * BlueUI.c
 *
 *  Created on: Jan 4, 2013
 *      Author: dell
 */

#include "GLobals.h"
#include "BlueUART.h"
#include "BlueUI.h"
#include "MotorDrive.h"
#include "UserInterface.h"
#include "MagAcc.h"
#include "utils/uartstdio.h"
#include "AutoHelm.h"
#include "HBridgecomms.h"
#include "PIDController.h"
#include "VariableMotor.h"
#include "AutoTunePID.h"
#include "Klist.h"

#include <string.h>

#ifdef DEBUG
#define DEBUG_PRINT UARTprintf

#endif
#define SEND_ONCE 1
#define DONT_SEND 0
#define FIRST_CHAR_AFTER_PERCENT 1
#define FIRST_DIGIT_OF_SETTING 2

char sendInitBluart;
char sendOnceShaftAtEnd;
char inFromDroid[30];
char droidCharCount;
char readDroidPosition;
char keysTriedCountDown;
char sendAutoTuneData;
char blueStr[50];
char sendPopUp;

//char desiredHeadingStr[5];
char stopBlueRead;

void initDroidComms(void){
	droidCharCount=0;
	readDroidPosition=0;
	keysTriedCountDown=0;
    sendAutoTuneData=FALSE;
//	desiredHeadingStr[0]='@';
	stopBlueRead=TRUE;
	sendInitBluart=TRUE;
	sendPopUp=FALSE;
}
void statusToBlueTooth_WhileForever() {
	char headingStr[5] = "@    ";
	char desiredStr[5] = "$    ";
	char keystriedStr[8]="&       ";
	if (sendInitBluart || lastautoHelmState != autoHelmState) {
		switch (autoHelmState) {
		case PAUSE:
			//DEBUG_PRINT("%s\n","PAUSED");
			sendToBlueTooth("PP\0");
			break;
		case AUTOPILOT:
			//DEBUG_PRINT("%s\n","AUTO_ON");
			sendToBlueTooth("**\0");
			break;
//		case SELECT_TACK_DIRECTION:
//			DEBUG_PRINT("SelectTack \n");
//			sendToBlueTooth("SelectTack \n\0");
//			break;
		case TACK_STARBOARD:
			//DEBUG_PRINT("%s\n","TACK_STARBOARD");
			sendToBlueTooth("++\0");
			break;
		case TACK_PORT:
			//DEBUG_PRINT("%s\n","TACK_PORT");
			sendToBlueTooth("--\0");
			break;
		}
		lastautoHelmState=autoHelmState;

	} else if (shaftPosition == ALLIN) {
		if(sendOnceShaftAtEnd){
			//DEBUG_PRINT("ALLIN \n");
			sendToBlueTooth("#N\0");
			sendOnceShaftAtEnd=DONT_SEND;
		}
	} else if (shaftPosition == ALLOUT) {
		if(sendOnceShaftAtEnd){
			//DEBUG_PRINT("ALLOUT \n");
			sendToBlueTooth("#O\0");
			sendOnceShaftAtEnd=DONT_SEND;
		}
	} else {
		switch (autoHelmState) {
		case AUTOPILOT:
			if (heading.lastDesired != heading.desired) {
				digits2String(heading.desired,desiredStr);
				//DEBUG_PRINT(desiredHeadingStr);
				sendToBlueTooth(desiredStr);
				heading.lastDesired=heading.desired;
			}
			break;
		}
		if(!sendOnceShaftAtEnd){
			//DEBUG_PRINT("SHAFT_MIDDLE \n\0");
			//sendToBlueTooth("SHAFT_MIDDLE \n\0");
			sendOnceShaftAtEnd=SEND_ONCE;
		}
	}
	if (!sendDesired) {
		testState=12;
		//DEBUG_PRINT("Current %03i\n", (int)heading.current);
		testState=10;
		digits2String((int)heading.current, headingStr);
		testState=11;
		//DEBUG_PRINT("Current %03i\n", heading.desired);
		testState=8;
		sendToBlueTooth(headingStr);
	}
	if(sendInitBluart || sendDesired){
		//DEBUG_PRINT("Desired %03i\n", heading.desired);
		digits2String(heading.desired, desiredStr);
		testState=9;
		sendToBlueTooth(desiredStr);
	}
	sendInitBluart=FALSE;
	if(vddOverVoltage){
		sendToBlueTooth("!VO\0");
	}
	if(vsUnderVoltage){
		sendToBlueTooth("!VU\0");
	}
	if(temperatureWarning){
		sendToBlueTooth("!TW\0");
	}
	if(overTemperatureShutdown){
		sendToBlueTooth("!TS\0");
	}
	if(currentLimitationReached){
		sendToBlueTooth("!CL\0");
	}
	if(overCurrentShutdown){
		sendToBlueTooth("!CS\0");
	}
//	if(kSetsTryed>lastkSetsTryed){
//		if(keysTriedCountDown<=0){
//			bigDigits2String(kSetsTryed,keystriedStr);
//			sendToBlueTooth(keystriedStr);
//			lastkSetsTryed=kSetsTryed;
//			keysTriedCountDown=10;
//		}
//	}
//	if(keysTriedCountDown>0){
//		keysTriedCountDown--;
//	}
    if(sendAutoTuneData){
        int blueLen=0;
        char kpStr[5] = "pxxxx";
        char kiStr[5] = "ixxxx";
        char kdStr[8] = "dxxxxxxx";
        char kpNegStr[5] = "Pxxxx";
        char kiNegStr[5] = "Ixxxx";
        char kdNegStr[8] = "Dxxxxxxx";
        digits2String(PID.posKp,kpStr);
        digits2String(PID.posKi,kiStr);
        bigDigits2String(PID.posKd,kdStr);
        bigDigits2String(PID.posID,keystriedStr);
        digits2String(PID.negKp,kpNegStr);
        digits2String(PID.negKi,kiNegStr);
        bigDigits2String(PID.negKd,kdNegStr);
        blueStr[0]='%';
        blueStr[1]='\0';
        strcat(blueStr,keystriedStr);
        strcat(blueStr,kpStr);
        strcat(blueStr,kiStr);
        strcat(blueStr,kdStr);
        strcat(blueStr,kpNegStr);
        strcat(blueStr,kiStr);
        strcat(blueStr,kdNegStr);
        blueLen=strlen(blueStr);
        blueStr[blueLen]=';';
        blueStr[blueLen+1]='\0';
    	sendToBlueTooth(blueStr);        
        sendAutoTuneData=FALSE;
    }
    else if(sendPopUp){
    	sendToBlueTooth(blueStr);
    	sendPopUp=FALSE;
    }

}

void sendPopUpMessage(char * message,int value){
	char smallValue[5] = ":xxxx";
	char bigValue[8] = ":xxxxxxx";
	char blueLen=0;
	blueStr[0]='%';
    blueStr[1]='\0';
    strcat(blueStr,message);
	if(value>999){
        bigDigits2String(value,bigValue);
        strcat(blueStr,bigValue);
	}
	else{
        digits2String(value,smallValue);
        strcat(blueStr,smallValue);
	}
    blueLen=strlen(blueStr);
    blueStr[blueLen]=';';
    blueStr[blueLen+1]='\0';
    sendPopUp=TRUE;
}


void handleCommsFromDroid(){
	int num3Digit=0;
	readDroidPosition=handleLoopAt29(readDroidPosition);
	if(readDroidPosition==droidCharCount){
		stopBlueRead=TRUE;
	}
	if(!stopBlueRead){
		switch(inFromDroid[readDroidPosition]){
		case '@':
			num3Digit=getPositiveDigitNumber(handleLoopAt29(readDroidPosition+1),3);
			if(num3Digit!=-1 && num3Digit>=0 && num3Digit<360){
				setDesired(num3Digit);
				signalReceiveDesired();
			}
			break;
		case '+':
//			num3Digit=get3DigitNumber();
//			if(num3Digit!=-1){
				autoHelmState = TACK_STARBOARD;
//				setDesired(num3Digit);
//			}
			break;
		case '-':
//			num3Digit=get3DigitNumber();
//			if(num3Digit!=-1){
				autoHelmState = TACK_PORT;
//				setDesired(num3Digit);
//			}
			break;
		case 'P':
			autoHelmState = PAUSE;
			break;
		case '*':
			autoHelmState =AUTOPILOT;
			break;
		case '!':
			sendInitBluart=TRUE;
			break;
        case '&':
            sendAutoTuneData=TRUE;
            break;
		case '%':
			readDroidPosition=handleLoopAt29(readDroidPosition+getSetting());
			break;
		}
		readDroidPosition++;
	}
}
int getSetting(){
	int newSetting=-1;
	//TODO: this could break if its still reading and hasnt read the ';' before it trys to
	//  		read the entire message
	int currPos=FIRST_DIGIT_OF_SETTING;
	while(getCharLeftCount(readDroidPosition)>=currPos&&inFromDroid[readDroidPosition+currPos]!=';'){
		if(isDigit(inFromDroid[readDroidPosition+currPos])){
			currPos++;
		}
		else{
			return 0;
		}
	}
	if(inFromDroid[readDroidPosition+currPos]==';'){
		currPos--;
		newSetting=getPositiveDigitNumber(readDroidPosition+FIRST_DIGIT_OF_SETTING,currPos-FIRST_CHAR_AFTER_PERCENT);
		if(newSetting!=-1){
			switch(inFromDroid[readDroidPosition+FIRST_CHAR_AFTER_PERCENT]){
			case 'M':
				pid2MotorDivisor=(float)newSetting;
				sendPopUpMessage("MotoDiv",(int)pid2MotorDivisor);
				break;
			case 'N':
				sigma=(float)newSetting;
				sendPopUpMessage("TuneNoise",(int)sigma);
				break;
			case 'T':
				allowedTolerance=(char)newSetting;
				sendPopUpMessage("Tolernce",(int)allowedTolerance);
				break;
			case 'D':
				derivativeSmooth=(float)(((float)newSetting)/100.0f);
				sendPopUpMessage("DervSm",(int)(derivativeSmooth*100));
				break;
			case 'C':
				compassSmooth=(float)(((float)newSetting)/100.0f);
				sendPopUpMessage("CompSm",(int)(compassSmooth*100));
				break;
			default:
				return 0;
			}
		}
	}
	return currPos;
}
int isDigit(char ch){
	if(ch<58&&ch>47){
		return TRUE;
	}
	return FALSE;
}
int getCharLeftCount(char position){
	if(droidCharCount-position<0){
		return droidCharCount+30-position;
	}
	else{
		return droidCharCount-position;
	}
}
int getPositiveDigitNumber(int currPosition,int length){
	int i=0;
	int num3Digit=0;
	if(getCharLeftCount(currPosition)>=length){
		for(i=0;i<length;i++){
			if(isDigit(inFromDroid[currPosition])){
				num3Digit=(num3Digit*10)+(inFromDroid[currPosition]-48);
			}
			else{
				return -1;
			}
			currPosition=handleLoopAt29(++currPosition);
		}
		return num3Digit;
	}
	return -1;
}
char handleLoopAt29(char num){
	if(num>29){
		num-=30;
	}
	return num;
}

