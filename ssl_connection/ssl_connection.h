#ifndef SSL_CONNECTION_H
#define SSL_CONNECTION_H

void init_openssl();
SSL_CTX * createContext();

int check_certificate(SSL_CTX *ctx);

SSL *create_ssl(SSL_CTX *ctx, int socket,char* domain);

int verify_certificate(SSL *ssl);

#endif

