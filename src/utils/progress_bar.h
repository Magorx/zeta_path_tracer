#pragma once

#include <cstdio>
#include <chrono>

#include <utils/logger.h>

namespace kctf {

class ProgressBar {
    int cur_tick;
    int capacity;
    int cur_step;
    bool stopped_;

    int non_tty_progress_counter = 0;

    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
    
    LoggerT::LoggerStreamT &stream_;
    std::string id_;

    static constexpr const char* wheel_chars = "|/-\\";
    static constexpr const int wheel_chars_len = 4;

    void print_prefix();

    void on_start();
    void on_tick ();
    void on_stop ();

    void turn_wheele() const;

public:
    ProgressBar(int capacity_=100, std::string id="", LoggerT::LoggerStreamT &stream=kctf::logger.nc_info);

    bool start(int new_capacity=-1);

    bool tick(int tick_step = 1);
    bool update(double frac);
};

} // namespace kctf
