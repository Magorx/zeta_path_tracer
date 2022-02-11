#include <cstdarg>
#include "logger.h"


Logger logger;


Logger::Logger(FILE *fileptr, int log_level, int reset_max_lens_counter):
fileptr(fileptr),
last_announcer(nullptr),
last_announcer_len(0),
last_code(nullptr),
last_code_len(0),
max_code_len(0),
max_announcer_len(0),
to_print_announcer(false),
to_print_code(true),
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

    if (!fileptr) {
        print_nullptr_passed_error();
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

void Logger::print_n_spaces(size_t n) {
    ++n;
    while (--n) {
        fputc(' ', fileptr);
    }
}

void Logger::_log(bool to_ignore_log_level, const char* code, const char* announcer, const char *message, va_list arglist) { // dirty code, I suppose it can be cleaned
    if (log_level < verb_level && !to_ignore_log_level) return;

    if (!announcer || !code || !message) {
        print_nullptr_passed_error();
        return;
    }

    tick();

    update_announcer(announcer);
    update_code(code);

    if (to_print_code) {
        fprintf(fileptr, "[%s]", code);
    } else if (to_print_announcer) {
        fputc('[', fileptr);
        print_n_spaces(last_code_len);
        fputc(']', fileptr);
    } else {
        print_n_spaces(last_code_len + 2);
    }
    print_n_spaces(max_code_len - last_code_len);

    if (to_print_announcer) {
        fprintf(fileptr, "<%s>", announcer);
    } else if (to_print_code) {
        fputc('<', fileptr);
        print_n_spaces(last_announcer_len);
        fputc('>', fileptr);
    } else {
        print_n_spaces(last_announcer_len + 2);
    }
    print_n_spaces(max_announcer_len - last_announcer_len);
    
    fprintf(fileptr, " : ");

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

void Logger::error(const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, "ERROR", announcer, message, args);
    va_end(args);
}

void Logger::ERROR(const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, "!--ERROR--!", announcer, message, args);
    va_end(args);
}

void Logger::info(const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, "INFO", announcer, message, args);
    va_end(args);
}

void Logger::warning(const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, "~~~~", announcer, message, args);
    va_end(args);
}

void Logger::doubt   (const char* announcer, const char *message, ...) {
    va_list args;
    va_start(args, message);
    _log(false, "????", announcer, message, args);
    va_end(args);
}

void Logger::n() {
    fputc('\n', fileptr);
}

void Logger::page_cut(const char *page_name, int page_len, char symb) {
    while (page_len-- > 0) fputc(symb, fileptr);
    
    if (page_name) {
        fprintf(fileptr, " [%d] %s", page_cnt++, page_name);
    } else if (paging_mode) {
        fprintf(fileptr, " [%d]", page_cnt++);
    }
    n();

    resets();
}

int  Logger::get_log_level() const {
    return log_level;
}

void Logger::set_log_level(int log_level_) {
    log_level = log_level_;
}

int  Logger::get_verb_level() const {
    return verb_level;
}

void Logger::set_verb_level(int verb_level_) {
    verb_level = verb_level_;
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
