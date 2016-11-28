module parser.xml_parser;

import std.xml;
import std.file;
import std.conv;

import std.stdio;

import parser.parser;

import serialization.task.task;
import serialization.branch;
import serialization.log.log;

/*XML File parser*/
class XMLParser : Parser{

	private:
		Branch[] branchs;

	public:
		this(string fileName){
			super(fileName);
		}

		/*Parse a XML file*/
		override
		void parse(){
			string s = to!string(std.file.read(super.getFileName()));

			/*Check if the XML file is valid*/
			check(s);

			Element configElement = new Document(s);
			Element infoElement = configElement.elements[0];
			Element builderElement = configElement.elements[1];

			Element logFileElement = infoElement.elements[0];
			Element[] branchElements = builderElement.elements;

			/*Creating log object*/
			Log log = new Log(logFileElement.tag.attr["name"], "NB_LINES = " ~ to!string(branchElements.length));

			/*Parsing and build result*/
			for(int i = 0; i < branchElements.length; i++){
				Branch branch;
				string branchName = branchElements[i].tag.attr["name"];
				int branchTime = to!int(branchElements[i].tag.attr["time"]);
				Element[] taskElements = branchElements[i].elements;
				Task[] tasks;

				for(int j = 0; j < taskElements.length; j++){
					Task task;
					string taskName = taskElements[j].tag.attr["name"];
					Element functionElement = taskElements[j].elements[0];
					string functionFile = functionElement.tag.attr["file"];
					string functionName = functionElement.tag.attr["name"];

					task = new Task(taskName, functionFile, functionName);
					tasks ~= task;
				}

				branch = new Branch(tasks, branchTime, branchName, log);
				branchs ~= branch;
			}

		}

		/*Get parsing result*/
		override
		Branch[] getResult(){
			return branchs;
		}
}
