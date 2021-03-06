#include "path_tracer.h"
#include "collection_hittables.h"
#include "collection_materials.h"

// settings ===================================================================

const int PERCENT_STEP      = 25;

const int SCREEN_WIDTH      = 256;
const int SCREEN_HEIGHT     = 256;
const int MAX_TRACE_DEPTH   = 3;
const int PIXEL_SAMPLING    = 5; 

// ============================================================================

int main() {
    conf_Render     conf_render(SCREEN_WIDTH, SCREEN_HEIGHT, MAX_TRACE_DEPTH, PIXEL_SAMPLING);
    conf_Verbosity  conf_verbos(PERCENT_STEP);
    conf_PathTracer conf_pt(conf_render, conf_verbos);

    Material *matr1 = new m_Lambertian({100, 75, 150});
    Hittable *s1 = new h_Sphere({20, 0, 0}, 4, matr1);

    Camera *cam = new Camera({0, 0, 0}, {1, 0, 0}, conf_render.SCREEN_WIDTH * 2, conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT, 1);

    render_image(cam, s1, conf_pt);
}
