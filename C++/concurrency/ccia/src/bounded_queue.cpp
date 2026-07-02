#include "bounded_queue.h"

namespace BOUNDED_QUEUE_NAIVE
{
    int NaiveBoundedQueue::get()
    {
        std::unique_lock<std::mutex> size_lock(size_mutex);
        while (size == 0)
            read_cond_var.wait(size_lock);
        auto head_node = std::move(head->next);
        int result = head_node->data;
        head->next = std::move(head_node->next);
        if (head->next == nullptr)
            tail = head.get();
        size -= 1;
        size_lock.unlock();
        write_cond_var.notify_one();
        return result;
    }
    void NaiveBoundedQueue::put(int data)
    {
        auto node = std::make_unique<Node>(data);
        std::unique_lock<std::mutex> size_lock(size_mutex);
        while (size == capacity)
            write_cond_var.wait(size_lock);
        tail->next = std::move(node);
        tail = tail->next.get();
        size += 1;
        size_lock.unlock();
        read_cond_var.notify_one();
    }

    void NaiveQueue::put(int data)
    {
        while (true)
        {
            std::unique_lock<std::mutex> queue_lock(queue_mutex);
            if (queue.size() < capacity)
            {
                queue.push(2);
                return;
            }
        }
    }
    int NaiveQueue::get()
    {
        while (true)
        {
            std::unique_lock<std::mutex> queue_lock(queue_mutex);
            if (queue.size() > 0)
            {
                int value = queue.front();
                queue.pop();
                return value;
            }
        }
        return -1;
    }
}