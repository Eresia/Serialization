#include "serialization/task.h"

/*======Private======*/

/*Load a function with dlfcn library*/
void* loadFunction(char* functionFile, char* functionName){
	void (*func)();

	// Opening library
	void *hndl = dlopen(functionFile, RTLD_LAZY);
	if(hndl == NULL)
	{
		printf("Error with dlopen : %s\n", dlerror());
		return NULL;
	}

	// Load function
	func = dlsym(hndl, functionName);
	if (func == NULL)
	{
		printf("Error with dlsym : %s\n", dlerror());
		dlclose(hndl);
		return NULL;
	}

	return func;
}

/*Thread of the task*/
void* launchTask(void* taskInfo_void){

	TaskInfo* taskInfo = (TaskInfo*) taskInfo_void;

	while(true){
		waitMutex(taskInfo);
		taskInfo->function();
	}

	pthread_exit(NULL);
}

/*Function to the condition variable gestion*/
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

/*Create task struct*/
Task* createTask(char* name, char* functionFile, char* functionName){

	Task* newTask = (Task*) malloc(sizeof(Task));

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	newTask->taskInfo.mutex = mutex;
	newTask->taskInfo.state = STATE_WAIT;

	newTask->taskInfo.function = loadFunction(functionFile, functionName);

	if(newTask->taskInfo.function == NULL){
		free(newTask);
		newTask = NULL;
	}
	else{
		pthread_create(&newTask->thread, NULL, launchTask, &newTask->taskInfo);
		newTask->name = name;
	}

	return newTask;
}
