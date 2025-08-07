#include <openssl/ssl.h>
#include <openssl/err.h>
#include "ssl_connection.h"

void init_openssl()
{
    SSL_load_error_strings();
    OPENSSL_init_ssl(0, NULL);
}

SSL_CTX * createContext()
{
    const SSL_METHOD *method;
    method = TLS_client_method();
    SSL_CTX *ctx;
    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

int check_certificate(SSL_CTX *ctx)
{
    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
    {
        fprintf(stderr, "Failed to load trust store\n");
        ERR_print_errors_fp(stderr);
        return -1;
    }
    return 0;
}

SSL *create_ssl(SSL_CTX *ctx, int socket,char* domain)
{
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    SSL_set_fd(ssl, socket);

    SSL_set_tlsext_host_name(ssl, domain);

    if (SSL_connect(ssl) != 1)
    {
        fprintf(stderr, "tls handshake failed \n");
        SSL_free(ssl);
        ERR_print_errors_fp(stderr);
        return NULL;
    }
    printf(" tls handshake successful \n");
    return ssl;
}

int verify_certificate(SSL *ssl)
{
    long verify_result = SSL_get_verify_result(ssl);
    if (verify_result != X509_V_OK)
    {
        fprintf(stderr, "Certificate verification failed: %s\n",
                X509_verify_cert_error_string(verify_result));
        return -1;
    }
    printf(" certificates verified \n");
    return 0;
}

