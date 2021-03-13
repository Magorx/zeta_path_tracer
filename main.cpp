#include "path_tracer.h"
#include "collection_hittables.h"
#include "collection_materials.h"
#include "collection_lights.h"

// settings ===================================================================

const int 	 PERCENT_STEP     = 10;

const int 	 SCREEN_WIDTH     = 256;
const int 	 SCREEN_HEIGHT    = 256;
const double RESOLUTION_COEF  = 2;
const int 	 MAX_TRACE_DEPTH  = 10;
const int 	 PIXEL_SAMPLING   = 1000;
const double GAMMA_CORRECTION = 0.55;
const Vec3d  BACKGROUND_COLOR = {100, 100, 100};

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

    Texture  *chkd_flr = new t_Checkered({30, 190, 70}, {1.5, 1, 1});
    Material *matr_flr = new m_Lambertian(chkd_flr);
    h_Sphere *flr = new h_Sphere({0, 0, -10000}, 10000, matr_flr);

    Camera *cam = new Camera({0, 0, 20}, {1, 0, -0.46}, 
    						 conf_render.SCREEN_WIDTH * 2,
    						 conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT,
    						 1);

    HittableList scene = scene_gen(10, {40, 0, 3});
    scene.insert(flr);
    
    BVH_Node bvh(scene);
    render_image(cam, &bvh, conf_pt);
}

HittableList scene_gen(int sphere_cnt, Vec3d delta) {
	HittableList scene;

	Material *matr = new m_Lambertian({0, 0, 0});
	matr->set_emitter(new l_Diffuse(Vec3d(255, 0, 0) * 5));
	scene.insert(new h_Sphere(delta + Vec3d(0, 0, 5), 1, matr));

	matr = new m_Dielectric({200, 100, 255}, 1.3);
	//matr->set_emitter(new l_Diffuse(Vec3d(255, 255, 255) * 5));
	scene.insert(new h_Sphere(delta + Vec3d(-4, 0, -0.95), 1.4, matr));

	for (int i = 0; i < sphere_cnt; ++i) {
		Material *matr;
		long roll = i % 3 + 1;
		if (roll == 1) {
			matr = new m_Lambertian(randcolor(100, 255));
		} else if (roll == 2) {
			matr = new m_Dielectric(randcolor(100, 255), 1.15);
		} else {
			matr = new m_Metal(randcolor(100, 255), vec3d_randdouble());
		}

		//h_Sphere *sph = new h_Sphere(delta + Vec3d(vec3d_randdouble(0, 100), vec3d_randdouble(-25, +25), vec3d_randdouble(-1, 1)),
		//							 vec3d_randdouble(1, 5), matr);

		h_Sphere *sph = nullptr;
		roll = i % 8;
		if (roll == 0) {
			sph = new h_Sphere(delta + Vec3d(i, i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 1) {
			sph = new h_Sphere(delta + Vec3d(i, -i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 2) {
			sph = new h_Sphere(delta + Vec3d(-i, i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 3){
			sph = new h_Sphere(delta + Vec3d(-i, -i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 4) {
			sph = new h_Sphere(delta + Vec3d(i, i / 2, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 5) {
			sph = new h_Sphere(delta + Vec3d(i, -i / 2, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else if (roll == 6) {
			sph = new h_Sphere(delta + Vec3d(-i / 2, i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		} else {
			sph = new h_Sphere(delta + Vec3d(-i / 2, -i, 0) / 2.5, (i % 3 + 2) / 2, matr);
		}
		scene.insert(sph);
	}

	return scene;
}
