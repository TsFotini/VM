#include "header.h"

int main (int argc, char **argv){

	int shmid, semid;
	struct sembuf oper;
	FILE* fp;
	char reference[9];
	char RW;
	int countReferences = 0;

	int semaphore = atoi(argv[1]);
	int max = atoi(argv[2]);

	char filename[10];
	strcpy(filename, argv[3]);

	char typeOfProcess[4];
	strcpy(typeOfProcess, argv[4]);

	if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem in PM");
		exit(1);
	}

	requests *shmem;
	shmem=shmat(shmid,NULL,0);

	if ((semid = semget(SEMKEY, 2, PERMS)) <0) {		//Two semaphores required
		perror("semaphores in PM");
		exit(1);
	}

	if((fp=fopen(filename,"r"))==NULL)
	{ 
		printf("File cannot be opened\n"); 
		exit(-2); 
	}

	fscanf(fp,"%s %c", reference, &RW);
	countReferences++;

	requests req;
	strcpy(req.typeOfProcess, typeOfProcess);
	req.finished = 0;
	
	while(!feof(fp) && countReferences <= max)
	{
		strcpy(req.address, reference);
		req.rw = RW;
		if(countReferences == max)
			req.finished = 1;

		Sem_Down(oper,semaphore,semid);
		memcpy(shmem,&req,sizeof(requests));
		Sem_Up(oper, 2, semid);

		fscanf(fp,"%s %c", reference, &RW);
		countReferences++;
	}

	fclose(fp);
	exit(EXIT_SUCCESS);
}