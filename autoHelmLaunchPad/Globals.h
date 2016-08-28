#ifndef GLOBALS_H
#define GLOBALS_H

#define TRUE 1
#define FALSE 0
typedef struct var {
	//int offset;
	float current;
	float currentAvg;
	int desired;
	int lastDesired;
	float tilt;
	int desiredTilt;
	//char strCurrent[5];
	//char strDesired[4];
} compass;
extern float compassSmooth;
extern compass heading;
extern char pidTimer;
extern char adjustShaft;
//extern char useTilt;
//extern int c_ticks;
extern char allowedTolerance;
//extern int moveCount;
//extern char key;
//extern volatile int motorRunning;
//extern int paused;
extern char shaftPosition;
extern char motorState;
extern char autoHelmState;
extern char lastautoHelmState;
extern char toggleLED;
extern char testState;
extern char tacking;
extern char keelState;
extern char lastKeelState;
extern char tiltChanged;
enum {
	KEEL_FLAT, KEEL_STARBOARD, KEEL_PORT
};
enum {
	STARBOARD, PORT
};
enum {
	ADJUST_OUT, ADJUST_IN, ADJUST_STOP
};
enum {
	PAUSE, AUTOPILOT, SELECT_TACK_DIRECTION, TACK_STARBOARD, TACK_PORT
};

//*****************************************************************************
//
// Global variable to flag if error occured on I2C
//
//*****************************************************************************

extern char Send_I2C_error;
extern char Receive_I2C_error;
// added object from compass.h

void initGlobal(void);
float getCompassReading(char avg);
int degreeDifferance(int absoluteValue, int current, int desired); //calc error for pid
float degreeDifferancef(int absoluteValue,float current,float desired); //calc error for pid
//void tiltToStr(char strNum[9], int value);
void digits2String(int value, char strNum[5]);
void bigDigits2String(int value, char strNum[8]);
void setCompass(float compassReading);
void setDesired(int userSelectedDegrees);
void changeDesired(int change);
int handle360(int value);
float handle360f(float value);
void resetMedianArr(void);
float getMedian(float compassValue);
void setKeelTilt(void);
#endif  
