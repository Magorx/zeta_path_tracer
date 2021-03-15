#include "path_tracer_constants.h"
#include "path_tracer.h"
#include "collection_hittables.h"
#include "collection_materials.h"
#include "collection_lights.h"
#include "collection_instances.h"

// settings ===================================================================

const int 	 PERCENT_STEP     = 10;

const int 	 SCREEN_WIDTH     = 100;
const int 	 SCREEN_HEIGHT    = 100;
const double RESOLUTION_COEF  = 256 / SCREEN_HEIGHT;
const int 	 MAX_TRACE_DEPTH  = 10;
const int 	 PIXEL_SAMPLING   = 100;
const double GAMMA_CORRECTION = 0.55;
const Vec3d  BACKGROUND_COLOR = {0, 0, 0};

// ============================================================================

HittableList scene_gen(int sphere_cnt = 1, Vec3d delta = VEC3D_ZERO);
HittableList cornell_box_scene();

int main() {
	srand(time(NULL));

	conf_Render conf_render(SCREEN_WIDTH * RESOLUTION_COEF, SCREEN_HEIGHT * RESOLUTION_COEF,
							MAX_TRACE_DEPTH,
							PIXEL_SAMPLING,
							GAMMA_CORRECTION,
							BACKGROUND_COLOR);

    conf_Verbosity conf_verbos(PERCENT_STEP);
    conf_PathTracer conf_pt(conf_render, conf_verbos);

    // Texture  *chkd_flr = new t_Checkered({190, 190, 190}, {50, 50, 50}, {1, 1, 1});
    // Material *matr_flr = new m_Lambertian(chkd_flr);
    // h_Sphere *flr = new h_Sphere({0, 0, -10000}, 10000, matr_flr);

    // Camera *cam = new Camera({0, 0, 20}, {1, 0, -0.46}, 
    // 						 conf_render.SCREEN_WIDTH * 2,
    // 						 conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT,
    // 						 1);
    // HittableList scene = scene_gen(10, {40, 0, 3});
    // scene.insert(flr);

    // Texture  *chkd_flr = new t_Checkered({190, 190, 190}, {50, 50, 50}, {1, 1, 1});
    // Material *matr_flr = new m_Lambertian(chkd_flr);
    // h_Sphere *flr = new h_Sphere({0, 0, -10000 - 5}, 10000, matr_flr);

    // Camera *cam = new Camera({0, 0, 3}, {1, 0, 0}, 
    // 						 conf_render.SCREEN_WIDTH * 2,
    // 						 conf_render.SCREEN_WIDTH, conf_render.SCREEN_HEIGHT,
    // 						 1);
    // HittableList scene;
    // scene.insert(flr);

    // Texture *chk_s1 = new t_Checkered({100, 200, 255}, {140, 35, 20}, {0.25, 0.25, 0.25});
    // Material *ms1 = new m_Lambertian(chk_s1);
    // ms1->set_emitter(new l_Diffuse({255, 255, 255}));
    // h_Sphere *s1 = new h_Sphere({60, 0, 5}, 7, ms1);
    // scene.insert(s1);

    // Texture *chk_r1 = new t_Checkered({200, 100, 255}, {100, 100, 100}, {1, 1, 1});
    // Material *mr1 = new m_Dielectric(chk_r1, 1.3);
    // // mr1->set_emitter(new l_Diffuse({255, 255, 255}));
    // h_Box *rec1 = new h_Box({50, 10 / 2, 8}, {30, -10 / 2, 0}, mr1);
    // scene.insert(rec1);

    HittableList scene = cornell_box_scene();
    Camera *cam = new Camera({-100, 50, 50}, {1, 0, 0}, 
    						 100,
    						 SCREEN_WIDTH, SCREEN_HEIGHT,
    						 RESOLUTION_COEF);

    Material *ms1 = new m_Lambertian({255, 255, 255});
    ms1->set_emitter(new l_Diffuse({255, 0, 0}));
    h_Sphere *s1 = new h_Sphere({100, 50, 50}, 5, ms1);
    scene.insert(s1);
    
    BVH_Node bvh(scene);
    render_image(cam, &bvh, conf_pt);
}

HittableList cornell_box_scene() {
	HittableList scene;

	Material *m_white = new m_Lambertian({255, 255, 255});
	Material *m_red   = new m_Lambertian({255,   0,   0});
	Material *m_green = new m_Lambertian({  0, 255,   0});

	Material *m_box_1 = new m_Lambertian({255, 255, 255});
	Material *m_box_2 = new m_Lambertian({255, 255, 255});

	Light *l_rect_light = new l_Diffuse({255, 255, 255});

	m_Lambertian *m_rect_light = new m_Lambertian({255, 255, 255});
	m_rect_light->set_emitter(l_rect_light);
	m_rect_light->to_affect_emitter = 0;

	const double heigh = 100;
	const double width = 100;
	const double depth = 100;

	const double light_size_coef = 0.3;
	const double box_coef = 0.1;

	const double l_h = heigh - VEC3_EPS;
	const double l_w = width * light_size_coef;
	const double l_d = depth * light_size_coef;

	Hittable *rect_ceil  = new h_RectXY({    0,     0, heigh}, {depth, width, heigh}, m_white);
	Hittable *rect_floor = new h_RectXY({    0,     0,     0}, {depth, width, heigh}, m_white);
	Hittable *rect_fwall = new h_RectYZ({depth,     0,     0}, {depth, width, heigh}, m_white);
	Hittable *rect_lwall = new h_RectXZ({    0,     0,     0}, {depth,     0, heigh}, m_red  );
	Hittable *rect_rwall = new h_RectXZ({    0, width,     0}, {depth, width, heigh}, m_green);
 	
	Hittable *rect_light = new h_RectXY({depth / 2 - l_d / 2, width / 2 - l_w / 2, l_h}, {depth / 2 + l_d / 2, width / 2 + l_w / 2, l_h}, m_rect_light);

	Hittable *box_1 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.75},
								m_box_1);

	Hittable *box_2 = new h_Box({ depth * box_coef,  width * box_coef, 0},
								{-depth * box_coef, -width * box_coef, heigh * 0.25},
								m_box_2);

	Hittable *rot_box_1 = new inst_Translate(new inst_RotY(box_1, Pi/4), {60, 70, 0});
	Hittable *rot_box_2 = new inst_Translate(new inst_RotZ(box_2, -Pi/3), {30, 25, 0});

	scene.insert(rect_ceil );
	scene.insert(rect_floor);
	scene.insert(rect_fwall);
	scene.insert(rect_lwall);
	scene.insert(rect_rwall);
	scene.insert(rect_light);

	scene.insert(rot_box_1);
	scene.insert(rot_box_2);

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
