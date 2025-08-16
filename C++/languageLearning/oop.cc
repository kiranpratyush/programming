#include<iostream>
#include<string>

/* Basic introduction to class types in C++*/

struct IntPair{
    int x{};
    int y{};
    void print()
    {
        std::cout<<"Pair("<<x<<","<<y<<")"<<std::endl;
    }
    bool isEqual()
    {
        return x==y;
    }
};

/* Example of Const objects
1. Remember in JavaScript : inside data members can be modifiable,but in C++ the internal members
are also not modifiable.



*/

struct Point{
    int x{};
    int y{};
    void print() const // const member function is allowed in the const declaration
    {
        std::cout<<"X:"<<x<<"Y:"<<y<<std::endl;
    }
    int getX() const{
        return x;
    }
    void reset(){
        x = 0;
        y=0;
    }
};

/* Const member function overloading */

struct Point2{
    int x{};
    int y{};
    int getX() const{
        return x;
    }
    int& getX(){
        return x;
    }
};



int main() {
    IntPair pair1;
    pair1.print();
    IntPair pair2 ={2,3};
    pair2.print();  
    std::cout<<pair1.isEqual()<<std::endl;
    std::cout<<pair2.isEqual()<<std::endl; 
    /*Const examples*/
    Point p2;
    const Point p3;
    p2.getX();
    p2.reset();
    /*P2 can call both const and non const member function*/
    p3.getX();
    /*p3 can call only the const member function*/

    /*const overloading member function*/
    const Point2 constPoint;
    Point2 normalPoint;
    std::cout<<constPoint.getX()<<std::endl;
    int &x = normalPoint.getX();
    x = 30;
    std::cout<<normalPoint.x<<std::endl;



}
