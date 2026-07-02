#include <thread>
#include <iostream>

struct funct
{
    int &i;
    funct(int &i_) : i(i_) {}
    void operator()()
    {
        for (unsigned j = 0; j < 1000000; j++)
        {
            std::cout << j << std::endl;
        }
    }
};

void oops()
{
    int some_local_state = 0;
    funct my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();
    std::this_thread::get_id();
}