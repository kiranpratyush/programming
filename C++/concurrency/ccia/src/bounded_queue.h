#ifndef BOUNDED_QUEUE
#define BOUNDED_QUEUE
#include <queue>
#include <condition_variable>
#include <memory>
#include <climits>
#include <mutex>

namespace BOUNDED_QUEUE_NAIVE
{
    class Node
    {
    public:
        int data;
        std::unique_ptr<Node> next{};
        Node(int _data) : data(_data) {}
    };

    class AbstractBoundedQueue
    {
    public:
        virtual void put(int item) = 0;
        virtual int get() = 0;
    };

    class NaiveQueue : public AbstractBoundedQueue
    {
    private:
        int capacity;
        std::mutex queue_mutex;
        std::queue<int> queue;

    public:
        int get() override;
        void put(int item) override;
    };

    class NaiveBoundedQueue : public AbstractBoundedQueue
    {
    private:
        int capacity;
        int size;
        std::mutex size_mutex;
        std::unique_ptr<Node> head;
        Node *tail;
        std::condition_variable read_cond_var;
        std::condition_variable write_cond_var;

    public:
        explicit NaiveBoundedQueue(int _capacity) : capacity(_capacity), size(0)
        {
            head = std::make_unique<Node>(INT_MIN);
            tail = head.get();
        }
        int get() override;
        void put(int data) override;
    };

}
#endif