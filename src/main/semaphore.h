#ifndef _SEM_H_
#define _SEM_H_
#include "process.h"

union semun{
	int val;
	struct semid_ds *buf;
	struct seminfo **__buf;
	unsigned short *array;
};
int create_sem(int key, int initval);
void sem_p(int semid);
void sem_v(int semid);

struct producer_consumer{
	int mutx;
	int s1;
	int s2;
};

void init_pc(struct producer_consumer*pc, int key, int num);
void lock_produce(struct producer_consumer *pc);
void unlock_produce(struct producer_consumer *pc);
void lock_consume(struct producer_consumer *pc);
void unlock_consume(struct producer_consumer *pc);
#endif
