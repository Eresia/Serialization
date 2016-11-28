#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "serialization/task.h"

#include "useful/bool.h"

/*Log informations struct*/
typedef struct Log Log;
struct Log{
	int file;
	pthread_mutex_t fileMutex;
};

/*Log gestion informations struct*/
typedef struct LogGestion LogGestion;
struct LogGestion{
	Log* log;
	char** message;
	pthread_mutex_t messageMutex;
};

/*======Private======*/

/*Thread of log gestion*/
void* logGestion(void* logMessage_void);

/*Create message sending to log*/
char* createLogMessage(char* branchName, Task** tasks, int lastTask, bool aborted, int time);

/*Send message to log thread*/
void sendToLog(LogGestion log, char* message);

/*======Public======*/

/*Create log struct*/
Log* createLog(char* fileName, char* firstLine);

#endif
