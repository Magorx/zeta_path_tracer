#include "render_task.h"

RenderTask::RenderTask(const int min_x_, const int max_x_, const int min_y_, const int max_y_, const int id_):
id(id_),
min_x(min_x_),
max_x(max_x_),
min_y(min_y_),
max_y(max_y_)
{}

void RenderTask::dump(FILE *fout) {
	fprintf(fout, "RTask[ % 5d ]\n", id);
	fprintf(fout, "min_x[ % 5d ]_max_x_[ % 5d ]\n", min_x, max_x);
	fprintf(fout, "min_y[ % 5d ]_max_y_[ % 5d ]\n", min_y, max_y);
}

bool RenderTask::is_valid(const int strict_mode) {
	if (min_x < 0 || min_x > max_x || min_y < 0 || min_y > max_y) {
		if (strict_mode) {
			return false;
		}

		if (min_x > max_x) {
			std::swap(min_x, max_x);
		}
		if (min_y > max_y) {
			std::swap(min_y, max_y);
		}
	}

	return true;
}

void RenderTask::save(const char *filename) {
	assert(filename);
	FILE *fout = fopen(filename, "w");
	assert(fout);

	fprintf(fout, "RTask[ % 5d ]\n", id);
	fprintf(fout, "min_x[ % 5d ]_max_x_[ % 5d ]\n", min_x, max_x);
	fprintf(fout, "min_y[ % 5d ]_max_y_[ % 5d ]\n", min_y, max_y);

	fclose(fout);
}

void RenderTask::load(const char *filename) {
	assert(filename);
	FILE *fin = fopen(filename, "r");
	assert(fin);

	char strdump[50];

	fscanf(fin, "%s %d %s", strdump, &id, strdump);
	fscanf(fin, "%s %d %s", strdump, &min_x, strdump);
	fscanf(fin, "%s %d %s", strdump, &max_x, strdump);
	fscanf(fin, "%s %d %s", strdump, &min_y, strdump);
	fscanf(fin, "%s %d %s", strdump, &max_y, strdump);
	dump();

	fclose(fin);
}