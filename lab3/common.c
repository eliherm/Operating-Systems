/* 	ELEC 377, Lab 3
 *
 * common.c contains routines to be used from both the  producer, and the  consumer
 *   Mutual Exclusion routines will be here
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <asm/system.h>


#include "common.h"

#define FALSE 0
#define TRUE 1

static struct shared *sharedptr;
initialized = 0;

int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}


void mutexInit(struct shared *memptr){
    // TODO: initialize the only mutex once, from the producer... 
    if(initialized == 0) {
        sharedptr = memptr;
        sharedptr->lock = FALSE;
        sharedptr->numProducers = 0;
        sharedptr->in = 0;
        sharedptr->out = 0;
        sharedptr->count = 0;
        initialized = 1;
    }
    
}



void getMutex(short pid){
	// this should not return until it has mutual exclusion. Note that many versions of 
	// this will probobly be running at the same time.
    waiting[pid] = TRUE;
    int key = TRUE;
    while(waiting[pid] && key){
        key = test_and_set(sharedptr->lock);
    }
    waiting[pid] = FALSE;
}

void releaseMutex(short pid){
	// set the mutex back to initial state so that somebody else can claim it
    short nextPid = (pid + 1) % NUMPROCS;
    while((nextPid != pid) && (!waiting[nextPid]))
        nextPid = (nextPid + 1) % NUMPROCS;
    if(pid == nextPid){
        sharedptr->lock = FALSE;
    }else{
        waiting[nextPid] = FALSE;
    }
}

