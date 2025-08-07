#include <stdio.h>
#include <unistd.h>
#include <arpa/telnet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int socket_start(void)
{
    
    int tcp_socket = -1;
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0)
    {
        perror("socket() ");
        return 1;
    }
    else
    {
        return tcp_socket;
    }
}

