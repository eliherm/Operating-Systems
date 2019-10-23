/*
 *  common.h - Common definiton of the shared
 *     memory segment and prototypes for semaphore functions
 * 
 *  Created by Thomas Dean
 *  Copyright 2005 Queen's University.
 *
 */

#define MEMSIZE 200
#define BUFFSIZE 5
#define NUMPROCS 5

struct shared {
};


void mutexInit(struct shared *memptr);
void getMutex(short pid);
void releaseMutex(short pid);
