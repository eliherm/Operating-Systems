/* 	ELEC 377,  Lab 3
 *
 *  producer.c
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1

int main (int argc, char *argv[]) {

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t key;
	struct shared *memptr;
	int shmid;
	int c, stored;
    
    if (argc != 2) {
        fprintf(stderr,"Usage: %s processid\n",argv[0]);
        exit(1);
    }

    // no error checking...
    int pid = atoi(argv[1]);
    if (pid < 0 || pid >= NUMPROCS) {
        fprintf(stderr,"pid between 0 and %d\n",NUMPROCS-1);
        exit(1);
    }

	// shared memory init
	key = ftok(".", 'S');
	if ((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ) {
		if ((shmid = shmget(key, MEMSIZE, 0)) == -1) {
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if ((int)(memptr = (struct shared *) shmat(shmid, 0, 0)) == -1) {
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}

	mutexInit(memptr);

	getMutex(pid);
	// update the number of producers in shared memory
	memptr->numProducers++;
	releaseMutex(pid);

	int currChar;
	while ((currChar = getchar()) != EOF) {
		stored = FALSE;
		while (stored == FALSE) {
			getMutex(pid);
			if (memptr->count < BUFFSIZE) {
				memptr->buffer[memptr->in] = currChar;
				memptr->in = (memptr->in + 1) % BUFFSIZE;
				memptr->count++;
				stored = TRUE;
			}
			releaseMutex(pid);
		}
	}
	
	getMutex(pid);
	memptr->numProducers--;
	releaseMutex(pid);

	return 0;
}
