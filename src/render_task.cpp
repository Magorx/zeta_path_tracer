#include "render_task.h"

RenderTask::RenderTask():
id(0),
min_x(0),
max_x(0),
min_y(0),
max_y(0)
{}


RenderTask::RenderTask(const int min_x_, const int max_x_, const int min_y_, const int max_y_, const int id_):
id(id_),
min_x(min_x_),
max_x(max_x_),
min_y(min_y_),
max_y(max_y_)
{}

int RenderTask::width() const {
	return std::abs(max_x - min_x);
}

int RenderTask::height() const {
	return std::abs(max_y - min_y);
}

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
	fscanf(fin, "%d %s", &max_x, strdump);
	fscanf(fin, "%s %d %s", strdump, &min_y, strdump);
	fscanf(fin, "%d %s", &max_y, strdump);

	fclose(fin);
}

void RenderTask::linear_split(const int parts_cnt, const int random_name_modifier) {
	is_valid();

	char rtask_ext[50] = {};
	sprintf(rtask_ext, "_%d.rt", random_name_modifier);

	int width  = max_x - min_x;
	int height = max_y - min_y;

	printf("h %d w %d\n", height, width);

	linear_render_task_split(width, height, parts_cnt, rtask_ext, min_x, min_y);
}

void linear_render_task_split(const int width, const int height, const int parts_cnt, const char *rtask_ext,
							  const int offset_x, const int offset_y) {
	char rtask_file_name[50] = {};

	int rt_height_cnt = height / parts_cnt;
	int rt_width_cnt = width;

	for (int i = 0; i < parts_cnt - 1; ++i) {
		sprintf(rtask_file_name, "%d%s", i, rtask_ext);
		RenderTask rt(offset_x, offset_x + rt_width_cnt, offset_y + i * rt_height_cnt, offset_y + (i + 1) * rt_height_cnt, i);
		rt.save(rtask_file_name);
	}

	sprintf(rtask_file_name, "%d%s", parts_cnt - 1, rtask_ext);
	RenderTask rt(offset_x, offset_x + rt_width_cnt, offset_y + (parts_cnt - 1) * rt_height_cnt, offset_y + height, parts_cnt - 1);
	rt.save(rtask_file_name);
}
