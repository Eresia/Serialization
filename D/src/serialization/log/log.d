module serialization.log.log;

import std.file;
import core.sync.mutex;

class Log{

	private:
		string fileName;

	public:
		this(string fileName, string firstLine){
			this.fileName = fileName;

			if(firstLine.length > 0){
				write(fileName, firstLine ~ "\n");
			}
		}

		/*Write on log file the given message*/
		void writeOnFile(string message){
			synchronized{
				append(fileName, message ~ "\n");
			}
		}
}
