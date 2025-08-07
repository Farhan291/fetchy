#ifndef REQUEST_H
#define REQUEST_H

#include <openssl/ssl.h>


void request(SSL* ssl , char *domain, char *path);

#endif