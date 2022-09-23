#include "logger.h"

namespace kctf {
    Logger logger;
}


Logger::Logger(FILE *fileptr, int log_level, int reset_max_lens_counter):
fileptr(fileptr),
to_close_file(false),
last_announcer(nullptr),
last_announcer_len(0),
last_code(nullptr),
last_code_len(0),
max_code_len(0),
max_announcer_len(0),
to_print_announcer(false),
to_print_code(true),
offset(0),
reset_max_lens_counter(reset_max_lens_counter),
tick_timer(0),
log_level(log_level),
verb_level(5),
page_cnt(0),
paging_mode(0)
{
    if (!reset_max_lens_counter) {
        reset_max_lens_counter = 1;
    }
}

Logger::Logger(const std::string &filename, int log_level, int reset_max_lens_counter):
Logger(nullptr, log_level, reset_max_lens_counter)
{
    if (filename == "" || filename==":stdout:") {
        fileptr = stdout;
    } else if (filename == ":stderr:") {
        fileptr = stderr;
    } else {
        fileptr = fopen(filename.c_str(), "w");
        to_close_file = true;
    }

    if (!fileptr) {
        print_nullptr_passed_error();
    }
}

Logger::~Logger() {
    if (to_close_file) {
        fclose(fileptr);
    }
}

void Logger::print_nullptr_passed_error() const {
    fprintf(stdout, "[ERR] logger called with something == nullptr, it will crush your programm\n");
    fprintf(stderr, "[ERR] logger called with something == nullptr, it will crush your programm\n");
}

void Logger::reset_max_lens() {
    max_code_len = 0;
    max_announcer_len = 0;
}

void Logger::tick() {
    ++tick_timer;
    if ((tick_timer % reset_max_lens_counter) == 0) {
        reset_max_lens();

        reset_lasts();
    }
}

void Logger::reset_lasts() {
    if (last_announcer) {
        free(last_announcer);
        last_announcer = nullptr;
    }

    if (last_code) {
        free(last_code);
        last_code = nullptr;
    }
}

void Logger::update_announcer(const char* announcer) {
    if (announcer && (!last_announcer || strcmp(announcer, last_announcer))) {
        to_print_announcer = true;
        // to_print_code = true;

        if (last_announcer) free(last_announcer);

        last_announcer = strdup(announcer);
        last_announcer_len = strlen(last_announcer);
    } else {
        to_print_announcer = false;
    }

    max_announcer_len = max_announcer_len > last_announcer_len ? max_announcer_len : last_announcer_len;
}

void Logger::update_code(const char *code) {
    if (code && (!last_code || strcmp(code, last_code))) {
        // to_print_announcer = true;
        to_print_code = true;

        if (last_code) free(last_code);

        last_code = strdup(code);
        last_code_len = strlen(last_code);
    } else {
        to_print_code = false;
    }

    max_code_len = max_code_len > last_code_len ? max_code_len : last_code_len;
}

void Logger::print(const char *message, ...) {
    va_list args;
    va_start(args, message);
    vfprintf(fileptr, message, args);
    va_end(args);
}

void Logger::print_n_spaces(int n) {
    ++n;
    while (--n > 0) {
        fputc(' ', fileptr);
    }
}

void Logger::print_aligned(Align align, int size, const char *format, ...) {
    static const int BUFFER_LEN = 256;
    static char BUFFER[BUFFER_LEN];

    memset(BUFFER, 0, BUFFER_LEN);

    va_list args;
    va_start(args, format);
    vsnprintf(BUFFER, BUFFER_LEN, format, args);
    va_end(args);

    size_t str_len = strlen(BUFFER);
    size_t spare_spaces = size - str_len;

    if ((int) align < 0) {
        print("%s", BUFFER);
        print_n_spaces(spare_spaces);
    } else if ((int) align > 0) {
        print("%s", BUFFER);
        print_n_spaces(spare_spaces);
    } else {
        size_t left_spaces = spare_spaces / 2;
        size_t right_spaces = spare_spaces - left_spaces;

        print_n_spaces(left_spaces);
        print("%s", BUFFER);
        print_n_spaces(right_spaces);
    }
}

void Logger::print_log_prefix(
    const char* code,
    const char* announcer,
    bool to_tick,
    bool force_code,
    bool force_announcer
) {
    if (to_tick) {
        tick();
    }

    update_announcer(announcer);
    update_code(code);

    if (to_print_code || force_code) {
        fputc('[', fileptr);
        print_aligned(Align::middle, max_code_len, "%s", code);
        fputc(']', fileptr);
    } else if (to_print_announcer || force_announcer) {
        fputc('[', fileptr);
        print_n_spaces(max_code_len);
        fputc(']', fileptr);
    } else {
        print_n_spaces(max_code_len + 2);
    }
    // print_n_spaces(max_code_len - last_code_len);

    if (to_print_announcer || force_announcer) {
        if (!htlm_mode) {
            fputc('<', fileptr);
            print_aligned(Align::middle, max_announcer_len, "%s", announcer);
            fputc('>', fileptr);
            // fprintf(fileptr, "<%s>", announcer);
        } else {
            print("&lt;");
            print_aligned(Align::middle, max_announcer_len, "%s", announcer);
            print("&gt;");
            // fprintf(fileptr, "%s", announcer);
        }
    } else if (to_print_code || force_code) {
        fputc('<', fileptr);
        print_n_spaces(max_announcer_len);
        fputc('>', fileptr);
    } else {
        print_n_spaces(max_announcer_len + 2);
    }
    // print_n_spaces(max_announcer_len - last_announcer_len);
    
    fprintf(fileptr, " : ");
    print_n_spaces(offset);
}

void Logger::_log(bool to_ignore_log_level, const char* code, const char* announcer, const char *message, va_list arglist) { // dirty code, I suppose it can be cleaned
    if (log_level > verb_level && !to_ignore_log_level) return;

    if (!announcer || !code || !message) {
        print_nullptr_passed_error();
        return;
    }

    print_log_prefix(code, announcer);
    vfprintf(fileptr, message, arglist);

    n();
}

void Logger::log(const char* code, const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, code, announcer, message, args);
    va_end(args);
}

void Logger::logr(const char* code, const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, code, announcer, message, args);
    va_end(args);

    resets();
}

void Logger::log(int override_log_level, const char* code, const char* announcer, const char *message, ...) {
    if (override_log_level < verb_level) return;

    va_list args;
    va_start(args, message);
    _log(true, code, announcer, message, args);
    va_end(args);
}

void Logger::logv(int override_log_level, const char* code, const char* announcer, const char *message, va_list args) {
    if (override_log_level < verb_level) return;

    _log(true, code, announcer, message, args);
}

void Logger::error(const char* announcer, const char *message, ...) {
    int prev_log_level = log_level;
    set_log_level(Level::error);

    va_list args;
    va_start(args, message);
    _log(false, "ERROR", announcer, message, args);
    va_end(args);

    log_level = prev_log_level;
}

void Logger::ERROR(const char* announcer, const char *message, ...) {
    int prev_log_level = log_level;
    set_log_level(Level::error);

    va_list args;
    va_start(args, message);
    _log(false, "!--ERROR--!", announcer, message, args);
    va_end(args);

    log_level = prev_log_level;
}

void Logger::info(const char* announcer, const char *message, ...) {
    int prev_log_level = log_level;
    set_log_level(Level::info);

    va_list args;
    va_start(args, message);
    _log(false, "info", announcer, message, args);
    va_end(args);

    log_level = prev_log_level;
}

void Logger::warning(const char* announcer, const char *message, ...) {
    int prev_log_level = log_level;
    set_log_level(Level::warning);

    va_list args;
    va_start(args, message);
    _log(false, "~~~~", announcer, message, args);
    va_end(args);

    log_level = prev_log_level;
}

void Logger::doubt(const char* announcer, const char *message, ...) {
    int prev_log_level = log_level;
    set_log_level(Level::info);

    va_list args;
    va_start(args, message);
    _log(false, "????", announcer, message, args);
    va_end(args);

    log_level = prev_log_level;
}

void Logger::n() {
    fputc('\n', fileptr);
}

void Logger::page_cut(const char *page_name, Level log_level_, int page_len, char symb) {
    int prev_log_level = log_level;
    set_log_level(log_level_);

    if (log_level > verb_level) return;

    while (page_len-- > 0) fputc(symb, fileptr);
    
    if (page_name) {
        fprintf(fileptr, " [%d] %s", page_cnt++, page_name);
    } else if (paging_mode) {
        fprintf(fileptr, " [%d]", page_cnt++);
    }
    n();

    resets();
    set_log_level(prev_log_level);
}

int  Logger::get_log_level() const {
    return log_level;
}

void Logger::set_log_level(int log_level_) {
    log_level = log_level_;
}

void Logger::set_log_level(Logger::Level log_level_) {
    log_level = (int) log_level_;
}

int  Logger::get_verb_level() const {
    return verb_level;
}

void Logger::set_verb_level(int verb_level_) {
    verb_level = verb_level_;
}

void Logger::set_verb_level(Logger::Level verb_level_) {
    verb_level = (int) verb_level_;
}

void Logger::set_offset(int new_offset) {
    offset = new_offset;
}

void Logger::shift_offset(int shift) {
    set_offset(offset + shift);
}


LogLevel::LogLevel(Logger &logger, int log_level, int verb_level):
logger(logger),
prev_log_level(logger.get_log_level()),
prev_verb_level(logger.get_verb_level())
{   
    if (log_level  > 0) logger.set_log_level(log_level);
    if (verb_level > 0) logger.set_verb_level(verb_level);
}

LogLevel::~LogLevel() {
    logger.set_log_level(prev_log_level);
    logger.set_verb_level(prev_verb_level);
}
