module serialization.task.task;

import core.thread;

import serialization.task.state;
import serialization.task.task_info;

class Task : Thread{

	private:
		TaskInfo taskInfo;
		string name;

		void run(){

		}

	public:
		this(string name, string functionFile, string functionName){
			super(&run);

			this.name = name;
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
