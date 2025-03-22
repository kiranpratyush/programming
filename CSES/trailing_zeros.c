#include<stdio.h>



int main(){
  int number;
  scanf("%d",&number);
  int num1 = number;
  int num2 = number;
  int multiple_of_5=0;
  int multiple_of_2=0;
  while(num1>=2){
    multiple_of_2+=(num1/2);
    num1 = num1/2;
  }
  while(num2>=5){
    multiple_of_5+=(num2/5);
    num2 = num2/5;
  }
  if(multiple_of_2<multiple_of_5){
    printf("%d",multiple_of_2);
  }
  else{
    printf("%d",multiple_of_5);
  }

}
