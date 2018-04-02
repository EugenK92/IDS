#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "lib/xml.h"
 
void parseNode(xmlDocPtr doc, xmlNodePtr cur, char* subchild, char* paths, int show) {
 
    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)subchild)))
        {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            if (show == 1) {
                printf("%s\n", key);
            }
            strcat(paths, ";");
            strcat(paths, (const char*)key);
            xmlFree(key);
        }
        cur = cur->next;
    }
    return;
}
 
void parseDoc(char *docname, char *child, char *subchild, char* paths, int show) {
 
    xmlDocPtr doc;
    xmlNodePtr cur;
 
    doc = xmlParseFile(docname);
 
    if (doc == NULL )
        printf("Document parsing failed. \n");
 
    cur = xmlDocGetRootElement(doc);
 
    if (cur == NULL)
    {
        xmlFreeDoc(doc);
        printf("Document is Empty!!!\n");
    }
 
    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)child)))
        {
            parseNode(doc, cur, subchild, paths, show);
        }
        cur = cur->next;
    }
 
    xmlFreeDoc(doc);
}
