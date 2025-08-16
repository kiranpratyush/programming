#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>



int main()
{
    void *ptr = malloc(1);
    int *intptr = (int *)ptr;
    *intptr = 3;
    printf("%d",*intptr);


}