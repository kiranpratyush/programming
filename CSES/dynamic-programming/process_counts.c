#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    int i=0;
    for(i=3;i>0;i++)
    {
        fork();
    }
    printf("Something\n");
    exit(0);
    
}