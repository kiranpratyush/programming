#include <atomic>
#include <iostream>

static inline int fetch_and_add(std::atomic<int> &variable, int value)
{ // Equivalent to lock; xadd with sequential consistency
    return variable.fetch_add(value, std::memory_order_seq_cst);
}

int main()
{
    std::atomic<int> counter{0};

    int old1 = fetch_and_add(counter, 5);
    std::cout << "Old: " << old1 << ", New: " << counter.load() << "\n";

    int old2 = fetch_and_add(counter, 3);
    std::cout << "Old: " << old2 << ", New: " << counter.load() << "\n";

    return 0;
}
