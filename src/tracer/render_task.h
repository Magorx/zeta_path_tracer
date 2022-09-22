#ifndef RENDER_TASK
#define RENDER_TASK

#include "color/color.h"
#include <cstdio>
#include <cassert>
#include <vector>

struct RenderTask {
	int id;
	int min_x;
	int max_x;
	int min_y;
	int max_y;

	RenderTask();	
	RenderTask(const int min_x_, 
			   const int max_x_,
			   const int min_y_,
			   const int max_y_,
			   const int id_ = 0);

	int width()  const;
	int height() const;

	bool is_valid(const int strict_mode = 0);

	void save(const char *filename);
	void load(const char *filename);

	void dump(FILE *fout = stdout);

	std::vector<RenderTask> linear_split(const int parts_cnt);
};

void linear_render_task_split(const int width, const int height, const int parts_cnt, const char *rtask_ext = ".rt", 
							  const int offset_x = 0, const int offset_y = 0);

#endif // RENDER_TASK