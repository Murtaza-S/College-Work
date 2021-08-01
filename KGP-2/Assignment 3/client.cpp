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

//#define PORT 2644

using namespace std;

int connect_server(int &fd_socket, struct sockaddr_in &sckt, int PORT);
void terminate_client(int fd_socket);
void delete_file(int fd_socket);
void retrieve_file(int fd_socket, int PORT);
void list_files(int fd_socket);

int main(int argc, char *argv[])
{
    int fd_socket, ret_value;
    struct sockaddr_in sckt;
    int length = sizeof(sckt);
    char buffer[1024];
    int optval = 1;

    //Extracting Port number from command line argument
    if (argc < 1)
    {
        cout << "\nAttach port number as command line argument.\n";
        exit(0);
    }

    int PORT = atoi(argv[1]);

    //Connecting with the server
    ret_value = connect_server(fd_socket, sckt, PORT);
    //Checking if connection is successful
    if (ret_value)
    {
        cout << "Connection was unsuccessful. \n";
        exit(0);
    }

    //Running of client side

    while (1)
    {
        int choice;

        cout << "\n";
        cout << "Menu : \n";
        cout << "1. List    Files\t2. Store   File\n3. Retrive File\t\t4. Delete  File\n5. Quit\n";
    select_choice:
        cout << "\nChoice (1,3,..): ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            list_files(fd_socket);
            break;

        case 2:
            break;

        case 3:
            retrieve_file(fd_socket, PORT);
            break;

        case 4:
            delete_file(fd_socket);
            break;

        case 5:
            cout << "\nClient is terminating.\n";
            terminate_client(fd_socket);
            break;

        dafault:
            cout << "Invalid choice.Input again.\n";
            goto select_choice;
        }
    }
}

int connect_server(int &fd_socket, struct sockaddr_in &sckt, int PORT)
{
    int length = sizeof(sckt);

    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket not created. \n");
        return 1;
    }
    printf("Client socket has been created.\n");

    sckt.sin_family = AF_INET;
    sckt.sin_port = htons(PORT);
    sckt.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(fd_socket, (struct sockaddr *)&sckt, length)) < 0)
    {
        printf("Connection has failed.\n");
        return 1;
    }

    return 0;
}

void terminate_client(int fd_socket)
{
    char msg[50];

    memset(msg, '\0', sizeof(msg));

    //Sending server command to quit.
    strcpy(msg, "QUIT");
    write(fd_socket, msg, sizeof(msg));

    //read(fd_socket, msg, sizeof(msg));

    //Closing the socket connection and exit the program.
    close(fd_socket);
    cout << "\nClient has terminated.\n";
    exit(0);
}

void delete_file(int fd_socket)
{
    //delete the fucking file.
    char msg[50], file_name[25];

    cout << "Enter the file name to be deleted : ";
    cin >> file_name;

    while (1)
    {

        memset(msg, '\0', sizeof(msg));

        strcpy(msg, "DELE ");
        strcat(msg, file_name);

        write(fd_socket, msg, sizeof(msg));

        string temp1(file_name);
        if (temp1 == "0000")
        {
            cout << "\nFile deletion aborted.\n";
            return;
        }

        memset(msg, '\0', sizeof(msg));
        read(fd_socket, msg, sizeof(msg));
        string temp(msg);

        //Upon success the client exits
        if (temp == "SUCCESS")
        {
            cout << "\nFile \"" << file_name << "\" has been deleted from server.\n";
            return;
        }

        //If the file name error occured the Server contacts back.
        else
        {
            memset(file_name, '\0', sizeof(file_name));

            cout << "\n[Server] : " << temp << "\n";
            cout << "Enter file name again or to quit enter \"0000\" : ";
            cin >> file_name;
        }
    }
}

void retrieve_file(int fd_socket, int PORT)
{
    //Sending the command and file name.
    char msg[50], file_name[25];

    cout << "Enter the file name to be retrived : ";
    cin >> file_name;

    while (1)
    {

        memset(msg, '\0', sizeof(msg));

        strcpy(msg, "RETR ");
        strcat(msg, file_name);

        send(fd_socket, msg, sizeof(msg), 0);
        string temp1(file_name);
        if (temp1 == "0000")
        {
            cout << "\nFile deletion aborted.\n";
            return;
        }

        memset(msg, '\0', sizeof(msg));
        read(fd_socket, msg, sizeof(msg));
        string temp(msg);

        //Upon success the client exits
        if (temp == "SUCCESS")
            break;

        //If the file name error occured the Server contacts back.
        else
        {
            memset(file_name, '\0', sizeof(file_name));

            cout << "\n[Server] : " << temp << "\n";
            cout << "Enter file name again or to quit enter \"0000\" : ";
            cin >> file_name;
        }
    }

    //File retival Code
    int fd_socket1;
    struct sockaddr_in sckt;

    memset(msg, '\0', sizeof(msg));
    read(fd_socket, msg, sizeof(msg));
    string temp(msg);

    if (temp == "CONNECT")
    {

        //Creating connection for data
        int ret_val = connect_server(fd_socket1, sckt, PORT + 1);
        if (ret_val)
        {
            cout << "Data connection with server failed.\n";
            return;
        }

        read(fd_socket1, msg, 8);
        cout << strtol(msg, NULL, 10) << " size.\n";

        close(fd_socket1);
    }
    return;
}

void list_files(int fd_socket)
{
    //Code to list files from the server's directory

    char msg[1024];
    strcpy(msg, "LIST");

    write(fd_socket, msg, 5);

    memset(msg, '\0', sizeof(msg));
    read(fd_socket, msg, sizeof(msg));

    cout << "\nThe following is the list of files at server :\n\n"
         << msg << "\n";
}