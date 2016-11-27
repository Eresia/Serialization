module serialization.branch;

debug{
	import std.stdio;
	import std.conv;
}

import std.datetime;
import core.thread;

import serialization.task.state;
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

			LogGestion logGestion = new LogGestion(log);

			debug{
				writeln("Branch \"" ~ this.name ~ "\" : Creating Log Thread");
			}

			logGestion.start();

			while(true){
				long beginTime, endTime;
				State taskInfo;
				int lastTask;
				string message;

				debug{
					writeln("Branch \"" ~ this.name ~ "\" : Begin new loop");
				}

				debug{
					writeln("Branch \"" ~ this.name ~ "\" : Get Begin Time");
				}

		    	beginTime = (Clock.currTime().stdTime / 10000);

				for(int i = 0; i < tasks.length; i++){
					Task task = tasks[i];

					debug{
						writeln("Branch \"" ~ this.name ~ "\" : Begin Task \"" ~ task.getName() ~ "\"");
					}

					task.setState(State.STATE_IN_PROGRESS);

					do{
						taskInfo = task.getState();
						endTime = (Clock.currTime().stdTime / 10000);

						if(outDeadline(beginTime, endTime, maxTime)){
							taskInfo = State.STATE_OUT_DEADLINE;
						}

						Thread.sleep(dur!("msecs")(20));
					}while((taskInfo != State.STATE_ENDED) && (taskInfo != State.STATE_OUT_DEADLINE));

					task.setState(State.STATE_WAIT);

					lastTask = i;

					if(taskInfo == State.STATE_OUT_DEADLINE){
						debug{
							writeln("Branch \"" ~ this.name ~ "\" : DeadLine out at task  \"" ~ task.getName() ~ "\" at " ~ to!string(endTime - beginTime) ~ " ms");
						}
						break;
					}

				}

				debug{
					writeln("Branch \"" ~ this.name ~ "\" : End of loop at " ~ to!string(endTime - beginTime) ~ " ms");
				}

				message = LogGestion.generateMessage(name, tasks, lastTask, (taskInfo == State.STATE_OUT_DEADLINE), endTime - beginTime);
				logGestion.sendToLog(message);
			}

		}

		bool outDeadline(long beginTime, long nowTime, int maxTime){
			return ((nowTime - beginTime) > maxTime);
		}

	public:
		this(Task[] tasks, int maxTime, string name, Log log){
			super(&run);
			this.tasks = tasks;
			this.maxTime = maxTime;
			this.name = name;
			this.log = log;
		}

		static void serialize(Branch[] branchs){

			debug{
				writeln("Launching " ~ to!string(branchs.length) ~ " branchs\n");
			}

			for(int i = 0; i < branchs.length; i++){
				branchs[i].start();
			}

			for(int i = 0; i < branchs.length; i++){
				branchs[i].join();
			}
		}
}
