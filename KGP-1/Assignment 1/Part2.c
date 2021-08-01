//Murtaza Saifee
//20CS60R61

//File name: Test.txt

//The function is working for path when both the string are exactly same and a brute force length is already applied but fails in other. 
//I couldn't get time to solve this issue in time.
//Path length defined in line 240 and for input string of exact same path and defined legth this does work.


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


struct Node
{
    int path;
    struct Node *next;
    struct Node *prev;
};  



struct User
{
    int id;
    struct Node *next;
};

struct hash_node
{
    struct Node *start;
    struct hash_node *extra;
};




int search(struct Node *p1 , struct Node *p2, int length)
{
    for(int i = 0; i < length; ++i)
    {
        if(p1->path != p2->path)
            return -1;
    }
    return 1;
}



void rolling_hash(struct User *U[2] , int length) 
{
   int hash = 0 , pow = 1, i, find;
   

    int base= 3;
    int mod = 7;
    struct hash_node hash_table[mod];
    for(int k = 0; k < mod ; ++k)
    {
        hash_table[k].start = NULL;
        hash_table[k].extra = NULL;
    }

   

    struct Node *start, *ptr;


    for (i = 0; i < length; i++)
      pow = ( pow * base) % mod;                                                 //reducing the size of base^length and comparing in integer format
   
    //Hashing for User 1
    i=0;
    for (start = ptr = U[0]->next; ptr != NULL; ptr = ptr->next) 
    {
        hash = ( ( hash * base ) + ptr->path ) % mod;                             //hashing each path in window
        i++;

        if (i >= length) 
        {
            if(hash_table[hash].start == NULL)                                     //Checking for collision
            {
                hash_table[hash].start = start;
            }
            else                                                                    //If a collision occurs
            {
                struct hash_node *flag = hash_table[hash].extra;   
                
                if(hash_table[hash].extra != NULL)
                {
                    for( ; flag->extra != NULL; flag = flag->extra);
                    flag->extra = (struct hash_node*)malloc(sizeof(struct hash_node));
                    flag = flag->extra;
                    flag->start = start;
                    flag->extra = NULL;
                }
                

                else
                {
                    flag->extra = (struct hash_node*)malloc(sizeof(struct hash_node));
                    flag = flag->extra;
                    flag->start = start;struct hash_node *flag = hash_table[hash].extra;
                    flag->extra = NULL;
                }

            }
            printf("\n%d" , hash);

            hash -= (pow * start->path) % mod;                                            //Sliding the window
            if (hash < 0)
                hash += mod;    

            start = start->next;
        }

    }


    //Hashing for User 2
    hash = 0;
    i=0;
    for (start = ptr = U[1]->next; ptr != NULL; ptr = ptr->next) 
    {
        hash = ( ( hash * base ) + ptr->path ) % mod;                             //hashing each path in window
        i++;

        if (i >= length) 
        {
            if(hash_table[hash].start != NULL)                                     //Checking for collision
            {
            
                find = search(hash_table[hash].start, start , length);
                
                if(find == 1)
                {
                    printf("Maximum path length = %d" , length);
                    exit(1);
                }
                else
                {
                    if(hash_table[hash].extra != NULL)
                    {
                        struct hash_node *flag = hash_table[hash].extra;
                        for( ; flag!= NULL; flag = flag->extra)
                        {
                            find = search(hash_table[hash].start, start, length);
                            if(find == 1)
                            {
                                printf("Maximum path length = %d" , length);
                                exit(1);
                            }
                        }
                    }
                }

            }
            printf("\n%d" , hash);

            hash -= (pow * start->path) % mod;                                            //Sliding the window
            if (hash < 0)
                hash += mod;    

            start = start->next;
        }

    }

    


}



void read(struct User *U[2]) 
{
    int uid = 0,i,j;
    int size[2] = {1,1};

    for(int k = 0 ; k < 2 ; k++)
    {
        U[k] = (struct User*)malloc(sizeof(struct User));                                           //Allocting memory for user nodes
        U[k]->next = (struct Node*)malloc(sizeof(struct Node));
    }
    


    struct Node *ptr;

    FILE *fptr;
    if ((fptr = fopen("/home/mrtz/Computing Lab - 1/Assignment 1/Test.txt", "r")) == NULL)      
    {
        printf("Error! opening file");                                                              //Checking if file is open
        exit(1);
    }

    char c[4100];

    while(fgets(c, sizeof(c) , fptr))
    {
        

        U[uid]->id = 0;
        for(j = 0 ; c[j] != ':' ; ++j)
        {
            U[uid]->id = ( U[uid]->id * 10 ) + c[j] - 48;                                   //Reading User ID
        }


        ptr = U[uid]->next;
        ptr->prev = NULL;                                                                   // assigning null to previous pointer
        ptr->path = 0;                                                                      //since user struct is different pointer
        ptr->next = NULL;


        for(j++ ; c[j] != '\0' ; ++j)
        {
            if(isdigit(c[j]))
            {                       
                ptr->path = (ptr->path*10) + c[j] - 48;                                     //Reading the path no
            }


            else
            {
                ptr->next = (struct Node*)malloc(sizeof(struct Node));
                ptr->next->prev = ptr;                                                      //Creating the next node for next path
                ptr = ptr->next;
                ptr->path = 0;
                ptr->next = NULL;
                size[uid]++;
            }
        
        }
        uid++;                                                                              //updating user id value
    }
    rolling_hash(U,2);

}




int main() 
{
   struct User *U[2];

   read(U);


   return 0;
}