#ifndef RENDER_TASK
#define RENDER_TASK

#include "color.h"
#include <cstdio>
#include <cassert>

struct RenderTask {
	int id;
	int min_x;
	int max_x;
	int min_y;
	int max_y;
	
	RenderTask(const int min_x_, 
			   const int max_x_,
			   const int min_y_,
			   const int max_y_,
			   const int id_ = 0);

	bool is_valid(const int strict_mode = 0);

	void save(const char *filename);
	void load(const char *filename);

	void dump(FILE *fout = stdout);
};

#endif // RENDER_TASK