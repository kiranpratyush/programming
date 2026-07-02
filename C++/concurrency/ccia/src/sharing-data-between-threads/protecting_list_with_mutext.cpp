#include "protecting_list_with_mutex.h"

namespace PROTECT_NAMESPACE
{
    void add_to_list(int new_value)
    {
        std::lock_guard<std::mutex> guard(some_mutex);
        some_list.push_back(new_value);
    }
    bool list_contains(int values_to_find)
    {
        std::lock_guard<std::mutex> guard(some_mutex);
        return std::find(some_list.begin(), some_list.end(), values_to_find) != some_list.end();
    }
}