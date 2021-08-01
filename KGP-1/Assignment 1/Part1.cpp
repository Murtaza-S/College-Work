//Murtaza Saifee
//20CS60R61

//File to be read has end of line as \n instead of endl I couldn't get the bug out for endl and still dont know why.
//File name : Test.txt

#include<bits/stdc++.h> 

using namespace std; 


int CDist(int A[], int B[], int n, int m) 
{
    int maxm = 0;
      

    int dp[n + 1][m + 1]; 
    for(int i = 0; i <= n ;i++) 
        for(int j = 0; j <= m ;j++) 
            dp[i][j] = 0; 
      
    
    for (int i = 1 ; i <= n; i++) 
    { 
        for (int j = 1 ; j <= m; j++) 
        { 

            if (A[i-1] == B[j-1]) 
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                maxm = max(maxm, dp[i][j]);
            }

        } 
    } 
    
    return maxm; 
}
  

void inalpha()
{

    ifstream meetup;
    char alpha;
    int num,A[2][1000],size[2];


    meetup.open("Test.txt"); 
    if (meetup.is_open())
    {
        int i,j;
        for(i = 0; i<2 ; ++i)
        {
            alpha = ':';
            meetup>>num>>alpha;
            cout<<alpha<<num<<endl;
            j = 0;
            while(!meetup.eof() && (alpha == ',' || alpha == ':'))
            {
                meetup>>num>>alpha;
                A[i][j] = num;
                j++;
            }
            cout<<alpha;
            size[i] = j;
            cout<<j<<"  "<<i<<'\n';
            
        }
        meetup.close();
    }
    cout << (CDist(A[0], A[1], size[0], size[1]));
}








int main() 
{ 
    inalpha();
} 
