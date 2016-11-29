#ifndef BRANCH_H
#define BRANCH_H

#include <unistd.h>
#include <sys/time.h>

#include "task.h"
#include "log.h"

#include "useful/bool.h"

/*Information of the branch*/
typedef struct Branch Branch;
struct Branch{
	int maxTime;
	int nbTasks;
	Task** tasks;
	char* name;

	Log* log;
};

/*======Private======*/

/*Thread of the branch gestion*/
void* launchBranch(void* branch_void);

/*Return a time beetween two timeval struct*/
long getCompleteTime(struct timeval beginTime, struct timeval endTime);

/*Return if a DeadLine is out*/
bool outDeadline(struct timeval beginTime, struct timeval nowTime, int maxTime);

/*======Public======*/

/*Create the branch struct*/
Branch* createBranch(Task** tasks, int nbTasks, int maxTime, char* name, Log* log);

/*Begin serialize processus*/
int serialize(Branch** branchs, int nbBranch);

#endif
