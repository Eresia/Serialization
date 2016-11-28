module parser.parser;

import serialization.branch;

/*General parser*/
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

		/*Parse a file*/
		abstract void parse();

		/*Get parsing result*/
		abstract Branch[] getResult();

}
