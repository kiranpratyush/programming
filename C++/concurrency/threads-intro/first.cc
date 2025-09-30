#include <pthread.h>
#include <iostream>
#include "common.h"
#include "common_threads.h"

static volatile int x{};
extern "C"
{

    void *my_thread(void *arg)
    {
        for (int i = 0; i < 10000; i++)
        {
            x++;
        }
        return nullptr;
    }
    int main()
    {

        pthread_t thread1, thread2;
        Pthread_create(&thread1, nullptr, my_thread, (void *)"A");
        Pthread_create(&thread2, nullptr, my_thread, (void *)"B");
        Pthread_join(thread1, nullptr);
        Pthread_join(thread2, nullptr);
        std::cout << "x is " << x;
    }
}