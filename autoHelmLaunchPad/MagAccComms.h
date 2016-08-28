

#ifndef MAGACCCOMMS_h
#define MAGACCCOMMS_h


/*
*********************************************************************************************************
*                                         TYPES DEFINED
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         variables
*********************************************************************************************************
*/
enum{FIRST_I2C,LAST_I2C,SINGLE_I2C,SINGLE_RECEIVE_I2C,MULTI_RECEIVE_I2C};
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

extern void MyI2C_send(unsigned char address,unsigned char number,char sendType);
extern void MyI2C_requestFrom(unsigned char address, char sendType,int count);
extern unsigned char MyI2C_receive(int pos);
extern int MyI2C_available();
extern void initI2Ccomms(void);
#endif
