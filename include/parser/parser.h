#ifndef PARSER_H
#define PARSER_H

#include <libxml/parser.h>

#include "serialization/log.h"
#include "serialization/branch.h"

typedef struct ParsedData ParsedData;
struct ParsedData{
	int nbBranch;
	Branch** branchs;
};

/*======Public======*/
ParsedData* parseTreeXML(xmlDocPtr doc);


/*======Public======*/
ParsedData* parseXML(char* fileName);

#endif
