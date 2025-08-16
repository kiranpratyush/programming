#include<iostream>

struct Date{
    int month;
    int day;
    int year;
};
int main()
{
   Date d = {3,3,3};
   std::cout<<d.day<<d.month<<d.year<<std::endl;

}

/*

Invariants : Some property 

*/