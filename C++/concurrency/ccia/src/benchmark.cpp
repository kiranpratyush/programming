#include "benchmark.h"

// Benchmark configuration

static void producer_task(NaiveBoundedQueue &queue, int producer_id, int num_items, atomic<long long> &produced_count)
{
    for (int i = 0; i < num_items; ++i)
    {
        queue.put(producer_id * 1000000 + i);
        produced_count.fetch_add(1, memory_order_relaxed);
    }
}

static void consumer_task(NaiveBoundedQueue &queue, int num_items, atomic<long long> &consumed_count)
{
    for (int i = 0; i < num_items; ++i)
    {
        queue.get();
        consumed_count.fetch_add(1, memory_order_relaxed);
    }
}

void run_benchmark(const BenchmarkConfig &config)
{
    cout << "\n========================================" << endl;
    cout << "Benchmark Configuration:" << endl;
    cout << "  Queue Capacity: " << config.queue_capacity << endl;
    cout << "  Producers: " << config.num_producers << endl;
    cout << "  Consumers: " << config.num_consumers << endl;
    cout << "  Items per Producer: " << config.items_per_producer << endl;
    cout << "  Total Items: " << (config.num_producers * config.items_per_producer) << endl;
    cout << "========================================" << endl;

    NaiveBoundedQueue queue(config.queue_capacity);
    vector<thread> producers;
    vector<thread> consumers;
    atomic<long long> produced_count{0};
    atomic<long long> consumed_count{0};

    long long total_items = config.num_producers * config.items_per_producer;
    int items_per_consumer = total_items / config.num_consumers;
    int remainder = total_items % config.num_consumers;

    auto start_time = chrono::high_resolution_clock::now();

    // Start producers
    for (int i = 0; i < config.num_producers; ++i)
    {
        producers.emplace_back(producer_task, ref(queue), i, config.items_per_producer, ref(produced_count));
    }

    // Start consumers
    for (int i = 0; i < config.num_consumers; ++i)
    {
        int items_for_this_consumer = items_per_consumer + (i < remainder ? 1 : 0);
        consumers.emplace_back(consumer_task, ref(queue), items_for_this_consumer, ref(consumed_count));
    }

    // Wait for all producers to finish
    for (auto &producer : producers)
    {
        producer.join();
    }

    // Wait for all consumers to finish
    for (auto &consumer : consumers)
    {
        consumer.join();
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    // Calculate and display results
    double seconds = duration.count() / 1000.0;
    double throughput = total_items / seconds;

    long long actual_produced = produced_count.load();
    long long actual_consumed = consumed_count.load();
    bool counts_match = (actual_produced == total_items) && (actual_consumed == total_items) && (actual_produced == actual_consumed);

    cout << "\nResults:" << endl;
    cout << "  Duration: " << duration.count() << " ms (" << seconds << " s)" << endl;
    cout << "  Items Produced: " << actual_produced << " / " << total_items << (actual_produced == total_items ? " ✓" : " ✗") << endl;
    cout << "  Items Consumed: " << actual_consumed << " / " << total_items << (actual_consumed == total_items ? " ✓" : " ✗") << endl;
    cout << "  Correctness: " << (counts_match ? "PASSED ✓" : "FAILED ✗") << endl;
    cout << "  Throughput: " << fixed << throughput << " items/sec" << endl;
    cout << "  Throughput: " << fixed << (throughput / 1000000.0) << " million items/sec" << endl;
    cout << "========================================\n"
         << endl;
}