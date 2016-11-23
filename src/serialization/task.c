#include "serialization/task.h"

/*======Private======*/
void* launchTask(void* taskInfo_void){

	TaskInfo* taskInfo = (TaskInfo*) taskInfo_void;

	while(true){
		waitMutex(taskInfo);
		taskInfo->function();
	}

	pthread_exit(NULL);
}

void waitMutex(TaskInfo* taskInfo){

	State needWait;

	pthread_mutex_lock(&taskInfo->mutex);
	if(taskInfo->state == STATE_IN_PROGRESS){
		taskInfo->state = STATE_ENDED;
	}
	pthread_mutex_unlock(&taskInfo->mutex);

	do{
		pthread_mutex_lock(&taskInfo->mutex);
		needWait = taskInfo->state;
		pthread_mutex_unlock(&taskInfo->mutex);
	}while(needWait != STATE_IN_PROGRESS);
}

/*======Public======*/
Task* createTask(void (*function)(void), char* name){

	Task* newTask = (Task*) malloc(sizeof(Task));

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	newTask->taskInfo.mutex = mutex;
	newTask->taskInfo.state = STATE_WAIT;
	newTask->taskInfo.function = function;

	pthread_create(&newTask->thread, NULL, launchTask, &newTask->taskInfo);

	newTask->name = name;
	return newTask;
}
