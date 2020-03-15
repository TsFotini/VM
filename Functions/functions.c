#include "header.h"

void Sem_Up(struct sembuf oper,int semNum, int semid){
	oper.sem_flg  = 0;
	oper.sem_num  = semNum;
	oper.sem_op   = 1;				// This value is the one that turn on the semaphore
	if(semop(semid, &oper, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
}

void Sem_Down(struct sembuf oper,int semNum, int semid){
	oper.sem_flg  = 0;
	oper.sem_num  = semNum;
	oper.sem_op   = -1;			// This tells us that the semaphore is down
	if(semop(semid, &oper, 1) == -1)
	{
		perror("semop");
		exit(1);
	}
}

int switchTurn(int turn){
	if(turn == 2)
		return 1;
	else 
		return 2;
}