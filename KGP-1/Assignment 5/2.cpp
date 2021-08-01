//Murtaza Saifee
//20CS60R61

//No added instruction
//The input file is taken as command line argument.

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
using namespace std;

#define MIN(x, y) ((x < y) ? (x) : (y))

struct Process
{
    int vertex, numofChildren = 0;
    Process *next; //For the child node
};

//To store the node information
struct Node_info
{
    int vertex, numofChildren;
    int pfd[2];
    int cfd[20][2] = {0};
};

//Function to read the file
int reading_file(Process P[], char *file_name);

//Function to built the process tree.
void buildSubtree(Node_info *InfoNode, Node_info *TempNode, Process *P, int size);

//Function to find the root node
int find_root(Process P[], int size);

//Function to condunct the election
int election(Node_info *InfoNode);

//Function to sum up the nodes value.
void SumOfNodes(Node_info *InfoNode, int);

//Function to terminate the Program
void terminate_prog(Node_info *);

//Functions to delete the tree
void free_node(Process *ptr);
void delete_tree(Process *P, int size);

int main(int argc, char *argv[])
{
    int size = 0, root, i, leader;
    Process P[100];
    P->next = NULL;

    //Reading the file.
    size = reading_file(P, argv[1]);

    //Opening file to clear all previous output.
    ofstream file;
    file.open("Output.txt", ios::trunc);
    file.close();

    //Finding the root of the tree
    root = find_root(P, size);
    cout << "\nRoot Node   : " << root << "\n";

    Node_info *InfoNode = new Node_info;
    for (i = 0; i < size; i++)
    {
        if (P[i].vertex == root)
        {
            //Copying the data to structure Node Information
            InfoNode->vertex = P[i].vertex;
            InfoNode->numofChildren = P[i].numofChildren;

            //Pipe for Parent to loop back to itself.
            int alpha[2];
            pipe(alpha);
            InfoNode->pfd[0] = alpha[0];
            InfoNode->pfd[1] = alpha[1];
            break;
        }
    }

    Node_info *TempNode = new Node_info;
    buildSubtree(InfoNode, TempNode, P, size);

    //Calling election Function
    leader = election(InfoNode);

    //Calling the sum of Nodes Function
    SumOfNodes(InfoNode, leader);

    //Termination the program.
    terminate_prog(InfoNode);

    return 0;
}

int reading_file(Process P[], char *file_name)
{
    int parent, child, find, i, size;
    ifstream file;
    file.open(file_name);
    Process *ptr;

    string str, word;
    //reading n
    while (getline(file, str))
    {
        stringstream alpha(str);
        getline(alpha, word, ',');
        child = stoi(word);

        getline(alpha, word, ',');
        parent = stoi(word);

        find = 0;

        //If the tree is empty
        if (!size)
        {
            P[0].vertex = parent;
            P[0].next = new Process;
            P[0].next->vertex = child;
            P[0].numofChildren++;
            P[0].next->next = NULL;
            size += 1;
        }

        else
        {
            //Checking if parent node present
            for (i = 0; i < size; ++i)
            {
                if (P[i].vertex == parent)
                {
                    find = 1;
                    break;
                }
            }

            //If parent node is already present.
            if (find == 1)
            {
                ptr = &P[i];
                while (ptr->next)
                {
                    ptr = ptr->next;
                }
                ptr->next = new Process;
                ptr->next->vertex = child;
                ptr->next->next = NULL;
                P[i].numofChildren++;
            }

            //If parent node is absent.
            else
            {
                i = size;
                P[size].vertex = parent;
                P[size].numofChildren++;

                P[size].next = new Process;
                P[size].next->vertex = child;
                P[size].next->next = NULL;
                size += 1;
            }
        }
    }

    return size;
}

void buildSubtree(Node_info *InfoNode, Node_info *TempNode, Process *P, int size)
{

    Process *ptr;
    int i, j, flag = 0;

    for (i = 0; i < size; i++)
    {
        if (InfoNode->vertex == P[i].vertex)
        {
            ptr = P[i].next;
            break;
        }
    }

    //Begin making subtree.
    for (i = 0; ptr; ++i)
    {
        int pid;
        int a[2]; // data flow from Parent to Child
        int b[2]; // data flow from Child to Parent

        //Creating 2 pipes to maintain a bi-directional flow of data.
        pipe(a);
        pipe(b);

        //parent node contacting child via child nodes
        //We can use dynamic allocation to save the space vis tracking them by no of child.
        InfoNode->cfd[i][0] = b[0];
        InfoNode->cfd[i][1] = a[1];
        //cout << "InfoNode : CFD : " << InfoNode->cfd[i][0] << b[0];

        //ptr used to keep track of child node
        TempNode->vertex = ptr->vertex;
        //TempNode->numofChildren = ptr->numofChildren;

        //Childe node contacting with parent via pfd.
        TempNode->pfd[0] = a[0];
        TempNode->pfd[1] = b[1];

        if (!(pid = fork()))
        {
            InfoNode->vertex = TempNode->vertex;

            //Finding no of children for the child node.
            for (j = 0; j < size; ++j)
            {
                if (P[j].vertex == InfoNode->vertex)
                {
                    flag = 1;
                    InfoNode->numofChildren = P[j].numofChildren;
                    break;
                }
            }
            if (flag == 0)
            {
                InfoNode->numofChildren = 0;
            }

            InfoNode->pfd[0] = TempNode->pfd[0];
            InfoNode->pfd[1] = TempNode->pfd[1];

            //Calling subtree function for nodes having children.
            if (InfoNode->numofChildren)
            {
                buildSubtree(InfoNode, TempNode, P, size);
            }

            return;
        }

        //If the node is parent process wake up other nodes.
        else
        {
            ptr = ptr->next;
        }
    }

    return;
}

int find_root(Process P[], int size)
{
    int nodes[size], check[size] = {0}, i, j;
    Process *ptr;
    for (i = 0; i < size; i++)
    {
        nodes[i] = P[i].vertex;
    }
    for (i = 0; i < size; i++)
    {
        ptr = P[i].next;
        while (ptr)
        {
            for (j = 0; j < size; ++j)
            {
                if (nodes[j] == ptr->vertex)
                {
                    check[j] = 1;
                    break;
                }
            }
            ptr = ptr->next;
        }
    }

    for (i = 0; i < size; ++i)
    {
        if (check[i] == 0)
            return nodes[i];
    }

    return -1;
}

void free_node(Process *ptr)
{
    if (ptr->next)
        free_node(ptr->next);

    free(ptr);
}

void delete_tree(Process *P, int size)
{
    Process *ptr;

    for (int i = 0; i < size; ++i)
    {
        free_node(P[i].next);
    }
}

int election(Node_info *InfoNode)
{
    int leader = InfoNode->vertex;
    int msg_count = 0, i, msg;

    //Waiting to receive msgs from all the children.
    while (msg_count != InfoNode->numofChildren)
    {
        msg_count = 0;
        for (i = 0; i < InfoNode->numofChildren; ++i)
        {
            read(InfoNode->cfd[i][0], &msg, sizeof(msg));
            if (msg > 0)
            {
                leader = MIN(leader, msg);
                msg_count++;
            }
        }
    }
    write(InfoNode->pfd[1], &leader, sizeof(leader));

    read(InfoNode->pfd[0], &msg, sizeof(msg));

    leader = MIN(leader, msg);

    //Broadcasting the leader node in the process tree.
    for (i = 0; i < InfoNode->numofChildren; ++i)
    {
        write(InfoNode->cfd[i][1], &leader, sizeof(leader));
    }

    //Writing output in the file.
    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    if (leader == InfoNode->vertex)
    {
        file << "I am agent " << InfoNode->vertex << " and I am the leader.\n";
    }
    else
    {
        file << "I am agent " << InfoNode->vertex << " and my leader is " << leader << ".\n";
    }

    file.close();

    return leader;
}

void SumOfNodes(Node_info *InfoNode, int leader)
{
    int sum = InfoNode->vertex;
    int msg_count = 0, i, msg;

    //Waiting to receive msgs from all the children node
    while (msg_count != InfoNode->numofChildren)
    {
        msg_count = 0;

        for (i = 0; i < InfoNode->numofChildren; ++i)
        {
            read(InfoNode->cfd[i][0], &msg, sizeof(msg));
            if (msg > 0)
            {
                sum = sum + msg;
                msg_count++;
            }
        }
    }
    write(InfoNode->pfd[1], &sum, sizeof(sum));

    read(InfoNode->pfd[0], &msg, sizeof(msg));

    if (sum < msg)
        sum = msg;

    //Broadcasting the sum in the Tree to reach the leader.
    for (i = 0; i < InfoNode->numofChildren; ++i)
    {
        write(InfoNode->cfd[i][1], &sum, sizeof(sum));
    }

    //Writing output in the file.
    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    if (leader == InfoNode->vertex)
    {
        cout << "Leader Node : " << leader << "\n";
        file << "\nI am leader " << InfoNode->vertex << " and the sum of nodes is : " << sum << "\n\n";
        cout << "\nOutput written in file Output.txt\n\n";
    }

    file.close();
}

void terminate_prog(Node_info *InfoNode)
{
    int msg_count = 0, i, msg = 0;

    while (msg_count != InfoNode->numofChildren)
    {
        msg_count = 0;

        //Waiting to receive calls from all the children.
        for (i = 0; i < InfoNode->numofChildren; ++i)
        {
            read(InfoNode->cfd[i][0], &msg, sizeof(msg));
            if (msg == 0)
            {
                msg_count++;
            }
        }
    }
    write(InfoNode->pfd[1], &msg, sizeof(msg));

    read(InfoNode->pfd[0], &msg, sizeof(msg));

    for (i = 0; i < InfoNode->numofChildren; ++i)
    {
        write(InfoNode->cfd[i][1], &msg, sizeof(msg));
    }

    //Waiting for all the children node to terminate
    for (i = 0; i < InfoNode->numofChildren; i++)
    {
        wait(NULL);
    }

    //Writing output in File
    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    file << "I am agent " << InfoNode->vertex << " and I am going to terminate.\n";

    file.close();
}