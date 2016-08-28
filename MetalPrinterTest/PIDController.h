#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


#define HANDLE_LOOP_ERRORS 180
#define NEGATIVE_PID 1
#define POSITIVE_PID 2
#define VEHICLE_STABLE 15
#define MAX_TAKE_CHANGE_TIME 20
#define AMOUNT_OF_TIME_BEFORE_CHECK_PID 12000//12000/60sec/10persec=20min
#define AMOUNT_OF_TIME_FOR_STABLE_PID_CONSTANTS 1800//1800/60sec/10persec=3min
//once a whole iterations readings have been read after allowedtolerance turned off try to autotune
#define AMOUNT_OF_TIME_BEFORE_AUTUNE_CHECK_START AMOUNT_OF_TIME_BEFORE_CHECK_PID+ITERATIONS
#define DERIVATIVE_SMOOTHING	0.50f//0.76f//0.15f//0.50f//0.60f
#define ERROR_SMOOTH_FOR_FEEDBACK 0.016f


//extern int pidPrior;
//extern int pidTaskTime;
extern char stabilized;
extern char previoslyChanged;
extern char changeTime;
extern char vehicleStable;
extern char runPID;
extern float derivativeSmooth;
//extern int tillerStable;
extern char startKd;
extern char KdCount;
extern unsigned short pidConstantsStable;
extern unsigned short getStableReadingCount;
extern float errorAvg;
typedef struct {
	float lastHeading;
	float integral; //pid intral variable
	int posKp, posKi, posKd, posID; //pid constants
	int negKp,negKi,negKd,negID;
	float out;
	float lastPIDout;
	float derivative;
} pidCon;
extern pidCon PID;

void initPID(void);
void modPID_task(void);
char motorChatter(float currentPIDout);
char isStable(float pidOut,float error);
void handleNewPIDConstants(char PIDtype);
void storeErrorSignChanges(float error);
void storePIDHistory(float error);
void storePIDConstantStableData(void);

#endif  
