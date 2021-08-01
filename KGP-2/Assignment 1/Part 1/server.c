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
    int fd_socket, sckt2, ret_value;
    struct sockaddr_in sckt;
    int length = sizeof(sckt);
    char buffer[1024];
    int optval = 1;
    int PORT = atoi(argv[1]);

    char *msg = "I received the message.";

    if ((fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        printf("\n Socket not created.");
        exit(0);
    }
    printf("Socket has been created.\n");

    if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
    {
        exit(0);
    }

    sckt.sin_family = AF_INET;
    sckt.sin_addr.s_addr = INADDR_ANY;
    sckt.sin_port = htons(PORT);

    if (bind(fd_socket, (struct sockaddr *)&sckt, sizeof(sckt)) < 0)
    {
        printf("\n Binding has failed.");
        exit(0);
    }
    printf("Binding has been done.\n");

    if ((listen(fd_socket, 2)) < 0)
    {
        exit(0);
    }
    printf("Server is listening ...\n\n\n");

    if ((sckt2 = accept(fd_socket, (struct sockaddr *)&sckt, &length)) < 0)
    {
        exit(0);
    }
    printf("Connection established\n");

    memset(buffer, '\0', sizeof(buffer));

    ret_value = read(sckt2, buffer, 1024);
    printf("Client's message: %s \n ", buffer);

    memset(buffer, '\0', sizeof(buffer));

    write(sckt2, msg, 25);
    printf("\nMessage has been sent.\n");

    memset(buffer, '\0', sizeof(buffer));
    printf("Server is terminating.\n");

    close(sckt2);
    close(fd_socket);

    return 0;
}
