module main.main;

import std.stdio;

import serialization.branch;
import parser.parser;
import parser.xml_parser;

int main(string[] argv){

	if(argv.length < 2){
		writeln("Need the config file name in argument");
		return -1;
	}

	Parser parser = new XMLParser(argv[1]);
	Branch[] branchs;

	try{
		parser.parse();
	} catch(std.xml.CheckException e){
		writeln("Bad config file");
		return -2;
	}

	branchs = parser.getResult();

	Branch.serialize(branchs);

	return 0;
}
