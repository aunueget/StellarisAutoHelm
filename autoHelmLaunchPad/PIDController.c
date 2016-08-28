
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "Globals.h"
#include "PIDController.h"
#include "AutoTunePID.h"
#include "AutoHelm.h"
#include "Klist.h"

//Stable at <3500 WAVY 1
//DERIVATIVE_SMOOTHING	0.25f
//pidOut=abs((int)(PID.out)/30.0f);
//HEADING_SMOOTH_CONSTANT 0.3f
//NOISE_VALUE_SIGMA 200
//INTEGRAL ZEROED
//Before derivative jumper on non zero decrease
//Stable at <=65 WAVY 3
//DERIVATIVE_SMOOTHING	0.15f
//pidOut=abs((int)(PID.out)/30.0f);
//HEADING_SMOOTH_CONSTANT 0.3f
//NOISE_VALUE_SIGMA 200
//INTEGRAL ZEROED
//Before derivative jumper on non zero decrease
//Stable at <=34 WAVY 5  ALSO <3600
//DERIVATIVE_SMOOTHING	0.1f
//pidOut=abs((int)(PID.out)/30.0f);
//HEADING_SMOOTH_CONSTANT 0.3f
//NOISE_VALUE_SIGMA 200
//INTEGRAL ZEROED
//Before derivative jumper on non zero decrease


pidCon PID;
float derivativeSmooth;
//int pidPrior;
//int pidTaskTime;
char stabilized;
char previoslyChanged;
char changeTime;
char vehicleStable;
float errorAvg;
char runPID;
//int tillerStable;
char KdCount;
//this is to turn off autotune while useing allowed tolerance after this count in 10ths of a second
unsigned short pidConstantsStable;
//this is to get turn off allowed tolerance to get normal readings in prep for autune check
//to see if new constants are needed
unsigned short getStableReadingCount;

void initPID(void){
	unsigned long ulPeriod=0;
	//pidPrior=1;
	//pidTaskTime=100;
	//init PID variables
	PID.lastHeading=-1;
	PID.integral=0;//PID intral variable
	PID.posKp=12;
	PID.posKi=0;//.08;//.00001;
	PID.posKd=25;
	PID.posID=-1;
	PID.negKp=12;
	PID.negKi=0;//.08;//.00001;
	PID.negKd=25;
	PID.negID=-1;
	PID.out=0;
	stabilized=FALSE;
	previoslyChanged=FALSE;
	PID.lastPIDout=0;
	PID.derivative=0;
	changeTime=0;
//	tillerStable=0;
	vehicleStable=0;
	KdCount=0;
	pidConstantsStable=0;
	getStableReadingCount=0;
	runPID=FALSE;
	derivativeSmooth=DERIVATIVE_SMOOTHING;
	initKlist();

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	ROM_SysCtlDelay(100);
	ROM_TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	//set timer for 10 times a second
	ulPeriod = (ROM_SysCtlClockGet() / 10);
	ROM_TimerLoadSet(TIMER3_BASE, TIMER_A, ulPeriod - 1);

	ROM_IntEnable(INT_TIMER3A);
	ROM_TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

	ROM_IntMasterEnable();
	ROM_TimerEnable(TIMER3_BASE, TIMER_A);
	ROM_IntMasterDisable();

}
void pidTaskTimer(void){
	ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	readCompass=TRUE;
}
/*****************************************************************************
Name:         AD_sample_task 
Parameters:                     
Returns:        
Description:  calculates the pidOutput

EXTRA CODE FOR TILT
//	if(useTilt){
//		error=degreeDifferance(0,heading.desiredTilt,heading.tilt);
//		if(++PID.KdCount>3){
//			PID.dterm=(degreeDifferance(0,heading.tilt,PID.dvar))/3.0;
//			PID.dvar=heading.tilt;
//			PID.KdCount=0;
//		}
//	}
*****************************************************************************/
void modPID_task(void)
{
	float error=0;
	float currDerivative=0;
	//keep track of whether PID Constants have reached stable for turning off autotune
	storePIDConstantStableData();
	//calculate error
	error=degreeDifferancef(FALSE,(float)heading.desired,heading.current);
	//if first time we have calculated derivative set it to no movement so derivative will come out zero
	if(PID.lastHeading==-1){
		PID.lastHeading=heading.current;
	}
	//calculate derivative
	currDerivative=degreeDifferancef(FALSE,PID.lastHeading,heading.current);
	PID.derivative=PID.derivative + (derivativeSmooth * (currDerivative-PID.derivative));
	//set lastHeading to be used to calculate derivative next time
	PID.lastHeading=heading.current;
	//reset integral to zero when crossing desired heading
	if(error*PID.integral<0){
		PID.integral=0;
	}
	else{
		//calculate integral
		PID.integral = (PID.integral + error);
	}
	//TODO: last thing i did was up Ki value by a 10 factor
	//if error is positive use positive pid constants
	if(error>=0){
		PID.out=(((float)PID.posKp)*error) + (((float)(PID.posKi/1000.0))*PID.integral) + (((float)PID.posKd)*PID.derivative);
		if(posList.testPID.id==PID.posID){
			posList.testPID.sec++;
		}
		pidData.posDataCount++;
	}
	//error is negative use negative pid constants
	else{
		PID.out=(((float)PID.negKp)*error) + (((float)(PID.negKi/1000.0))*PID.integral) + (((float)PID.negKd)*PID.derivative);
		if(negList.testPID.id==PID.negID){
			negList.testPID.sec++;
		}
		pidData.negDataCount++;
	}
	//TODO: test changing this from < to >=
	if(pidData.currCount>=ITERATIONS){
		pidData.currCount=0;
		pidData.resetCount++;
	}
	//store data for keeping track of negative and positive error
	storeErrorSignChanges(error);
	//store pid output and error for autotune
	storePIDHistory(error);
	//if we have reached close to zero error for a while don't run the motor
	if(isStable(PID.out, error)){
		PID.out=0;
	}
	//if we under normal operation check for back and forth chatter(motor reversing) and autotune
	else if(motorChatter(PID.out)){
		autotunePID();
	}
	//if motor chattering(reversing) autotune and turn off motor
	else{
		autotunePID();
		PID.out=0;
	}
}

char isStable(float pidOut,float error){
	if(allowedTolerance>fabs(error)&&stabilized&&pidConstantsStable>AMOUNT_OF_TIME_FOR_STABLE_PID_CONSTANTS&&getStableReadingCount==0){
		PID.integral=0;
		return TRUE;
	}
	else if(stabilized){
		stabilized=FALSE;
		vehicleStable=0;
//		tillerStable=0;
	}
	if(fabs(error)<1){
		vehicleStable++;
	}
	else{
		vehicleStable=0;
	}		
//	if(pidOut<30){
//		tillerStable++;
//	}
//	else{
//		tillerStable=0;
//	}
	if(vehicleStable>VEHICLE_STABLE){//(tillerStable>15&&vehicleStable>30)
		stabilized=TRUE;
		vehicleStable=VEHICLE_STABLE+1;
		return TRUE;
	}
	return FALSE;
}
char motorChatter(float currentPIDout){
	if(previoslyChanged){
		if(PID.lastPIDout*currentPIDout<0){
			previoslyChanged=FALSE;
			PID.lastPIDout=currentPIDout;
			changeTime=0;
			return TRUE;
		}
		else if((PID.lastPIDout>=0&&PID.lastPIDout<currentPIDout)||(PID.lastPIDout<0&&PID.lastPIDout>currentPIDout)){
			previoslyChanged=FALSE;
			PID.lastPIDout=currentPIDout;
			changeTime=0;
			return TRUE;
		}
		//2 second delay reset
		else if(changeTime++>MAX_TAKE_CHANGE_TIME){
			changeTime=0;
			previoslyChanged=FALSE;
			PID.lastPIDout=currentPIDout;
			return TRUE;
		}
	}
	else if(PID.lastPIDout*currentPIDout>=0){
		previoslyChanged=FALSE;
		PID.lastPIDout=currentPIDout;
		changeTime=0;
		return TRUE;
	}
	else{
		previoslyChanged=TRUE;
	}
	PID.lastPIDout=currentPIDout;
	return FALSE;
}
void handleNewPIDConstants(char PIDtype){
	//reset integral to smooth transition to new ki,kp,kd
	PID.integral=0;
	//reset derivative to smooth transition to new ki,kp,kd
	PID.lastHeading=heading.current;
	PID.derivative=0;
	//get random kp,ki,kd
	//System.out.println("Old-----------------"+this.Kp+","+this.Ki+","+this.Kd);
	if(NEGATIVE_PID==PIDtype){
		PID.negKd=negList.testPID.d;
		PID.negKi=negList.testPID.i;
		PID.negKp=negList.testPID.p;
		PID.negID=negList.testPID.id;
	}
	else{
		PID.posKd=posList.testPID.d;
		PID.posKi=posList.testPID.i;
		PID.posKp=posList.testPID.p;
		PID.posID=posList.testPID.id;
	}
	pidConstantsStable=0;
	getStableReadingCount=0;
	//keep track of the ones we have used
	// pidData.Kd[pidData.currKSet]=this.Kd;
	// pidData.Ki[pidData.currKSet]=this.Ki;
	// pidData.Kp[pidData.currKSet]=this.Kp;
	//System.out.println("New-----------------"+this.Kp+","+this.Ki+","+this.Kd);

	//pidData.currKSet++;

	//tell it we are ready to try a new set
//	pidData.isReset=FALSE;
}
void storeErrorSignChanges(float error){
	if((((float)pidData.lastError+0.001f)*error)<0){
		if(pidData.errorChangeLast+1>=ITERATIONS){
			pidData.errorChangeLast=0;
			pidData.resetErrorChange=TRUE;
		}
		else{
			pidData.errorChangeLast++;
		}
		pidData.errorSignedCount[pidData.errorChangeLast]=1;
	}
	else{
		pidData.errorSignedCount[pidData.errorChangeLast]++;
	}
	if(pidData.resetCount>0){
		if(pidData.y[handlePIDDataLoop(pidData.currCount-1)]>=HANDLE_LOOP_ERRORS){
			pidData.posDataCount--;
		}
		else{
			pidData.negDataCount--;
		}
		if(pidData.errorChangeFirst<=1){
			pidData.errorChangeFirst=handlePIDDataLoop(pidData.errorChangeFirst+1);
		}
		else{
			pidData.errorChangeFirst--;
		}
	}
}
void storePIDHistory(float error){
	pidData.lastError=(int)error;
	pidData.u[pidData.currCount]=(int)(PID.out);
	pidData.y[pidData.currCount]=(unsigned short)HANDLE_LOOP_ERRORS+error;
	pidData.currCount++;
}
void storePIDConstantStableData(){
	pidConstantsStable++;
	if(pidConstantsStable>AMOUNT_OF_TIME_BEFORE_CHECK_PID){
		if(pidConstantsStable>AMOUNT_OF_TIME_BEFORE_AUTUNE_CHECK_START){
			getStableReadingCount=0;
			pidConstantsStable=AMOUNT_OF_TIME_FOR_STABLE_PID_CONSTANTS+1;
		}
		else{
			getStableReadingCount++;
		}
	}
}
