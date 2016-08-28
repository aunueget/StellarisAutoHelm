/*
 * AutoTunePID.h
 *
 *  Created on: Mar 19, 2013
 *      Author: dell
 */

#ifndef AUTOTUNEPID_H_
#define AUTOTUNEPID_H_

//defines
#define ITERATIONS 450
#define POSSIBLE_KIS 100

//structs
typedef struct {
	//saved pid data r,y at each time
	//rFiction is the calculated r (from y,u,integral)
	//that would give u with the proper Ki, Kd, Kp(i think need to double check this)
	unsigned short y[ITERATIONS];
	//saved pid data
	int u[ITERATIONS];
	unsigned short errorSignedCount[ITERATIONS];
	unsigned short errorChangeLast;
	unsigned short errorChangeFirst;
	unsigned short currErrorChangePos;
	//the current position of the most recent data
	unsigned short currCount;
	//this is the number of times we have stored a full array of
	//pid data and started overwriting from 0 on reading at a time
	int resetCount;
	char resetErrorChange;
	char currKSet;
	int lastError;
	//does the Ki Kp Kd need reset
	char isReset;
	int posDataCount,negDataCount;
} PIDData;

//variables
extern PIDData pidData;
extern char startCalcTspec;
extern float sigma;
//methods
void initAutoTune();
double calcFictiousR(int sVal,int position,int Kp,float Ki,int Kd);
int calcTspec(short int checkFirst, short int checkAmount,int Kp,float Ki,int Kd);
//formula for w1
double calcWOne(int s);
//formula for w2
double calcWTwo(int s);
//convo of the ficticious error (rFiction-y) with w1
double productW1Error(int sVal,int timeXminus,double rFiction);
//convo of the pid output with w2
double prouductW2U(int sVal, int timeXminus);
int handlePIDDataLoop(int position);
void autotunePID(void);
#endif /* AUTOTUNEPID_H_ */
