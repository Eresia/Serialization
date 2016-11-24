module serialization.log.log;

import std.stdio;
import core.sync.mutex;

class Log{

	private:
		File file;

	public:
		this(string fileName, string firstLine){
			File file = File(fileName, "w+");
			if(firstLine.length > 0){
				file.write(firstLine);
			}

			this.file = file;
		}

		void writeOnFile(string message){
			synchronized{
				file.write(message);
			}
		}
}
