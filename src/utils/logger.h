#pragma once


#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <string>
#include <vector>


#define LOOG(format, ...) printf(format "\n", ##__VA_ARGS__);


class Logger {
    FILE *fileptr;
    bool to_close_file;
    
    char  *last_announcer;
    size_t last_announcer_len;

    char  *last_code;
    size_t last_code_len;

    size_t max_code_len;
    size_t max_announcer_len;

    bool to_print_announcer;
    bool to_print_code;

    int offset;

    int reset_max_lens_counter;
    int tick_timer;

    int log_level;
    int verb_level;

    int page_cnt;

    bool htlm_mode = false;

    void update_announcer(const char* announcer);
    void update_code(const char *code);

    void reset_lasts();

    void print_nullptr_passed_error() const;

    void tick();

    void _log(bool to_ignore_log_level, const char* code, const char* announcer, const char *message, va_list arglist);

public:
    int paging_mode;

    enum class Level {
        none = 0,
        error = 2,
        warning = 4,
        info = 6,
        debug = 10,
    };

    enum class Align {
        left = -1,
        middle = 0,
        right = 1
    };

    Logger(const std::string &filename="", int log_level=5, int reset_max_lens_counter=50);
    Logger(FILE *fileptr, int log_level=5, int reset_max_lens_counter=50);

    ~Logger();

    Logger(const Logger &other) = delete;
    Logger &operator=(const Logger &other) = delete;

    void print(const char *message, ...);

    void print_log_prefix(const char* code, const char* announcer);

    void log(const char* code, const char* announcer, const char *message, ...); // normal logging
    void log(int override_log_level, const char* code, const char* announcer, const char *message, ...);
    void logv(int override_log_level, const char* code, const char* announcer, const char *message, va_list args);
    void logr(const char* code, const char* announcer, const char *message, ...); // reset maxlens and lasts
    
    void error   (const char* announcer, const char *message, ...);
    void ERROR   (const char* announcer, const char *message, ...);
    void info    (const char* announcer, const char *message, ...);
    void warning (const char* announcer, const char *message, ...);
    void doubt   (const char* announcer, const char *message, ...);

    void print_n_spaces(int n);
    void n();
    void print_aligned(Align align, int size, const char *format, ...);
    void page_cut(const char *page_name=nullptr, Level log_level_=Level::info, int page_len = 80, char symb = '=');

    inline void resets() {
        reset_lasts();
        reset_max_lens();
        tick_timer = 0;
    }

    int  get_log_level() const;
    void set_log_level(int log_level_);
    void set_log_level(Logger::Level log_level_);

    int  get_verb_level() const;
    void set_verb_level(int verb_level_);
    void set_verb_level(Logger::Level verb_level_);

    void reset_max_lens();

    void set_offset(int new_offset);
    void shift_offset(int shift);

    void tag_close(int tag_cnt=1);

    void set_html_mode(bool new_html_mode) {
        htlm_mode = new_html_mode;
    }

    inline FILE *get_log_file() { return fileptr; }
};

namespace kctf {
    extern Logger logger;
}

class LogLevel {
    Logger &logger;
    int prev_log_level;
    int prev_verb_level;

public:
    LogLevel(Logger &logger, int log_level = -1, int verb_level = -1);
    ~LogLevel();
};

using kctf::logger;
