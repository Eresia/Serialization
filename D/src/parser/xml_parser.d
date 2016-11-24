module parser.xml_parser;

import std.xml;

import parser.parser;

class XMLParser : Parser{

	public:
		this(string fileName){
			super(fileName);
		}

		override
		void parse(){

		}
}
