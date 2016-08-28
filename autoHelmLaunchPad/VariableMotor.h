#ifndef INTERUPTS_H
#define INTERUPTS_H

//variables
extern char toggleMotorOn;
extern float pid2MotorDivisor;
extern float percentNeg;

//methods
void delay_ticker(void);
void motorToggle(void);
void motorKill(void);
void startMotorToggle(void);
void stopMotorToggle(void);
void initVariableMotor(void);
void turnOffPWM(void);
void turnOnPWM(void);

#endif  
