#include "timer.h"

Timer::Timer(int id, FILE *fileptr, bool not_to_start):
is_stopped(false),
fileptr(fileptr),
id(id),
elapsed(0)
{
    if (!fileptr) {
        printf("[ERR] timer started with specified fileptr = nullptr\n");
        return;
    }

    if (!not_to_start) start_timestamp = std::chrono::system_clock::now();
}

void Timer::start() {
    is_stopped = false;
    start_timestamp = std::chrono::system_clock::now();
}

void Timer::restart() {
    is_stopped = false;
    start_timestamp = std::chrono::system_clock::now();
}

void Timer::stop() {
    is_stopped = true;
    stop_timestamp = std::chrono::system_clock::now();

    stop_timestamp = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop_timestamp - start_timestamp).count();
}

void Timer::print(bool put_new_line, const char *format) {
    stop();

    if (!format) {
        if (id) {
            format = "[TMR]<%d> %ldms";
        } else {
            format = "[TMR] %ldms";
        }
    }

    if (id) {
        fprintf(fileptr, format, id, elapsed);
    } else {
        fprintf(fileptr, format, elapsed);
    }

    if (put_new_line) {
        fprintf(fileptr, "\n");
    }
}
