//Murtaza Saifee
//20CS60R61

//File name: Test.txt
//No added instructions.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <glpk.h>

void linear_prog(int store[], int camp_size[], int demand[], int cost[][100])
{
    int i, j, flag;

    //Objective Function
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "Relocation");
    glp_set_obj_dir(lp, GLP_MIN);

    //Creating columns
    glp_add_cols(lp, store[0] * store[1]);

    int count = 1;
    for (i = 1; i <= store[0]; i++)
    {
        for (j = 1; j <= store[1]; j++)
        {
            glp_set_col_name(lp, count, "c");
            glp_set_col_bnds(lp, count, GLP_DB, 0.0, 1.0);
            glp_set_obj_coef(lp, count, cost[i][j] * demand[j]);
            count++;
        }
    }

    //Creating rows
    glp_add_rows(lp, store[0] + store[1]);
    for (i = 1; i <= (store[0] + store[1]); i++)
    {
        if (i <= store[0])
        {
            glp_set_row_name(lp, i, "a");
            glp_set_row_bnds(lp, i, GLP_UP, 0.0, camp_size[i]);
        }
        else
        {
            glp_set_row_name(lp, i, "b");
            glp_set_row_bnds(lp, i, GLP_LO, 1.0, 0.0);
        }
    }

    // set row coefficients
    int size = 1 + (store[1] * store[0]) * (store[1] + store[0]);
    int ia[100] = {0};
    int ja[100] = {0};
    double ar[100] = {0.0};

    int x = 1, y = store[1], count;

    flag = 1;
    for (i = 1; i <= store[0]; i++)
    {
        count = 1;
        for (j = 1; j <= store[1] * store[0]; j++)
        {
            ia[flag] = i;
            ja[flag] = j;
            if (j >= x && j <= y)
            {
                ar[flag] = demand[count];
                count++;
            }
            flag++;
        }
        x += store[1];
        y += store[1];
    }

    count = 0;
    for (i = store[0] + 1; i <= store[1] + store[0]; i++)
    {
        for (j = 1; j <= store[1] * store[0]; j++)
        {
            ia[flag] = i;
            ja[flag] = j;
            if ((j) % store[1] == count + 1)
            {
                ar[flag] = 1;
            }
            flag++;
        }
        count++;
        if (count == store[1] - 1)
        {
            count = -1;
        }
    }

    //Loading matrix
    glp_load_matrix(lp, size - 1, ia, ja, ar);

    //Calling Simplex
    glp_simplex(lp, NULL);

    //Solution
    count = 1;
    for (i = 0; i < store[0]; i++)
    {
        for (j = 1; j <= store[1]; j++, count++)
        {
            printf("%f ", demand[j] * glp_get_col_prim(lp, count));
        }
        printf("\n");
    }

    glp_delete_prob(lp);

    return;
}

void read()
{
    int store[2] = {0}; //Store the locations and battleground values
    int flag = 0, flag2 = 0, j;

    //Opening the file.
    FILE *fptr;
    if ((fptr = fopen("/home/mrtz/Computing Lab - 1/Assignment 4/input.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    char c[1000];

    //reading the Locations and Battlegrounds values
    fgets(c, sizeof(c), fptr);
    for (j = 0; c[j] != '\0'; ++j)
    {
        if (isdigit(c[j]))
        {
            store[flag] = (store[flag] * 10) + c[j] - 48;
        }
        else
            flag++;
    }

    int camp_size[100] = {0};
    int demand[100] = {0};
    int cost[100][100] = {0};

    //Storing the camp sizes
    flag = 0;
    fgets(c, sizeof(c), fptr);
    for (j = 0; c[j] != '\0'; ++j)
    {
        if (isdigit(c[j]))
        {
            camp_size[flag] = (camp_size[flag] * 10) + c[j] - 48;
        }
        else
            flag++;
    }

    //Storing the battleground demand
    flag = 0;
    fgets(c, sizeof(c), fptr);
    for (j = 0; c[j] != '\0'; ++j)
    {
        if (isdigit(c[j]))
        {
            demand[flag] = (demand[flag] * 10) + c[j] - 48;
        }
        else
            flag++;
    }

    flag = 0;
    while (fgets(c, sizeof(c), fptr))
    {

        for (j = 0; c[j] != '\0'; ++j)
        {
            if (isdigit(c[j]))
            {
                cost[flag][flag2] = (cost[flag][flag2] * 10) + c[j] - 48;
                printf("%d", cost[flag][flag2]);
            }

            else
                flag2++;
        }
        flag++;
    }

    //Calling the Linear Programming function
    linear_prog(store, camp_size, demand, cost);
}

void main()
{
    read();

    return;
}