#include "semaphore.h"

int create_sem(int key, int initval) {
	int mutx = semget((key_t)key, 1, IPC_CREAT|IPC_EXCL|0666);
	if (mutx == -1) {
		perror("SEMGET ERROR");
		LogFatal("Create message mutex failed");
	}
	union semun semunion;
	semunion.val = initval;
	int err = semctl(mutx, 0, SETVAL, semunion);
	if (err == -1) {
		perror("SEMCTL ERROR");
		LogFatal("Init message mutex failed");
	}
	return mutx;
}
void sem_p(int semid) {
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	if (semop(semid, &op, 1) == -1) {
		perror("SEMOP ERROR");
		LogFatal("Run Sem p operator failed");
	}
}

void sem_v(int semid) {
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	if (semop(semid, &op, 1) == -1) {
		perror("SEMOP ERROR");
		LogFatal("Run Sem v operator failed");
	}
}
// num is producer can produce num
void init_pc(struct producer_consumer*pc, int key, int num) {
	pc->mutx = create_sem(key, 1);
	pc->s1 = create_sem(2*key, num);
	pc->s2 = create_sem(3*key, 0);
}

// before produce 
void lock_produce(struct producer_consumer *pc) {
	sem_p(pc->s1);
	sem_p(pc->mutx);
}

//after produce
void unlock_produce(struct producer_consumer *pc) {
	sem_v(pc->mutx);
	sem_v(pc->s2);
}

//before consume
void lock_consume(struct producer_consumer *pc) {
	sem_p(pc->s2);
	sem_p(pc->mutx);
}

//after consume
void unlock_consume(struct producer_consumer *pc) {
	sem_v(pc->mutx);
	sem_v(pc->s1);
}

void print_value(struct producer_consumer *pc) {
	union semun semtmp;
	union semun sem2;
	printf("s1:%d][s2:%d\n", semctl(pc->s1, 0, GETVAL, semtmp), 
			semctl(pc->s2, 0, GETVAL, sem2));
}
