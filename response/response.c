#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "response.h"

respo* response(char* string, char* delimeter,respo* reply) {

    char *buff =strdup(string);
    
    reply->original=buff;

    char* separetor;
    separetor = strstr(buff,delimeter);
    if(separetor!=NULL){
        *separetor = '\0';
        separetor = separetor +strlen(delimeter);
        reply->header=buff;
        reply->body=separetor;
    }
    else{
        reply->header=NULL;
        reply->body =NULL;
    }
    return reply;


    
    
}