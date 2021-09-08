#include "utils/PathTracer.hpp"

#include <cstring>
#include <unistd.h>

// settings ===================================================================

const int 	 VERBOSITY 		  = 2; // 2 for detailed log of some things

const int 	 SCREEN_WIDTH     = 100;
const int 	 SCREEN_HEIGHT    = 100;
const double RESOLUTION_COEF  = 100 / SCREEN_WIDTH; // actual image resolution is W*H here
const int 	 MAX_TRACE_DEPTH  = 10;
const int 	 PIXEL_SAMPLING   = 100; // >= 1000 for pretty images
const double GAMMA_CORRECTION = 0.55;
const Vec3d  BACKGROUND_COLOR = {0, 0, 0};

// ============================================================================

HittableList scene_gen(int sphere_cnt = 1, Vec3d delta = VEC3D_ZERO);

HittableList *cornell_box_objects();
Scene *cornell_box_scene();

HittableList *test_objects();
Scene *test_scene();

int main(int argc, char* argv[]) {
	srand(time(NULL));
	long randomstamp = vec3d_randlong() % 10000;

	conf_Render conf_render(SCREEN_WIDTH * RESOLUTION_COEF, SCREEN_HEIGHT * RESOLUTION_COEF,
							MAX_TRACE_DEPTH,
							PIXEL_SAMPLING,
							GAMMA_CORRECTION,
							BACKGROUND_COLOR);

    conf_Verbosity  conf_verbos(VERBOSITY);
    conf_SystemInfo conf_sysinf(randomstamp, 1, nullptr);
    conf_PathTracer conf_pt(conf_render, conf_verbos, conf_sysinf);

    // reading command line
    extern char *optarg;
    bool carg_to_reload_rtask_file = false;
    while (true) {
    	switch(getopt(argc, argv, "k:t:v:R")) {
    		case '?':
    		case 'h':
    		default:
    			printf("run ./pather -k #threads-count -t #render-task-file\n");
    			printf("use -t only if you understand what it does\n");
    			break;

    		case -1:
    			break;

    		case 'k':
    			sscanf(optarg, "%d", &conf_pt.sysinf.kernel_cnt);
    			continue;

    		case 't':
    			conf_pt.sysinf.rtask_filename = strdup(optarg);
    			continue;

    		case 'R':
    			carg_to_reload_rtask_file = true;
    			continue;
    	}
    	break;
    }
 
    if (VERBOSITY >= 2) printf("threads    : %d\n", conf_pt.sysinf.kernel_cnt);
    if (VERBOSITY >= 2) printf("rt_filename: %s\n", conf_pt.sysinf.rtask_filename);

    Scene *scene = cornell_box_scene();
    RenderTask rtask(0, scene->camera->res_w, 0, scene->camera->res_h);

    if (carg_to_reload_rtask_file) rtask.save("rtask.rt");

    render_from_rtask_file(scene, conf_pt);
}

//=============================================================================

Scene *cornell_box_scene() {
	Camera *camera = new Camera({-100, 50, 50}, {1, 0, 0}, 
    							100,
    							SCREEN_WIDTH, SCREEN_HEIGHT,
    							RESOLUTION_COEF);
	HittableList *objects = cornell_box_objects();
	
	Scene *scene = new Scene(camera, new BVH_Node(*objects));
	return scene;
}

HittableList *cornell_box_objects() {
	HittableList *scene = new HittableList;

	Material *m_white = new m_Lambertian({255, 255, 255});
	Material *m_red   = new m_Lambertian({255,   0,   0});
	Material *m_green = new m_Lambertian({  0, 255,   0});

	Material *m_box_1 = new m_Lambertian({255, 255, 255});
	Material *m_box_2 = new m_Lambertian({255, 255, 255});

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
	Hittable *rect_rwall = new h_RectXZ({    0,     0,     0}, {depth,     0, heigh}, m_red  );
	Hittable *rect_lwall = new h_RectXZ({    0, width,     0}, {depth, width, heigh}, m_green);
 	
	Hittable *rect_light = new h_RectXY({depth / 2 - l_d / 2, width / 2 - l_w / 2, l_h}, {depth / 2 + l_d / 2, width / 2 + l_w / 2, l_h}, m_rect_light);

	Hittable *box_1 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.75},
								m_box_1);

	Hittable *box_2 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.25},
								m_box_2);

	Hittable *rot_box_1 = new inst_Translate(new inst_RotZ(box_1,  Pi/4), {60, 70, 0});
	Hittable *rot_box_2 = new inst_Translate(new inst_RotZ(box_2, -Pi/3), {30, 25, 0});

	scene->insert(rect_ceil );
	scene->insert(rect_floor);
	scene->insert(rect_fwall);
	scene->insert(rect_lwall);
	scene->insert(rect_rwall);
	scene->insert(rect_light);

	scene->insert(rot_box_1);
	scene->insert(rot_box_2);

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
