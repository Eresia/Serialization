module serialization.task.task_info;

import serialization.task.state;

class TaskInfo{

	private:
		shared State state;
		void function() func;

	public:
		this(void function() func){
			this.func = func;
			this.state = State.STATE_WAIT;
		}

		/*Ask the loaded function*/
		void launchFunction(){
			func();
		}

		State getState(){
			return state;
		}

		void setState(State newState){
			state = newState;
		}
}
