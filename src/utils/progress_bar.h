#ifndef PROGRESS_BAR
#define PROGRESS_BAR

#include <cstdio>
#include <chrono>

class ProgressBar {
	int cur_tick;
	int capacity;
	int cur_step;
	int verbosity;
    std::chrono::time_point<std::chrono::system_clock> start_timestamp;
	FILE *file_ptr;

	static constexpr const char* wheel_chars = "|/-\\";
	static constexpr const int wheel_chars_len = 4;

	void on_start() {
        start_timestamp = std::chrono::system_clock::now();
		fprintf(file_ptr, "[PRG] [>         ] |  0%%| [|]");
	}

	void on_tick() {
		if ((int) (((double) cur_tick / capacity) * 100) <= cur_step) {
			return;
		} else {
			cur_step = (int) (((double) cur_tick / capacity) * 100);
		}

		fprintf(file_ptr, "\r"); // \x1b[K - clear

		fprintf(file_ptr, "[PRG] [");
		int i = 0;
		for (; i < 10 && i < ((double) cur_tick / capacity) * 10 - 1; ++i) {
			fprintf(file_ptr, "=");
		}
		if (i < 10) {
			++i;
			fprintf(file_ptr, ">");
		}
		for (; i < 10; ++i) {
			fprintf(file_ptr, " ");
		}

		fprintf(file_ptr, "] |% 3d%%| [|]", cur_step);
	}

	void on_stop() {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_timestamp);
		fprintf(file_ptr, "\r[PRG] [==========] |100%%| [+] (%ldms)\n",  elapsed.count());
	}

	void turn_wheele() const {
		fprintf(file_ptr, "\b\b%c]", wheel_chars[cur_tick % wheel_chars_len]);
	}

public:
	ProgressBar(FILE *file_ptr_, int capacity_ = 100, int verbosity_ = 1):
	cur_tick(0),
	capacity(capacity_),
	cur_step(1),
	verbosity(verbosity_),
	file_ptr(file_ptr_)
	{}

	bool start(int new_capacity=-1) {
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

	bool tick(const int tick_step = 1) {
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
};

#endif // PROGRESS_BAR
