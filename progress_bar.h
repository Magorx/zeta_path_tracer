#ifndef PROGRESS_BAR
#define PROGRESS_BAR

#include <cstdio>

class ProgressBar {
	int cur_tick;
	int capacity;
	int    cur_step;
	double step_scale;
	FILE *file_ptr;

	void on_start() {
		fprintf(file_ptr, "[PRG]<progress_bar>|+++++|\n");
		fprintf(file_ptr, "[PRG] % 3d%%         |     |\n", 0);
	}

	void on_tick() {
		cur_tick++;
		if (capacity * step_scale * cur_step <= cur_tick && abs(cur_tick * 100 / capacity - 100.0) > 0.0001) {
			fprintf(file_ptr, "[PRG] % 3d%%         |     |\n", cur_tick * 100 / capacity);
		}
		cur_step = ((double) cur_tick / capacity / step_scale) + 1;
	}

	void on_stop() {
		fprintf(file_ptr, "[PRG] %d%%         |     |\n", 100);
		fprintf(file_ptr, "[PRG]<progress_bar>|-----|\n");
	}

public:
	ProgressBar(FILE *file_ptr_, int capacity_ = 100, double step_scale_ = 0.1):
	cur_tick(0),
	capacity(capacity_),
	cur_step(1),
	step_scale(step_scale_),
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
		cur_step = 1;
		on_start();
		return true;
	}

	bool tick(const int tick_step = 1) {
		if (cur_step < 0) {
			return false;
		}

		cur_tick += tick_step;
		if (cur_tick < capacity) {
			on_tick();
			return true;
		} else {
			cur_step = -1;
			on_stop();
			return false;
		}
	}
};

#endif // PROGRESS_BAR