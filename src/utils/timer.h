#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

#include <cstdio>
#include <chrono>

class Timer {
    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
    std::chrono::time_point<std::chrono::system_clock> stop_timestamp;
    bool is_stopped;

    FILE *fileptr;

public:
    int id;
    long int elapsed;

    Timer(int id = 0, FILE *fileptr = stdout, bool not_to_start = false);

    void start();
    void restart();
    void stop();
    void print(bool put_new_line = true, const char *format = nullptr);
};

#endif // UTIL_TIMER_H