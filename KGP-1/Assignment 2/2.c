//Murtaza Saifee
//20CS60R61

//File name: Test.txt
//I couldn't implement the program, cause of some misinterpretation I had till last moment and had to change 1 and 3 as well but I have completed the task 1,3 of Assignment.
//No added instructions. The input be made of User we search distance K contents for and Distance k.
//Output will come in format of x distance contents (1 <= x <= k)

#include <stdio.h>
#include <stdlib.h> 
#include<ctype.h>


struct Node
{
	int value;
	struct Node *next;
};	



struct User
{
	int id;
	struct Node *users;
	struct Node *contents;
};




void kDist(struct User *U[],int no_users , int no_contents)					
{
	int user , check_n[no_users] , k, check_c[no_contents];
	int nodes[no_users],flag=0;
	struct Node *ptr;

	for(int i = 0; i < no_users; i++) {check_n[i] = 0;}
	for(int i = 0; i < no_contents; i++) {check_c[i] = 0;}


	printf("Enter the User whose K-dist Contents we search: ");
	scanf("%d", &user);
	
	check_n[user] = 1;

	printf("Enter the K-distance: ");
	scanf("%d", &k);
	int last_k[k+1];
	last_k[0] = 0;
	
	
	

	for(int i = 0; i < k-1; i++)
	{
		printf("\n\nDistance %d Neighbours ", i+1);
		

		if(i == 0)										//For distance 1 neighbours we include all 
		{
			ptr = U[user]->users;
			while(ptr)
			{
				nodes[flag] = ptr->value;
				printf(" -> %d", nodes[flag]);
				flag++;
				check_n[(ptr->value)] = 1;
				ptr = ptr->next;
			}
		}

		else											//for remaining distance neighbours
		{
			for(int j = last_k[i-1]; j < last_k[i] ; j++ )
			{
				ptr = U[nodes[j]]->users;
				while(ptr)
				{
					
					if(check_n[(ptr->value)] == 0 )
					{
						nodes[flag] = ptr->value;
						printf(" -> %d", nodes[flag]);
						flag++;
						check_n[(ptr->value)] = 1;
					}
					ptr = ptr->next;
				}
			}
		}
		last_k[i+1] = flag;
	}

	flag = 0;
	for(int i = 0; i < k ; ++i)
	{
		printf("\n\nDistance %d Contents ", i+1);

		//For distance 1 content we include all
		if(i == 0)	 
		{
			ptr = U[user]->contents;
			while(ptr)
			{
				printf(" -> %d", ptr->value);
				check_c[(ptr->value)] = 1;
				ptr = ptr->next;
			}
		}

		//for remaining distance contents
		else											
		{
			for(int j = last_k[i-1]; j < last_k[i] ; j++ )
			{
				ptr = U[nodes[j]]->contents;
				while(ptr)
				{
					if(check_c[(ptr->value)] == 0 )
					{
						printf(" -> %d", ptr->value);
						check_c[(ptr->value)] = 1;
					}
					ptr = ptr->next;
				}
			}
		}
	}
}




void read() 		//Function to read from the file and call Kdist function
{
    int uid = 0,i,j , no_users = 0 , no_contents = 0, user_match = 0;
  	
  	char c[4100];



    struct Node *ptr;

    FILE *fptr;
    if ((fptr = fopen("Test.txt", "r")) == NULL) 		
    {
        printf("Error! opening file");																//check_ning if file is open
        exit(1);
    }



    fgets(c, sizeof(c) , fptr);
    for(j = 0 ; c[j+1] != '\0' ; ++j) {no_users = (no_users*10) + c[j] - 48;}


    struct User *U[no_users];
	//Allocting memory for user nodes
	for(int k = 0 ; k < no_users ; k++)
    {
    	U[k] = (struct User*)malloc(sizeof(struct User));											
    	U[k]->users = (struct Node*)malloc(sizeof(struct Node));
    	U[k]->contents = (struct Node*)malloc(sizeof(struct Node));
    }

    


    for(uid = 0 ; uid < no_users ; ++uid)
    {
    	
    	fgets(c, sizeof(c) , fptr);
    	
    	//Reading User ID
    	U[uid]->id = 0;
        for(j = 0 ; c[j] != ':' ; ++j)
		{
	    	U[uid]->id = ( U[uid]->id * 10 ) + c[j] - 48;									
		}


		ptr = U[uid]->users;
		ptr->value = 0;																		
		ptr->next = NULL;


		for(j++ ; c[j+1] != '\0' ; ++j)
		{
		    if(isdigit(c[j]))	{ptr->value = (ptr->value*10) + c[j] - 48;}										//Reading the value no


		    else
		    {
	        	ptr->next = (struct Node*)malloc(sizeof(struct Node));							//Creating the next node for next value
	        	ptr = ptr->next;
	        	ptr->value = 0;
	        	ptr->next = NULL;
		    }
	    
		}																				
    }//end of loop for neighbours reading




    fgets(c, sizeof(c) , fptr);					//Store no of contents
    for(j = 0 ; c[j+1] != '\0' ; ++j) {no_contents = (no_contents*10) + c[j] - 48;}



    for(uid = 0 ; uid < no_users ; ++uid)				//Loop to read all content nodes and store it in linked list
    {
    	
    	fgets(c, sizeof(c) , fptr);


        for(j = 0 ; c[j] != ':' ; ++j)
		{
	    	user_match = ( user_match * 10 ) + c[j] - 48;									//Reading User ID
		}


		ptr = U[uid]->contents;
		ptr->value = 0;																		//since user struct is different pointer
		ptr->next = NULL;


		for(j++ ; c[j+1] != '\0' ; ++j)
		{
		    if(isdigit(c[j]))	{ptr->value = (ptr->value*10) + c[j] - 48;}										//Reading the value no


		    else
		    {
	        	ptr->next = (struct Node*)malloc(sizeof(struct Node));						//Creating the next node for next value
	        	ptr = ptr->next;
	        	ptr->value = 0;
	        	ptr->next = NULL;
		    }
	    
		}																			
    }//end of loop for content reading



    kDist(U,no_users,no_contents);


}







int main()
{
	
	read();


	printf("\n\nEnd.\n\n");
	return 0;

}