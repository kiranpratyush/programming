#include "packaged_task.h"

namespace PACKAGED_TASK_NAMESPACE
{

    static int simple_add(int a, int b)
    {
        return a + b;
    }
    void run()
    {
        std::packaged_task<int(int, int)> task(simple_add);
        std::future<int> result = task.get_future();
        std::thread t(std::move(task), 5, 10);
        std::cout << "Waiting for result...\n";
        int value = result.get();
        std::cout << "Result" << value << std::endl;
        t.join();
    }

}