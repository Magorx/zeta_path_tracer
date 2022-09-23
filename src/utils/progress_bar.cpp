#include "progress_bar.h"

namespace kctf {

void ProgressBar::print_prefix() {
    logger_.print("\r");
    logger_.print_log_prefix("prog", "bar", false, true, true);
}

void ProgressBar::on_start() {
    start_timestamp = std::chrono::system_clock::now();

    print_prefix();
    logger_.print("[>         ] |  0%%| [|]");
}

void ProgressBar::on_tick() {
    if ((int) (((double) cur_tick / capacity) * 100) <= cur_step) {
        return;
    } else {
        cur_step = (int) (((double) cur_tick / capacity) * 100);
    }

    print_prefix();
    logger_.print("[");
    int i = 0;
    for (; i < 10 && i < ((double) cur_tick / capacity) * 10 - 1; ++i) {
        logger_.print("=");
    }
    if (i < 10) {
        ++i;
        logger_.print(">");
    }
    for (; i < 10; ++i) {
        logger_.print(" ");
    }

    logger_.print("] |% 3d%%| [|]", cur_step);
}

void ProgressBar::on_stop() {
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_timestamp);

    print_prefix();
    logger_.print("[==========] |100%%| [+] (%ldms)\n",  elapsed.count());
}

void ProgressBar::turn_wheele() const {
    logger_.print("\b\b%c]", wheel_chars[cur_tick % wheel_chars_len]);
}

ProgressBar::ProgressBar(int capacity_, int verbosity_, Logger &logger):
cur_tick(0),
capacity(capacity_),
cur_step(1),
verbosity(verbosity_),
logger_(logger)
{}

bool ProgressBar::start(int new_capacity) {
    if (new_capacity > 0) {
        capacity = new_capacity;
    }

    if (capacity < 1) {
        return false;
    }

    cur_tick = 0;
    cur_step = 0;
    if (verbosity) on_start();
    return true;
}

bool ProgressBar::tick(const int tick_step) {
    if (cur_step < 0) {
        return false;
    }

    cur_tick += tick_step;
    if (cur_tick < capacity) {
        if (verbosity) on_tick();
        if (verbosity) turn_wheele();
        return true;
    } else {
        if (verbosity) on_stop();
        return false;
    }
}

} // namespace kctf
