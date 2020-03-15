#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include "functions.h"

#define SHMKEY (key_t)9551
#define SEMKEY (key_t)8559
#define SHMSIZE 256
#define PERMS 0600

#define PROCNUM 3

#define DEBUGFLAG 0

union semnum{
	int val;
	struct sem_ds *buff;
	unsigned short *array;
};

typedef struct{
	char address[9];
	char rw;
	char typeOfProcess[4];
	int finished;
}requests;

#endif