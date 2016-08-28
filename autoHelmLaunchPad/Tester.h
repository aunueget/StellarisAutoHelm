/*
 * Tester.h
 *
 *  Created on: Jan 2, 2013
 *      Author: dell
 */

#ifndef TESTER_H_
#define TESTER_H_
extern char variableMotorTester;
extern char motorTester;
extern char compassReadTester;

void testLEDDisplay(void);
void testI2Ccompass(void);
void testI2CBlueUart(void);
void testMotorEndStops(void);
void testDriveMotor(void);
void testI2CBlueUartWithUI(void);
void testBlueUI(void);
void testMotorwithUI(void);
void testIsDigit(void);
void testGetCharLeftCount(void);
void testGet3DigitNumber(void);
void testHandleLoopAt29(void);
void testSimpleBlueUI();
void testFailed(char* str);
void testAllAndroidCommsWithUI(void);
void testHbridgeComms(void);
void testDriveMotorOneWay(void);
void testVariableMotor(void);
void testMotorWithCompassPID(void);
void testAll(void);
void testAutoTuneClocks(void);
void testFloat2String(void);
void testBitOper(void);
void testRandom(void);
void getCompassForCalibrate(void);
void testVariableMotorWithAllComms(void);
void testBigDigits2String(void);
void testAndroidcommsWithWarningsNKeytrys(void);
void testAllInAllOutWithAllComms(void);
void testGetMedian(void);
//test all Klist stuff
void testEntireKlist(void);
void testInitKlist(void);
void testAddKpid(void);
void testAddKpid2(void);
void testCurrentTestKpidFAILED(void);
void testFindInsert(void);
void testSelectNextKpid(void);
void testInListAlreadyAt(void);
void testModPID_Task(void);
void testAutoTuneClocks(void);
void testStoreErrorSignChanges(void);
void testStorePIDHistory(void);
//end test all Klist stuff

#endif /* TESTER_H_ */
