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
#include <dirent.h>
#include <fstream>

//#define PORT 2644
using namespace std;

int check_file(string file_name);
int create_connection(int sockets[20], struct sockaddr_in &sckt, int PORT, int i = 0);
void retrive_file(string file_name, int fd_socket, int sockets[20], int PORT);
void delete_file(string file_name, int fd_socket, int sockets[20], int sock_size);
int send_file(string temp);
void terminate_client(int fd_socket, int sockets[], int sock_size, fd_set &active_sockets);
void list_files(int fd_socket, int sockets[20], int sock_size);

int main(int argc, char *argv[])
{
    int sockets[20], s = 1; //0 -> server, rest client
    struct sockaddr_in sckt;

    //Extracting Port number from command line argument
    if (argc < 1)
    {
        cout << "\nAttach port number as command line argument.\n";
        exit(0);
    }

    int PORT = atoi(argv[1]);

    // Making server active(socket creation)
    int ret_value = create_connection(sockets, sckt, PORT, 0);
    if (ret_value)
    {
        cout << "The connection could not be made.\n";
        exit(0);
    }

    //

    while (true)
    {

        fd_set active_sockets;

        FD_ZERO(&active_sockets);
        FD_SET(sockets[0], &active_sockets);

        //Interaction with the clients
        while (1)
        {
            char msg[50];
            fd_set temp_sockets = active_sockets;

            ret_value = select(FD_SETSIZE, &temp_sockets, NULL, NULL, NULL);
            if (ret_value < 0)
            {
                cout << "Select error occured.\n";
                exit(0);
            }

            for (int z = 0; z < FD_SETSIZE; z++)
            {
                if (FD_ISSET(z, &temp_sockets))
                {
                    //New client invoking connection.
                    if (z == sockets[0])
                    {
                        cout << "Client in connecting.\n";
                        int length = sizeof(sckt);
                        int client_fd = accept(sockets[0], (struct sockaddr *)&sckt, (socklen_t *)&length);
                        //Error in connectiong
                        if (client_fd < 0)
                        {
                            cout << "connection with client error.\n";
                            exit(0);
                        }
                        //Adding connection to the active sockets.
                        else
                        {
                            FD_SET(client_fd, &active_sockets);
                            sockets[s++] = client_fd;
                            cout << "Client " << s - 1 << " has been connected.\n\n";
                        }
                    }

                    //This is the data request from one of the clients.
                    else
                    {
                        //Reading the command sent from the client
                        memset(msg, '\0', sizeof(msg));

                        ret_value = recv(z, msg, sizeof(msg), 0);
                        //cout << "Client :" << z << " Ret val : " << ret_value;
                        //printf("Client's message: %s \n", msg);

                        string alpha(msg);
                        stringstream temp(alpha);
                        string command, file_name;

                        getline(temp, command, ' ');

                        if (command == "QUIT")
                        {
                            terminate_client(z, sockets, s, active_sockets);
                        }
                        else if (command == "DELE")
                        {
                            getline(temp, file_name);
                            delete_file(file_name, z, sockets, s);
                        }
                        else if (command == "RETR")
                        {
                            getline(temp, file_name);
                            retrive_file(file_name, z, sockets, PORT);
                        }
                        else if (command == "LIST")
                        {
                            list_files(z, sockets, s);
                        }
                    }
                }
            }
        }

        //select and perferom the task

        //to perform the task make the buffer into string and split it

        //do accoording to the task given

        //close the connection.
    }
}

int create_connection(int sockets[20], struct sockaddr_in &sckt, int PORT, int i)
{
    int optval = 1;

    //Creating the socket
    if ((sockets[0] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
    {
        printf("\n Socket not created.");
        return 1;
    }
    if (i == 0)
        printf("Socket has been created.\n");

    if (setsockopt(sockets[0], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
    {
        return 1;
    }

    sckt.sin_family = AF_INET;
    sckt.sin_addr.s_addr = INADDR_ANY;
    sckt.sin_port = htons(PORT);

    //Binding the socket
    if (bind(sockets[0], (struct sockaddr *)&sckt, sizeof(sckt)) < 0)
    {
        printf("\n Binding has failed.");
        return 1;
    }
    if (i == 0)
        printf("Binding has been done.\n");

    //Listening to the line ..
    if ((listen(sockets[0], 19)) < 0)
    {
        return 1;
    }
    if (i == 0)
        printf("Server is listening ...\n\n\n");

    return 0;
}

void terminate_client(int fd_socket, int sockets[20], int sock_size, fd_set &active_sockets)
{
    int i;

    //The socket number of client is checked.
    for (i = 0; i < sock_size; ++i)
    {
        if (sockets[i] == fd_socket)
            break;
    }
    cout << "Client " << i << " wishes to close the connection.\nTerminating the client.\n\n";

    //Clear the socket from the FD SET and close the socket as well.
    FD_CLR(fd_socket, &active_sockets);
    close(fd_socket);
}

void delete_file(string file_name, int fd_socket, int sockets[20], int sock_size)
{
    char msg[50];

    while (1)
    {
        //The file is being removed
        int ret_val = remove(file_name.c_str());

        //If it doesn't exist the loop requests to send the name again or terminates based o client's choice.
        if (ret_val)
        {
            strcpy(msg, "Wrong file name");
            write(fd_socket, msg, sizeof(msg));

            memset(msg, '\0', sizeof(msg));
            cout << "Reading the file name again.\n";
            read(fd_socket, msg, sizeof(msg));
            string alpha(msg);
            stringstream temp(alpha);
            string command;

            getline(temp, command, ' ');
            getline(temp, file_name);

            //If client wishes to terminate the delete function.
            if (file_name == "0000")
            {
                cout << "Deleting file function revoked.\n";
                return;
            }
            /*else
            {

                string alpha(msg);
                stringstream temp(alpha);
                string command, file_name;

                getline(temp, command, ' ');
                getline(temp, file_name);
            }*/
        }
        else
        {
            memset(msg, '\0', sizeof(msg));
            strcpy(msg, "SUCCESS");
            write(fd_socket, msg, sizeof(msg));

            int i;
            for (i = 0; i < sock_size; ++i)
            {
                if (sockets[i] == fd_socket)
                    break;
            }

            cout << "Client " << i << " : DELE " << file_name << "\n";

            cout << "File \"" << file_name << "\" has been deleted.\n\n";
            return;
        }
    }
}

void retrive_file(string file_name, int fd_socket, int sockets[20], int PORT)
{
    //Code to retrive file from server

    //Checking on client side if the file exists or not.
    while (1)
    {
        char msg[50];
        int ret_val = check_file(file_name);

        if (!ret_val)
        {
            //Send msg file does not exist
            strcpy(msg, "Wrong file name");
            write(fd_socket, msg, sizeof(msg));

            memset(msg, '\0', sizeof(msg));
            cout << "Reading the file name again.\n";
            read(fd_socket, msg, sizeof(msg));
            string alpha(msg);
            stringstream temp(alpha);
            string command;

            getline(temp, command, ' ');
            getline(temp, file_name);

            //If client wishes to terminate the read function.
            if (file_name == "0000")
            {
                cout << "Reading file function revoked.\n";
                return;
            }
        }
        else
        {
            memset(msg, '\0', sizeof(msg));
            strcpy(msg, "SUCCESS");
            write(fd_socket, msg, sizeof(msg));
            cout << "Corret file name.\n";
            break;
        }
    }

    //File retireval code
    if (fork() == 0)
    {
        cout << "Chlid process created.\n";
        close(fd_socket);
        close(sockets[0]);
        int socks[20];

        struct sockaddr_in sckt;
        int length = sizeof(sckt);

        int ret_val = create_connection(socks, sckt, PORT + 1, 1);
        if (ret_val == 1)
        {
            cout << "Error in creating socket.\n";
            exit(0);
        }
        int child_socket = accept(socks[0], (struct sockaddr *)&sckt, (socklen_t *)&length);

        //Finding the size of file
        ofstream fout;
        fout.open(file_name.c_str(), ios::binary | ios::app);
        long int filesize = fout.tellp();
        cout << "File size : " << filesize;
        string alpha = to_string(filesize);
        write(child_socket, alpha.c_str(), 8);

        fout.close();
        close(socks[0]);
        exit(0);
    }
    char msg[50];
    strcpy(msg, "CONNECT");
    write(fd_socket, msg, sizeof(msg));
    return;
}

int check_file(string file_name)
{
    ifstream fin;

    fin.open(file_name.c_str());
    if (fin)
    {
        fin.close();
        return 1;
    }
    else
        return 0;
}

void list_files(int fd_socket, int sockets[20], int sock_size)
{
    DIR *dirctry;
    if ((dirctry = opendir(".")) == NULL)
    {
        cout << "Directory Could not be opened.";
        return;
    }

    struct dirent *dir;
    string files = "";
    int i;

    while ((dir = readdir(dirctry)) != NULL)
    {
        string file_name(dir->d_name);

        if (file_name != "." && file_name != "..")
        {
            files += file_name + "\n";
        }
    }

    for (i = 0; i < sock_size; ++i)
    {
        if (sockets[i] == fd_socket)
            break;
    }

    cout << "Client " << i << " : LIST\n\n";
    write(fd_socket, files.c_str(), 1024);
}