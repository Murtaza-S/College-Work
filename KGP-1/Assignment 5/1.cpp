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
    //int pfd[2];
    //int cfd[20][2] = {0};
    Process *next; //For the child node
};

struct Node_info
{
    int vertex, numofChildren;
    int pfd[2];
    int cfd[20][2] = {0};
};

//Function to read the file
int reading_file(Process P[], char *file_name);

//Function to built the process tree.
void buildSubtree(Node_info *P1, Node_info *P2, Process *P, int size);

//Function to find the root node
int find_root(Process P[], int size);

//Function to condunct the election
int election(Node_info *P1);

//Function to sum up the nodes value.
void SumOfNodes(Node_info *P1, int);

//Function to terminate the Program
void terminate_prog(Node_info *);

int main(int argc, char *argv[])
{
    int size = 0, root, i, leader;
    Process P[100];
    P->next = NULL;

    size = reading_file(P, argv[1]);
    //cout << size << endl;

    ofstream file;
    file.open("Output.txt", ios::trunc);
    file.close();

    root = find_root(P, size);
    cout << "\nRoot : " << root << "\n";

    Node_info *P1 = new Node_info;
    for (i = 0; i < size; i++)
    {
        if (P[i].vertex == root)
        {
            P1->vertex = P[i].vertex;
            P1->numofChildren = P[i].numofChildren;
            int alpha[2];
            pipe(alpha);
            P1->pfd[0] = alpha[0];
            P1->pfd[1] = alpha[1];
            /*for (int j = 0; j < 20; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    P1->cfd[j][k] = 0;
                }
            }*/
            break;
        }
    }
    Node_info *P2 = new Node_info;
    buildSubtree(P1, P2, P, size);

    /*cout << "Node : " << P1->vertex << "\n";
    for (int m = 0; m < 2; m++)
    {

        for (int n = 0; n < 20; n++)
            cout << P1->cfd[n][m] << " ";

        cout << "\n";
    }*/

    leader = election(P1);

    SumOfNodes(P1, leader);

    terminate_prog(P1);

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
        //Empty Tree
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
            for (i = 0; i < size; ++i)
            {
                if (P[i].vertex == parent)
                {
                    find = 1;
                    break;
                }
            }

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

                //cout << "Process : " << P[i].vertex << " No of children : " << P[i].numofChildren << "\n";
            }
            else
            {
                i = size;
                P[size].vertex = parent;
                P[size].numofChildren++;

                //cout << "Process : " << P[i].vertex << " No of children : " << P[i].numofChildren << "\n";
                P[size].next = new Process;
                P[size].next->vertex = child;
                P[size].next->next = NULL;
                size += 1;
            }
        }
    }

    /*for(int i =0; i<size; i++)
    { 
        //cout<<"parent : "<<P[i].vertex;
        Process *temp;
        temp = &P[i];
        while(temp != NULL)
        {
            
            cout<<temp->vertex;
            temp =temp->next;
        }
        
        cout<<endl;
        cout<<P[i].numofChildren<<endl;
    }
   */
    return size;
}

void buildSubtree(Node_info *P1, Node_info *P2, Process *P, int size)
{

    /*int a[2]; // data flow P2 -> P1
    int b[2]; // data flow P1 -> P2
    pipe(a);
    pipe(b);

    P1->pfd[0] = b[0];
    P1->pfd[1] = a[1];*/

    Process *ptr;
    int i, j, flag = 0;

    for (i = 0; i < size; i++)
    {
        if (P1->vertex == P[i].vertex)
        {
            ptr = P[i].next;
            break;
        }
    }
    //remove the required coments in the for loop given below uske bina sab comment mai rakha hai to trace back
    for (i = 0; ptr; ++i)
    {
        int a[2]; // data flow P2 -> P1
        int b[2]; // data flow P1 -> P2
        pipe(a);
        pipe(b);

        //parent node contacting child via child nodes
        //We can use dynamic allocation to save the space vis tracking them by no of child.
        P1->cfd[i][0] = b[0];
        P1->cfd[i][1] = a[1];
        //cout << "P1 : CFD : " << P1->cfd[i][0] << b[0];

        //ptr used to keep track of child node
        P2->vertex = ptr->vertex;
        //P2->numofChildren = ptr->numofChildren;

        //Childe node contacting with parent via pfd.
        P2->pfd[0] = a[0];
        P2->pfd[1] = b[1];

        int pid;

        //cout << "Parent : " << P1->vertex << " Child : " << P2->vertex << "\n";

        if (!(pid = fork()))
        {
            //close(a[0]);
            //close(b[1]);
            P1->vertex = P2->vertex;
            //loop to check if it is a parent node in the tree and then assign the no of children it has
            for (j = 0; j < size; ++j)
            {
                if (P[j].vertex == P1->vertex)
                {
                    flag = 1;
                    P1->numofChildren = P[j].numofChildren;
                    break;
                }
            }
            if (flag == 0)
            {
                P1->numofChildren = 0;
            }

            P1->pfd[0] = P2->pfd[0];
            P1->pfd[1] = P2->pfd[1];

            /*cout << "Fork success for child : " << P1->vertex << " with children no : " << P1->numofChildren << "\n";
            int x;
            read(P1->pfd[0], &x, sizeof(x));
            //cout << "Read in child " << getpid() << " " << getppid() << " " << x << endl;
            x += 1;
            write(P1->pfd[1], &x, sizeof(x));
            //cout << "Wrote after increment " << x << endl;

            //close(b[1]);
            //close(a[0]);*/

            //call subtree only if no of children is > = 1;
            if (P1->numofChildren)
            {
                //cout << "Node : " << P1->vertex << " creating child nodes.\n";
                buildSubtree(P1, P2, P, size);
            }

            return;
        }
        else
        {
            //close(a[1]);
            //close(b[0]);

            /*int x = 1 * P1->vertex;
            write(P1->cfd[i][1], &x, sizeof(x));
            cout << "Wrote from parent " << getpid() << " " << x << endl;
            read(P1->cfd[i][0], &x, sizeof(x));
            cout << "read in the parent " << x << endl;

            //close(b[1]);
            //close(a[0]);*/

            ptr = ptr->next;

            //wait(NULL);
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

int election(Node_info *P1)
{
    int leader = P1->vertex;
    int msg_count = 0, i, msg;

    //cout << "Election is going on.\n";

    while (msg_count != P1->numofChildren)
    {
        msg_count = 0;
        for (i = 0; i < P1->numofChildren; ++i)
        {
            read(P1->cfd[i][0], &msg, sizeof(msg));
            //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";
            if (msg > 0)
            {
                leader = MIN(leader, msg);
                msg_count++;
            }
            //cout << "Leader : " << leader << " \n";
        }
    }
    write(P1->pfd[1], &leader, sizeof(leader));

    //wait(2);
    //sleep(1);
    read(P1->pfd[0], &msg, sizeof(msg));
    //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";
    leader = MIN(leader, msg);
    for (i = 0; i < P1->numofChildren; ++i)
    {
        write(P1->cfd[i][1], &leader, sizeof(leader));
    }

    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    if (leader == P1->vertex)
    {
        file << "I am agent " << P1->vertex << " and I am the leader.\n";
    }
    else
    {
        file << "I am agent " << P1->vertex << " and my leader is " << leader << ".\n";
    }

    //file<< message << endl;

    //We need to close every file which you open.
    file.close();

    return leader;
}

void SumOfNodes(Node_info *P1, int leader)
{
    int sum = P1->vertex;
    int msg_count = 0, i, msg;

    //cout << "Election is going on.\n";

    while (msg_count != P1->numofChildren)
    {
        msg_count = 0;
        for (i = 0; i < P1->numofChildren; ++i)
        {
            read(P1->cfd[i][0], &msg, sizeof(msg));
            //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";
            if (msg > 0)
            {
                sum = sum + msg;
                msg_count++;
            }
            //cout << "Leader : " << sum << " \n";
        }
    }
    write(P1->pfd[1], &sum, sizeof(sum));

    //wait(2);
    //sleep(1);
    read(P1->pfd[0], &msg, sizeof(msg));

    //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";

    if (sum < msg)
        sum = msg;

    for (i = 0; i < P1->numofChildren; ++i)
    {
        write(P1->cfd[i][1], &sum, sizeof(sum));
    }

    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    if (leader == P1->vertex)
    {
        file << "\nI am leader " << P1->vertex << " and the sum of nodes is : " << sum << "\n\n";
    }

    file.close();
}

void terminate_prog(Node_info *P1)
{
    //int sum = P1->vertex;
    int msg_count = 0, i, msg = 0;

    //cout << "Election is going on.\n";

    while (msg_count != P1->numofChildren)
    {
        msg_count = 0;
        for (i = 0; i < P1->numofChildren; ++i)
        {
            read(P1->cfd[i][0], &msg, sizeof(msg));
            //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";
            if (msg == 0)
            {
                //sum = sum + msg;
                msg_count++;
            }
            //cout << "Leader : " << sum << " \n";
        }
    }
    write(P1->pfd[1], &msg, sizeof(msg));

    //cout << "Node : " << P1->vertex << "\n";
    //wait(2);
    //sleep(1);
    read(P1->pfd[0], &msg, sizeof(msg));

    //cout << "Node : " << P1->vertex << " Msg : " << msg << " leader : " << leader << ".\n";

    //if (sum < msg)
    //  sum = msg;

    for (i = 0; i < P1->numofChildren; ++i)
    {
        write(P1->cfd[i][1], &msg, sizeof(msg));
    }

    for (i = 0; i < P1->numofChildren; i++)
    {
        wait(NULL);
    }

    ofstream file;
    file.open("Output.txt", ios::app);

    while (!(file.is_open()))
    {
        file.open("Output.txt", ios::app);
    }

    file << "I am agent " << P1->vertex << " and I am going to terminate.\n";

    file.close();
}