//Murtaza Saifee
//20CS60R61

//File name: Test2.txt
//No added instructions.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <glpk.h>

void linear_prog(int store[], int camp_size[], int fixed_cost[], int demand[], int cost[][100])
{
    int i, j, flag;

    //Objective Function
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "Relocation");
    glp_set_obj_dir(lp, GLP_MIN);

    //Creating columns
    glp_add_cols(lp, store[0] * (store[1] + 1));

    int count = 1;
    for (i = 1; i <= store[0]; i++)
    {
        for (j = 1; j <= store[1] + 1; j++)
        {
            if (j != store[1])
            {
                glp_set_col_name(lp, count, "c");
                glp_set_col_bnds(lp, count, GLP_DB, 0.0, 1.0);
                glp_set_obj_coef(lp, count, cost[i][j] * demand[j]);
            }
            else
            {
                glp_set_col_kind(lp, count, GLP_BV); //0 or 1              //no clue
                glp_set_obj_coef(lp, count, fixed_cost[i]);
            }
            count++;
        }
    }

    //Creating rows
    glp_add_rows(lp, store[0] + store[1] + 1);
    for (i = 1; i <= (store[0] + store[1]); i++)
    {
        if (i < store[0])
        {
            glp_set_row_name(lp, i, "a");
            glp_set_row_bnds(lp, i, GLP_UP, 0.0, 0.0);
        }
        else if (i < store[0] + store[1] + 1)
        {
            glp_set_row_name(lp, i, "b");
            glp_set_row_bnds(lp, i, GLP_LO, 1.0, 0.0);
        }
        else
        {
            glp_set_row_name(lp, i + 1, "row");
            glp_set_row_bnds(lp, i + 1, GLP_UP, 0.0, store[2]);
        }
    }

    // set row coefficients
    int ia[100] = {0};
    int ja[500] = {0};
    double ar[100] = {0.0};

    int x = 1, y = store[1], flag2, flag3 = store[1] + 1, s = 0;

    flag = 1;
    for (i = 1; i <= store[0]; i++)
    {
        count = 1;
        flag2 = 0;
        for (j = 1; j <= (store[1] + 1) * store[0]; j++, flag++)
        {
            ia[flag] = i;
            ja[flag] = j;
            if (j >= x && j <= y && (j % (store[1] + 1)))
            {
                ar[flag] = demand[count];
                count++;
            }
            if (!(j % (i * (store[1] + 1))) && j == flag3 && !flag2)
            {
                ar[flag] = (-1) * camp_size[s++];
                flag3 = (s + 1) * (store[1] + 1);
                flag2++;
            }
        }
        x += store[1];
        y += store[1];
    }

    count = 0;
    for (i = store[0] + 1; i <= store[1] + store[0] + 1; i++)
    {
        flag2 = 0;
        for (j = 1; j <= (store[1] + 1) * store[0]; j++)
        {
            flag++;
            ia[flag] = i;
            ja[flag] = j;
            if ((j - 1) % (store[1] + 1) == count && i != (store[0] + store[1] + 1))
                ar[flag] = 1;

            else if ((j) % (store[1] + 1) == count && i == (store[0] + store[1] + 1))
                ar[flag] = 1;
        }
        count++;
        if (count == store[1] - 1)
        {
            count = -1;
        }
    }

    //Loading matrix
    int size;
    glp_load_matrix(lp, (store[0] + store[1] + 1) * ((store[1] + 1) * store[0]), ia, ja, ar);

    //Calling Simplex
    glp_simplex(lp, NULL);
    glp_intopt(lp, NULL);

    //Solution
    count = store[1] + 1;
    for (int j = 1; j <= store[0]; j++)
    {
        printf("%f ", glp_mip_col_val(lp, count * j));
    }
    printf("\n");

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
    if ((fptr = fopen("/home/mrtz/Computing Lab - 1/Assignment 4/input2.txt", "r")) == NULL)
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
    int fixed_cost[100] = {0};
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

    flag = 0;
    fgets(c, sizeof(c), fptr);
    for (j = 0; c[j] != '\0'; ++j)
    {
        if (isdigit(c[j]))
        {
            fixed_cost[flag] = (fixed_cost[flag] * 10) + c[j] - 48;
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
    linear_prog(store, camp_size, fixed_cost, demand, cost);
}

void main()
{
    read();

    return;
}