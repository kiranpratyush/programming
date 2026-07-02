#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <iostream>

class ThreadSafeQueue
{
    std::queue<int> dataQueue;
    std::mutex mut;
    std::condition_variable dataCond;

public:
    void push(int new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataQueue.push(new_value);
        dataCond.notify_one();
    }
    void pop(int &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this]()
                      { return !dataQueue.empty(); });
        value = dataQueue.front();
        dataQueue.pop();
    }
};

void experiment()
{
    ThreadSafeQueue q;
    std::thread producer([&]()
                         {
        for (int i = 0; i < 10; ++i) {
            q.push(i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        for (int i = 0; i < 5; ++i) {
            q.push(-1);
        } });
    std::vector<std::thread> consumers;
    for (int i = 0; i < 5; ++i)
    {
        consumers.emplace_back([&]()
                               {
            while (true) {
                int val;
                q.pop(val);
                if (val == -1) break;
                std::cout << "Consumer " << std::this_thread::get_id() << " consumed: " << val << std::endl;
            } });
    }
    producer.join();

    for (auto &t : consumers)
    {
        t.join();
    }

    std::cout << "Experiment completed." << std::endl;
}