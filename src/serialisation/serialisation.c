#include "serialisation/serialisation.h"

Task* createTask(void* function, char* name){

	Task* newTask = (Task*) malloc(sizeof(Task));

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	newTask->crit.mutex = mutex;
	newTask->crit.canContinue = false;

	pthread_create(&newTask->thread, NULL, function, &newTask->crit);

	newTask->name = name;
	return newTask;
}

Branch* createBranch(Task** tasks, int nbTasks, int time, char* name){

	Branch* newBranch = (Branch*) malloc(sizeof(Branch));

	newBranch->tasks = tasks;
	newBranch->nbTasks = nbTasks;
	newBranch->time = time;
	newBranch->name = name;

	return newBranch;
}

pthread_t* serialize(Branch** branchs, int nbBranch){

	pthread_t* threads = (pthread_t*) malloc(nbBranch*sizeof(pthread_t));

	for(int i = 0; i < nbBranch; i++){
		pthread_create(&threads[i], NULL, launchBranch, &branchs[i]);
	}

	return threads;
}

void* launchBranch(void* branch_void){

	//Branch** branch = (Branch**) branch_void;

	pthread_exit(NULL);
}
