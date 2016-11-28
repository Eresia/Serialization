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

		void writeOnFile(string message){
			synchronized{
				append(fileName, message ~ "\n");
			}
		}
}
