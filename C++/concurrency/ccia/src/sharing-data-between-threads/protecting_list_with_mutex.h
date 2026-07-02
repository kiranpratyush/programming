#pragma once

#include <list>
#include <thread>
#include <mutex>
#include <algorithm>

namespace PROTECT_NAMESPACE
{
    std::list<int> some_list{};
    std::mutex some_mutex;

    void add_to_list(int new_value);

    bool list_contains(int values_to_find);

    inline void run()
    {
        std::thread t1(add_to_list, 20);
        std::thread t2(list_contains, 20);
        t1.join();
        t2.join();
    }

}