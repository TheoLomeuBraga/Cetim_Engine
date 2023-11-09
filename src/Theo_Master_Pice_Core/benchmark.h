#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <string>

struct benchmark_data_struct
{
    size_t calls;
    size_t miliseconds_time;
};
typedef struct benchmark_data_struct benchmark_data;

std::map<std::string, benchmark_data> benchmark_results;

class Benchmark_Timer
{
private:
    using clock = std::chrono::high_resolution_clock;
    std::chrono::time_point<clock> start_time;
    std::string benchmark_class;

public:
    Benchmark_Timer(std::string benchmark_class)
    {
        start_time = std::chrono::high_resolution_clock::now();
        this->benchmark_class = benchmark_class;
    }

    ~Benchmark_Timer()
    {
        auto end_timepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(start_time).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(end_timepoint).time_since_epoch().count();
        size_t duration_ms = static_cast<size_t>(end - start);

        if (benchmark_results.find(benchmark_class) != benchmark_results.end())
        {
            benchmark_data bd = benchmark_results[benchmark_class];
            bd.calls++;
            bd.miliseconds_time += duration_ms;
            benchmark_results[benchmark_class] = bd;
        }
        else
        {
            benchmark_data bd;
            bd.calls = 1;
            bd.miliseconds_time = duration_ms;
            benchmark_results[benchmark_class] = bd;
        }
    }
};

void print_benchmark_results()
{

    std::cout << std::setfill('-') << std::setw(30 * 4) << "" << std::setfill(' ') << std::endl;

    std::cout << std::setw(30) << std::left << "benchmark_class"
              << std::setw(30) << std::left << "calls"
              << std::setw(30) << std::left << "seconds_time"
              << std::setw(30) << std::left << "miliseconds_average_time" << std::endl;
              
    std::cout << std::setfill('-') << std::setw(30 * 4) << "" << std::setfill(' ') << std::endl;

    for (std::pair<std::string, benchmark_data> p : benchmark_results)
    {
        std::cout << std::setw(30) << std::left << p.first
                  << std::setw(30) << std::left << p.second.calls
                  << std::setw(30) << std::left << p.second.miliseconds_time / 1000.0f
                  << std::setw(30) << std::left << p.second.miliseconds_time / p.second.calls << std::endl;
    }

    std::cout << std::setfill('-') << std::setw(30 * 4) << "" << std::setfill(' ') << std::endl;
}