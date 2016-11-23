#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "serialization/task.h"

#include "useful/bool.h"

typedef struct Log Log;
struct Log{
	int file;
	pthread_mutex_t fileMutex;
};

typedef struct LogMessage LogMessage;
struct LogMessage{
	Log* log;
	char** message;
	pthread_mutex_t messageMutex;
};

/*======Private======*/
void* logGestion(void* logMessage_void);
char* createLogMessage(char* branchName, Task** tasks, int lastTask, bool aborted, int time);
void sendToLog(LogMessage log, char* message);

/*======Public======*/
Log* createLogInfo(int file);

#endif
