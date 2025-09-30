#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>
#include "common.h"
#include "common_threads.h"
#define nullptr NULL
int balance = 0;
typedef struct
{
    atomic_flag flag;
} spinlock_t;

void spinlock_init(spinlock_t *lock)
{
    atomic_flag_clear(&(lock->flag));
}

void spinlock_lock(spinlock_t *lock)
{
    while (atomic_flag_test_and_set_explicit(&(lock->flag), memory_order_acquire))
    {
    }
}
void spinlock_unlock(spinlock_t *lock)
{
    atomic_flag_clear_explicit(&(lock->flag), memory_order_relaxed);
}
spinlock_t spinlock;
void *increaseBalance(void *)
{
    spinlock_lock(&spinlock);
    for (int i = 0; i < 1000000; i++)
    {
        balance = balance + 1;
    }
    spinlock_unlock(&spinlock);
    return nullptr;
}

int main()
{
    spinlock_init(&spinlock);
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
    printf("%d", balance);
    return 0;
}