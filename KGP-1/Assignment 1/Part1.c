//Murtaza Saifee
//20CS60R61

//File name: Test.txt
//No added instructions. It will read from the file and print the maximum length as well as the path.



#include <stdio.h>
#include <stdlib.h> 
#include<ctype.h>
#define MAX(x,y) ((x > y)? x : y)


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

void disp(struct Node *ptr, int length)
{
	if(length)
	{
		disp(ptr->prev, length-1);														//Tail recursion to print the path
		printf(" --> %d", ptr->path);
	}
}


void dynamic(struct User *U[], int size[2]) 
{
    int max = 0;
    struct Node *u1 , *u2 , *last;														//last ptr for pointing last node of max chain
      

    int dp[size[0] + 1][size[1] + 1]; 
    u1 = U[0]->next;
    u2 = U[1]->next; 

    for(int k = 0 ; k <= size[1] ; k++)
    {
    	dp[0][k] = 0;
    }
    
    for (int i = 1 ; i <= size[0]; i++) 
    { 
        for (int j = 0 ; j <= size[1]; j++) 
        { 
        	if( j == 0)
        	{
        		dp[i][j] = 0;
        		continue;
        	}


            else if(u1->path == u2->path)
            {
            	dp[i][j] = dp[i - 1][j - 1] + 1;
            	if(max < dp[i][j])
            	{
            		max = dp[i][j];
            		last = u1;

            	}
            	
            }
            else
            {
            	dp[i][j] = 0 ;
            }
            u2 = u2->next;

        }
        u1 = u1->next;
        u2 = U[1]->next;
    } 
    
    printf("\nMaximum path lenght : %d" , max);
    printf("\n\nPath ");
    disp(last,max);
}


void read(struct User *U[2]) 
{
    int uid = 0,i,j;
  	int size[2] = {1,1};

    for(int k = 0 ; k < 2 ; k++)
    {
    	U[k] = (struct User*)malloc(sizeof(struct User));											//Allocting memory for user nodes
    	U[k]->next = (struct Node*)malloc(sizeof(struct Node));
    }
    


    struct Node *ptr;

    FILE *fptr;
    if ((fptr = fopen("/home/mrtz/Computing Lab - 1/Assignment 1/Test.txt", "r")) == NULL) 		
    {
        printf("Error! opening file");																//Checking if file is open
        exit(1);
    }

    char c[4100];

    while(fgets(c, sizeof(c) , fptr))
    {
    	

    	U[uid]->id = 0;
        for(j = 0 ; c[j] != ':' ; ++j)
		{
	    	U[uid]->id = ( U[uid]->id * 10 ) + c[j] - 48;									//Reading User ID
		}


		ptr = U[uid]->next;
		ptr->prev = NULL;																	// assigning null to previous pointer
		ptr->path = 0;																		//since user struct is different pointer
		ptr->next = NULL;


		for(j++ ; c[j] != '\0' ; ++j)
		{
		    if(isdigit(c[j]))
		    {						
	    	    ptr->path = (ptr->path*10) + c[j] - 48;										//Reading the path no
	    	}


		    else
		    {
	        	ptr->next = (struct Node*)malloc(sizeof(struct Node));
	        	ptr->next->prev = ptr; 														//Creating the next node for next path
	        	ptr = ptr->next;
	        	ptr->path = 0;
	        	ptr->next = NULL;
	        	size[uid]++;
		    }
	    
		}
		uid++; 																				//updating user id value
    }
    dynamic(U,size);

}

int main()
{
	struct User *U[2];

	read(U);

	printf("\n\nEnd.\n\n");
	return 0;

}