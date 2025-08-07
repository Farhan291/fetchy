#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
    char* header;
    char* body;
    char* original;
} respo;

respo* response(char* string, char* delimeter,respo* reply);

#endif