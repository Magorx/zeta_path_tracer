#include <cstdio>
#include <cstdlib>
#include <ctime>

double vec3d_randdouble(double dmin, double dmax) {
    double x = (double)rand() / RAND_MAX;
    return dmin + x * (dmax - dmin);
}

int main() {
	srand(time(NULL));
	for (int i = 0; i < 20; ++i) {
		printf("%lg\n", vec3d_randdouble(0, 1));
	}

	return 0;
}