#include "PathTracer.hpp"
#include "sfml_interface/interface.h"

// settings ===================================================================

const int 	 VERBOSITY = 2; // 2 for detailed log of some things

const int WINDOW_WIDTH  = 1000;
const int WINDOW_HEIGHT = 1000;

const int PIXEL_SCREEN_WIDTH  = 500;
const int PIXEL_SCREEN_HEIGHT = 500;

const int 	 PIXEL_SAMPLING   = 1;
const int 	 MAX_TRACE_DEPTH  = 7;

const double GAMMA_CORRECTION = 0.37;

const int DEFAULT_THREADS_CNT = 1;

// ============================================================================

const int REAL_SCREEN_WIDTH   = 100;
const int REAL_SCREEN_HEIGHT  = 100;

const double RESOLUTION_COEF  =   (double) PIXEL_SCREEN_WIDTH * (double) PIXEL_SCREEN_HEIGHT 
							    / (double)  REAL_SCREEN_WIDTH / (double)  REAL_SCREEN_HEIGHT;

const Vec3d  BACKGROUND_COLOR = {0, 0, 0};

// ============================================================================

HittableList scene_gen(int sphere_cnt = 1, Vec3d delta = VEC3D_ZERO);

HittableList *cornell_box_objects();
Scene 		 *cornell_box_scene();

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


	scene->insert(rect_ceil );
	scene->insert(rect_floor);
	scene->insert(rect_fwall);
	scene->insert(rect_lwall);
	scene->insert(rect_rwall);

	scene->insert(rect_light);
	scene->insert(rect_light_floor);

	Hittable *model = new Model("../models/ship.obj", {m_glass2}, {0, 0, 0}, 7); // remove ../ if you build tracer NOT in build dir
	model = new inst_RotY(new inst_RotZ(new inst_RotX(model, -Pi/10), -Pi/2.5), Pi/8);
	model = new inst_Translate(model, {35, 45, 40});
	
	scene->insert(model);

	if (0) { // so not to see "unused" warnings
		scene->insert(rot_box_1);
		scene->insert(rot_box_2);
		scene->insert(rot_box_3);
	}

	return scene;
}
