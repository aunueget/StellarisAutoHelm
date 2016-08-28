/*
 * MotorDrive.h
 *
 *  Created on: Dec 22, 2012
 *      Author: dell
 */

#ifndef MOTORDRIVE_H_
#define MOTORDRIVE_H_


#define REVERSE_MOTOR_COUNTDOWN 2

enum {
	ALLIN, ALLOUT, MIDDLE
};
enum {
	DRIVING_IN,
	DRIVING_OUT
};

extern char reverseMotorCount;
extern char reverseMotorDir;
extern char allInOutCount;
//extern short int lastMotorDriven;

void initMotor(void);
void turnOffMotor(void);

void turnOffMotor(void);
void turnOnMotor(void);
void check4ShaftNOut(void);
void initMotorEndsOnly(void);
void motorDrive_WhileForever(void);
char driveMotor(char dir,char checkPosition);
void setMotorDirection(char dir);

#endif /* MOTORDRIVE_H_ */
