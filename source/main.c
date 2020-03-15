#include "header.h"

int main (int argc, char **argv){

	int i;
	pid_t childPID;
	int status;

	//k, frames, q, max
	if(argc != 5){
		printf("Wrong Arguments! Try again!\n");
		return -1;
	}


	int k = atoi(argv[1]);
	int frames = atoi(argv[2]);
	int q = atoi(argv[3]);
	int max = atoi(argv[4]);

	if(k > frames/2)
	{
		printf("wrong value k=%d\n",k);
		return -1;
	}

	if(q >= max)
	{
		q = max/2;
	}

	//Creating shared memory

	int shmid, semid;

	if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS | IPC_CREAT)) < 0) 
	{
		perror("shared memory in main ");
		exit(1);
	}

	requests *shmem;
	
	shmem=shmat(shmid,NULL,0);

	//Creating the set of semaphores

	union semnum arg;

	if ((semid = semget(SEMKEY, 3, PERMS  | IPC_CREAT)) <0) 
	{
		perror("Semaphores in main ");
		exit(1);
	}


	arg.val=0;
	semctl(semid, 0, SETVAL, arg);				//This is the semaphore for PM1

	semctl(semid, 1, SETVAL, arg);				//Semphore of PM2

	semctl(semid, 2, SETVAL, arg);				//Semaphore for MM


	for( i = 0; i < PROCNUM; i++){		
		childPID = fork();	
		if(childPID == 0){
			switch(i){
				case 0:
				execlp("./PM","./PM.c", "0", argv[4], "gcc.trace", "PM1", (char *) NULL);
				break;	
				case 1:
				execlp("./PM","./PM.c", "1", argv[4], "bzip.trace", "PM2", (char *) NULL);
				break;	
				case 2:
				execlp("./MM","./MemoryManager.c",argv[1], argv[2], argv[3] , argv[4], (char *) NULL);
				break;	
			}
		}
	}

	for(i = 0; i < PROCNUM; i++){
		wait(&status);
	}

	shmdt(shmem);

	if (shmctl(shmid, IPC_RMID, (struct shmid_ds *)0 ) <0) 
	{
		perror("Error with semctl");
		return -1;
	}

	if (semctl(semid, 3, IPC_RMID, 0) <0 ) 
	{ 
		perror("Error with semctl");
		return -1;
	}
	return 0;
}