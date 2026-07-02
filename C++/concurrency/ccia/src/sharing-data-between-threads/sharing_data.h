#ifndef SHARING_DATA
#define SHARING_DATA
#include <thread>
#include <vector>
#include <mutex>
#include <shared_mutex>

namespace sharingData
{

    class SharingData
    {
    private:
        std::vector<int> data{49, 20};
        std::shared_mutex rwlock{};

    public:
        inline int read_data()
        {
            std::shared_lock<std::shared_mutex> lock(rwlock);
            int value = data.at(0);
            return value;
        }
        inline void write_data(int d)
        {
            std::unique_lock<std::shared_mutex> lock(rwlock);
            data.push_back(d);
        }
    };
    void run();
}

#endif