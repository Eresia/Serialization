#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>

#include "useful/bool.h"

typedef enum State State;
enum State{STATE_WAIT, STATE_IN_PROGRESS, STATE_ENDED, STATE_OUT_DEADLINE};

typedef struct TaskInfo TaskInfo;
struct TaskInfo{
	pthread_mutex_t mutex;
	State state;
	void (*function)(void);
};

typedef struct Task Task;
struct Task{
	pthread_t thread;
	TaskInfo taskInfo;
	char* name;
};

/*======Private======*/
void* loadFunction(char* functionFile, char* functionName);
void* launchTask(void* crit_void);
void waitMutex(TaskInfo* taskInfo);

/*======Public======*/
Task* createTask(char* name, char* functionFile, char* functionName);

#endif
