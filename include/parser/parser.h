#ifndef PARSER_H
#define PARSER_H

#include "serialization/branch.h"

typedef struct ParsedData ParsedData;
struct ParsedData{
	int nbBranch;
	Branch** branchs;
};

#endif
