/*
 * AutoTunePID.c
 *
 *  Created on: Mar 13, 2013
 *      Author: dell
 */

#include "AutoTunePID.h"
#include "PIDController.h"
#include "Globals.h"
#include "Klist.h"


#define NOISE_VALUE_SIGMA 200


//TODO:uncomment this for autune testing
PIDData pidData;
//PIDData pidData = {
//	{110,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,112,112,112,112,112,112,112,113,113,113,113,113,114,114,114,114,115,115,115,115,116,116,116,116,117,117,117,118,118,118,119,119,119,120,120,120,121,121,121,122,122,122,123,123,123,124,124,124,124,125,125,125,126,126,126,127,127,127,128,128,128,128,129,129,129,130,130,130,131,131,131,131,132,132,132,133,133,133,134,134,134,135,135,135,135,136,136,136,137,137,137,138,138,138,139,139,139,139,140,140,140,141,141,141,142,142,142,142,143,143,143,144,144,144,145,145,145,146,146,146,147,147,147,148,148,148,149,149,149,150,150,150,151,151,151,152,152,153,153,153,154,154,154,155,155,155,156,156,156,157,157,157,158,158,158,159,159,160,160,160,161,161,161,162,162,162,163,163,163,164,164,164,165,165,166,166,166,167,167,167,168,168,168,169,169,169,170,170,170,171,171,171,172,172,172,173,173,173,174,174,174,174,175,175,175,175,176,176,176,176,177,177,177,177,177,178,178,178,178,178,178,178,179,179,179,179,179,179,179,179,179,179,180,180,180,180,180,180,180,180,180,180,180,181,181,181,181,181,181,181,181,181,181,181,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,183,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182,182},
//	{-1050,-1035,-1035,-1035,-13042,-26980,-12975,-13386,-3036,-10697,-25187,-30363,-30367,-30370,-30374,-18768,-24703,-3403,-11563,-16118,-19380,-31688,-33050,-27268,-6502,-21912,-21915,-21918,-21921,-21598,-23828,-28843,-15182,-18136,-10921,-27236,-19502,-30656,-7296,-6979,-12096,-5796,-25326,-25327,-24926,-24927,-24927,-24526,-24527,-24456,-24055,-24056,-24056,-23655,-23584,-23585,-23184,-23185,-23185,-22784,-22785,-22786,-22384,-22385,-22457,-22458,-22057,-22057,-22058,-21585,-21586,-21586,-21185,-21186,-21258,-20857,-20857,-20858,-20859,-20386,-20386,-20387,-19986,-19986,-20059,-19657,-19658,-19658,-19659,-19186,-559,-17943,-19633,-6598,-25899,-17250,-7039,-17390,-19711,-20341,-2790,-4996,-6558,-18832,-4621,-19565,-4129,-14663,-14322,-14322,-14322,-13981,-11563,-9143,-19477,-8961,-10081,-20441,-17668,-23128,-2691,-18089,-8779,-14555,-8741,-17279,-17538,-8029,-15733,-4825,-1549,-18165,-14875,-17780,-3060,-17613,-8637,-15345,-7030,-8778,-9056,-18209,-15169,-3100,-4713,-10231,-14101,-1860,-4620,-14210,-4061,-13398,-1512,-8877,-2998,-1054,-6804,-14352,-13494,-4836,-4000,-12075,-13400,-13056,-6648,-8569,-3818,-5290,-10396,-2662,-8515,-2816,-2226,-5208,-3740,-7000,-5720,-5320,-10659,-2641,-8154,-5994,-7452,-7446,-544,-8687,-1760,-2096,-8944,-3630,-8250,-6272,-896,-7896,-6500,-13,-14,-13,-14,-15,-14,-15,-15,-15,4133,4133,4133,4133,4132,4133,4132,4132,4133,4132,2057,2058,2058,2058,2057,4133,4132,4132,4132,4133,2058,2058,2057,2058,2058,2058,2058,2058,2059,2059,2058,2058,2058,2058,2058,2059,2059,2059,2059,2059,-15,-15,-15,-15,-15,2060,2060,2060,2060,2060,-14,-14,-14,-14,-14,-14,-13,-13,-13,-13,2061,2061,2061,2061,2061,-13,-12,-11,-11,-11,2062,2063,2063,2063,2063,-11,-11,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-9,-9,-8,2066,2066,2066,2066,2066,-7,-7,-7,-7,-7,-7,-6,-6,-6,-6,-6,-6,-6,-5,-6,-2081,-2081,-2081,-2080,-2080,-6,-6,-6,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6},
//	0,
//	0,
//	0,
//	0
//};
float sigma;
int rho;
char startCalcTspec;
void initAutoTune(){
	sigma=NOISE_VALUE_SIGMA;
	rho=0;
	//PID data init
	pidData.currCount=0;
	pidData.resetCount=0;
	pidData.errorChangeLast=0;
	pidData.errorChangeFirst=0;
	pidData.currErrorChangePos=0;
	pidData.lastError=0;
	pidData.isReset=TRUE;
	pidData.resetErrorChange=FALSE;
	pidData.posDataCount=0;
	pidData.negDataCount=0;
	startCalcTspec=FALSE;

}
//start one past the first position because rFiction needs one previous values
double calcFictiousR(int sVal,int position,int Kp,float Ki,int Kd){
	//TODO: what about across 360 to 0 is this going to create bad calculations
	//TODO: use degree difference
	// if(position>0&&position<pidData.currCount){
		// return ((Kd*pidData.y[position])
					// +(Kd*(pidData.r[position-1]-pidData.y[position-1]))
					// +(Ki*(pidData.y[position]-pidData.integral[position-1]))
					// +(Kp*pidData.y[position])
					// +pidData.u[position])
				// /(Kd*Ki*Kp);
	// }
	// return 0;
	Ki+=0.0000000001;
	return (pidData.y[position-1]
		+((sVal/((sVal*Kp)+Ki))*(pidData.u[position]+(Kd*(pidData.y[position-1])))));

}
int calcTspec(short int checkFirst, short int checkAmount,int Kp,float Ki,int Kd){
	//TODO:make it so that the kp,ki,kd don't get changed until one of the new
	//chosen Kp,Ki,Kd pass on the current saved r,u,y pid data
	//TODO:part of new pos neg PID change
	int start=checkFirst+1;//1;
	int oppisiteStart=handlePIDDataLoop(checkFirst+checkAmount);//handlePIDDataLoop(pidData.currCount-1);
	//TODO:end of part of new pos neg PID change
	//int allowedLoops=handlePIDDataLoop(pidData.currCount-1);
	int i=0;
	double ans=0;
	double convoW2U=0;
	double convoW1Error=0;
	int sumR=0;
	double rFiction=0;
//	if(pidData.resetCount>0){
//		start=handlePIDDataLoop(pidData.currCount+2);
//		oppisiteStart=pidData.currCount;
//		//allowedLoops=ALLOWED_CALCS_PER_RUN;
//	}
	//start i one past the first because rFiction needs one previous values
	for(i=0;i<checkAmount/*handlePIDDataLoop(start+i)!=pidData.currCount&&tSpecCalcCount<ITERATIONS&&*/;i++){//i<ITERATIONS-1;i++){
		rFiction=calcFictiousR(i,handlePIDDataLoop(start+i),Kp,Ki,Kd);
		convoW1Error += productW1Error(i,handlePIDDataLoop(oppisiteStart-i),rFiction);
		convoW2U += prouductW2U(i,handlePIDDataLoop(oppisiteStart-i));
		sumR+=(rFiction*rFiction);
		ans=convoW1Error+convoW2U+(sigma*sigma)-sumR;
		if(ans<=0){
			pidData.isReset=TRUE;
			return i;
		}
	}
	return pidData.errorSignedCount[pidData.currErrorChangePos];//ITERATIONS;//(ITERATIONS+5);
}
//formula for w1
double calcWOne(int s){
	return (s+10)/(10*(s+0.1));//(s+10)/(10*(s+0.1));//(s+10)/(2*(s+0.1));
}
//formula for w2
double calcWTwo(int s){
	return 0.1/(1.2*((s+1)*(s+1)*(s+1)));//0.1/((1.2*(s+1))*(1.2*(s+1))*(1.2*(s+1)));
}
//convo of the ficticious error (rFiction-y) with w1
double productW1Error(int sVal,int timeXminus,double rFiction){
	//TODO:Determine if time value in W one and W two are correct
	//or if they are not going from 1 to 600 but (wrongly going) 30 passed 600 to 29
	double w1=calcWOne(sVal);
	//TODO: what about across 360 to 0 is this going to create bad calculations
	//TODO: use degree difference
	double error = degreeDifferancef(FALSE,rFiction,pidData.y[timeXminus]);//rFiction-pidData.y[timeXminus];
	double absConvo=abs(w1*error);
	return (absConvo*absConvo);//squared
}
//convo of the pid output with w2
double prouductW2U(int sVal, int timeXminus){
	//TODO:Determine if time value in W one and W two are correct
	//or if they are not going from 1 to 600 but (wrongly going) 30 passed 600 to 29
	double w2=calcWTwo(sVal);
	double absConvo=abs(w2*pidData.u[timeXminus]);
	return (absConvo*absConvo);//squared
}
int handlePIDDataLoop(int position){
	if(position>ITERATIONS){
		position-=ITERATIONS;
	}
	else if(position<0){
		position+=ITERATIONS;
	}
	return position;
}
void autotunePID(){
	short int posTestGoodCount=0;
	short int negTestGoodCount=0;
	short int errorChangePos=pidData.errorChangeFirst;
	int i=0;
	int testRunTime=0;
	if(pidData.resetCount>0){
		i=handlePIDDataLoop(pidData.currCount+1);
	}
	if(pidConstantsStable<=AMOUNT_OF_TIME_FOR_STABLE_PID_CONSTANTS||allowedTolerance==0||getStableReadingCount>ITERATIONS){
		for(i=0;i<ITERATIONS;i++){
			if(pidData.y[i]>=HANDLE_LOOP_ERRORS){
				testRunTime=calcTspec(i,pidData.errorSignedCount[errorChangePos],posList.testPID.p,(float)(posList.testPID.i/1000.0),posList.testPID.d);
				posTestGoodCount+=testRunTime;
				//TODO: figure out how to count seconds tested good how to add to or increment
				if(posList.testPID.id==PID.posID&&posTestGoodCount>posList.testPID.sec){
					posList.testPID.sec=posTestGoodCount;
				}
				else if(posList.testPID.sec>pidData.posDataCount){
					posList.testPID.sec++;
				}
				else{
					posList.testPID.sec=posTestGoodCount;
				}
				if(testRunTime!=pidData.errorSignedCount[pidData.currErrorChangePos]){
					currentTestKpidFAILED(&posList);//.id,testPID.p,testPID.i,testPID.d,testPID.sec);
					posTestGoodCount=0;
				}
				i=handlePIDDataLoop(i+testRunTime);
			}
			else{
				testRunTime=calcTspec(i,pidData.errorSignedCount[errorChangePos],negList.testPID.p,(float)(negList.testPID.i/1000.0),negList.testPID.d);
				negTestGoodCount+=testRunTime;
				if(negList.testPID.id==PID.negID&&negTestGoodCount>negList.testPID.sec){
					negList.testPID.sec=negTestGoodCount;
				}
				else if(negList.testPID.sec>pidData.negDataCount){
					negList.testPID.sec++;
				}
				else{
					negList.testPID.sec=negTestGoodCount;
				}
				if(testRunTime!=pidData.errorSignedCount[pidData.currErrorChangePos]){
					currentTestKpidFAILED(&negList);//.id,testPID.p,testPID.i,testPID.d,testPID.sec);
					negTestGoodCount=0;
				}
				i=handlePIDDataLoop(i+testRunTime);
			}
			errorChangePos++;
		}
		//TODO: make sure that testPID.sec can be greater than or equal to pidData.currCount
		//		when resetCount==0
		if(negList.testPID.id!=PID.negID&&(negList.testPID.sec>=pidData.negDataCount)){
			handleNewPIDConstants(NEGATIVE_PID);
		}
		if(posList.testPID.id!=PID.posID&&(posList.testPID.sec>=pidData.posDataCount)){
			handleNewPIDConstants(POSITIVE_PID);
		}
	}
}
