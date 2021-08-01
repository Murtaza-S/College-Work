//20CS60R61
//The server multiple request with forking. Current limit is of 2 processes given by for loop line 92.
//The expression is taken and written in the file.
//Compline with: ./<obj_file> <port_no)

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/wait.h>

using namespace std;

//#define PORT 2061

class stack_op
{
public:
    int exp[1000];

    float push(float alpha);
    float pop();
};

int check_file(char buffer[1000])
{
    char msg[100];
    fstream file;
    file.open("input.txt", ios::in);

    while (!file.eof())
    {
        file.getline(msg, 100);
        cout << msg << "\n";
        if (strcmp(msg, buffer))
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int fd_socket, sckt2, ret_value, childid = 0;
    struct sockaddr_in sckt;
    int length = sizeof(sckt);
    char buffer[1000];
    int optval = 1;
    int PORT = atoi(argv[1]);

    char msg[1000] = "I received the message.";

    //Creating the socket
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

    //Binding the socket
    if (bind(fd_socket, (struct sockaddr *)&sckt, sizeof(sckt)) < 0)
    {
        printf("\n Binding has failed.");
        exit(0);
    }
    printf("Binding has been done.\n");

    //Listening to the line ..
    if ((listen(fd_socket, 2)) < 0)
    {
        exit(0);
    }
    printf("Server is listening ...\n\n\n");

    for (int z = 0; z < 2; z++)
    {
        if ((sckt2 = accept(fd_socket, (struct sockaddr *)&sckt, (socklen_t *)&length)) < 0)
        {
            exit(0);
        }
        childid++;
        cout << "Connection established with child " << childid << "\n";

        if (fork() == 0)
        {

            int new_sckt = sckt2;

            //Starting of processing
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));

                ret_value = read(new_sckt, buffer, 1000);
                printf("Client's message: %s \n ", buffer);

                string alpha(buffer);
                stringstream temp(alpha);
                string work, line;

                getline(temp, work, ' ');
                getline(temp, line);

                //Pseudo code for infix to postfix

                /*
        if (work == "EvaluateX")
        {
            //Opening the reading file
            fstream file;
            file.open("input.txt", ios::in);

            for (int i = 0; i < stoi(line); ++i)
            {
                getline(file, msg);
            }

            for (int i = 0; i < len(msg); ++i)
            {
                switch (msg[i])
                {
                case "+":
                case "-":

                case "*":
                case "/":
                case "^":
                    //push till lower precedence operator doesn't come or end of stack;
                    break;

                case isnum():
                    read till number ends;
                    push onto stack;
                    break;
                }
            }
        }
        */

                // WriteX part of server when it receives the instruction

                if (work == "WriteX")
                {

                    //Opening input file for appending
                    fstream file;
                    file.open("input.txt", ios::out | ios::app);

                    //Asking for expression
                    strcpy(msg, "Send in the expression.");
                    write(new_sckt, msg, 25);

                    // Reading the expression and appending it to the file
                    read(new_sckt, buffer, 1000);
                    //ret_value = check_file(buffer);
                    cout << "Client's expression : " << buffer << "\nExpression been written into the file.\n";

                    file << "Child " << childid << " : " << buffer << "\n";

                    file.close();

                    //Sending the Success msg back to client.
                    strcpy(msg, "SUCCESS!!");
                    write(new_sckt, msg, 25);
                }
                break;
            }

            memset(buffer, '\0', sizeof(buffer));

            write(sckt2, msg, 25);
            printf("\nMessage has been sent.\n");

            memset(buffer, '\0', sizeof(buffer));
            cout << "Socket closing with child " << childid << "\n";

            close(new_sckt);
            close(fd_socket);
        }
        else
        {
            close(sckt2);
        }
    }
    wait(NULL);
    wait(NULL);
    return 0;
}