#define _POSIX_C_SOURCE 200809L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "server.h"
#include "request/request.h"
#include "response/response.h"
#include "ssl_connection/ssl_connection.h"

typedef struct
{
    char *protocol;
    char *domain;
    char *path;
    char *original;
} url_details;

url_details *url_parser(url_details *URL, char *url, char *first_delimeter, char *second_delimeter)
{

    char *buff = strdup(url);
    URL->original = buff;

    char *first_separator = strstr(buff, first_delimeter);
    if (first_separator != NULL)
    {
        *first_separator = '\0';
        URL->protocol = buff;
        first_separator = first_separator + strlen(first_delimeter);
    }
    else
    {
        URL->protocol = NULL;
        URL->domain = NULL;
        URL->path = NULL;
        return URL;
    }
    char *second_separator = strstr(first_separator, second_delimeter);
    if (second_separator != NULL)
    {
        *second_separator = '\0';
        second_separator = second_separator + strlen(second_delimeter);
        URL->domain = first_separator;
        URL->path = second_separator;
    }
    return URL;
}

int main(int argc, char *argv[])
{
    char *url = NULL;
    int is_flag = 0;
    if (argc == 3 && strcmp(argv[1], "-h") == 0)
    {
        url = argv[2];
        is_flag = 1;
    }
    else
    {
        url = argv[1];
    }

    url_details URL;
    url_details *pURL = &URL;
    url_details *resp = url_parser(pURL, url, "://", "/");
    // printf("%s \n",resp->protocol);
    const char *PORT;
    if (strcmp(resp->protocol, "https") == 0)
    {
        PORT = "443";
        printf("443 \n ");
    }
    else
    {
        PORT = "80";
        printf("80 \n");
    }

    printf("%s \n", resp->domain);
    printf("%s", resp->path);

    int tcp_socket = -1;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int s = getaddrinfo(resp->domain, PORT, &hints, &result); // result pointer points the head of linked list
    if (s != 0)
    {
        printf("getaddrinfo() error");
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) // loop over the linked list of addrinfo
    {
        char ip[INET6_ADDRSTRLEN];
        struct sockaddr_in *ip4 = (struct sockaddr_in *)rp->ai_addr; // ai_addr is generic both ip4 and ip6 , so have to convert to ip4
        inet_ntop(AF_INET, &(ip4->sin_addr), ip, sizeof(ip));
        printf(" ip4: %s \n", ip);
        /*if (result->ai_family == AF_INET)
        {
            struct sockaddr_in *ip4 = (struct sockaddr_in *)rp->ai_addr; // ai_addr contain the ip in binary form
            inet_ntop(AF_INET, &(ip4->sin_addr), ip, sizeof(ip));
            printf(" ip4: %s \n", ip);
        }
        else
        {
            struct sockaddr_in *ip6 = (struct sockaddr_in *)rp->ai_addr;
            inet_ntop(AF_INET6, &(ip6->sin_addr), ip, sizeof(ip));
            printf(" ip6: %s \n", ip);
        }*/
    }
    int success_socket = -1;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        int socket = socket_start();
        success_socket = connect(socket, rp->ai_addr, rp->ai_addrlen);
        if (success_socket != -1)
        {
            printf("success connect ");
            tcp_socket = socket;
            break;
        }
        perror("connect");
        close(socket);
    }
    if (strcmp(resp->protocol, "https") == 0)
    {

        init_openssl();
        SSL_CTX *ctx = createContext();
        if (ctx == NULL)
        {
            close(tcp_socket);
            return 1;
        }
        if (check_certificate(ctx) != 0)
        {
            close(tcp_socket);
            SSL_CTX_free(ctx);
        }
        SSL *ssl = create_ssl(ctx, tcp_socket, resp->domain);
        if (ssl == NULL)
        {
            SSL_CTX_free(ctx);
            return 1;
        }
        if (verify_certificate(ssl) != 0)
        {
            SSL_free(ssl);
            SSL_CTX_free(ctx);
            close(tcp_socket);
            return 1;
        }

        request(ssl, resp->domain, resp->path);
        ssize_t n;
        char buffer[4096];
        respo reply;
        respo *replypoint = &reply;
        respo *res;
        char *response_data = NULL;
        size_t bytes_recived = 0;

        while ((n = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0)
        {
            char *temp = realloc(response_data, bytes_recived + n);
            if (temp == NULL)
            {
                perror("realloc() ");
                free(response_data);
                break;
            }
            response_data = temp;
            memcpy(response_data + bytes_recived, buffer, n);
            bytes_recived += n;
        }
        if (response_data != NULL)
        {
            response_data[bytes_recived] = '\0';
        }

        res = response(response_data, "\r\n\r\n", replypoint);
        if (is_flag)
        {
            printf(" header: %s \n", res->header);
        }

        printf("body: %s", res->body);
        free(res->original);

        if (n < 0)
        {
            perror("recv()");
        }
        free(response_data);
        free(pURL->original);
        freeaddrinfo(result);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(tcp_socket);
    }
    else
    {
        http_request(tcp_socket, resp->domain, resp->path);
        ssize_t n;
        char buffer[4096];
        respo reply;
        respo *replypoint = &reply;
        respo *res;
        char *response_data = NULL;
        size_t bytes_recived = 0;
        while ((n = recv(tcp_socket, buffer, sizeof(buffer) - 1, 0)) > 0)

        {

            char *temp = realloc(response_data, bytes_recived + n);

            if (temp == NULL)
            {

                perror("realloc() ");

                free(response_data);

                break;
            }

            response_data = temp;

            memcpy(response_data + bytes_recived, buffer, n);

            bytes_recived += n;
        }

        if (response_data != NULL)
        {
            response_data[bytes_recived] = '\0';
        }

        res = response(response_data, "\r\n\r\n", replypoint);

        if (is_flag)
        {
            printf(" header: %s \n", res->header);
        }
        printf("body: %s", res->body);
        free(res->original);

        if (n < 0)
        {
            perror("recv()");
        }

        free(response_data);
        free(pURL->original);
        freeaddrinfo(result);
        close(tcp_socket);
    }
}