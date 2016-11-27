#ifndef BRANCH_H
#define BRANCH_H

#include <unistd.h>
#include <sys/time.h>

#include "task.h"
#include "log.h"

#include "useful/bool.h"

typedef struct Branch Branch;
struct Branch{
	int maxTime;
	int nbTasks;
	Task** tasks;
	char* name;

	Log* log;
};

/*======Private======*/
void* launchBranch(void* branch_void);
long getCompleteTime(struct timeval beginTime, struct timeval endTime);
bool outDeadline(struct timeval beginTime, struct timeval nowTime, int maxTime);

/*======Public======*/
Branch* createBranch(Task** tasks, int nbTasks, int maxTime, char* name, Log* log);
int serialize(Branch** branchs, int nbBranch);

#endif
