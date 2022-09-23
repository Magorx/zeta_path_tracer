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

    Logger &logger_;

    uint64_t cur_elapsed;

public:
    using IdType = std::string;
    const IdType id;

    Timer(const IdType &id = "", bool not_to_start = false, Logger &logger=kctf::logger);

    void start();
    void restart();
    void stop();

    uint64_t elapsed();

    void print(const char *format = nullptr);
};

} // namespace kctf
