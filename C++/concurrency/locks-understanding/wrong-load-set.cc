/*
This is has two issue
- correctness
- waste cpu cycle


*/

#include <iostream>
#include "common.h"
#include "common_threads.h"
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
typedef struct
{
    int flag{};
} lock_t;

void lock(lock_t *mutex)
{
    while (mutex->flag == 1)
        ;
    mutex->flag = 1;
}
void unlock(lock_t *mutex)
{
    mutex->flag = 0;
}

lock_t mutex;
int balance = 0;

void *increaseBalance(void *)
{
    Pthread_mutex_lock(&mutex_lock);
    // lock(&mutex);
    for (int i = 0; i < 1000000; i++)
    {
        balance = balance + 1;
    }
    Pthread_mutex_unlock(&mutex_lock);
    // unlock(&mutex);
    return nullptr;
}
int main()
{
    pthread_t thread1, thread2, thread3, thread4, thread5;
    Pthread_create(&thread1, nullptr, increaseBalance, nullptr);
    Pthread_create(&thread2, nullptr, increaseBalance, nullptr);
    Pthread_create(&thread3, nullptr, increaseBalance, nullptr);
    Pthread_create(&thread4, nullptr, increaseBalance, nullptr);
    Pthread_create(&thread5, nullptr, increaseBalance, nullptr);
    Pthread_join(thread1, nullptr);
    Pthread_join(thread2, nullptr);
    Pthread_join(thread3, nullptr);
    Pthread_join(thread4, nullptr);
    Pthread_join(thread5, nullptr);
    std::cout << "Balance is " << balance;
    return 0;
}