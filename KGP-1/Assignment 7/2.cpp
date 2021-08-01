#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sstream>

using namespace std;

// To store the data
struct T_data
{
    double **A;
    double **B;
    double **C;
    int len, i, j;
} t_data;

void *mult(void *arg)
{
    int sum = 0;

    for (int i = 0; i < t_data.len; i++)
    {
        sum += t_data.A[t_data.i][i] * t_data.B[i][t_data.j];
    }

    t_data.C[t_data.i][t_data.j] = sum;

    pthread_exit((void *)0);
}

int main(int argc, char const *argv[])
{
    //For rows and columns.
    int r[2], c[2], i, j;

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cout << "File not attached.\n";
        return 0;
    }

    //Matrix A
    string str, num;
    getline(file, str);
    stringstream a1(str);

    getline(a1, num, ' ');
    r[0] = stoi(num);

    getline(a1, num, ' ');
    c[0] = stoi(num);

    t_data.A = (double **)malloc(r[0] * sizeof(double *));

    cout << "\nMatrix A : " << r[0] << " x " << c[0] << "\n";

    for (i = 0; i < r[0]; i++)
    {
        t_data.A[i] = (double *)malloc(c[0] * sizeof(double));

        getline(file, str);
        stringstream a2(str);

        //For matrix A
        for (j = 0; j < c[0]; j++)
        {
            getline(a2, num, ' ');
            t_data.A[i][j] = stoi(num);
            cout << t_data.A[i][j] << " ";
        }
        cout << "\n";
    }

    //Matrix B
    getline(file, str);
    stringstream a3(str);

    getline(a3, num, ' ');
    r[1] = stoi(num);

    getline(a3, num, ' ');
    c[1] = stoi(num);

    t_data.B = (double **)malloc(r[1] * sizeof(double *));

    if (c[0] != r[1])
    {
        cout << "Matrix multiplication not feasible.\n";
        return 0;
    }

    cout << "\nMatrix B : " << r[1] << " x " << c[1] << "\n";

    for (i = 0; i < r[1]; i++)
    {
        t_data.B[i] = (double *)malloc(c[1] * sizeof(double));

        getline(file, str);
        stringstream a2(str);

        for (j = 0; j < c[1]; j++)
        {
            getline(a2, num, ' ');
            t_data.B[i][j] = stoi(num);
            cout << t_data.B[i][j] << " ";
        }
        cout << "\n";
    }

    //Matrix C
    t_data.C = (double **)malloc(r[0] * sizeof(double *));
    for (int i = 0; i < r[0]; i++)
    {
        t_data.C[i] = (double *)malloc(c[1] * sizeof(double));
    }

    //Threads
    int num_thread = r[0] * c[1];
    pthread_t threads[r[0] * c[1]];

    int count = 0;
    for (i = 0; i < r[0]; i++)
    {
        for (int j = 0; j < c[1]; j++)
        {
            t_data.i = i;
            t_data.j = j;
            t_data.len = c[0];

            pthread_create(&threads[count], NULL, mult, NULL);
            count++;
            pthread_join(threads[i + j], NULL);
        }
    }

    //Output

    cout << "\nResultant matrix : " << r[0] << " x " << c[1] << ")" << endl;

    for (i = 0; i < r[0]; i++)
    {
        for (int j = 0; j < c[1]; j++)
        {
            cout << t_data.C[i][j] << " ";
        }
        cout << "\n";
    }

    pthread_exit(NULL);

    return 0;
}
