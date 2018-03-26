#ifndef XML_H
#define XML_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseNode (xmlDocPtr, xmlNodePtr, char*);
void parseDoc(char*, char*, char*);

#endif /* XML_H */