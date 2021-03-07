#include "path_tracer.h"
#include "collection_hittables.h"
#include "collection_materials.h"

// settings ===================================================================

const int PERCENT_STEP       = 10;

const int SCREEN_WIDTH       = 256;
const int SCREEN_HEIGHT      = 256;
const double RESOLUTION_COEF = 4;
const int MAX_TRACE_DEPTH    = 500;
const int PIXEL_SAMPLING     = 100; 

// ============================================================================

int main() {
	srand(time(NULL));

    conf_Render     conf_render(SCREEN_WIDTH * RESOLUTION_COEF, SCREEN_HEIGHT * RESOLUTION_COEF, MAX_TRACE_DEPTH, PIXEL_SAMPLING);
    conf_Verbosity  conf_verbos(PERCENT_STEP);
    conf_PathTracer conf_pt(conf_render, conf_verbos);

    Material *matr1 = new m_Lambertian({100, 75, 150});
    h_Sphere *s1 = new h_Sphere({20, -3, 0}, 4, matr1);

    Material *matr2 = new m_Lambertian({100, 75, 150});
    h_Sphere *s2 = new h_Sphere({20, 3, 0}, 4, matr2);

    Material *matr_flr = new m_Lambertian({150, 120, 40});
    h_Sphere *flr = new h_Sphere({20, 0, -1000}, 1000-s1->radius*0.9, matr_flr);

    Camera *cam = new Camera({-10, 0, 5}, {1, 0, -0.1}, conf_render.SCREEN_WIDTH * 2, conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT, 1);

    HittableList scene;
    scene.insert(s1);
    scene.insert(s2);
    scene.insert(flr);
    render_image(cam, &scene, conf_pt);
}
