#include "serialization/branch.h"

/*======Private======*/
void* launchBranch(void* branch_void){

	Branch** branch = (Branch**) branch_void;
	LogMessage log;
	pthread_mutex_t messageMutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t logThread;

	#ifdef DEBUG
		printf("Branch \"%s\" : Locking all mutex\n", (*branch)->name);
	#endif

	for(int i = 0; i < (*branch)->nbTasks; i++){
		Task* task = (*branch)->tasks[i];
		pthread_mutex_lock(&task->taskInfo.mutex);
	}

	log.log = (*branch)->log;
	log.message = malloc(sizeof(char*));
	log.messageMutex = messageMutex;

	#ifdef DEBUG
		printf("Branch \"%s\" : Creating Log Thread\n", (*branch)->name);
	#endif

	pthread_create(&logThread, NULL, logGestion, &log);

	while(true){
		struct timeval beginTime, endTime;
		int completeTime;
		State taskInfo;
		int lastTask;
		char* message;

		#ifdef DEBUG
			printf("Branch \"%s\" : Begin new loop\n", (*branch)->name);
		#endif

		#ifdef DEBUG
			printf("Branch \"%s\" : Get Begin Time\n", (*branch)->name);
		#endif

    	gettimeofday (&beginTime, NULL);

		for(int i = 0; i < (*branch)->nbTasks; i++){
			Task* task = (*branch)->tasks[i];

			#ifdef DEBUG
				printf("Branch \"%s\" : Begin Task \"%s\"\n", (*branch)->name, task->name);
			#endif

			task->taskInfo.state = STATE_IN_PROGRESS;
			pthread_mutex_unlock(&task->taskInfo.mutex);

			do{
				pthread_mutex_lock(&task->taskInfo.mutex);
				taskInfo = task->taskInfo.state;
				pthread_mutex_unlock(&task->taskInfo.mutex);

				gettimeofday(&endTime, NULL);
				if(outDeadline(beginTime, endTime, (*branch)->maxTime)){
					taskInfo = STATE_OUT_DEADLINE;
				}

				usleep(20000);
			}while((taskInfo != STATE_ENDED) && (taskInfo != STATE_OUT_DEADLINE));

			pthread_mutex_lock(&task->taskInfo.mutex);
			task->taskInfo.state = STATE_WAIT;

			lastTask = i;

			if(taskInfo == STATE_OUT_DEADLINE){
				#ifdef DEBUG
					printf("Branch \"%s\" : DeadLine out at task \"%s\"\n", (*branch)->name, (*branch)->tasks[lastTask]->name);
				#endif
				break;
			}

		}

		#ifdef DEBUG
			printf("Branch \"%s\" : End of loop\n", (*branch)->name);
		#endif

		completeTime = ((endTime.tv_sec * 1000000) + (endTime.tv_usec)) - ((beginTime.tv_sec * 1000000) + (beginTime.tv_usec));
		message = createLogMessage((*branch)->name, (*branch)->tasks, lastTask, (taskInfo == STATE_OUT_DEADLINE), completeTime);
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

/*======Public======*/
Branch* createBranch(Task** tasks, int nbTasks, int maxTime, char* name, Log* log){

	Branch* newBranch = (Branch*) malloc(sizeof(Branch));

	newBranch->tasks = tasks;
	newBranch->nbTasks = nbTasks;
	newBranch->maxTime = maxTime;
	newBranch->name = name;
	newBranch->log = log;

	return newBranch;
}

int serialize(Branch** branchs, int nbBranch){

	pthread_t* threads = (pthread_t*) malloc(nbBranch*sizeof(pthread_t));

	#ifdef DEBUG
		printf("Launching %d branchs\n", nbBranch);
	#endif

	for(int i = 0; i < nbBranch; i++){
		pthread_create(&threads[i], NULL, launchBranch, &branchs[i]);
	}

	for(int i = 0; i < nbBranch; i++){
		pthread_join(threads[i] , NULL);
	}

	return 0;
}
