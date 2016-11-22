#include <stdio.h>
#include <pthread.h>
#include "serialisation/serialisation.h"

void* T1(void* mutex);
void* T2(void* mutex);
void* T3(void* mutex);
void* T4(void* mutex);
void* T5(void* mutex);

int main(int argc, char** argv){

	int nbBranch = 3;

	Task** tasks1 = (Task**) malloc(sizeof(Task*));
	Task** tasks2 = (Task**) malloc(3*sizeof(Task*));
	Task** tasks3 = (Task**) malloc(sizeof(Task*));

	Branch** branchs = (Branch**) malloc(nbBranch*sizeof(Branch*));

	pthread_t* threads;

	tasks1[0] = createTask(T1, "T1");
	tasks2[0] = createTask(T2, "T2");
	tasks2[1] = createTask(T3, "T3");
	tasks2[2] = createTask(T4, "T4");
	tasks3[0] = createTask(T5, "T5");



	branchs[0] = createBranch(tasks1, 1, 300, "B1");
	branchs[1] = createBranch(tasks2, 3, 300, "B2");
	branchs[2] = createBranch(tasks3, 1, 300, "B3");

	threads = serialize(branchs, nbBranch);

	for(int i = 0; i < nbBranch; i++){
		pthread_join(threads[i] , NULL);
	}

	return 0;
}

void* T1(void* mutex){
	pthread_exit(NULL);
}

void* T2(void* mutex){
	pthread_exit(NULL);
}

void* T3(void* mutex){
	pthread_exit(NULL);
}

void* T4(void* mutex){
	pthread_exit(NULL);
}

void* T5(void* mutex){
	pthread_exit(NULL);
}
