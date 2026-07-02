#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

class ThreadGuard
{
    std::thread &active_thread;

public:
    explicit ThreadGuard(std::thread &thread) : active_thread{thread}
    {
    }
    ThreadGuard(ThreadGuard const &) = delete;
    ThreadGuard &operator=(ThreadGuard const &) = delete;

    ~ThreadGuard()
    {
        if (active_thread.joinable())
        {
            std::cout << "Join is getting called\n";
            active_thread.join();
        }
    }
};

void worker()
{
    for (int i = 1; i < 10; i++)
    {
        std::cout << "Count for" << i << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main()
{
    try
    {
        std::thread t{worker};
        ThreadGuard tg{t};
        throw std::runtime_error("something went wrong");
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what();
    }
}