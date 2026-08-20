#include <pthread.h>
#include <iostream>
#include "common.h"
#include "common_threads.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int balance{};

void *increaseBalance(void *)
{
    Pthread_mutex_lock(&lock);
    balance = balance + 1;
    Pthread_mutex_unlock(&lock);
    return nullptr;
}

int main()
{
    pthread_t thread1, thread2;
    Pthread_create(&thread1, nullptr, increaseBalance, nullptr);
    Pthread_create(&thread2, nullptr, increaseBalance, nullptr);
    Pthread_join(thread1, nullptr);
    Pthread_join(thread2, nullptr);
    std::cout << "Balance is " << balance;
    return 0;
}