#include "header.h"
#include "list.h"

#define HASH 15 

typedef struct Statistics{
	int pagefault;
	int pageok;
	int MemoryWriting;
	int MemoryReading;
	int countOfRecords; 
}Statistics;
	
typedef struct PHT{
	typeOfNode* buckets;
}PHT;

typedef struct frameStruct{
	char  frameNumber[6];
    int dirty; //0 den exei ala3ei 1 exei ala3ei
	int setFree; // 1 einai elef8ero 0 einai katelimeno
}frameStruct;

int InsertionAll(PHT *HashTable, int k, frameStruct *memory, requests req, int startMemory, int endMemory, int pf, Statistics *statistics){
	char reference[6];
	phtElement element;

	int i;
	for(i = 0; i < 5; i++){
		reference[i] = req.address[i];
	}

	reference[5] = '\0';

	// printf( "%c %s %s\n", req.rw, reference, req.typeOfProcess);

	int proBucket;
	proBucket = ((int)reference[0]) + ((int)reference[1]) + ((int)reference[2]) + ((int)reference[3]) + ((int)reference[4]);
	
	int numBucket;
	numBucket = proBucket % HASH;
	int isExist = Search(HashTable->buckets[numBucket], reference, &element, req.typeOfProcess);

	if(isExist == 0){
		statistics->pagefault++;
		pf++;

		if(pf == k + 1){
			printf("Flush memory!\n");
			for(i = startMemory; i < endMemory; i++) {
				if(memory[i].dirty==1)
				{
					statistics->MemoryWriting++; //grapsimo pisw ston disko
				}

				int deletedBlock = ((int)memory[i].frameNumber[0]) + ((int)memory[i].frameNumber[1]) + ((int)memory[i].frameNumber[2]) + ((int)memory[i].frameNumber[3]) + ((int)memory[i].frameNumber[4]);
				findAndDelete(&(HashTable->buckets[deletedBlock]), memory[i].frameNumber, &element, req.typeOfProcess);

				memory[i].setFree = 1;
				memory[i].dirty = 0;
			}

			pf = 1;
		}

		for(i = startMemory; i < endMemory; i++) {
			if(memory[i].setFree == 1){
				strcpy(memory[i].frameNumber, reference);
				memory[i].setFree = 0;
				if(req.rw == 'W'){
					memory[i].dirty = 1;
				}
				break;
			}
		}

		element.position = i;
		strcpy(element.numberPage, reference);
		strcpy(element.pid, req.typeOfProcess);
		Insert(&(HashTable->buckets[numBucket]), element, NULL);

		statistics->MemoryReading++;
	} else {
		statistics->pageok++;

		if(memory[element.position].dirty == 0 && req.rw == 'W'){
			memory[element.position]. dirty = 1;
		}
	}

	statistics->countOfRecords++;
	return pf;
}

int main (int argc, char **argv){

	int shmid, semid;
	struct sembuf oper;

	int k = atoi(argv[1]);
	int frames = atoi(argv[2]);
	int q = atoi(argv[3]);
	int max = atoi(argv[4])*2;

	int turn=1;

	Statistics statistics;
	statistics.pagefault = 0;
	statistics.pageok = 0;
	statistics.MemoryWriting = 0;
	statistics.MemoryReading = 0;
	statistics.countOfRecords = 0; 

	if ((shmid = shmget (SHMKEY, SHMSIZE, PERMS)) < 0) 
	{
		perror("shmem in  Memory Manager");
		exit(1);
	}

	requests *shmem;
	shmem=shmat(shmid,NULL,0);

	if ((semid = semget(SEMKEY, 3, PERMS)) <0) {		//Two semaphores required
		perror("semaphores in Memory Manager");
		exit(1);
	}


	int i;
	//Initializing HashTable
	PHT HashTable;
	HashTable.buckets = malloc(HASH*sizeof(typeOfNode));
	for(i = 0; i < HASH; i++){
		Create(&(HashTable.buckets[i]));
	}

	frameStruct *memory;
	memory = malloc(frames*sizeof(frameStruct));
	for(i = 0; i < frames; i++){
		memory[i].setFree = 1;
		memory[i].dirty = 0;
	}

	int numOfFrames;
	if(frames % 2 == 0){
		numOfFrames = frames / 2;
	}
	else {
		numOfFrames = (frames / 2) + 1;
	}

	requests req;
	int finished = 0;
	int pfPM1 = 0;
	int pfPM2 = 0;

	for(i = 0; i < max; i++){

		if(finished == 0){
			if(i%q == 0){
				turn = switchTurn(turn);
			} 
		}

		if (turn == 1){
			Sem_Up(oper, 0, semid);
		} else if (turn == 2){
			Sem_Up(oper, 1, semid);
		}

		Sem_Down(oper, 2, semid);
		memcpy(&req, shmem, sizeof(requests));

		if(strcmp(req.typeOfProcess, "PM1") == 0){
			pfPM1 = InsertionAll(&HashTable, k, memory, req, 0 , numOfFrames, pfPM1, &statistics);
		} else {
			pfPM2 = InsertionAll(&HashTable, k, memory, req, numOfFrames+1, frames, pfPM2, &statistics);
		}

		if(req.finished == 1){
			finished = 1;
			turn = switchTurn(turn);
		}
	}

	printf("\n\nStatistika\n");
	printf("Diavasame %d egrafes apo ta 2 arxia kai eixame :\n", statistics.countOfRecords);
	printf("%d Epitiximenes anagnoseis apo tin ikoniki mnimi.\n", statistics.pageok);
    printf("%d Pagefaults.\n", statistics.pagefault);
	printf("%d Anagnoseis apo t disko.\n", statistics.MemoryWriting);
	printf("%d Isagoges ston disko.\n", statistics.MemoryReading);

	exit(EXIT_SUCCESS);
}