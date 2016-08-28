/*
 * Klist.h
 *
 *  Created on: Dec 13, 2013
 *      Author: dell
 */

#ifndef KLIST_H_
#define KLIST_H_

#define MAX_SIZE_OF_TESTED_LIST 10
//Kpid used status enum

struct Kpid{
    int id, p, i, d, sec,prevSec;
};
struct Klist{
	struct Kpid testPID;
	struct Kpid testedList[MAX_SIZE_OF_TESTED_LIST];
	int shortest;
	int testKpidCount;
	char tryagain;
	int currListLength;
	char useNext;
};
extern int kSetsTryed;
//extern struct Kpid testPID;
//extern struct Kpid testedList[];
extern struct Klist posList;
extern struct Klist negList;


void initKlist();
int getCurrTestingKpid(struct Klist *tempList);
char addKpid(struct Klist *tempList,struct Kpid *tempKpid);//int id, int pval, int ival, int dval, int sec);//, char add_to_end
void currentTestKpidFAILED(struct Klist *tempList);//int id, int pval, int ival, int dval, int sec);
char findInsert(struct Klist *tempList,int sec);
void selectNextKpid(struct Klist *tempList);
int inListAlreadyAdd(struct Klist *tempList,struct Kpid *tempKpid);//int id, int pval, int ival, int dval, int sec);
void shiftUpFor(struct Klist *tempList,int position);
void shiftDownFor(struct Klist *tempList,int position);
void print_list(struct Klist *tempList);

#endif /* KLIST_H_ */
