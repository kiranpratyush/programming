#include<cstdio>
#include<cstring>
#include<vector>

using namespace std;

#define mod 1000000007


int main(){

// freopen("input.txt","r",stdin);
// freopen("output.txt","w",stdout);
int total_width;
scanf("%d\n",&total_width);
vector<vector<int>>maze(total_width,vector<int>(total_width,0));
char stringinput[total_width+5];

for(int j=0;j<total_width;j++)
{
     fgets(stringinput,sizeof(stringinput),stdin);
    for(int i=0;i<total_width;i++)
    {
        if(stringinput[i]=='.')
        {
            maze[j][i] = 0;
        }
        else{
            maze[j][i] = -1;
        }
    }
}
if(maze[total_width-1][total_width-1]==0)
{
    maze[total_width-1][total_width-1] =1;
}
for(int row = total_width-1;row>=0;row--)
{
    for(int col = total_width-1;col>=0;col--)
    {   if(row==total_width-1 && col == total_width-1)
        {
            continue; 
            // This is just to ignore the bottom right cell again
        }
        if(maze[row][col]==-1)
        {
            maze[row][col]=0;
        }
        else{
            // go right
            int right = col+1;
            int down = row+1;
            int total_ways = 0;
            if(right<total_width)
            {
                total_ways = (total_ways+maze[row][right])%mod;

            }
            if(down<total_width)
            {
                total_ways = (total_ways+maze[down][col])%mod;
            }
            maze[row][col] = total_ways;
        }
    }
}
if(maze[0][0]<0)
{
    printf("%d",0);
}
else{
    printf("%d",maze[0][0]);
}



}