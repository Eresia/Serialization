module parser.parser;

import serialization.task.task;
import serialization.branch;
import serialization.log.log;

abstract class Parser {

	protected:
		string fileName;

		Branch[] result;

	public:
		this(string fileName){
			this.fileName = fileName;
			result = null;
		}

		abstract void parse();

		Branch[] getResult(){
			return result;
		}

}
