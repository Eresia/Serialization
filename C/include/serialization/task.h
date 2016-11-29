#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>

#include "useful/bool.h"

/*Represent the state of a task*/
typedef enum State State;
enum State{STATE_WAIT, STATE_IN_PROGRESS, STATE_ENDED, STATE_OUT_DEADLINE};

/*Informations of the task*/
typedef struct TaskInfo TaskInfo;
struct TaskInfo{
	pthread_mutex_t mutex;
	State state;
	void (*function)(void);
};

/*General informations of the task*/
typedef struct Task Task;
struct Task{
	pthread_t thread;
	TaskInfo taskInfo;
	char* name;
};

/*======Private======*/

/*Load a function with dlfcn library*/
void* loadFunction(char* functionFile, char* functionName);

/*Thread of the task gestion*/
void* launchTask(void* crit_void);

/*Function to the condition variable gestion*/
void waitMutex(TaskInfo* taskInfo);

/*======Public======*/

/*Create task struct*/
Task* createTask(char* name, char* functionFile, char* functionName);

#endif
