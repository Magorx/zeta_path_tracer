#ifndef UTIL_LOGGER_H
#define UTIL_LOGGER_H


#include <string.h>
#include <cstdio>
#include <cstdlib>


class Logger {
    FILE *fileptr;
    
    char  *last_announcer;
    size_t last_announcer_len;

    char  *last_code;
    size_t last_code_len;

    size_t max_code_len;
    size_t max_announcer_len;

    bool to_print_announcer;
    bool to_print_code;

    int reset_max_lens_counter;
    int tick_timer;


    void update_announcer(const char* announcer);
    void update_code(const char *code);

    void print_n_spaces(size_t n);

    void print_nullptr_passed_error() const;

    void tick();

public:
    Logger(FILE *fileptr = stdout, int reset_max_lens_counter = 10);

    void log(const char* code, const char* announcer, const char *message, ...);
    void n();
};

extern Logger LOGGER;

#endif // UTIL_LOGGER_H
