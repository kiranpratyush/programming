#include <thread>
#include <iostream>
/*
The function object itself is passed by value , but
as this contains a reference to the variable defined inside the local function
it will contain the dangling pointer
you can use the thread.join to wait for the thread to be finished.
*/
struct func
{
    int &i;
    func(int &i_) : i{i_} {};
    void operator()()
    {
        for (unsigned j = 0; j < 100000; j++)
        {
        }
    }
};

void oops()
{
    int local_i = 0;
    func my_func(local_i);
    std::thread my_thread(my_func);
    my_thread.detach();
}

int main()
{
    oops();
    return 0;
}