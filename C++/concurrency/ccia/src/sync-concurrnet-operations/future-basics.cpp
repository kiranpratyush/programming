#include <future>
#include <chrono>
#include <iostream>
#include <thread>
namespace FUTURE_NAMESPACE
{
    int find_answer(int x)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return x;
    }

    void response()
    {
        std::future<int> fut = std::async(find_answer, 40);
        std::cout << "Waiting for the answer..." << std::endl;
        int answer = fut.get();
        std::cout << "The answer is: " << answer << std::endl;
    }

}
