#include<stdio.h>

int toi(int n){
    if(n==1){
        return 1;
    }
    if(n==2){
        return 3;
    }
    return 2*toi(n-1)+1;
}


int main(){
    char buffer[10];
    fgets(buffer,10,stdin);
    int n;
    sscanf(buffer,"%d",&n);
    printf("%d",toi(n));
}