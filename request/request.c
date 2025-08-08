#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include "request.h"

void request(SSL* ssl, char *domain, char *path)
{

    char request[1024];
    

    int z =snprintf(request, sizeof(request) - 1, "GET /%s HTTP/1.1\r\n"
                                           "Host:  %s\r\n"
                                           "User-Agent: mini-curl/1.0\r\n"
                                           "Connection: close\r\n"
                                           "\r\n",
             path, domain);
    ssize_t n = SSL_write(ssl,request,z);
    if (n<0) perror("ssl_write() ");


} 
void http_request(int socket, char* domain, char* path){

    char request[1024];
    

    int z =snprintf(request, sizeof(request) - 1, "GET /%s HTTP/1.1\r\n"
                                           "Host:  %s\r\n"
                                           "User-Agent: mini-curl/1.0\r\n"
                                           "Connection: close\r\n"
                                           "\r\n",
             path, domain);
    ssize_t n = send(socket,request,z,0);
    if (n<0) perror("send() ");


}
   