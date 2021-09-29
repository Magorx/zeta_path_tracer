#include "logger.h"


Logger LOGGER(stdout);


Logger::Logger(FILE *fileptr, int reset_max_lens_counter):
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
tick_timer(0)
{
    if (!fileptr || !reset_max_lens_counter) {
        print_nullptr_passed_error();
    }
}

void Logger::print_nullptr_passed_error() const {
    fprintf(stdout, "[ERR] logger called with something == nullptr, it will crush your programm\n");
    fprintf(stderr, "[ERR] logger called with something == nullptr, it will crush your programm\n");
}

void Logger::tick() {
    ++tick_timer;
    if ((tick_timer % reset_max_lens_counter) == 0) {
        max_code_len = 0;
        max_announcer_len = 0;

        if (last_announcer) {
            free(last_announcer);
            last_announcer = nullptr;
        }

        if (last_code) {
            free(last_code);
            last_code = nullptr;
        }
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

void Logger::log(const char* code, const char* announcer, const char *message, ...) { // dirty code, I suppose it can be cleaned
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

    va_list args;
    va_start(args, message);
    vfprintf(fileptr, message, args);
    va_end(args);

    n();
}

void Logger::n() {
    fputc('\n', fileptr);
}
