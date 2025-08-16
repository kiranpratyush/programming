#include<iostream>

void function(int &x)
{
    std::cout<<x<<std::endl;
    std::cout<<"Address of x"<<&x<<std::endl;
}

int main()
{
    int x =20;
    std::cout<<"Address of x outside of function is : "<<&x<<std::endl;
    function(x);
    
}