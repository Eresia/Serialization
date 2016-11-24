module serialization.branch;

import std.datetime;
import core.thread;

import serialization.task.task;
import serialization.log.log;
import serialization.log.log_gestion;

class Branch : Thread{

	private:
		int maxTime;
		Task[] tasks;
		string name;

		Log log;

		void run(){

			LogGestion logGestion = LogGestion(log);

			logGestion.start();

			while(true){
				long beginTime, endTime;
				int completeTime;
				State taskInfo;
				int lastTask;
				string message;

		    	beginTime = Clock.currTime().stdTime;

				for(int i = 0; i < tasks.length; i++){
					Task task = tasks[i];

					task.setState(STATE_IN_PROGRESS);

					do{
						taskInfo = task.getState();
						endTime = Clock.currTime().stdTime;

						if(outDeadline(beginTime, endTime, maxTime)){
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

		}

		bool outDeadline(long beginTime, long nowTime, int maxTime){
			return ((nowTime - beginTime) > (maxTime*10000));
		}

	public:
		this(Task[] tasks, int maxTime, string name, Log log){
			super(&run);
			this.tasks = tasks;
			this.maxTime = maxTime;
			this.name = name;
			this.log = log;
		}

		void serialize(Branch[] branchs){

			for(int i = 0; i < branchs.length; i++){
				branchs[i].start();
			}

			for(int i = 0; i < branchs.length; i++){
				branchs[i].join();
			}
		}
}
