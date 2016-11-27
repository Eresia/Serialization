module parser.parser;

import serialization.branch;

abstract class Parser {

	private:
		string fileName;

	protected:

		string getFileName(){
			return fileName;
		}

	public:
		this(string fileName){
			this.fileName = fileName;
		}

		abstract void parse();

		abstract Branch[] getResult();

}
