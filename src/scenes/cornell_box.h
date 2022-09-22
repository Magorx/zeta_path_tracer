#include <PathTracer.hpp>
#include <settings.h>


HittableList *cornell_box_objects() {
	HittableList *scene = new HittableList;

	Material *m_white = new m_Lambertian({255, 255, 255});
	Material *m_red   = new m_Lambertian({255,   0,   0});
	Material *m_green = new m_Lambertian({  0, 255,   0});

    Material *m_mirror = new m_Metal({255, 255, 125}, 0.05);
    Material *m_glass  = new m_Dielectric({255, 255, 255}, 2);
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


	// Material *mat = new m_Lambertian({10, 145, 200});
	// Hittable *box = new h_Box({ 0,  0, 0},
	// 						  {7.5, 7.5, 150},
	// 						   mat);
	// box = new inst_RotX(new inst_RotZ(box, Pi/8), Pi/4);
	// box = new inst_Translate(box, {70, 0, 0});

	// scene->insert(box);

	// cenzure for the girl pic
	// Material *m_cenzure = new m_Dielectric({255, 255, 255}, 1, -1, 0.2);
	// Hittable *rect_cenze1 = new h_RectYZ({50, 44, 58}, {50, 56, 64}, m_cenzure);
	// scene->insert(rect_cenze1);

	scene->insert(rect_ceil );
	scene->insert(rect_floor);
	scene->insert(rect_fwall);
	scene->insert(rect_lwall);
	scene->insert(rect_rwall);

	scene->insert(rect_light);


	Material *m_model = new m_Lambertian({255, 255, 30});	

	Hittable *model = new Model("../models/whip.obj", {m_model}, {0, 0, 0}, 10, false); // remove ../ if you build tracer NOT in build dir
	model = new inst_RotZ(new inst_RotX(model, -Pi/2), Pi/2);
	model = new inst_Translate(model, {10, 50, 0});
	
	// scene->insert(model);

	// Hittable *sp = new h_Sphere({95, 25, 75}, 15, m_model);
	// scene->insert(sp);
	// sp = new h_Sphere({95, 75, 25}, 15, m_model);
	// scene->insert(sp);

	// Material *mt = new m_Dielectric({255, 0.874 * 255, 0.768 * 255}, 2.42);
	// Hittable *sp = new h_RectXY({40, 40, 50}, {60, 60, 50}, mt);
	// scene->insert(sp);

	// Material *mat = new m_Lambertian(new t_Checkered(Color{125, 175, 225}, VEC3D_ZERO, VEC3D_ONE * 0.25));
	// Light *lig = new l_Diffuse({255, 255, 255});
	// mat->set_emitter(lig);
	// Hittable *sphere = new h_Sphere({depth / 2, width / 3, heigh / 3}, heigh / 4, mat);
	// scene->insert(sphere);

	if (1) { // so not to see "unused" warnings
		scene->insert(rot_box_1);
		scene->insert(rot_box_2);
		scene->insert(rot_box_3);
		// scene->insert(rect_light_floor);
	}

	return scene;
}

Scene *cornell_box_scene() {
	Camera *camera = new Camera(CAMERA_POS, CAMERA_HORIZONTAL_ROT, CAMERA_VERTICAL_ROT, 
    							CAMERA_DIST,
    							REAL_SCREEN_WIDTH, REAL_SCREEN_HEIGHT,
    							RESOLUTION_COEF);
	HittableList *objects = cornell_box_objects();

    auto tree = objects->get_bvh_tree();
	
	Scene *scene = new Scene(camera, tree);
	return scene;
}
