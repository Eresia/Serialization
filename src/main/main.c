#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include "serialization/serialization.h"

void T1();
void T2();
void T3();
void T4();
void T5();

int main(int argc, char** argv){

	int nbBranch = 3;

	Task** tasks1 = (Task**) malloc(sizeof(Task*));
	Task** tasks2 = (Task**) malloc(3*sizeof(Task*));
	Task** tasks3 = (Task**) malloc(sizeof(Task*));

	Branch** branchs = (Branch**) malloc(nbBranch*sizeof(Branch*));

	pthread_t* threads;

	int logFile;
	Log* log;
	char* nbLineString = malloc((strlen("NB_LINES = ")+(nbBranch%10)+3) * sizeof(char));

	if(argc < 2){
		printf("Need the file name in argument");
		return 1;
	}

	#ifdef DEBUG
		printf("Opening file %s\n", argv[1]);
	#endif

	logFile = open(argv[1], O_WRONLY | O_TRUNC);
	if(sprintf(nbLineString, "NB_LINES = %d\n", nbBranch) > 0){
		#ifdef DEBUG
			printf("Writing on file : \"%s\"\n", nbLineString);
		#endif
		write(logFile, nbLineString, strlen(nbLineString));
	}

	#ifdef DEBUG
		printf("Initialize Tasks and Branchs\n");
	#endif

	tasks1[0] = createTask(&T1, "T1");
	tasks2[0] = createTask(&T2, "T2");
	tasks2[1] = createTask(&T3, "T3");
	tasks2[2] = createTask(&T4, "T4");
	tasks3[0] = createTask(&T5, "T5");

	log = createLogInfo(logFile);

	branchs[0] = createBranch(tasks1, 1, 300, "Line 1", log);
	branchs[1] = createBranch(tasks2, 3, 300, "Line 2", log);
	branchs[2] = createBranch(tasks3, 1, 300, "Line 3", log);

	#ifdef DEBUG
		printf("Begin Serialize\n");
	#endif

	threads = serialize(branchs, nbBranch);

	for(int i = 0; i < nbBranch; i++){
		pthread_join(threads[i] , NULL);
	}

	return 0;
}

void T1(){

}

void T2(){

}

void T3(){

}

void T4(){

}

void T5(){

}
