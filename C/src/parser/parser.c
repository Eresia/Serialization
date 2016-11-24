#include "parser/parser.h"

xmlNode* getNodeChildren(xmlNode* parent){
	return parent->children->next;
}

xmlNode* getNodeNext(xmlNode* parent){
	return parent->next->next;
}

ParsedData* parseTreeXML(xmlDocPtr doc){

	ParsedData* result = NULL;

	char* message;
	Log* log;

	Branch** branchs;

	int i, j;

	xmlNode* rootElement = xmlDocGetRootElement(doc);
	xmlNode* infoElement = getNodeChildren(rootElement);
	xmlNode* builderElement = getNodeNext(infoElement);

	int nbBranch = xmlChildElementCount(builderElement);
	char* logFileName = (char*) xmlGetProp(getNodeChildren(infoElement), (xmlChar*) "name");

	message = (char*) malloc((strlen("NB_LINES = ")+(nbBranch%10)+3) * sizeof(char));
	sprintf(message, "NB_LINES = %d\n", nbBranch);
	log = createLog(logFileName, message);

	branchs = (Branch**) malloc(nbBranch*sizeof(Branch*));

	i = 0;

	for(xmlNode* actBranch = getNodeChildren(builderElement); actBranch != NULL; actBranch = actBranch->next){
		if(actBranch->type == XML_ELEMENT_NODE){
			char* branchName = (char*) xmlGetProp(actBranch, (xmlChar*) "name");

			int branchTime = atoi((char*) xmlGetProp(actBranch, (xmlChar*) "time"));
			if(branchTime == 0){
				printf("Time of Branch %s is not correct\n", branchName);
				result = NULL;
				break;
			}

			int nbTasks = xmlChildElementCount(actBranch);
			Task** tasks = (Task**) malloc(nbTasks * sizeof(Task*));
			j = 0;
			for(xmlNode* actTask = getNodeChildren(actBranch); actTask != NULL; actTask = actTask->next){
				if(actTask->type == XML_ELEMENT_NODE){
					char* taskName = (char*) xmlGetProp(actTask, (xmlChar*) "name");
					char* taskFunctionFile = (char*) xmlGetProp(getNodeChildren(actTask), (xmlChar*) "file");
					char* taskFunctionName = (char*) xmlGetProp(getNodeChildren(actTask), (xmlChar*) "name");
					tasks[j] = createTask(taskName, taskFunctionFile, taskFunctionName);
					if(tasks[j] == NULL){
						return NULL;
					}
					j++;
				}
			}
			branchs[i] = createBranch(tasks, nbTasks, branchTime, branchName, log);
			i++;
		}
	}

	result = (ParsedData*) malloc(sizeof(ParsedData));
	result->nbBranch = nbBranch;
	result->branchs = branchs;

	return result;

}

/*======Public======*/
ParsedData* parseXML(char* fileName){

	ParsedData* result = NULL;
	xmlParserCtxtPtr ctxt; /* the parser context */
	xmlDocPtr doc; /* the resulting document tree */

	/*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
	LIBXML_TEST_VERSION

	/* create a parser context */
	ctxt = xmlNewParserCtxt();
	if (ctxt == NULL) {
		printf("Failed to allocate parser context\n");
		return NULL;
	}

	/* parse the file, activating the DTD validation option */
	doc = xmlCtxtReadFile(ctxt, fileName, NULL, XML_PARSE_DTDVALID);

	/* check if parsing suceeded */
	if (doc == NULL) {
		printf("Failed to parse %s\n", fileName);
		result = NULL;
	}
	else {
		/* check if validation suceeded */
		if (ctxt->valid == 0){
			printf("Failed to validate %s\n", fileName);
			result = NULL;
		}
		else{
			result = parseTreeXML(doc);
		}

		/* free up the resulting document */
		xmlFreeDoc(doc);
	}

	/* free up the parser context */
	xmlFreeParserCtxt(ctxt);

	xmlCleanupParser();

	return result;
}
