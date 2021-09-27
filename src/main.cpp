#include "PathTracer.hpp"

#include <cstring>

#include "sfml_interface/interface.h"

// settings ===================================================================

const int 	 VERBOSITY = 2; // 2 for detailed log of some things

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 1000;

const int PIXEL_SCREEN_WIDTH  = 200;
const int PIXEL_SCREEN_HEIGHT = 200;

const int 	 PIXEL_SAMPLING   = 1;
const int 	 MAX_TRACE_DEPTH  = 7;

const double GAMMA_CORRECTION = 0.37;

const int DEFAULT_THREADS_CNT = 8;

// ============================================================================

const int REAL_SCREEN_WIDTH   = 100;
const int REAL_SCREEN_HEIGHT  = 100;

const double RESOLUTION_COEF  = (double) PIXEL_SCREEN_WIDTH / (double) REAL_SCREEN_WIDTH ; // W *= RES_COEF, H *= RES_COEF

const Vec3d  BACKGROUND_COLOR = {0, 0, 0};

// ============================================================================

HittableList scene_gen(int sphere_cnt = 1, Vec3d delta = VEC3D_ZERO);

HittableList *cornell_box_objects();
Scene 		 *cornell_box_scene();

HittableList *test_objects();
Scene 		 *test_scene();

// ============================================================================

int main(int argc, char* argv[]) {
	Brans::srand_sse(time(NULL));
	srand(time(NULL));

	conf_Render conf_render(REAL_SCREEN_WIDTH * RESOLUTION_COEF, REAL_SCREEN_HEIGHT * RESOLUTION_COEF,
							MAX_TRACE_DEPTH,
							PIXEL_SAMPLING,
							GAMMA_CORRECTION,
							BACKGROUND_COLOR);

    conf_Verbosity  conf_verbos(VERBOSITY);
    conf_SystemInfo conf_sysinf(vec3d_randlong() % 10000, DEFAULT_THREADS_CNT, nullptr);
    conf_PathTracer conf_pt(conf_render, conf_verbos, conf_sysinf);

	if (VERBOSITY >= 1) printf("[INF] PathTracing process [%d] started\n", conf_pt.sysinf.timestamp);

    conf_pt.update_from_command_line_args(argc, argv);
 
    if (VERBOSITY >= 2) printf("[INF] threads: %d\n", conf_pt.sysinf.kernel_cnt);

    Scene *scene = cornell_box_scene();

	SFML_Interface interface("zether", scene, conf_pt, WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_SAMPLING);

	if (VERBOSITY >= 1) printf("[INF] launching sfml interface, press F to make a screenshot\n");

	interface.run();
	interface.stop();

	if (VERBOSITY >= 1) printf("[INF] PathTracing process [%d] finished\n", conf_pt.sysinf.timestamp);

	return 0;
}

//=============================================================================

Scene *cornell_box_scene() {
	Camera *camera = new Camera({-100, 50, 50}, {1, 0, 0}, 
    							100,
    							REAL_SCREEN_WIDTH, REAL_SCREEN_HEIGHT,
    							RESOLUTION_COEF);
	HittableList *objects = cornell_box_objects();

    auto tree = objects->get_bvh_tree();
	
	Scene *scene = new Scene(camera, tree);
	return scene;
}

HittableList *cornell_box_objects() {
	HittableList *scene = new HittableList;

	Material *m_white = new m_Lambertian({255, 255, 255});
	Material *m_red   = new m_Lambertian({255,   0,   0});
	Material *m_green = new m_Lambertian({  0, 255,   0});

    Material *m_mirror = new m_Metal({255, 255, 125}, 0.05);
    Material *m_glass  = new m_Dielectric({125, 255, 200}, 1.1);
    Material *m_glass2  = new m_Dielectric({130, 130, 255}, 2.4);

	// Material *m_box_1 = new m_Lambertian({255, 255, 255}); ^^^^^^^^^^^^^
	// Material *m_box_2 = new m_Lambertian({255, 255, 255});

	Light *l_rect_light = new l_Diffuse({255, 255, 255});

	m_Lambertian *m_rect_light = new m_Lambertian(Vec3d(255, 255, 255));
	m_rect_light->set_emitter(l_rect_light);
	m_rect_light->to_affect_emitter = 0;

	const double heigh = 100;
	const double width = 100;
	const double depth = 100;

	const double light_size_coef = 0.32;
	const double box_coef = 0.1;

	const double l_h = heigh - VEC3_EPS;
	const double l_w = width * light_size_coef;
	const double l_d = depth * light_size_coef;

	Hittable *rect_ceil  = new h_RectXY({    0,     0, heigh}, {depth, width, heigh}, m_white);
	Hittable *rect_floor = new h_RectXY({    0,     0,     0}, {depth, width, heigh}, m_white);
	Hittable *rect_fwall = new h_RectYZ({depth,     0,     0}, {depth, width, heigh}, m_white);
	Hittable *rect_rwall = new h_RectXZ({    0,     0,    -1}, {depth,     0, heigh + 1}, m_red  );
	Hittable *rect_lwall = new h_RectXZ({    -100, width,     0}, {depth, width, heigh}, m_green);
 	
	Hittable *rect_light = new h_RectXY({depth / 2 - l_d / 2, width / 2 - l_w / 2, l_h}, {depth / 2 + l_d / 2, width / 2 + l_w / 2, l_h}, m_rect_light);
	Hittable *rect_light_floor = new h_RectXY({depth / 2 - l_d / 2, width / 2 - l_w / 2, -10}, {depth / 2 + l_d / 2, width / 2 + l_w / 2, l_h}, m_rect_light);

	Hittable *box_1 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.75},
								m_mirror);

	Hittable *box_2 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.25},
                                m_glass);

    Hittable *box_3 = new h_Box({ depth * 3 * box_coef,  width * 2.5 *  box_coef, 0},
                                {-depth * 3 *  box_coef, -width *  2.5 * box_coef, heigh * 0.03},
                                m_glass2);

	Hittable *rot_box_1 = new inst_Translate(new inst_RotZ(box_1,  Pi/3.5), {60, 70, 0});
	Hittable *rot_box_2 = new inst_Translate(new inst_RotZ(box_2, -Pi/3), {30, 25, 0});
    Hittable *rot_box_3 = new inst_Translate(box_3, {depth / 2 - l_d / 2 + 6, width / 2 - l_w / 2 + 16.5, l_h - 22.5});


	// Hittable *sphere = new h_Sphere({40, 30, 10}, 20, m_white);

	scene->insert(rect_ceil );
	scene->insert(rect_floor);
	scene->insert(rect_fwall);
	scene->insert(rect_lwall);
	scene->insert(rect_rwall);

	scene->insert(rect_light);
	scene->insert(rect_light_floor);

	Hittable *model = new Model("../models/kit.obj", {m_glass2}, {0, 0, 0}, 35); // remove ../ if you build tracer NOT in build dir
	model = new inst_RotX(model, -Pi/2);
	model = new inst_Translate(model, {35, 60, 20});
	
	scene->insert(model);

//	 scene->insert(rot_box_1);
//	 scene->insert(rot_box_2);
    // scene->insert(rot_box_3);
	
	// scene->insert(sphere);

	return scene;
}

HittableList scene_gen(int sphere_cnt, Vec3d delta) {
	HittableList scene;

	Material *matr = new m_Lambertian({255, 255, 255});
	matr->set_emitter(new l_Diffuse(Vec3d(255, 0, 0) * 4));
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
