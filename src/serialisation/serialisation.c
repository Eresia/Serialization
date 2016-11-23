#include "serialisation/serialisation.h"

/*======Private======*/
void* launchBranch(void* branch_void){

	Branch** branch = (Branch**) branch_void;
	LogMessage log;
	pthread_mutex_t messageMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t logThread;

	for(int i = 0; i < (*branch)->nbTasks; i++){
		Task* task = (*branch)->tasks[i];
		pthread_mutex_lock(&task->crit.mutex);
	}

	log.log = (*branch)->log;
	log.message = malloc(sizeof(char*));
	log.messageMutex = messageMutex;

	pthread_create(&logThread, NULL, logGestion, &log);

	while(true){
		struct timeval beginTime, endTime;
		int completeTime;
		State taskState;
		int lastTask;
		char* message;
    	gettimeofday (&beginTime, NULL);

		for(int i = 0; i < (*branch)->nbTasks; i++){
			Task* task = (*branch)->tasks[i];

			printf("Begin Task \"%s\"\n", task->name);

			task->crit.state = STATE_IN_PROGRESS;
			pthread_mutex_unlock(&task->crit.mutex);

			do{
				pthread_mutex_lock(&task->crit.mutex);
				taskState = task->crit.state;
				pthread_mutex_unlock(&task->crit.mutex);

				gettimeofday(&endTime, NULL);
				if(outDeadline(beginTime, endTime, (*branch)->maxTime)){
					taskState = STATE_OUT_DEADLINE;
				}

				usleep(20000);
			}while((taskState != STATE_ENDED) && (taskState != STATE_OUT_DEADLINE));

			pthread_mutex_lock(&task->crit.mutex);
			task->crit.state = STATE_WAIT;

			lastTask = i;

			if(taskState == STATE_OUT_DEADLINE){
				printf("DeadLine of branch \"%s\" out at task \"%s\"", (*branch)->name, (*branch)->tasks[lastTask]->name);
				break;
			}

		}

		printf("Branch \"%s\" ended\n", (*branch)->name);
		completeTime = ((endTime.tv_sec * 1000000) + (endTime.tv_usec)) - ((beginTime.tv_sec * 1000000) + (beginTime.tv_usec));
		message = createLogMessage((*branch)->name, (*branch)->tasks, lastTask, (taskState == STATE_OUT_DEADLINE), completeTime);
		sendToLog(log, message);
	}

	pthread_exit(NULL);
}

bool outDeadline(struct timeval beginTime, struct timeval nowTime, int maxTime){
	gettimeofday(&nowTime, NULL);

	long int completeBeginTime = beginTime.tv_sec * 1000000 + beginTime.tv_usec;
	long int completeNowTime = nowTime.tv_sec * 1000000 + nowTime.tv_usec;

	return ((completeNowTime - completeBeginTime) > maxTime * 1000);
}

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
			pthread_mutex_lock(&logMessage->log->mutex);
			fputs(message, logMessage->log->file);
			pthread_mutex_unlock(&logMessage->log->mutex);

			free(message);
		}
	}

	pthread_exit(NULL);
}

char* createLogMessage(char* branchName, Task** tasks, int lastTask, bool aborted, int time){

	char* result;
	int completeSize = strlen(branchName);
	completeSize += strlen(" : ");
	for(int i = 0; i < lastTask; i++){
		completeSize += strlen(tasks[i]->name) + strlen(" -> ");
	}

	if(aborted){
		completeSize += strlen("ABORTED ");
	}
	else{
		completeSize += strlen("END ");
	}

	completeSize += (time % 10) + 1;

	completeSize += 1;

	result = malloc(completeSize * sizeof(char));

	sprintf(result, "%s : ", branchName);

	for(int i = 0; i < lastTask; i++){
		sprintf(result, "%s%s -> ", result, tasks[i]->name);
	}

	if(aborted){
		sprintf(result, "%s Aborted ", result);
	}
	else{
		sprintf(result, "%s End ", result);
	}

	sprintf(result, "%s%d", result, time);

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
Task* createTask(void* function, char* name){

	Task* newTask = (Task*) malloc(sizeof(Task));

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	newTask->crit.mutex = mutex;
	newTask->crit.state = STATE_WAIT;

	pthread_create(&newTask->thread, NULL, function, &newTask->crit);

	newTask->name = name;
	return newTask;
}

Log* createLogInfo(FILE* file){

	Log* fileInfo = (Log*) malloc(sizeof(Log));
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	fileInfo->file = file;
	fileInfo->mutex = mutex;

	return fileInfo;

}

Branch* createBranch(Task** tasks, int nbTasks, int maxTime, char* name, Log* log){

	Branch* newBranch = (Branch*) malloc(sizeof(Branch));

	newBranch->tasks = tasks;
	newBranch->nbTasks = nbTasks;
	newBranch->maxTime = maxTime;
	newBranch->name = name;
	newBranch->log = log;

	return newBranch;
}

pthread_t* serialize(Branch** branchs, int nbBranch){

	pthread_t* threads = (pthread_t*) malloc(nbBranch*sizeof(pthread_t));

	for(int i = 0; i < nbBranch; i++){
		pthread_create(&threads[i], NULL, launchBranch, &branchs[i]);
	}

	return threads;
}

void waitMutex(Crit* crit){

	State needWait;

	pthread_mutex_lock(&crit->mutex);
	if(crit->state == STATE_IN_PROGRESS){
		crit->state = STATE_ENDED;
	}
	pthread_mutex_unlock(&crit->mutex);

	do{
		pthread_mutex_lock(&crit->mutex);
		needWait = crit->state;
		pthread_mutex_unlock(&crit->mutex);
	}while(needWait != STATE_IN_PROGRESS);
}
