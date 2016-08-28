/*
 * Klist.c
 *
 *  Created on: Dec 13, 2013
 *      Author: dell
 */
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "Klist.h"
#include "PIDController.h"
#include "Globals.h"

#define MIN_TIME_SEC 20
#define EXISTS_DONT_REPLACE -5
#define DOESNT_EXIST_ADD -1

//int currListLength;
//char useNext;
//struct Kpid testPID;
//struct Kpid testedList[MAX_SIZE_OF_TESTED_LIST];
struct Klist posList;
struct Klist negList;
//int shortest;
//int testKpidCount;
//char tryagain;
int kSetsTryed;

void initKlist(){
	kSetsTryed=0;
	//pos
	posList.shortest = MIN_TIME_SEC;
	posList.testKpidCount = 0;
	posList.currListLength = 0;
	posList.useNext=0;
	posList.tryagain=FALSE;
	posList.testPID.id=kSetsTryed;
	posList.testPID.p=PID.posKp;
	posList.testPID.i=PID.posKi;
	posList.testPID.d=PID.posKd;
	posList.testPID.sec=0;
	//neg
	negList.shortest = MIN_TIME_SEC;
	negList.testKpidCount = 0;
	negList.currListLength = 0;
	negList.useNext=0;
	negList.tryagain=FALSE;
	negList.testPID.id=kSetsTryed;
	negList.testPID.p=PID.negKp;
	negList.testPID.i=PID.negKi;
	negList.testPID.d=PID.negKd;
	negList.testPID.sec=0;
	/* initialize random seed: */
	ROM_IntMasterDisable();
	srand (time(NULL));
}
int getcurrListLength(struct Klist *tempList){
	return tempList->currListLength;
}
char addKpid(struct Klist *tempList, struct Kpid *tempKpid){//int id, int pval, int ival, int dval, int sec) {//, char add_to_end
	int insert = findInsert(tempList, tempKpid->sec);
    if(insert<MAX_SIZE_OF_TESTED_LIST){
		shiftUpFor(tempList, insert);
		tempList->testedList[insert].id = tempKpid->id;
		tempList->testedList[insert].p = tempKpid->p;
		tempList->testedList[insert].i = tempKpid->i;
		tempList->testedList[insert].d = tempKpid->d;
		tempList->testedList[insert].sec = tempKpid->sec;
		if(tempList->currListLength<MAX_SIZE_OF_TESTED_LIST){
			tempList->currListLength++;
		}
    }
    if(tempList->currListLength >= MAX_SIZE_OF_TESTED_LIST ) {
    	tempList->shortest = tempList->testedList[tempList->currListLength-1].sec;
    }
    return insert;
}
void shiftUpFor(struct Klist *tempList, int position){
	int moveListPos=tempList->currListLength;
	while(moveListPos>position){
		if(moveListPos<MAX_SIZE_OF_TESTED_LIST){
			tempList->testedList[moveListPos].id = tempList->testedList[moveListPos-1].id;
			tempList->testedList[moveListPos].p = tempList->testedList[moveListPos-1].p;
			tempList->testedList[moveListPos].i = tempList->testedList[moveListPos-1].i;
			tempList->testedList[moveListPos].d = tempList->testedList[moveListPos-1].d;
			tempList->testedList[moveListPos].sec = tempList->testedList[moveListPos-1].sec;
		}
		moveListPos--;
	}
}
void shiftDownFor(struct Klist *tempList, int position){
	while(position<tempList->currListLength-1){
		tempList->testedList[position].id = tempList->testedList[position+1].id;
		tempList->testedList[position].p = tempList->testedList[position+1].p;
		tempList->testedList[position].i = tempList->testedList[position+1].i;
		tempList->testedList[position].d = tempList->testedList[position+1].d;
		tempList->testedList[position].sec = tempList->testedList[position+1].sec;
		position++;
	}
	tempList->currListLength--;
}
int inListAlreadyAdd(struct Klist *tempList,struct Kpid *tempKpid){//int id, int pval, int ival, int dval, int sec) {
	int currPos=0;
    while(currPos<tempList->currListLength) {
    	if(tempKpid->id==tempList->testedList[currPos].id){
    		if(tempKpid->sec>tempList->testedList[currPos].sec){
    			shiftDownFor(tempList, currPos);
    			addKpid(tempList, tempKpid);//id,pval,ival,dval,sec);
    		}
        	return currPos;
    	}
    	currPos++;
    }
    return DOESNT_EXIST_ADD;
}

void print_list(struct Klist *tempList) {
	int currPos=0;
    UARTprintf("\n -------Printing list Start------- \n");
    while(currPos<tempList->currListLength) {
        UARTprintf("\n id[%d],p[%d],i[%d],d[%d],sec[%d]\n",tempList->testedList[currPos].id, tempList->testedList[currPos].p, tempList->testedList[currPos].i, tempList->testedList[currPos].d,tempList->testedList[currPos].sec );
        currPos++;
    }
    UARTprintf("\n -------Printing list End------- \n");
    return;
}

char findInsert(struct Klist *tempList,int sec) {
	int currPos=0;
    if(0 == tempList->currListLength) {
        return 0;
    }
    while(currPos<tempList->currListLength && tempList->testedList[currPos].sec > sec) {
    	currPos++;
    }
    return currPos;
}

void currentTestKpidFAILED(struct Klist *tempList){//int id, int pval, int ival, int dval, int sec){
	int inListAt=inListAlreadyAdd(tempList, &(tempList->testPID));//id,pval,ival,dval,sec);
	if(inListAt==DOESNT_EXIST_ADD){
		if(tempList->testPID.sec>tempList->shortest){
			addKpid(tempList, &(tempList->testPID));//id,pval,ival,dval,sec);
		}
	}
//	else{
//		tempList->useNext=inListAt+1;
//	}
	selectNextKpid(tempList);
}
/*
 * If we have tried all previosly stored Kpid's
 * then get a random new one
 */
void selectNextKpid(struct Klist *tempList){
	//TODO: come up with a way to not reuse the one just added if added in the useNext position
	if(tempList->useNext<tempList->currListLength){
		tempList->testKpidCount++;
		tempList->testPID.p=tempList->testedList[tempList->useNext].p;
		tempList->testPID.i=tempList->testedList[tempList->useNext].i;
		tempList->testPID.d=tempList->testedList[tempList->useNext].d;
		tempList->testPID.id=tempList->testedList[tempList->useNext].id;
		tempList->testPID.sec=0;
		tempList->useNext++;
	}
	else{
		tempList->testPID.p=(int)(rand()%600);
		tempList->testPID.i=(int)(rand()%600);
		tempList->testPID.d=(int)(rand()%12000);
		tempList->testPID.id=++kSetsTryed;
		tempList->testPID.sec=0;
		tempList->useNext=0;
	}
}

//int main(void) {
//    int i = 0, ret = 0;
//    struct Kpid *ptr = NULL;
//    print_list();
//    for(i = 5; i<10; i++)
//        addKpid(i,TRUE);
//    print_list();
//    for(i = 4; i>0; i--)
//        addKpid(i,false);
//    print_list();
//    for(i = 1; i<10; i += 4) {
//        ptr = search_in_list(i, NULL);
//        if(NULL == ptr) {
//            UARTprintf("\n Search [val = %d] failed, no such element found\n",i);
//        }
//        else {
//            UARTprintf("\n Search passed [val = %d]\n",ptr->val);
//        }
//        print_list();
//        ret = delete_from_list(i);
//        if(ret != 0) {
//            UARTprintf("\n delete [val = %d] failed, no such element found\n",i);
//        }
//        else {
//            UARTprintf("\n delete [val = %d] passed \n",i);
//        }
//        print_list();
//    } return 0;
//}

