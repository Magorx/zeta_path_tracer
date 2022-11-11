#pragma once

#include <cstdio>
#include <chrono>
#include <string>

#include <utils/logger.h>

namespace kctf {

class Timer {
    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
    std::chrono::time_point<std::chrono::system_clock> stop_timestamp;
    bool is_stopped;

    LoggerT::LoggerStreamT &stream_;

    uint64_t cur_elapsed;

public:
    using IdType = std::string;
    const IdType id;

    Timer(const IdType &id = "", bool not_to_start = false, LoggerT::LoggerStreamT &stream=kctf::logger.nc_info);

    void start();
    void restart();
    void stop();

    uint64_t elapsed();

    void print();
};

} // namespace kctf
