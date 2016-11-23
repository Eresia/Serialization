#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

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

typedef struct Log Log;
struct Log{
	int file;
	pthread_mutex_t fileMutex;
};

typedef struct Branch Branch;
struct Branch{
	int maxTime;
	int nbTasks;
	Task** tasks;
	char* name;

	Log* log;
};

typedef struct LogMessage LogMessage;
struct LogMessage{
	Log* log;
	char** message;
	pthread_mutex_t messageMutex;
};

/*======Private======*/
void* launchBranch(void* branch_void);
void* launchTask(void* crit_void);
void waitMutex(TaskInfo* taskInfo);
bool outDeadline(struct timeval beginTime, struct timeval nowTime, int maxTime);
void* logGestion(void* logMessage_void);
char* createLogMessage(char* branchName, Task** tasks, int lastTask, bool aborted, int time);
void sendToLog(LogMessage log, char* message);

/*======Public======*/
Task* createTask(void (*function)(void), char* name);
Log* createLogInfo(int file);
Branch* createBranch(Task** tasks, int nbTasks, int maxTime, char* name, Log* log);
pthread_t* serialize(Branch** branchs, int nbBranch);

#endif
