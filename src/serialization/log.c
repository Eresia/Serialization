#include "serialization/log.h"

/*======Private======*/
void* logGestion(void* logMessage_void){
	LogMessage* logMessage = (LogMessage*) logMessage_void;
	char* message = NULL;

	while(true){
		pthread_mutex_lock(&logMessage->messageMutex);
		if(*logMessage->message != NULL){
			message = *logMessage->message;
			*logMessage->message = NULL;
		}
		pthread_mutex_unlock(&logMessage->messageMutex);

		if(message != NULL){
			pthread_mutex_lock(&logMessage->log->fileMutex);
			#ifdef DEBUG
				printf("Test Writing on file : \"%s\"\n", message);
			#endif
			write(logMessage->log->file, message, strlen(message));
			//fputs(message, logMessage->log->file);
			pthread_mutex_unlock(&logMessage->log->fileMutex);

			if(message != NULL){
				free(message);
				message = NULL;
			}

		}

		usleep(20000);
	}

	pthread_exit(NULL);
}

char* createLogMessage(char* branchName, Task** tasks, int lastTask, bool aborted, int time){

	char* result;
	int completeSize = strlen(branchName);
	completeSize += strlen(" : ");
	for(int i = 0; i <= lastTask; i++){
		completeSize += strlen(tasks[i]->name) + strlen(" -> ");
	}

	if(aborted){
		completeSize += strlen("ABORTED ");
	}
	else{
		completeSize += strlen("END ");
	}

	completeSize += (time % 10) + 1;

	completeSize += 2;

	result = malloc(completeSize * sizeof(char));

	sprintf(result, "%s : ", branchName);

	for(int i = 0; i <= lastTask; i++){
		sprintf(result, "%s%s -> ", result, tasks[i]->name);
	}

	if(aborted){
		sprintf(result, "%sAborted ", result);
	}
	else{
		sprintf(result, "%sEnd ", result);
	}

	sprintf(result, "%s%d\n", result, time/1000);

	return result;
}

void sendToLog(LogMessage log, char* message){
	pthread_mutex_lock(&log.messageMutex);
	if(*log.message == NULL){
		*log.message = malloc((strlen(message)+1)*sizeof(char));
		strcpy(*log.message, message);
	}
	else{
		*log.message = realloc(*log.message, (strlen(*log.message)+strlen(message)+1)*sizeof(char));
		if(*log.message != NULL){
			strcat(*log.message, message);
		}
	}
	pthread_mutex_unlock(&log.messageMutex);
}

/*======Public======*/
Log* createLogInfo(int file){

	Log* fileInfo = (Log*) malloc(sizeof(Log));
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	fileInfo->file = file;
	fileInfo->fileMutex = mutex;

	return fileInfo;

}
