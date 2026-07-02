#pragma once
#include "bounded_queue.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

using namespace std;
using namespace BOUNDED_QUEUE_NAIVE;

struct BenchmarkConfig
{
    int queue_capacity;
    int num_producers;
    int num_consumers;
    int items_per_producer;

    BenchmarkConfig(int cap, int prod, int cons, int items)
        : queue_capacity(cap), num_producers(prod),
          num_consumers(cons), items_per_producer(items) {}
};

void run_benchmark(const BenchmarkConfig &config);