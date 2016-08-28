#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "MagAcc.h"
#include <math.h>
#include "utils/uartstdio.h"
#include "MagAccComms.h"
#include "Globals.h"
#include "Tester.h"
// Defines ////////////////////////////////////////////////////////////////
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifdef DEBUG
#define DEBUG_PRINT UARTprintf

#endif
#define STRING_MAX 15
#define STRING_DECIMAL_POINT 7
#define SMOOTH_CONSTANT .7

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define ACC_ADDRESS (0x18)
#define MAG_ADDRESS (0x1E)

//Global variables////////////////////////////////////////////////////////////

vector a; // accelerometer readings
vector m; // magnetometer readings
vector aPrev;//previos accelerometer
vector mPrev;//previos magnetometer
float previosHeading;
vector m_max; // maximum magnetometer values, used for calibration
vector m_min; // minimum magnetometer values, used for calibration

//vector test_max;
//vector test_min;
int tilt;
//char outputX[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
//char outputY[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
//char outputZ[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

// Constructors ////////////////////////////////////////////////////////////////

void initMagAcc(void) {
	// These are just some values for a particular unit; it is recommended that
	// a calibration be done for your particular unit.
	//TODO: get rid of these and recalibrate
	m_max.x = 750;
	m_max.y = 900;
	m_max.z = 1000;
	m_min.x = -750;
	m_min.y = -900;
	m_min.z = -1000;
//	test_max.x = 0;
//	test_max.y = 0;
//	test_max.z = 0;
//	test_min.x = 0;
//	test_min.y = 0;
//	test_min.z = 0;
	mPrev.x=0;
	mPrev.y=0;
	mPrev.z=0;
	aPrev.x=0;
	aPrev.y=0;
	aPrev.z=0;
	previosHeading=0;
	tilt = 0;
	ROM_IntMasterEnable();
	enableDefault();
	ROM_IntMasterDisable();
//	getCalibrateReadings();
}
// void getCalibrateReadings(){
//	 int i=0;
//	 File *dataFile;
//	 dataFile=fopen("testData.csv","w");
//	 for(i=0;i<100;i++){
//		 readAcc();
//		 readMag();
//	 }
//	 fclose(dataFile);
// }

// Public Methods //////////////////////////////////////////////////////////////

// Turns on the MagAcc's accelerometer and magnetometers and places them in normal
// mode.
void enableDefault(void) {
	unsigned char  regReceived=0;
// Enable Accelerometer
// 0x27 = 0b00100111
	//50Hz
// Normal power mode, all axes enabled
	writeAccReg(MagAcc_CTRL_REG1_A, 0x27);
	regReceived=readAccReg(MagAcc_CTRL_REG1_A);
	//DEBUG_PRINT("CTRL_REG1 reg A is: %03x\n", regReceived);
	writeAccReg(MagAcc_CTRL_REG4_A, 0x00);
	regReceived=readAccReg(MagAcc_CTRL_REG4_A);
	//DEBUG_PRINT("CTRL_REG4 reg A is: %03x\n", regReceived);

// Enable Magnetometer
	//30Hz
// 0x00 = 0b00000000
// Continuous conversion mode
	writeMagReg(MagAcc_MR_REG_M, 0x00);
	regReceived=readMagReg(MagAcc_MR_REG_M);
	//DEBUG_PRINT("MR reg M is: %03x\n", regReceived);
	writeMagReg(MagAcc_CRA_REG_M, 0x14);
	regReceived=readMagReg(MagAcc_CRA_REG_M);
	//DEBUG_PRINT("CRA reg M is: %03x\n", regReceived);

}

// Writes an accelerometer register
void writeAccReg(unsigned char reg, unsigned char value) {
	MyI2C_send(ACC_ADDRESS,reg, FIRST_I2C);
	MyI2C_send(ACC_ADDRESS,value, LAST_I2C);
	//MyI2C_endTransmission();

//	if(MyI2C_beginTransmission(ACC_ADDRESS))
//		if(MyI2C_send(reg))
//			if(MyI2C_send(value))
//				//MyI2C_endTransmission();
}

// Reads an accelerometer register
unsigned char readAccReg(unsigned char reg) {
	unsigned char value = 0;
	MyI2C_send(ACC_ADDRESS,reg, FIRST_I2C);
	//multi because repeated start required
	MyI2C_requestFrom(ACC_ADDRESS, SINGLE_RECEIVE_I2C, 1);
	//wait for read
	while (MyI2C_available() < 1)
		;

	value = MyI2C_receive(0);

//	if(MyI2C_beginTransmission(ACC_ADDRESS))
//		if(MyI2C_send(reg))
//	//MyI2C_endTransmission();
//			if(MyI2C_requestFrom(ACC_ADDRESS, 1)){
//				value = MyI2C_receive(0);
//				MyI2C_endTransmission();
//			}

	return value;
}

// Writes a magnetometer register
void writeMagReg(unsigned char reg, unsigned char value) {
	MyI2C_send(MAG_ADDRESS,reg, FIRST_I2C);
	MyI2C_send(MAG_ADDRESS,value, LAST_I2C);

//	if(MyI2C_beginTransmission(MAG_ADDRESS))
//		if(MyI2C_send(reg))
//			if(MyI2C_send(value))
//				MyI2C_endTransmission();

}

// Reads a magnetometer register
unsigned char readMagReg(unsigned char reg) {
	unsigned char value = 0;
	MyI2C_send(MAG_ADDRESS,reg, FIRST_I2C);
	//multi because repeated start required
	MyI2C_requestFrom(MAG_ADDRESS, SINGLE_RECEIVE_I2C, 1);
	//wait for read
	while (MyI2C_available() < 1)
		;

	value = MyI2C_receive(0);


	return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void readAcc(void) {
	int xla;
	int xha;
	int yla;
	int yha;
	int zla;
	int zha;
// assert the MSB of the address to get the accelerometer
// to do slave-transmit subaddress updating.
	compassReadTester=41;
	MyI2C_send(ACC_ADDRESS,MagAcc_OUT_X_L_A | (1 << 7), FIRST_I2C);
//MyI2C_endTransmission();
	compassReadTester=42;
	MyI2C_requestFrom(ACC_ADDRESS,MULTI_RECEIVE_I2C,6);
	compassReadTester=43;
	while (MyI2C_available() < 6)
		;
	compassReadTester=44;
	unsigned char temp=MyI2C_receive(1);
	xla =MyI2C_receive(0);
	xha =*(signed char*)(&temp);
	yla =MyI2C_receive(2);
	temp=MyI2C_receive(3);
	yha =*(signed char*)(&temp);
	zla =MyI2C_receive(4);
	temp=MyI2C_receive(5);
	zha =*(signed char*)(&temp);
	compassReadTester=45;
	a.x = (xha << 8 | xla) >> 4;
	a.y = (yha << 8 | yla) >> 4;
	a.z = (zha << 8 | zla) >> 4;

//	DEBUG_PRINT("%s %s %s\n", floatToString(outputX,a.x),floatToString(outputY,a.y),floatToString(outputZ,a.z));
	calibrateAcc(&a);
	compassReadTester=46;
	smoothVector(&a,&aPrev);
	compassReadTester=47;

//		if (a.x > test_max.x)
//			test_max.x = a.x;
//		else if (a.x < test_min.x)
//			test_min.x = a.x;
//		if (a.y > test_max.y)
//			test_max.y = a.y;
//		else if (a.y < test_min.y)
//			test_min.y = a.y;
//		if (a.z > test_max.z)
//			test_max.z = a.z;
//		else if (a.z < test_min.z)
//			test_min.z = a.z;
}

// Reads the 3 magnetometer channels and stores them in vector m
void readMag(void) {

	int xhm;
	int xlm;
	int yhm;
	int ylm;
	int zhm;
	int zlm;

	MyI2C_send(MAG_ADDRESS,MagAcc_OUT_X_H_M, FIRST_I2C);
//MyI2C_endTransmission();
	MyI2C_requestFrom(MAG_ADDRESS,MULTI_RECEIVE_I2C, 6);

	while (MyI2C_available() < 6)
		;
	unsigned char temp=MyI2C_receive(0);
	xhm =*(signed char*)(&temp);
	xlm =MyI2C_receive(1);
	temp=MyI2C_receive(2);
	yhm =*(signed char*)(&temp);
	ylm =MyI2C_receive(3);
	temp=MyI2C_receive(4);
	zhm =*(signed char*)(&temp);
	zlm =MyI2C_receive(5);

	m.x = (xhm << 8 | xlm);
	m.y = (yhm << 8 | ylm);
	m.z = (zhm << 8 | zlm);

//	DEBUG_PRINT("M %d, %d, %d\n", (int)m.y, (int)m.x,(int)m.z);
//	DEBUG_PRINT("%s %s %s\n", floatToString(outputX,m.x),floatToString(outputY,m.y),floatToString(outputZ,m.z));

	calibrateMag(&m);
	smoothVector(&m,&mPrev);
}

// Reads all 6 channels of the MagAcc and stores them in the object variables
//void read(void) {
//	compassReadTester=32;
//	readAcc();
//	compassReadTester=33;
//	readMag();
//	compassReadTester=34;
//}

// Returns the number of degrees from the -Y axis that it
// is pointing.
float get_digital_heading(void) {
	compassReadTester=21;
	vector tempVector = { 1, 0, 0 };
//	float currHeading=0;
	compassReadTester=22;
	readAcc();
	compassReadTester=33;
	readMag();
	compassReadTester=23;
//	currHeading=heading_from(tempVector);
//	currHeading=previosHeading+((currHeading-previosHeading)*.3);
//	previosHeading=currHeading;
	return heading_from(tempVector);//handle360fl(currHeading);
}

// Returns the number of degrees from the From vector projected into
// the horizontal plane is away from north.
//
// Description of heading algorithm:
// Shift and scale the magnetic reading based on calibration data to
// to find the North vector. Use the acceleration readings to
// determine the Down vector. The cross product of North and Down
// vectors is East. The vectors East and North form a basis for the
// horizontal plane. The From vector is projected into the horizontal
// plane and the angle between the projected vector and north is
// returned.
float heading_from(vector from) {
	vector temp_a;
	vector tiltFrom = { 0, 1, 0 };//{ 0, 0, 1 };
	vector E;
	vector N;
	float heading;
	// shift and scale
	//shift x because its low
	//TODO: get rid of these and recalibrate
	m.x = (m.x - m_min.x) / (m_max.x - m_min.x) * 2 - 1.0;
	m.y = (m.y - m_min.y) / (m_max.y - m_min.y) * 2 - 1.0;
	m.z = (m.z - m_min.z) / (m_max.z - m_min.z) * 2 - 1.0;

	temp_a.x = a.x;
	temp_a.y = a.y;
	temp_a.z = a.z;
	// normalize
	vector_normalize(&temp_a);
	//vector_normalize(&m);

	// compute E and N
	vector_cross(&m, &temp_a, &E);
	vector_normalize(&E);
	vector_cross(&temp_a, &E, &N);

	//compute tilt
	tilt = (int) (atan2f(1,
			vector_dot(&temp_a, &tiltFrom)) * 180.0 / M_PI); //deleted round
	if (tilt < 0)
		tilt += 360;
	// compute heading
	compassReadTester=25;
	heading = (atan2f(vector_dot(&E, &from), vector_dot(&N, &from)) * 180
			/ M_PI); //deleted round
	if (heading < 0)
		heading += 360;
	compassReadTester=26;
	return heading;
}

void vector_cross(const vector *a, const vector *b, vector *out) {
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
}

float vector_dot(const vector *a, const vector *b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vector_normalize(vector *a) {
	float mag = sqrtf(vector_dot(a, a));
	a->x /= mag;
	a->y /= mag;
	a->z /= mag;
}
//char* floatToString(char* numberAsString,float num){
//	int digit=0;
//	int count=6;
//	char negNum=FALSE;
//	int wholeNums=abs((int)num);
//	float fraction=fabsf(num)-wholeNums;
//	clearOutput(numberAsString);
//	if(num<0){
//		negNum=TRUE;
//	}
//	while(count>=0&&wholeNums>=10){
//		digit=(int)(wholeNums%10);
//		numberAsString[count--]=digit+48;
//		wholeNums/=10;
//	}
//	numberAsString[count--]=wholeNums+48;
//	if(negNum){
//		numberAsString[count--]='-';
//	}
//	numberAsString[STRING_DECIMAL_POINT]='.';
//	count=STRING_DECIMAL_POINT+1;
//	while(count<STRING_MAX-1){
//		digit=((int)(fraction*10));
//		numberAsString[count++]=digit+48;
//		fraction=(fraction*10)-digit;
//	}
//	numberAsString[STRING_MAX-1]='\0';
//	return numberAsString;
//}

//void clearOutput(char* output){
//	int i=0;
//	for(i;i<STRING_MAX;i++){
//		output[i]=' ';
//	}
//}

void calibrateAcc(vector *a) {
	float axMinusb= a->x-119.359558;
	float ayMinusb= a->y+32.662273;
	float azMinusb= a->z-52.508848;
	a->x = (0.966254*axMinusb)+(.016304 * ayMinusb)+ (-0.015231*azMinusb);
	a->y = (0.016304*axMinusb)+(0.951454 * ayMinusb)+ (-0.025227*azMinusb);
	a->z = (-0.015231*axMinusb)+(-0.025227 * ayMinusb)+ (0.955379*azMinusb);
}
void calibrateMag(vector *m) {
	m->x = (1.021436*(m->x+177.557309))+(0.020491 * (m->y-38.149444))+ (-0.009378*(m->z+32.975981));
	m->y = (0.020491*(m->x+177.557309))+(1.032806 * (m->y-38.149444))+ (0.004962*(m->z+32.975981));
	m->z = (-0.009378*(m->x+177.557309))+(0.004962 * (m->y-38.149444))+ (1.173352*(m->z+32.975981));
}
void smoothVector(vector *curr,vector *previos){
	curr->x = previos->x + ((curr->x-previos->x ) * SMOOTH_CONSTANT);
	curr->y = previos->y + ((curr->y-previos->y ) * SMOOTH_CONSTANT);
	curr->z = previos->z + ((curr->z-previos->z ) * SMOOTH_CONSTANT);
	previos->x=curr->x;
	previos->y=curr->y;
	previos->z=curr->z;
}
