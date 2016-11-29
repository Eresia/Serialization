#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

#include "serialization/branch.h"

#include "parser/parser.h"

int main(int argc, char** argv){

	ParsedData* parsed;

	if(argc < 2){
		printf("Need the config file name in argument");
		return -1;
	}

	parsed = parseXML(argv[1]);

	if(parsed == NULL){
		printf("Bad config file\n");
		return -2;
	}

	return serialize(parsed->branchs, parsed->nbBranch);

}
