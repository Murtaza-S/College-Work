//Murtaza Saifee
//20CS60R61

//File name: Test3.txt
//No added instructions. The input be provided for whom we recommend the contents. And program should output the result.

#include <stdio.h>
#include <stdlib.h> 
#include<ctype.h>


struct Node
{
	int value;
	struct Node *next;
};	

struct Content
{
	int value;
	int visibility;
	struct Content *next;
};



struct User
{
	int id;
	struct Node *users;
	struct Content *contents;
};




void contentVisibility(struct User *U[],int no_users , int no_contents , int public_content[] , int public_no)					
{
	int user , check_n[no_users] , k, check_c[no_contents];
	int nodes[no_users],flag=0;
	struct Node *ptr;
	struct Content *ptr_c;

	for(int i = 0; i < no_users; i++) {check_n[i] = 0;}
	for(int i = 0; i < no_contents; i++) {check_c[i] = 0;}


	printf("Enter the User for whom we recommend the contents : ");
	scanf("%d", &user);
	
	check_n[user] = 1;

	
	//find the distance 1 and 2 friends to recommend contents
	int last_k[3];
	last_k[0] = 0;
	
	
	

	for(int i = 0; i < 2; i++)
	{
		
		//For distance 1 friends we include all 
		if(i == 0)										
		{
			ptr = U[user]->users;
			while(ptr)
			{
				nodes[flag] = ptr->value;
				flag++;
				check_n[(ptr->value)] = 1;
				ptr = ptr->next;
			}
		}


		//for distance 2 : friends of friends
		else											
		{
			for(int j = 0; j < last_k[1] ; j++ )
			{
				ptr = U[nodes[j]]->users;
				while(ptr)
				{
					
					if(check_n[(ptr->value)] == 0 )
					{
						nodes[flag] = ptr->value;
						flag++;
						check_n[(ptr->value)] = 1;
					}
					ptr = ptr->next;
				}
			}
		}
		last_k[i+1] = flag;
	}

	//This to mark all the content that user already follows
	ptr_c = U[user]->contents;
	while(ptr_c)
	{
		check_c[(ptr_c->value)] = 1;
		ptr_c = ptr_c->next;
	}








	flag = 0;
	for(int i = 0; i < 3 ; ++i)
	{
		//For distance 1 content i.e. Public content 
		if(i == 0)										
		{
			printf("\n\nPublic based contents recommended : ");
			for(int j = 0 ; j < public_no ; ++j) 
			{
				if(check_c[public_content[j]] == 0)
					printf(" -> %d",public_content[j]);
			}
		}

		
		//for remaining distance contents
		else											
		{
			if( i == 1) {printf("\nContents recommendation based on friends ");}
			else {printf("\nContents recommendation based on friends of friends ");}
			for(int j = last_k[i-1]; j < last_k[i] ; j++ )
			{
				ptr_c = U[nodes[j]]->contents;
				while(ptr_c)
				{
					if(check_c[(ptr_c->value)] == 0 )
					{
						if( i <= ptr_c->visibility )
						{
							printf(" -> %d", ptr_c->value);
							check_c[(ptr_c->value)] = 1;

						}
						
					}
					ptr_c = ptr_c->next;
				}
			}
		}
	}
}




void read() 		//Function to read from the file and call function
{
    int uid = 0,i,j , no_users = 0 , no_contents = 0, user_match = 0;
  	
  	char c[4100] , last;




    struct Node *ptr;
    struct Content *ptr_c;

    FILE *fptr;
    if ((fptr = fopen("Test3.txt", "r")) == NULL) 		
    {
        printf("Error! opening file");																//check_ning if file is open
        exit(1);
    }



    fgets(c, sizeof(c) , fptr);
    for(j = 0 ; c[j+1] != '\0' ; ++j) {no_users = (no_users*10) + c[j] - 48;}


    struct User *U[no_users];
	for(int k = 0 ; k < no_users ; k++)
    {
    	U[k] = (struct User*)malloc(sizeof(struct User));											//Allocting memory for user nodes
    	U[k]->users = (struct Node*)malloc(sizeof(struct Node));
    	U[k]->contents = (struct Content*)malloc(sizeof(struct Content));
    }

    


    for(uid = 0 ; uid < no_users ; ++uid)
    {
    	
    	fgets(c, sizeof(c) , fptr);
    	U[uid]->id = 0;
        for(j = 0 ; c[j] != ':' ; ++j)
		{
	    	U[uid]->id = ( U[uid]->id * 10 ) + c[j] - 48;									//Reading User ID
		}


		ptr = U[uid]->users;
		ptr->value = 0;																		//since user struct is different pointer
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

    int public_content[no_contents] , public_no = 0;



    for(uid = 0 ; uid < no_users ; ++uid)				//Loop to read all content nodes and store it in linked list
    {
    	
    	fgets(c, sizeof(c) , fptr);


        for(j = 0 ; c[j] != ':' ; ++j)
		{
	    	user_match = ( user_match * 10 ) + c[j] - 48;									//Reading User ID
		}


		ptr_c = U[uid]->contents;
		ptr_c->value = 0;
		ptr_c->visibility = 0;																		//since user struct is different pointer
		ptr_c->next = NULL;
		last = ';';


		for(j++ ; c[j+2] != '\0' ; ++j)
		{
		    if(isdigit(c[j]))	
		    {	
		    	if(last == ';')
		    	{
		    		ptr_c->value = (ptr_c->value*10) + c[j] - 48;
		    			
		    	}
		    	if(last == ',')
		    	{
		    		ptr_c->visibility = c[j] - 48;

		    		if(ptr_c->visibility == 0)
		    		{
		    			public_content[public_no] = ptr_c->value;
		    			public_no++;
		    		}

		    	}
		    	
		    }										


		    else if(last == ',')
		    {
	        	ptr_c->next = (struct Content*)malloc(sizeof(struct Content));						//Creating the next node for next value
	        	ptr_c = ptr_c->next;
	        	ptr_c->value = 0;
	        	ptr_c->visibility = 0;
	        	ptr_c->next = NULL;
	        	last = ';';
		    }

		    else
		    {
		    	last = ',';
		    }
	    
		}																			
    }//end of loop for content reading


    contentVisibility(U,no_users,no_contents,public_content,public_no);


}







int main()
{
	
	read();


	printf("\n\nEnd.\n\n");
	return 0;

}