#pragma once

#include <cstdio>
#include <chrono>

#include <utils/logger.h>

namespace kctf {

class ProgressBar {
    int cur_tick;
    int capacity;
    int cur_step;
    int verbosity;
    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
    Logger &logger_;

    static constexpr const char* wheel_chars = "|/-\\";
    static constexpr const int wheel_chars_len = 4;

    void print_prefix();

    void on_start();
    void on_tick ();
    void on_stop ();

    void turn_wheele() const;

public:
    ProgressBar(int capacity_=100, int verbosity_=1, Logger &logger=kctf::logger);

    bool start(int new_capacity=-1);

    bool tick(const int tick_step = 1);
};

} // namespace kctf
