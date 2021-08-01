//20CS60R61
//Fully working

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

//#define PORT 2061

int main(int argc, char *argv[])
{
    int fd_socket, ret_value;
    struct sockaddr_in sckt;
    int length = sizeof(sckt);
    char buffer[1024];
    int optval = 1;
    int PORT = atoi(argv[1]);

    char *msg = "Hello (Client msg)";

    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket not created. \n");
        exit(0);
    }
    printf("Socket has been created.\n\n");

    sckt.sin_family = AF_INET;
    sckt.sin_port = htons(PORT);
    sckt.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(fd_socket, (struct sockaddr *)&sckt, length)) < 0)
    {
        printf("Connection has failed.\n");
        exit(0);
    }

    printf("Connection established\n");

    memset(buffer, '\0', sizeof(buffer));

    write(fd_socket, msg, 19);
    printf("Message has been sent. \n\n");

    memset(buffer, '\0', sizeof(buffer));
    ret_value = read(fd_socket, buffer, 1024);
    printf("Server's message: %s \n\n", buffer);

    memset(buffer, '\0', sizeof(buffer));
    printf("Client is terminating.\n");

    close(fd_socket);

    return 0;
}
