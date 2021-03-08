#include "path_tracer.h"
#include "collection_hittables.h"
#include "collection_materials.h"

// settings ===================================================================

const int 	 PERCENT_STEP     = 10;

const int 	 SCREEN_WIDTH     = 256;
const int 	 SCREEN_HEIGHT    = 256;
const double RESOLUTION_COEF  = 4;
const int 	 MAX_TRACE_DEPTH  = 50;
const int 	 PIXEL_SAMPLING   = 1;
const double GAMMA_CORRECTION = 0.5;
const Vec3d  BACKGROUND_COLOR = {230, 230, 230};

// ============================================================================

HittableList scene_gen(int sphere_cnt = 1, Vec3d delta = VEC3D_ZERO);

int main() {
	srand(time(NULL));

	conf_Render conf_render(SCREEN_WIDTH * RESOLUTION_COEF, SCREEN_HEIGHT * RESOLUTION_COEF,
							MAX_TRACE_DEPTH,
							PIXEL_SAMPLING,
							GAMMA_CORRECTION,
							BACKGROUND_COLOR);

    conf_Verbosity conf_verbos(PERCENT_STEP);
    conf_PathTracer conf_pt(conf_render, conf_verbos);
/*
    Material *matr0 = new m_Lambertian({30, 20, 240});
    h_Sphere *s0 = new h_Sphere({38, 1, -2.5}, 1, matr0);

    Material *matr1 = new m_Dielectric({255, 255, 255}, 1.12);
    h_Sphere *s1 = new h_Sphere({40, -4, 0}, 4, matr1);

    Material *matr2 = new m_Metal({200, 140, 50}, 0.5);
    h_Sphere *s2 = new h_Sphere({40, 5, 0}, 4, matr2);

    Material *matr3 = new m_Metal({200, 20, 50}, 0);
    h_Sphere *s3 = new h_Sphere({50, 0, 1}, 5, matr3);
*/
    Material *matr_flr = new m_Lambertian({30, 220, 70});
    h_Sphere *flr = new h_Sphere({0, 0, -10000}, 10000-3.75, matr_flr);

    Camera *cam = new Camera({-5, 0, 20}, {1, 0, -0.4}, conf_render.SCREEN_WIDTH * 2, conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT, 1);

    HittableList scene = scene_gen(100, {30, 0, 0});
/*
    scene.insert(s0);
    scene.insert(s1);
    scene.insert(s2);
    scene.insert(s3);

*/
    scene.insert(flr);
    BVH_Node bvh(scene);

    render_image(cam, &scene, conf_pt);
}

HittableList scene_gen(int sphere_cnt, Vec3d delta) {
	HittableList scene;

	for (int i = 0; i < sphere_cnt; ++i) {
		Material *matr;
		long roll = vec3d_randlong() % 3 + 1;
		if (roll == 1) {
			matr = new m_Lambertian(randcolor());
		} else if (roll == 2) {
			matr = new m_Dielectric(randcolor(), 1.15);
		} else {
			matr = new m_Metal(randcolor(), vec3d_randdouble());
		}

		h_Sphere *sph = new h_Sphere(delta + Vec3d(vec3d_randdouble(0, 100), vec3d_randdouble(-35, +35), vec3d_randdouble(-1, 1)),
									 vec3d_randdouble(1, 5), matr);
		scene.insert(sph);
	}

	return scene;
}
