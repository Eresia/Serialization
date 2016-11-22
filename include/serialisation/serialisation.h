#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "useful/bool.h"

typedef struct Crit Crit;
struct Crit{
	pthread_mutex_t mutex;
	bool canContinue;
};

typedef struct Task Task;
struct Task{
	pthread_t thread;
	Crit crit;
	char* name;
};

typedef struct Branch Branch;
struct Branch{
	int time;
	int nbTasks;
	Task** tasks;
	char* name;
};

Task* createTask(void* function, char* name);

Branch* createBranch(Task** tasks, int nbTasks, int time, char* name);

pthread_t* serialize(Branch** branchs, int nbBranch);

void* launchBranch(void* b);

#endif
