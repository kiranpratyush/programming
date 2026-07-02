#include "sharing_data.h"
#include <iostream>

namespace sharingData
{
    void run()
    {
        SharingData x{};
        std::thread t1([&x]()
                       { std::cout << x.read_data(); });
        std::thread t2([&x](int data)
                       { x.write_data(data); }, 5);
        t1.join();
        t2.join();
    }
}