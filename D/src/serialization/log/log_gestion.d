module serialization.log.log_message;

import std.conv;
import std.algorithm;
import core.stdc.string;
import core.sync.mutex;
import core.thread;

import serialization.task.task;
import serialization.log.log;

class LogGestion : Thread{

	private:
		Log log;
		shared string message;

		void run(){
			string newMessage = "";

			while(true){
				if(this.message.length > 0){
					newMessage = this.message;
					setMessage("");
				}

				if(newMessage.length > 0){
					this.log.writeOnFile(newMessage);
					newMessage = "";
				}

				Thread.sleep(dur!("msecs")(20));
			}

		}

		void setMessage(string message){
			this.message = message;
		}

	public:
		this(Log log){
			super(&run);
			this.log = log;
			this.message = "";
		}

		static string generateMessage(string branchName, Task[] tasks, int lastTask, bool aborted, int time){
			string result;

			result = branchName;
			result ~= " : ";

			for(int i = 0; i <= lastTask; i++){
				result ~= tasks[i].getName();
				result ~= " -> ";
			}

			if(aborted){
				result ~= "Aborted ";
			}
			else{
				result ~= "End ";
			}

			result ~= to!string(time/1000);

			return result;
		}

		void sendToLog(string message){
			synchronized{
				setMessage(this.message ~ message);
			}
		}


}
