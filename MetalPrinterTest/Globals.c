
#include "Globals.h"
#include "MagAcc.h"
#include "PIDController.h"
#include "MotorDrive.h"
#include "Tester.h"
#include <math.h>

#define HEADING_SMOOTH_CONSTANT 0.03f//0.3f//0.5f
#define MEDIAN_SMOOTH_HALF 5
#define MEDIAN_SMOOTH_TOTAL (2*MEDIAN_SMOOTH_HALF)-1
#define SHIFT_DOWN 1
#define SHIFT_UP   0

float medianCompassArr[MEDIAN_SMOOTH_TOTAL];
short int medianPositionArr[MEDIAN_SMOOTH_TOTAL];
short int currMedianEntry;
short int medianValueCount;
float compassSmooth;
//testing variables
char testState;//=0;
//int c_ticks;

//PID values
char allowedTolerance;
char pidTimer;

//int moveCount;
//char key;
//volatile int motorRunning;
//int paused;
//char useTilt;//=0;
int bailOutBadCompass;//=0;

//Motor values
char shaftPosition;//=MIDDLE;
char adjustShaft;
char motorState;

//UI values
char autoHelmState;
char toggleLED;
char lastautoHelmState;
char tacking;

char tiltChanged;
char keelState;
char lastKeelState;

//compass value
compass heading;//={0,0,0,0,0};


/*****************************************************************************
Name:        getCompassReading 
Parameters:        none             
Returns:        a newly read compass heading from the digital compass
Description:  returns a newly read compass heading from the digital compass   

*****************************************************************************/
void initGlobal(){
	heading.current=0;
	heading.currentAvg=0;
	heading.desired=0;
	heading.tilt=0;
	heading.desiredTilt=0;
	heading.lastDesired=0;
	shaftPosition=MIDDLE;
//	useTilt=FALSE;
	bailOutBadCompass=0;
	testState=0;
	allowedTolerance=0;
	tacking=FALSE;
	compassSmooth=HEADING_SMOOTH_CONSTANT;
	medianValueCount=0;
	currMedianEntry=0;
	medianPositionArr[currMedianEntry]=0;
	resetMedianArr();
	keelState=KEEL_FLAT;
	tiltChanged=FALSE;
	lastKeelState=keelState;
}
void setKeelTilt(void){
	char tempState;
	if(tilt<90){
		tempState=KEEL_STARBOARD;
	}
	else if(tilt>100){
		tempState=KEEL_PORT;
	}
//	else{
//		tempState=KEEL_FLAT;
//	}
	if(tempState!=keelState){
		tiltChanged=TRUE;
		lastKeelState=keelState;
		keelState=tempState;
	}
}
//float getMedianOld(float compassValue){
//	char arrPos=0;
//	while(medianCompassArr[arrPos]<compassValue && arrPos<MEDIAN_SMOOTH_HALF){
//		medianCompassArr[arrPos]=medianCompassArr[arrPos+1];
//		arrPos++;
//	}
//	if(arrPos>0){
//		medianCompassArr[arrPos-1]=compassValue;
//	}
//	medianValueCount++;
//	if(medianValueCount==MEDIAN_SMOOTH_TOTAL){
//		medianValue=medianCompassArr[0];
//		resetMedianArr();
//		medianValueCount=0;
//	}
//	return medianValue;
//}
float getMedian(float compassValue){
	char shiftDir=SHIFT_DOWN;
	int currPos=medianPositionArr[currMedianEntry];
	if(medianCompassArr[currPos]>compassValue){
		shiftDir=SHIFT_UP;
		while(currPos>0&&medianCompassArr[currPos-1]>compassValue){
			medianCompassArr[currPos]=medianCompassArr[currPos-1];
			currPos--;
		}
	}
	else{
		shiftDir=SHIFT_DOWN;
		while(currPos<medianValueCount-1&&medianCompassArr[currPos+1]<compassValue){
			medianCompassArr[currPos]=medianCompassArr[currPos+1];
			currPos++;
		}
	}
	int resetPos=0;
	while(resetPos<medianValueCount){//&&currPos>=0){
		if(shiftDir==SHIFT_DOWN){
			if(medianPositionArr[resetPos]<=currPos&&medianPositionArr[resetPos]>medianPositionArr[currMedianEntry]){
				medianPositionArr[resetPos]--;
			}
		}
		else{
			if(medianPositionArr[resetPos]>=currPos&&medianPositionArr[resetPos]<medianPositionArr[currMedianEntry]){
				medianPositionArr[resetPos]++;
			}
		}
		resetPos++;
	}
	medianPositionArr[currMedianEntry]=currPos;
	medianCompassArr[currPos]=compassValue;
	currMedianEntry++;
	if(currMedianEntry>=MEDIAN_SMOOTH_TOTAL){
		currMedianEntry=0;
	}
	if(medianValueCount<MEDIAN_SMOOTH_TOTAL){
		medianValueCount++;
	}
	if(medianValueCount==MEDIAN_SMOOTH_TOTAL){
		return medianCompassArr[MEDIAN_SMOOTH_HALF-1];
	}
	return -1;
}
void resetMedianArr(){
	short int arrPos=0;
	while(arrPos<MEDIAN_SMOOTH_TOTAL){
		medianCompassArr[arrPos]=450;
		medianPositionArr[arrPos]=arrPos;
		arrPos++;
	}
}
void setCompass(float compassReading){
	heading.current=compassReading;
//	heading.tilt=tilt;
}
void setDesired(int userSelectedDegrees){
	heading.lastDesired=heading.desired;
	heading.desired=userSelectedDegrees;
//	heading.desiredTilt=tilt;
}
void changeDesired(int change){
	heading.lastDesired=heading.desired;
	heading.desired=handle360(heading.desired+=change);
}
int handle360(int value){
	if(value>359){
		return value-360;
	}
	else if(value<0){
		return value+360;
	}
	return value;
}
float handle360f(float value){
	if(value>360){
		return value-360.0;
	}
	else if(value<0){
		return value+360.0;
	}
	return value;
}
float getCompassReading(char avg){
	float newDigitHeading=0;
	float medianHeading=0.0f;
	compassReadTester=1;
//	float old_value;
	//txs=0;
	//get digital reading
	//TODO:make only one place that reads the compass for final build
	compassReadTester=2;
	newDigitHeading=get_digital_heading();
	medianHeading=getMedian(newDigitHeading);
	setKeelTilt();
	//txs=1;
	//old_value=heading.current;
//	newDigitHeading-=180;//flip the compass to correct digital compass position
//    if (newDigitHeading < 0) newDigitHeading += 360; //correct for 0 turns to 360

	//if we want the average (avg=true) and the true difference is less than 70 degrees
	compassReadTester=3;
	if(avg){//(avg&&degreeDifferance(1,newDigitHeading,(int)heading.currentAvg)<=70){
		//the difference in lastSmoothed and new heading reading
		compassReadTester=4;
		float degreeDiff=degreeDifferancef(0,medianHeading,heading.currentAvg);
		//if across 0 360
//		if(abs(heading.current-newDigitHeading)>180){
//			testState=1;
//			//set the previos value to be the new value plus
//			//shortest difference between the two
//			//example(350old 3new ->  -10old 3new)
//			//those handling accross 360 0
//			old_value=newDigitHeading+degreeDiff;
//			//old_value=(float)newDigitHeading+degreeDifferancef(0,heading.currentAvg,(float)newDigitHeading);
//		}
//		else{
//			testState=2;
//			//if not across 0 360 then just use the current without modifying it
//			old_value=heading.currentAvg;
//		}
		//take the current average
		//heading.currentAvg=((11.0/16.0)*old_value)+(((16.0-11.0)/16.0)*((float)newDigitHeading));
		compassReadTester=5;
		heading.currentAvg=heading.currentAvg+(degreeDiff*compassSmooth);
		compassReadTester=6;
		heading.currentAvg=handle360f(heading.currentAvg);
		compassReadTester=7;
		testState=3;
		bailOutBadCompass=0;
		testState=4;
		return heading.currentAvg;
	}
//	else if(avg){
//		if(bailOutBadCompass++>3){
//			heading.currentAvg=newDigitHeading;
//			bailOutBadCompass=0;
//			testState=5;
//			return newDigitHeading;
//		}
//		newDigitHeading=heading.currentAvg;
//		testState=6;
//		return newDigitHeading;
//	}
	compassReadTester=8;
	heading.currentAvg=newDigitHeading;
	testState=7;
	compassReadTester=9;
	return heading.currentAvg;
}

/* if the absoluteValue is set to 0 then degreeDifferance will have a sign pos or  neg */
int degreeDifferance(int absoluteValue,int current,int desired){
	int compDiff=0;
	int accrossN=0;
	compDiff=abs(current-desired);
	if(compDiff>180){
		compDiff=abs(compDiff-360);
		accrossN=1;
	}
	if(absoluteValue || ((current>desired && accrossN==0) || (accrossN==1 && current<desired))){
		return compDiff;
	}
	else{
		return (-1*compDiff);
	}
}
/* if the absoluteValue is set to 0 then degreeDifferance will have a sign pos or  neg */
float degreeDifferancef(int absoluteValue,float current,float desired){
	float compDiff=0;
	char accrossN=0;
	compDiff=fabs(current-desired);
	if(compDiff>180){
		compDiff=fabs(compDiff-360.0);
		accrossN=1;
	}
	if(absoluteValue || ((current>desired && accrossN==0) || (accrossN==1 && current<desired))){
		return compDiff;
	}
	else{
		return (-1.0*compDiff);
	}
}

void digits2String(int value,char strNum[5]){
	strNum[1]=(value/100)+48;
	strNum[2]=(((value%100)/10))+48;
	strNum[3]=(value%10)+48;
	strNum[4]='\0';
}
void bigDigits2String(int value,char strNum[8]){
	if(value>=1000000){
		value=999999;
	}
	strNum[1]=(value/100000)+48;
	value%=100000;
	strNum[2]=(value/10000)+48;
	value%=10000;
	strNum[3]=(value/1000)+48;
	value%=1000;
	strNum[4]=(value/100)+48;
	value%=100;
	strNum[5]=(value/10)+48;
	strNum[6]=(value%10)+48;
	strNum[7]='\0';
}
//void tiltToStr(char strNum[9],int value){
//	strNum[0]='T';
//	strNum[1]='I';
//	strNum[2]='L';
//	strNum[3]='T';
//	strNum[4]=' ';
//	digits2String(value,5,strNum);
//	strNum[8]='\0';
//}

