#ifndef PARSER_H
#define PARSER_H

#include <libxml/parser.h>

#include "serialization/log.h"
#include "serialization/task.h"
#include "serialization/branch.h"

/*Contains data from parsing*/
typedef struct ParsedData ParsedData;
struct ParsedData{
	int nbBranch;
	Branch** branchs;
};

/*======Private======*/
/*Parse a XML tree to ParsedData struct*/
ParsedData* parseTreeXML(xmlDocPtr doc);


/*======Public======*/

/*Parse a XML file to ParsedData struct*/
ParsedData* parseXML(char* fileName);

#endif
