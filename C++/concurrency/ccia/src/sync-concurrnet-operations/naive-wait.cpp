#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

namespace SYNC_NAMESPACE
{
    class NaiveWait
    {
    private:
        std::thread thread1;
        std::thread thread2;
        bool flag;
        std::mutex m{};

    public:
        NaiveWait()
        {
            thread1 = std::thread([this]()
                                  { set_flag(); });
            thread2 = std::thread([this]()
                                  { wait_for_flag(); });
            thread1.join();
            thread2.join();
        }
        void set_flag()
        {
            std::this_thread::sleep_for(std::chrono::seconds(100));
            std::lock_guard<std::mutex> lk(m);
            std::cout << "Marked flag true \n";
            flag = true;
        }

        void wait_for_flag()
        {
            std::unique_lock<std::mutex> lk(m);
            while (!flag)
            {
                lk.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Slept for 100 ms for the flag to become true\n";
                lk.lock();
            }
        }
    };
}
