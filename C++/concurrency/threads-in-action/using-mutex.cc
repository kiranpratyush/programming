#include <thread>
#include <iostream>
#include <algorithm>
#include <list>
#include <mutex>
class ListOperation
{
    int value{};
    std::mutex some_mutex{};
    std::list<int> some_list{};

public:
    void add_to_list(int);
    bool list_contains(int);
    int &get_value();
};

void ListOperation::add_to_list(int value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(value);
}

bool ListOperation::list_contains(int value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value) != some_list.end();
}
/*
This is problematic now I have shared the private data as reference to
outside world.
Now any one can update it.
A safer alternative is to use
const int& get_value() const {
    return value;

}
*/
int &ListOperation::get_value()
{
    return value;
}

int main()
{
    ListOperation List{};
    std::thread t1(&ListOperation::add_to_list, &List, 20);
    std::thread t2(&ListOperation::add_to_list, &List, 59);
    std::thread t3(&ListOperation::list_contains, &List, 20);
    int &x = List.get_value();
    x = 20;
    std::cout << List.get_value();
    t1.join();
    t2.join();
    t3.join();
}

/*

*/