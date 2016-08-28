/*
 * Tester.c
 *
 *  Created on: Jan 2, 2013
 *      Author: dell
 */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "MagAccComms.h"
#include "MagAcc.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "VariableMotor.h"
#include "BlueUART.h"
#include "Globals.h"
#include "MotorDrive.h"
#include "UserInterface.h"
#include "Tester.h"
#include "BlueUI.h"
#include "PIDController.h"
#include "HBridgeComms.h"
#include "AutoTunePID.h"
#include "Klist.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#define DEBUG_PRINT UARTprintf

char variableMotorTester;
char motorTester;
char compassReadTester;

void testFailed(char* str){
	DEBUG_PRINT("Test Failed");
	DEBUG_PRINT(str);
}
//void testLEDDisplay(void) {
//	initUI();
//  ROM_IntMasterEnable();
//	ROM_SysCtlDelay(40000000);
//	autoHelmState = AUTOPILOT;
//	ROM_SysCtlDelay(40000000);
//	autoHelmState = PAUSE;
//	ROM_SysCtlDelay(40000000);
//	autoHelmState = SELECT_TACK_DIRECTION;
//	ROM_SysCtlDelay(40000000);
//	autoHelmState = PAUSE;
//	ROM_SysCtlDelay(40000000);
//	autoHelmState =TACK_STARBOARD;
//	ROM_SysCtlDelay(40000000);
//}
//
//void testI2Ccompass(void) {
//	char toggle = 4;
//	float digiHeading = 0;
//	initI2Ccomms();
//	initMagAcc();
//  ROM_IntMasterEnable();
//	while (1) {
//		ROM_SysCtlDelay(133333);
//		if (toggle > 100) {
//			//TODO:make only one place that reads the compass for final build
//			digiHeading = get_digital_heading();
//			DEBUG_PRINT("New Heading %03i\n", (int)digiHeading);
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//	}
//
//}
void getCompassForCalibrate(void) {
	char toggle = 4;
	initI2Ccomms();
	initMagAcc();
	ROM_IntMasterEnable();
	while (1) {
		ROM_SysCtlDelay(13333);
		if (toggle > 100) {
			//TODO:make only one place that reads the compass for final build
			get_digital_heading();
			toggle = 0;
		} else {
			toggle++;
		}
	}

}
//void testI2CBlueUart(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	initBlueUart();
//  ROM_IntMasterEnable();
//	float digiHeading = 0;
//	char headingStr[5] = "@    ";
//	char desiredStr[5] = "$    ";
//	initI2Ccomms();
//	initMagAcc();
//	while (1) {
//		ROM_SysCtlDelay(133333);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				DEBUG_PRINT("Desired Heading %03i\n", heading.desired);
//				digits2String(heading.desired, desiredStr);
//				sendToBlueTooth(desiredStr);
//				sendDesiredToggle=0;
//			}
//			else{
//				//TODO:make only one place that reads the compass for final build
//				digiHeading = get_digital_heading();
//				DEBUG_PRINT("New Heading %03d\n", (int)digiHeading);
//				digits2String((int)digiHeading, headingStr);
//				sendToBlueTooth(headingStr);
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//	}
//
//}
//void testI2CBlueUartWithUI(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	float digiHeading = 0;
//	char headingStr[5] = "@    ";
//	char desiredStr[5] = "$    ";
//	initBlueUart();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//  ROM_IntMasterEnable();
//	//TODO:make only one place that reads the compass for final build
//	heading.current=getCompassReading(0);
//	while (1) {
//		ROM_SysCtlDelay(133333);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				DEBUG_PRINT("Desired Heading %03i\n", heading.desired);
//				digits2String(heading.desired, desiredStr);
//				sendToBlueTooth(desiredStr);
//				sendDesiredToggle=0;
//			}
//			else{
//				//TODO:make only one place that reads the compass for final build
//				digiHeading = getCompassReading(1);
//				DEBUG_PRINT("New Heading %03d\n", (int)digiHeading);
//				digits2String((int)digiHeading, headingStr);
//				sendToBlueTooth(headingStr);
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//	}
//
//}
//void testAllAndroidCommsWithUI(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	char sendCurrent=FALSE;
//	char sendDesired=FALSE;
//	initDroidComms();
//	initBlueUart();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//  ROM_IntMasterEnable();
//	//set the initial compass by getting a reading
//	//TODO:make only one place that reads the compass for final build
//	setCompass(getCompassReading(0));
//	while (1) {
//		statusToBlueTooth_WhileForever(sendCurrent,sendDesired);
//		if(sendDesired){
//			sendDesired=FALSE;
//		}
//		if(sendCurrent){
//			sendCurrent=FALSE;
//		}
//		handleCommsFromDroid();
//		ROM_SysCtlDelay(23333);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				sendDesiredToggle=0;
//				sendDesired=TRUE;
//			}
//			else{
//				sendCurrent=TRUE;
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//	}
//
//}
//void testBlueUI(void){
//	initBlueUart();
//	initUI();
//  ROM_IntMasterEnable();
//	while(1){
//		statusToBlueTooth_WhileForever(0,0);
//	}
//}
//void testMotorwithUI(void){
//	initBlueUart();
//	initHBridgeComms();
//	initVariableMotor();
//	startMotorToggle();
//	initMotor();
//	initUI();
//  ROM_IntMasterEnable();
//	while(1){
//		statusToBlueTooth_WhileForever(0,0);
//		motorDrive_WhileForever();
//	}
//}
//void testMotorWithCompassPID(void){
//	initDroidComms();
//	initBlueUart();
//	initHBridgeComms();
//	initVariableMotor();
//	initMotor();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//	initPID();
//  ROM_IntMasterEnable();
//	startMotorToggle();
//	while(1){
//		if(pidTimer){
//			pidTimer=FALSE;
//			//TODO:make only one place that reads the compass for final build
//			heading.desired= (int)get_digital_heading();
//			if(autoHelmState!=PAUSE){
//				modPID_task();
//			}
//		}
//		statusToBlueTooth_WhileForever(0,0);
//		motorDrive_WhileForever();
//	}
//}
//void testAll(void) {
//	int toggleSendCurrent=3;
//	int sendDesiredToggle=10;
//	char sendCurrent=0;
//	char sendDesired=0;
//	initDroidComms();
//	initBlueUart();
//	initHBridgeComms();
//	initVariableMotor();
//	initMotor();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//	initPID();
//  ROM_IntMasterEnable();
//	startMotorToggle();
//
//	while (1) {
//		if(pidTimer){
//			pidTimer=FALSE;
//			//TODO:make only one place that reads the compass for final build
//			heading.current= get_digital_heading();
//			if(autoHelmState!=PAUSE){
//				modPID_task();
//			}
//			toggleSendCurrent--;
//			sendDesiredToggle--;
//		}
//		if(toggleSendCurrent<0){
//			sendCurrent=1;
//			toggleSendCurrent=3;
//		}
//		else{
//			sendCurrent=0;
//		}
//		if(sendDesiredToggle<0){
//			sendDesired=1;
//			sendDesiredToggle=10;
//		}
//		else{
//			sendDesired=0;
//		}
//
//		statusToBlueTooth_WhileForever(sendCurrent,sendDesired);
//		handleCommsFromDroid();
//		motorDrive_WhileForever();
//
//	}
//
//}
//void testAutoTuneClocks(void){
//	int keep=1;
//	int pidu[]={6,6,6,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,-2066,-2066,-2066,-2065,-2065,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,-2065,-2065,-2065,-2065,-2065,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,-2066,-2066,-2066,-2066,-2066,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,5,-2069,-2069,-2069,-2069,-2069,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,-2075,-2075,-2075,-2075,-2076,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-3,-3,-3,-3,-3,-3,-3,-4,-4,-4,-4,-4,-4,-4,-5,-5,-5,-5,-5,-5,-5,-6,-6,-6,-6,-6,-6,-7,-7,-7,-7,-7,-7,-7,-8,-8,-8,-8,-8,-8,-8,-9,-9,-9,-9,-9,-9,-9,-10,-10,-10,-10,-10,-10,-11,-11,-11,-11,-10,-10,2063,2063,2063,2063,2063,-11,-11,-11,-11,-11,-11,-11,-11,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-13,-13,-13,-12,-12,-12,2062,2062,2062,2061,2061,-12,-12,-12,-12,-12,-12,-13,-13,-13,-13,-13,-12,-12,-12,-12,2062,2062,2062,2062,2062,-12,-12,-12,-12,-12,-12,-11,-11,-11,-11,2063,2063,2063,2063,2063,-10,-10,-10,-10,-10,2065,2065,2065,2065,2065,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-5,-5,-5,-5,-5,-5,-5,-5,-5,2070,2070,2070,2071,2071,-3,-3,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,5,5,2080,2080};
//	unsigned short pidy[]={182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,181,181,181,181,181,181,181,181,181,181,181,181,181,181,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,184,184,184,184};
//
//	initPID();
//	ROM_TimerDisable(TIMER3_BASE, TIMER_A);
//	initAutoTune();
//	/* initialize random seed: */
//	srand (time(NULL));
//  ROM_IntMasterEnable();
//	while(pidData.currCount<=(ITERATIONS*2)-3){
//		if(pidData.isReset){
//			//reset integral to smooth transition to new ki,kp,kd
//			PID.integral=0;
//			//reset derivative to smooth transition to new ki,kp,kd
//			startKd=TRUE;
//			//get random kp,ki,kd
//			//System.out.println("Old-----------------"+this.Kp+","+this.Ki+","+this.Kd);
//			PID.Kp=(int)(rand()%600);
//			PID.Ki=(int)(rand()%600);
//			PID.Kd=(int)(rand()%12000);
//			pidConstantsStable=0;
//			getStableReadingCount=0;
//			//keep track of the ones we have used
//			// pidData.Kd[pidData.currKSet]=this.Kd;
//			// pidData.Ki[pidData.currKSet]=this.Ki;
//			// pidData.Kp[pidData.currKSet]=this.Kp;
//			//System.out.println("New-----------------"+this.Kp+","+this.Ki+","+this.Kd);
//
//			//pidData.currKSet++;
//			kSetsTryed++;
//			//tell it we are ready to try a new set
//			pidData.isReset=FALSE;
//		}
//		pidData.currCount++;
////		if(pidTimer){
////			pidTimer=FALSE;
////			heading.current = handle360f(heading.current+1);
////			modPID_task();
////		}
//		if(pidData.currCount>=ITERATIONS){
//			pidData.currCount=0;
//			pidData.resetCount++;
//		}
//		if(pidData.resetCount>0){
//			pidData.u[pidData.currCount]=pidu[pidData.currCount];
//			pidData.y[pidData.currCount]=pidy[pidData.currCount];
//		}
//		keep=calcTspec(PID.Kp,(float)(PID.Ki/1000.0),PID.Kd);
//	}
//	keep=calcTspec(PID.Kp,(float)(PID.Ki/1000.0),PID.Kd);
//	pidData.currCount=0;
//}
//void testMotorEndStops(void) {
//	initMotor();
//  ROM_IntMasterEnable();
////	initMotorEndsOnly();
//	while (1) {
//		turnOnMotorB();
//	}
//}
//void testDriveMotorOneWay(void) {
//	char toggle = 1;
//	initPID();
//	PID.out=130;
//	initHBridgeComms();
//	initVariableMotor();
//	startMotorToggle();
//	initMotor();
//
//	while (1) {
//		if (toggle) {
//			//driveBIn();
//			driveBOut();
//			toggle = 0;
//			ROM_SysCtlDelay(13333);
//			toggle = 0;
//		} else {
////			turnOffMotorB();
////			toggle = 1;
////			SysCtlDelay(133333);
////			toggle = 1;
//		}
//
//	}
//}
//void testDriveMotor(void) {
//	char toggle = 1;
//	initPID();
//	PID.out=130;
//	initHBridgeComms();
//	initVariableMotor();
//	startMotorToggle();
//	initMotor();
//	initUI();
//
//	while (1) {
//		if (toggle) {
//			driveBOut();
//			toggle = 0;
//			ROM_SysCtlDelay(40333333);
//			toggle = 0;
//		} else {
//			driveBIn();
//			toggle = 1;
//			ROM_SysCtlDelay(30333333);
//			toggle = 1;
//		}
//
//	}
//}
//void testVariableMotor(void) {
//	char toggle = 1;
//	int changePWM=5;
//	initPID();
//	PID.out=130;
//	initHBridgeComms();
//	initVariableMotor();
//	startMotorToggle();
//	initMotor();
//	initUI();
//	PID.out=5;
//	while (1) {
//		if (toggle) {
//			driveBIn();
//			toggle = 0;
//			ROM_SysCtlDelay(30333333);
//			toggle = 0;
//		} else {
//			driveBOut();
//			toggle = 1;
//			ROM_SysCtlDelay(30333333);
//			toggle = 1;
//		}
//		PID.out+=changePWM;
//		if(PID.out>50){
//			changePWM=-5;
//		}
//		else if(PID.out<10){
//			changePWM=5;
//		}
//
//	}
//}
//void testVariableMotorWithAllComms(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	char sendCurrent=FALSE;
//	char sendDesired=FALSE;
//	char toggleMotor = 1;
//	int motorWaitCount=0;
//	int changePWM=5;
//	PID.out=130*25;
//	initDroidComms();
//	initBlueUart();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//	initHBridgeComms();
//	initVariableMotor();
//	initMotor();
//	ROM_IntMasterEnable();
//	setCompass(getCompassReading(0));
//	while (1) {
//		statusToBlueTooth_WhileForever(sendCurrent,sendDesired);
//		motorWaitCount++;
//		if(sendDesired){
//			sendDesired=FALSE;
//		}
//		if(sendCurrent){
//			sendCurrent=FALSE;
//		}
//		handleCommsFromDroid();
//		ROM_SysCtlDelay(23333);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				sendDesiredToggle=0;
//				sendDesired=TRUE;
//			}
//			else{
//				sendCurrent=TRUE;
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//		if(motorWaitCount>800){
//			if (toggleMotor) {
//				sendHBridge();
//				driveMotor(DRIVING_IN,ALLIN);
//				toggleMotor = 0;
//			} else {
//				sendHBridge();
//				driveMotor(DRIVING_OUT,ALLOUT);
//				toggleMotor = 1;
//			}
//			PID.out+=changePWM*25;
//			motorWaitCount=0;
//			if(PID.out/25>90){
//				changePWM=-5;
//			}
//			else if(PID.out/25<10){
//				changePWM=5;
//			}
//
//		}
//
//	}
//}

//void testHbridgeComms(void){
//	initHBridgeComms();
//  ROM_IntMasterEnable();
//	while(1){
//		sendHBridge();
//		ROM_SysCtlDelay(40333333);
//	}
//}
//void testIsDigit(void){
//	if(isDigit('/')){
//		testFailed("isDigit/\n\0\0");
//	}
//	if(!isDigit('0')){
//		testFailed("isDigit0\n\0\0");
//	}
//	if(!isDigit('9')){
//		testFailed("isDigit9\n\0\0");
//	}
//	if(isDigit(':')){
//		testFailed("isDigit:\n\0\0");
//	}
//}
//void testGetCharLeftCount(void){
//	droidCharCount=29;
//	readDroidPosition=0;
//	if(!getCharLeftCount(readDroidPosition)==29){
//		testFailed("getCharLeftCount0,29\n\0");
//	}
//	droidCharCount=0;
//	readDroidPosition=29;
//	if(!getCharLeftCount(readDroidPosition)==1){
//		testFailed("getCharLeftCount29,0\n\0");
//	}
//	droidCharCount=10;
//	readDroidPosition=20;
//	if(!getCharLeftCount(readDroidPosition)==20){
//		testFailed("getCharLeftCount10,20\n\0");
//	}
//	droidCharCount=20;
//	readDroidPosition=20;
//	if(!getCharLeftCount(readDroidPosition)==0){
//		testFailed("getCharLeftCount20,20\n\0");
//	}
//	droidCharCount=20;
//	readDroidPosition=5;
//	if(!getCharLeftCount(readDroidPosition)==15){
//		testFailed("getCharLeftCount5,20\n\0");
//	}
//
//}
//void testGet3DigitNumber(void){
//	droidCharCount=20;
//	readDroidPosition=3;
//	inFromDroid[4]='3';
//	inFromDroid[5]='0';
//	inFromDroid[6]='0';
//	if(get3DigitNumber(4)!=300){
//		testFailed("get3Digit,300\n\0");
//	}
//	droidCharCount=20;
//	readDroidPosition=3;
//	inFromDroid[4]='3';
//	inFromDroid[5]='0';
//	inFromDroid[6]='@';
//	if(get3DigitNumber(4)!=-1){
//		testFailed("get3Digit,30@\n\0");
//	}
//	droidCharCount=20;
//	readDroidPosition=3;
//	inFromDroid[4]='3';
//	inFromDroid[5]='a';
//	inFromDroid[6]='0';
//	if(get3DigitNumber(4)!=-1){
//		testFailed("get3Digit,3a0\n\0");
//	}
//	droidCharCount=6;
//	readDroidPosition=3;
//	inFromDroid[4]='3';
//	inFromDroid[5]='0';
//	inFromDroid[6]='0';
//	if(get3DigitNumber(4)!=-1){
//		testFailed("get3Digit,read3,start4,count6\n\0");
//	}
//	droidCharCount=7;
//	readDroidPosition=3;
//	inFromDroid[4]='3';
//	inFromDroid[5]='0';
//	inFromDroid[6]='0';
//	if(get3DigitNumber(4)!=300){
//		testFailed("get3Digit,300,read3,start4,count7");
//	}
//	droidCharCount=2;
//	readDroidPosition=28;
//	inFromDroid[29]='2';
//	inFromDroid[0]='5';
//	inFromDroid[1]='6';
//	if(get3DigitNumber(29)!=256){
//		testFailed("get3Digit,256,read28,start29,count2");
//	}
//	droidCharCount=1;
//	readDroidPosition=28;
//	inFromDroid[29]='2';
//	inFromDroid[0]='5';
//	inFromDroid[1]='6';
//	if(get3DigitNumber(29)!=-1){
//		testFailed("get3Digit,256,read28,start29,count1");
//	}
//
//}
//void testHandleLoopAt29(void){
//	if(handleLoopAt29(30)!=0){
//		testFailed("handleLoop,30");
//	}
//	if(handleLoopAt29(35)!=5){
//		testFailed("handleLoop,35");
//	}
//	if(handleLoopAt29(1)!=1){
//		testFailed("handleLoop,1");
//	}
//	if(handleLoopAt29(0)!=0){
//		testFailed("handleLoop,0");
//	}
//
//}
//void testFloat2String(void){
//	char output[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
//	float num=1.0/8.0;
//	floatToString(output,num);
//	DEBUG_PRINT("\n1/8 = %s\n",output);
//	num=17.0/8.0;
//	floatToString(output,num);
//	DEBUG_PRINT("\n17/8 = %s\n",output);
//	num=17.0/16.0;
//	floatToString(output,num);
//	DEBUG_PRINT("\n17/16 = %s\n",output);
//	num=-3.0/8.0;
//	floatToString(output,num);
//	DEBUG_PRINT("\n-3/8 = %s\n",output);
//	num=-27.0/4.0;
//	floatToString(output,num);
//	DEBUG_PRINT("\n-27/4 = %s\n",output);
//
//}
//
//void testSimpleBlueUI(){
//	initDroidComms();
//  ROM_IntMasterEnable();
//	testHandleLoopAt29();
//	testIsDigit();
//	testGetCharLeftCount();
//	testGet3DigitNumber();
//
//
//}
//void testBitOper(){
//	unsigned long testLong=0;
//	short int testInt=0;
//	testLong|=1;
//	testInt=testLong|0;
//	DEBUG_PRINT("\n bitwiseOr = %d\n",testInt);
//	testLong=0;
//	testInt=0;
//	testLong|=1;
//	testLong<<=1;
//	testLong|=1;
//	testLong<<=1;
//	testLong|=1;
//	testInt=testLong|0;
//	DEBUG_PRINT("\n bitwiseOr = %d\n",testInt);
//	testLong=0;
//	testInt=0;
//	testLong|=1;
//	testLong<<=1;
//	testLong<<=1;
//	testLong|=1;
//	testInt=testLong|0;
//	DEBUG_PRINT("\n bitwiseOr = %d\n",testInt);
//
//}
//void testRandom(){
//	/* initialize random seed: */
//	srand (time(NULL));
//  ROM_IntMasterEnable();
//	while(1){
//		DEBUG_PRINT("\n Kp = %d Ki = %d Kd= %d \n",(int)(rand()%600),(int)(rand()%600),(int)(rand()%12000));
//	}
//}
//void testBigDigits2String(){
//	char keystriedStr[8]="&       ";
//	bigDigits2String(1000000,keystriedStr);
//	DEBUG_PRINT("1000000 as a string: %s\n",keystriedStr);
//	bigDigits2String(54321,keystriedStr);
//	DEBUG_PRINT("54321 as a string: %s\n",keystriedStr);
//	bigDigits2String(7,keystriedStr);
//	DEBUG_PRINT("7 as a string: %s\n",keystriedStr);
//	bigDigits2String(123456,keystriedStr);
//	DEBUG_PRINT("123456 as a string: %s\n",keystriedStr);
//	bigDigits2String(43,keystriedStr);
//	DEBUG_PRINT("43 as a string: %s\n",keystriedStr);
//	bigDigits2String(0,keystriedStr);
//	DEBUG_PRINT("0 as a string: %s\n",keystriedStr);
//
//}
//void testAndroidcommsWithWarningsNKeytrys(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	char sendCurrent=FALSE;
//	char sendDesired=FALSE;
//	int doTestCount=0;
//	initDroidComms();
//	initBlueUart();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//	ROM_IntMasterEnable();
//	//set the initial compass by getting a reading
//	//TODO:make only one place that reads the compass for final build
//	setCompass(getCompassReading(0));
//	while (1) {
//		statusToBlueTooth_WhileForever(sendCurrent,sendDesired);
//		doTestCount++;
//		switch(doTestCount){
//			case 40:
//				DEBUG_PRINT("VO\n");
//				vddOverVoltage=TRUE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 50:
//				DEBUG_PRINT("VU\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=TRUE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 60:
//				DEBUG_PRINT("TW\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=TRUE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 70:
//				DEBUG_PRINT("TS \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=TRUE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 80:
//				DEBUG_PRINT("CL \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=TRUE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 90:
//				DEBUG_PRINT("CS \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=TRUE;
//				kSetsTryed=0;
//			break;
//			case 100:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=100;
//			break;
//			case 103:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=103;
//			break;
//			case 106:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=106;
//			break;
//			case 109:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=109;
//			break;
//			case 112:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=112;
//			break;
//			case 115:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=115;
//			break;
//			case 240:
//				DEBUG_PRINT("VO\n");
//				vddOverVoltage=TRUE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 250:
//				DEBUG_PRINT("VU \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=TRUE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 260:
//				DEBUG_PRINT("TW \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=TRUE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 270:
//				DEBUG_PRINT("TS \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=TRUE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 280:
//				DEBUG_PRINT("CL \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=TRUE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=0;
//			break;
//			case 290:
//				DEBUG_PRINT("CS \n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=TRUE;
//				kSetsTryed=0;
//			break;
//			case 300:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=100;
//			break;
//			case 303:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=103;
//			break;
//			case 306:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=106;
//			break;
//			case 309:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=109;
//			break;
//			case 312:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=112;
//			break;
//			case 315:
//				DEBUG_PRINT("keys\n");
//				vddOverVoltage=FALSE;
//				vsUnderVoltage=FALSE;
//				temperatureWarning=FALSE;
//				overTemperatureShutdown=FALSE;
//				currentLimitationReached=FALSE;
//				overCurrentShutdown=FALSE;
//				kSetsTryed=115;
//			break;
//		}
//		if(sendDesired){
//			sendDesired=FALSE;
//		}
//		if(sendCurrent){
//			sendCurrent=FALSE;
//		}
//		handleCommsFromDroid();
//		ROM_SysCtlDelay(8000000);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				sendDesiredToggle=0;
//				sendDesired=TRUE;
//			}
//			else{
//				sendCurrent=TRUE;
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//	}
//
//}
//void testAllInAllOutWithAllComms(void) {
//	char toggle = 4;
//	char sendDesiredToggle=0;
//	char sendCurrent=FALSE;
//	char sendDesired=FALSE;
//	int motorWaitCount=0;
//	PID.out=110*50;
//	initDroidComms();
//	initBlueUart();
//	initI2Ccomms();
//	initMagAcc();
//	initUI();
//	initHBridgeComms();
//	initVariableMotor();
//	initMotor();
//	ROM_IntMasterEnable();
//	setCompass(getCompassReading(0));
//	driveMotor(DRIVING_OUT,ALLOUT);
//	while (1) {
//		statusToBlueTooth_WhileForever(sendCurrent,sendDesired);
//		motorWaitCount++;
//		if(sendDesired){
//			sendDesired=FALSE;
//		}
//		if(sendCurrent){
//			sendCurrent=FALSE;
//		}
//		handleCommsFromDroid();
//		ROM_SysCtlDelay(23333);
//		if (toggle > 100) {
//			if(sendDesiredToggle++>4){
//				sendDesiredToggle=0;
//				sendDesired=TRUE;
//			}
//			else{
//				sendCurrent=TRUE;
//			}
//			toggle = 0;
//		} else {
//			toggle++;
//		}
//		if(motorWaitCount>800){
//			sendHBridge();
//			if (shaftPosition == ALLOUT) {
//				driveMotor(DRIVING_IN,ALLIN);
//			}
//			if(shaftPosition == ALLIN){
//				driveMotor(DRIVING_OUT,ALLOUT);
//			}
////			PID.out+=changePWM*25;
////			motorWaitCount=0;
////			if(PID.out/25>90){
////				changePWM=-5;
////			}
////			else if(PID.out/25<10){
////				changePWM=5;
////			}
//
//		}
//
//	}
//}
//void testGetMedian(void){
//	//mixed
//	DEBUG_PRINT("\nMedian at 100 is: %d\n",(int)getMedian(100));
//	DEBUG_PRINT("\nMedian at 52 is: %d\n",(int)getMedian(52));
//	DEBUG_PRINT("\nMedian at 150 is: %d\n",(int)getMedian(150));
//	DEBUG_PRINT("\nMedian at 77 is: %d\n",(int)getMedian(77));
//	DEBUG_PRINT("\nMedian at 33 is: %d\n",(int)getMedian(33));
//	DEBUG_PRINT("\nMedian at 355 is: %d\n",(int)getMedian(355));
//	DEBUG_PRINT("\nMedian at 19 is: %d\n",(int)getMedian(19));
//	DEBUG_PRINT("\nMedian at 200 is: %d\n",(int)getMedian(200));
//	DEBUG_PRINT("\nMedian at 270 is: %d\n",(int)getMedian(270));
//	//in order
//	DEBUG_PRINT("\nMedian at 1 is: %d\n",(int)getMedian(1));
//	DEBUG_PRINT("\nMedian at 2 is: %d\n",(int)getMedian(2));
//	DEBUG_PRINT("\nMedian at 3 is: %d\n",(int)getMedian(3));
//	DEBUG_PRINT("\nMedian at 4 is: %d\n",(int)getMedian(4));
//	DEBUG_PRINT("\nMedian at 5 is: %d\n",(int)getMedian(5));
//	DEBUG_PRINT("\nMedian at 6 is: %d\n",(int)getMedian(6));
//	DEBUG_PRINT("\nMedian at 7 is: %d\n",(int)getMedian(7));
//	DEBUG_PRINT("\nMedian at 8 is: %d\n",(int)getMedian(8));
//	DEBUG_PRINT("\nMedian at 9 is: %d\n",(int)getMedian(9));
//	DEBUG_PRINT("\nMedian at 10 is: %d\n",(int)getMedian(10));
//	DEBUG_PRINT("\nMedian at 11 is: %d\n",(int)getMedian(11));
//	DEBUG_PRINT("\nMedian at 12 is: %d\n",(int)getMedian(12));
//	DEBUG_PRINT("\nMedian at 13 is: %d\n",(int)getMedian(13));
//	DEBUG_PRINT("\nMedian at 14 is: %d\n",(int)getMedian(14));
//	DEBUG_PRINT("\nMedian at 15 is: %d\n",(int)getMedian(15));
//	DEBUG_PRINT("\nMedian at 16 is: %d\n",(int)getMedian(16));
//	DEBUG_PRINT("\nMedian at 17 is: %d\n",(int)getMedian(17));
//	DEBUG_PRINT("\nMedian at 18 is: %d\n",(int)getMedian(18));
//	DEBUG_PRINT("\nMedian at 19 is: %d\n",(int)getMedian(19));
//	DEBUG_PRINT("\nMedian at 20 is: %d\n",(int)getMedian(20));
//	DEBUG_PRINT("\nMedian at 21 is: %d\n",(int)getMedian(21));
//	DEBUG_PRINT("\nMedian at 22 is: %d\n",(int)getMedian(22));
//	DEBUG_PRINT("\nMedian at 23 is: %d\n",(int)getMedian(23));
//	DEBUG_PRINT("\nMedian at 24 is: %d\n",(int)getMedian(24));
//	DEBUG_PRINT("\nMedian at 25 is: %d\n",(int)getMedian(25));
//	DEBUG_PRINT("\nMedian at 26 is: %d\n",(int)getMedian(26));
//	//now reverse
//	DEBUG_PRINT("\nMedian at 26 is: %d\n",(int)getMedian(26));
//	DEBUG_PRINT("\nMedian at 25 is: %d\n",(int)getMedian(25));
//	DEBUG_PRINT("\nMedian at 24 is: %d\n",(int)getMedian(24));
//	DEBUG_PRINT("\nMedian at 23 is: %d\n",(int)getMedian(23));
//	DEBUG_PRINT("\nMedian at 22 is: %d\n",(int)getMedian(22));
//	DEBUG_PRINT("\nMedian at 21 is: %d\n",(int)getMedian(21));
//	DEBUG_PRINT("\nMedian at 20 is: %d\n",(int)getMedian(20));
//	DEBUG_PRINT("\nMedian at 19 is: %d\n",(int)getMedian(19));
//	DEBUG_PRINT("\nMedian at 18 is: %d\n",(int)getMedian(18));
//	DEBUG_PRINT("\nMedian at 17 is: %d\n",(int)getMedian(17));
//	DEBUG_PRINT("\nMedian at 16 is: %d\n",(int)getMedian(16));
//	DEBUG_PRINT("\nMedian at 15 is: %d\n",(int)getMedian(15));
//	DEBUG_PRINT("\nMedian at 14 is: %d\n",(int)getMedian(14));
//	DEBUG_PRINT("\nMedian at 13 is: %d\n",(int)getMedian(13));
//	DEBUG_PRINT("\nMedian at 12 is: %d\n",(int)getMedian(12));
//	DEBUG_PRINT("\nMedian at 11 is: %d\n",(int)getMedian(11));
//	DEBUG_PRINT("\nMedian at 10 is: %d\n",(int)getMedian(10));
//	DEBUG_PRINT("\nMedian at 9 is: %d\n",(int)getMedian(9));
//	DEBUG_PRINT("\nMedian at 8 is: %d\n",(int)getMedian(8));
//	DEBUG_PRINT("\nMedian at 7 is: %d\n",(int)getMedian(7));
//	DEBUG_PRINT("\nMedian at 6 is: %d\n",(int)getMedian(6));
//	DEBUG_PRINT("\nMedian at 5 is: %d\n",(int)getMedian(5));
//	DEBUG_PRINT("\nMedian at 4 is: %d\n",(int)getMedian(4));
//	DEBUG_PRINT("\nMedian at 3 is: %d\n",(int)getMedian(3));
//	DEBUG_PRINT("\nMedian at 2 is: %d\n",(int)getMedian(2));
//	DEBUG_PRINT("\nMedian at 1 is: %d\n",(int)getMedian(1));
//}
void testEntireKlist(void){
	testInitKlist();
}
void testInitKlist(void){
	initKlist();
	ROM_IntMasterEnable();

	DEBUG_PRINT("\nRandom number from 0 to 600 is: %d\n",(int)(rand()%600));

}
void testAddKpid(void){
	struct Kpid temp;
	temp.id=10;
	temp.p=10;
	temp.i=10;
	temp.d=10;
	temp.sec=10;
	initKlist();
	ROM_IntMasterEnable();
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=20;
	temp.p=20;
	temp.i=20;
	temp.d=20;
	temp.sec=20;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=30;
	temp.p=30;
	temp.i=30;
	temp.d=30;
	temp.sec=30;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=40;
	temp.p=40;
	temp.i=40;
	temp.d=40;
	temp.sec=40;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=50;
	temp.p=50;
	temp.i=50;
	temp.d=50;
	temp.sec=50;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=60;
	temp.p=60;
	temp.i=60;
	temp.d=60;
	temp.sec=60;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=70;
	temp.p=70;
	temp.i=70;
	temp.d=70;
	temp.sec=70;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=80;
	temp.p=80;
	temp.i=80;
	temp.d=80;
	temp.sec=80;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=90;
	temp.p=90;
	temp.i=90;
	temp.d=90;
	temp.sec=90;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=100;
	temp.p=100;
	temp.i=100;
	temp.d=100;
	temp.sec=100;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=110;
	temp.p=110;
	temp.i=110;
	temp.d=110;
	temp.sec=110;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=120;
	temp.p=120;
	temp.i=120;
	temp.d=120;
	temp.sec=120;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=130;
	temp.p=130;
	temp.i=130;
	temp.d=130;
	temp.sec=130;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=140;
	temp.p=140;
	temp.i=140;
	temp.d=140;
	temp.sec=140;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=150;
	temp.p=150;
	temp.i=150;
	temp.d=150;
	temp.sec=150;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=160;
	temp.p=160;
	temp.i=160;
	temp.d=160;
	temp.sec=160;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=170;
	temp.p=170;
	temp.i=170;
	temp.d=170;
	temp.sec=170;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=180;
	temp.p=180;
	temp.i=180;
	temp.d=180;
	temp.sec=180;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=95;
	temp.p=95;
	temp.i=95;
	temp.d=95;
	temp.sec=95;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=155;
	temp.p=155;
	temp.i=155;
	temp.d=155;
	temp.sec=155;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=155;
	temp.p=155;
	temp.i=155;
	temp.d=155;
	temp.sec=155;
	addKpid(&negList,&temp);
	print_list(&negList);
//	if(tempTester->next){
//		testFailed("Creating the list next should be Null and is not. \n\0");
//	}
}
void testAddKpid2(void){
	struct Kpid temp;
	initKlist();
	temp.id=180;
	temp.p=180;
	temp.i=180;
	temp.d=180;
	temp.sec=180;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=155;
	temp.p=155;
	temp.i=155;
	temp.d=155;
	temp.sec=155;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=95;
	temp.p=95;
	temp.i=95;
	temp.d=95;
	temp.sec=95;
	addKpid(&negList,&temp);
	print_list(&negList);
	temp.id=155;
	temp.p=155;
	temp.i=155;
	temp.d=155;
	temp.sec=155;
	addKpid(&negList,&temp);
	print_list(&negList);
}
void testCurrentTestKpidFAILED(void){
	initKlist();
	negList.testPID.id=1;
	negList.testPID.p=25;
	negList.testPID.i=120;
	negList.testPID.d=3000;
	negList.testPID.sec=10;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=45;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=100;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=2;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=12;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=450;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=5;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=6;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=110;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=55;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);
	negList.testPID.sec=1000;
	currentTestKpidFAILED(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	print_list(&negList);


}
void testFindInsert(void){
	initKlist();
	testAddKpid();
	DEBUG_PRINT("\nInsert should be at 2 for 165: %d\n",findInsert(&negList,165));
	DEBUG_PRINT("\nInsert should be at 6 for 145: %d\n",findInsert(&negList,145));
	DEBUG_PRINT("\nInsert should be at 10 for 90: %d\n",findInsert(&negList,90));
	DEBUG_PRINT("\nInsert should be at 0 for 200: %d\n",findInsert(&negList,200));
}
void testSelectNextKpid(void){
	initKlist();
	testAddKpid2();
	print_list(&negList);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
	selectNextKpid(&negList);
	DEBUG_PRINT("\nNew Test Kpid p=%d i=%d d=%d id=%d sec=%d\n",negList.testPID.p,negList.testPID.i,negList.testPID.d,negList.testPID.id,negList.testPID.sec);
}
void testInListAlreadyAt(void){
	struct Kpid temp;
	initKlist();
	testAddKpid();
	temp.id=140;
	temp.p=140;
	temp.i=140;
	temp.d=140;
	temp.sec=130;
	DEBUG_PRINT("\nNothing changes id 140 should stay at pos 6: %d\n",inListAlreadyAdd(&negList,&temp));
	print_list(&negList);
	temp.sec=165;
	DEBUG_PRINT("\nInsert should be at 2 for id 140 at 165 sec return should be 6 because previos position: %d\n",inListAlreadyAdd(&negList,&temp));
	print_list(&negList);
	temp.id=93;
	temp.p=93;
	temp.i=93;
	temp.d=93;
	temp.sec=250;
	DEBUG_PRINT("\nShould be negative -1 for not in list for id 93 at 250 sec: %d\n",inListAlreadyAdd(&negList,&temp));
	print_list(&negList);
}


void testAutoTuneClocks(void){
	int keep=1;
	int pidu[]={6,6,6,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,-2066,-2066,-2066,-2065,-2065,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,-2065,-2065,-2065,-2065,-2065,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,-2066,-2066,-2066,-2066,-2066,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,5,-2069,-2069,-2069,-2069,-2069,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,-2075,-2075,-2075,-2075,-2076,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-3,-3,-3,-3,-3,-3,-3,-4,-4,-4,-4,-4,-4,-4,-5,-5,-5,-5,-5,-5,-5,-6,-6,-6,-6,-6,-6,-7,-7,-7,-7,-7,-7,-7,-8,-8,-8,-8,-8,-8,-8,-9,-9,-9,-9,-9,-9,-9,-10,-10,-10,-10,-10,-10,-11,-11,-11,-11,-10,-10,2063,2063,2063,2063,2063,-11,-11,-11,-11,-11,-11,-11,-11,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-13,-13,-13,-12,-12,-12,2062,2062,2062,2061,2061,-12,-12,-12,-12,-12,-12,-13,-13,-13,-13,-13,-12,-12,-12,-12,2062,2062,2062,2062,2062,-12,-12,-12,-12,-12,-12,-11,-11,-11,-11,2063,2063,2063,2063,2063,-10,-10,-10,-10,-10,2065,2065,2065,2065,2065,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-5,-5,-5,-5,-5,-5,-5,-5,-5,2070,2070,2070,2071,2071,-3,-3,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,5,5,2080,2080};
	unsigned short pidy[]={182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,181,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,179,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,181,181,181,181,181,181,181,181,181,181,181,181,181,181,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,184,184,184,184};
	initKlist();
	initPID();
	ROM_TimerDisable(TIMER3_BASE, TIMER_A);
	initAutoTune();
	/* initialize random seed: */
	ROM_IntMasterEnable();
}
void testModPID_Task(){
	initGlobal();
	initKlist();
	initPID();
	ROM_TimerDisable(TIMER3_BASE, TIMER_A);
	initAutoTune();
	/* initialize random seed: */
	ROM_IntMasterEnable();
	heading.current=185;
	heading.desired=180;
	modPID_task();
	heading.current=184;
	modPID_task();
	heading.current=182;
	modPID_task();
	heading.current=181;
	modPID_task();
	heading.current=180;
	modPID_task();
	heading.current=179;
	modPID_task();
	heading.current=178;
	modPID_task();
	heading.current=177;
	modPID_task();
	heading.current=176;
	modPID_task();
	heading.current=176;
	modPID_task();
	heading.current=176;
	modPID_task();
	heading.current=178;
	modPID_task();
	heading.current=179;
	modPID_task();
	heading.current=180;
	modPID_task();
	heading.current=180;
	modPID_task();
}
void testStoreErrorSignChanges(){

}
void testStorePIDHistory(){

}
