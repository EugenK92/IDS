#ifndef XML_H
#define XML_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseNode (xmlDocPtr, xmlNodePtr, char*, char*, int);
void parseDoc(char*, char*, char*,  char*, int);

#endif /* XML_H */