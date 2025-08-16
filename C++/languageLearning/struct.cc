#include<iostream>

struct Date{
    int month;
    int day;
    int year;
};


struct Pair{
    int start;
    int end;
};
// Here the value to start and end can be assigned independently
int main()
{
   Date d = {3,3,3};
   std::cout<<d.day<<d.month<<d.year<<std::endl;

}

/*

Invariants : Some property which needs to be satisfied to make sure that the state is valid
*/

