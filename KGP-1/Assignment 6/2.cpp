//Name : Murtaza Saifee
//Roll No: 20CS60R61

//No additional requirement and the input files are given as command line args.
#include <iostream>
#include <fstream>
#include <sys/shm.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define max_b 10
#define max_t 3
#define limit 1000
using namespace std;

#define P(s) semop(s, &pop, 1)
#define V(s) semop(s, &vop, 1)

struct Reserve
{
    int pnr;
    char name[20];
    char sex[1];
    int age;
    char class_t[4]; // AC2, AC3, or SC
    int status;      // waitlisted
};
struct train
{
    int train_id;
    int AC2, AC3, SC;
    struct Reserve rlist[limit];
};
struct train *train;

int stringTointeger(string str)
{
    int temp = 0;
    for (int i = 0; i < str.length(); i++)
    {
        temp = temp * 10 + (str[i] - '0');
    }
    return temp;
}

int *reader_c_arr[max_t];
int shmid, shmid1, shmid2;
struct sembuf pop, vop;
int read_Sema[max_t];
int ex_Sema[max_t];
int write_Sema[max_t];

int *reservations;

void getWriteLock(int train_id)
{
    int i = train_id;
    P(write_Sema[i]);
    P(ex_Sema[i]);
}

void releaseWriteLock(int train_id)
{
    int i = train_id;
    V(ex_Sema[i]);
    V(write_Sema[i]);
}

void getReadLock(int train_id)
{
    int i = train_id;
    P(read_Sema[i]);
    int v;
    v = semctl(write_Sema[i], 0, GETVAL);
    while (v < 0)
        ;
    ++(*reader_c_arr[i]);
    if (*reader_c_arr[i] == 1)
    {
        P(ex_Sema[i]);
    }
    V(read_Sema[i]);
}

void releaseReadLock(int train_id)
{
    int i = train_id;
    P(read_Sema[i]);
    --(*reader_c_arr[i]);
    if ((*reader_c_arr[i]) == 0)
    {
        V(ex_Sema[i]);
    }
    V(read_Sema[i]);
}

int main(int argc, char *argv[])
{
    shmid = shmget(IPC_PRIVATE, 4 * sizeof(int), 0777 | IPC_CREAT);
    shmid1 = shmget(IPC_PRIVATE, 3 * sizeof(train), 0777 | IPC_CREAT);
    shmid2 = shmget(IPC_PRIVATE, sizeof(int), 0777 | IPC_CREAT);

    train = (struct train *)shmat(shmid1, 0, 0);
    reservations = (int *)shmat(shmid2, 0, 0);
    reservations[0] = 0;

    for (int i = 0; i < max_t; i++)
    {
        train[i].train_id = i;
        train[i].AC2 = max_b;
        train[i].AC3 = max_b;
        train[i].SC = max_b;
        for (int j = 0; j < limit; j++)
        {
            train[i].rlist[j].age = -1;
            train[i].rlist[j].pnr = -1;
            train[i].rlist[j].status = -1;
        }
    }

    for (int i = 0; i < max_t; i++)
    {

        read_Sema[i] = semget(IPC_PRIVATE, 1, 0777 | IPC_CREAT);
        write_Sema[i] = semget(IPC_PRIVATE, 1, 0777 | IPC_CREAT);
        ex_Sema[i] = semget(IPC_PRIVATE, 1, 0777 | IPC_CREAT);

        semctl(read_Sema[i], 0, SETVAL, 1);
        semctl(write_Sema[i], 0, SETVAL, 1);
        semctl(ex_Sema[i], 0, SETVAL, 1);
    }
    pop.sem_num = vop.sem_num = 0;
    pop.sem_flg = vop.sem_flg = SEM_UNDO;
    pop.sem_op = -1;
    vop.sem_op = 1;

    int proc_id, i;
    for (i = 0; i < 4; i++)
    {
        proc_id = fork();
        if (proc_id == 0)
        {
            break;
        }
        else if (proc_id == -1)
        {
            cout << "Error while creating process" << endl;
        }
    }

    string document;
    string s;
    ifstream infile;
    int process_id = getpid();
    //getline(infile, s, '\n');
    if (i == 0)
    {
        document = argv[1];
    }
    else if (i == 1)
    {
        document = argv[2];
    }
    else if (i == 2)
    {
        document = argv[3];
    }
    else if (i == 3)
    {
        document = argv[4];
    }
    else
    {
        // cout << "I am parent process and my process_id is waiting for everyone to be executed " << process_id << endl;
        while (wait(NULL) > 0)
            ;
        shmdt(train);
        shmdt(reservations);

        shmctl(shmid, IPC_RMID, NULL);
        shmctl(shmid1, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);
        cout << "I am parent  and I am exiting my process id is: " << process_id << endl;
        return 0;
    }

    infile.open(document);
    //cout<<"file is opened "<<endl;
    int num = 0;
    while (!infile.eof())
    {
        string str, string_read;
        string parameters[7];
        int k = 0;
        getline(infile, str);
        stringstream ss(str);
        while (ss >> string_read)
        {
            parameters[k] = string_read;
            k++;
        }

        if (parameters[0] == "reserve")
        {
            int t_no = stringTointeger(parameters[5]);
            string name = parameters[1] + parameters[2];
            getWriteLock(t_no);
            train[t_no].train_id = t_no;
            train[t_no].rlist[reservations[0]].pnr = reservations[0] * 10 + t_no;
            train[t_no].rlist[reservations[0]].age = stringTointeger(parameters[3]);
            strcpy(train[t_no].rlist[reservations[0]].name, name.c_str());
            strcpy(train[t_no].rlist[reservations[0]].class_t, parameters[6].c_str());
            strcpy(train[t_no].rlist[reservations[0]].sex, parameters[4].c_str());

            if (parameters[6].compare("AC2") == 0)
            {
                if (train[t_no].AC2 > 0)
                {
                    train[t_no].rlist[reservations[0]].status = 0;
                    train[t_no].AC2--;
                    //cout << "Entered into booking" << endl;
                    cout << train[t_no].rlist[reservations[0]].name << "Booking confirmed : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n Seat No :  " << train[t_no].AC2 << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
                else
                {

                    train[t_no].rlist[reservations[0]].status = 1;
                    cout << train[t_no].rlist[reservations[0]].name << " Booking waitlisted : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
            }

            else if (parameters[6].compare("AC3") == 0)
            {
                if (train[t_no].AC3 > 0)
                {
                    train[t_no].rlist[reservations[0]].status = 0;
                    train[t_no].AC3--;
                    cout << train[t_no].rlist[reservations[0]].name << "Booking confirmed : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n Seat No :  " << train[t_no].AC3 << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
                else
                {
                    train[t_no].rlist[reservations[0]].status = 1;
                    cout << train[t_no].rlist[reservations[0]].name << " Booking waitlisted : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
            }

            else if (parameters[6].compare("SC") == 0)
            {
                if (train[t_no].SC > 0)
                {
                    train[t_no].rlist[reservations[0]].status = 0;
                    train[t_no].SC--;
                    cout << train[t_no].rlist[reservations[0]].name << "Booking confirmed : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n Seat No :  " << train[t_no].AC3 << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
                else
                {
                    train[t_no].rlist[reservations[0]].status = 1;
                    cout << train[t_no].rlist[reservations[0]].name << " Booking waitlisted : \n Train id : " << train[t_no].train_id << "\n Class :" << train[t_no].rlist[reservations[0]].class_t << "\n PNR :  " << train[t_no].rlist[reservations[0]].pnr << endl;
                }
            }

            reservations[0]++;
            releaseWriteLock(t_no);
        }
        else
        {
            int pnr = stringTointeger(parameters[1]);
            int t_no = pnr % 10;
            int ind = pnr / 10;
            char class_t[4] = {};
            getWriteLock(t_no);
            bool check = false;
            if (train[t_no].rlist[ind].pnr == pnr)
            {
                train[t_no].rlist[ind].status = 2;
                if (strcmp("AC3", train[t_no].rlist[ind].class_t) == 0)
                {
                    if (train[t_no].AC3 == 0)
                        check = true;

                    train[t_no].AC3++;
                }
                else if (strcmp("AC2", train[t_no].rlist[ind].class_t) == 0)
                {
                    if (train[t_no].AC2 == 0)
                        check = true;

                    train[t_no].AC2++;
                }
                else
                {
                    if (train[t_no].SC == 0)
                        check = true;

                    train[t_no].SC++;
                }
                cout << "Reservation cancelled\n Name : " << train[t_no].rlist[ind].name << " " << check << endl;
                if (check)
                {

                    for (int m = ind; m < limit; m++)
                    {
                        string cclass(train[t_no].rlist[ind].class_t);
                        string wclass(train[t_no].rlist[ind].class_t);
                        int status = train[t_no].rlist[m].status;
                        if ((cclass == wclass) && status == 1)
                        {
                            train[t_no].rlist[m].status = 0;
                            cout << train[t_no].rlist[m].name << " Your waiting has been cleared " << t_no << "\n PNR :  " << train[t_no].rlist[m].pnr << endl;
                            break;
                        }
                    }
                }
            }
            else
            {
                cout << "PNR is invalid " << endl;
            }
            releaseWriteLock(t_no);
        }
    }
    infile.close();
    cout << "Process id : " << process_id << " ended. " << endl;
    return 0;
}
