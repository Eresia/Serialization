#include "serialization/log.h"

/*======Private======*/

/*Thread of log gestion*/
void* logGestion(void* logGestion_void){
	LogGestion* logGestion = (LogGestion*) logGestion_void;
	char* message = NULL;

	while(true){
		/*Take message in the buffer*/
		pthread_mutex_lock(&logGestion->messageMutex);
		if(*logGestion->message != NULL){
			message = *logGestion->message;
			*logGestion->message = NULL;
		}
		pthread_mutex_unlock(&logGestion->messageMutex);

		if(message != NULL){
			pthread_mutex_lock(&logGestion->log->fileMutex);
			#ifdef DEBUG
				printf("Test Writing on file : \"%s\"\n", message);
			#endif
			write(logGestion->log->file, message, strlen(message));
			
			pthread_mutex_unlock(&logGestion->log->fileMutex);

			if(message != NULL){
				free(message);
				message = NULL;
			}

		}

		usleep(20000);
	}

	pthread_exit(NULL);
}

/*Create message which will be sending to log*/
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

/*Send message to log thread*/
void sendToLog(LogGestion log, char* message){
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

/*Create log struct*/
Log* createLog(char* fileName, char* firstLine){

	int file = open(fileName, O_WRONLY | O_TRUNC);
	if(firstLine != NULL){
		write(file, firstLine, strlen(firstLine));
	}

	Log* fileInfo = (Log*) malloc(sizeof(Log));
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	fileInfo->file = file;
	fileInfo->fileMutex = mutex;

	return fileInfo;

}
