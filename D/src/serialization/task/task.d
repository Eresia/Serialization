module serialization.task.task;

import std.stdio;
import std.conv;

import core.thread;
import core.sys.posix.dlfcn;

import serialization.task.state;
import serialization.task.task_info;

class Task : Thread{

	private:
		TaskInfo taskInfo;
		string name;

		void run(){
			while(true){
				waitMutex(taskInfo);
				taskInfo.launchFunction();
			}
		}

		void function() loadFunction(string functionFile, string functionName){
			void function() func;

			// Opening library
			void *hndl = dlopen(cast(char*) (functionFile ~ "\0"), RTLD_LAZY);
			if(hndl == null)
			{
				writeln("Error with dlopen : " ~  to!string(dlerror()));
				return null;
			}

			// Load function
			func = cast(void function()) dlsym(hndl, cast(char*) (functionName ~ "\0"));
			if (func == null)
			{
				writeln("Error with dlsym : %" ~  to!string(dlerror()));
				dlclose(hndl);
				return null;
			}

			// Exécution de la fonction "func"
			return func;
		}

		void waitMutex(TaskInfo taskInfo){
			State needWait;

			if(taskInfo.getState() == State.STATE_IN_PROGRESS){
				taskInfo.setState(State.STATE_ENDED);
			}

			do{
				Thread.sleep(dur!("msecs")(20));
				needWait = taskInfo.getState();
			}while(needWait != State.STATE_IN_PROGRESS);
		}

	public:
		this(string name, string functionFile, string functionName){
			super(&run);

			this.name = name;
			this.taskInfo = new TaskInfo(loadFunction(functionFile, functionName));
			this.start();
		}

		string getName(){
			return name;
		}

		State getState(){
			return taskInfo.getState();
		}

		void setState(State newState){
			taskInfo.setState(newState);
		}

}
