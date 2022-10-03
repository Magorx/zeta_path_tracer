#include <PathTracer.hpp>
#include <settings.h>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"



HittableList *cornell_box_objects() {
    HittableList *scene = new HittableList;

    Material *m_white = new m_Lambertian({255, 255, 255});
    Material *m_red   = new m_Lambertian({255,   0,   0});
    Material *m_green = new m_Lambertian({  0, 255,   0});

    Material *m_mirror = new m_Metal({255, 255, 125}, 0.05);
    Material *m_glass  = new m_Dielectric({255, 255, 255}, 2);
    Material *m_glass2  = new m_Dielectric({130, 130, 255}, 2.4);

    Light *l_rect_light = new l_Diffuse({255, 255, 255});

    m_Lambertian *m_rect_light = new m_Lambertian(Vec3d(255, 255, 255));
    m_rect_light->set_emitter(l_rect_light);
    m_rect_light->to_affect_emitter = 0;

    Material *m_checkered = new m_Lambertian(new t_Checkered(
        new t_SolidColor({255, 0, 0}),
        new t_Checkered(
            {0, 255, 0},
            {0, 0, 255},
            VEC3D_ONE / 4
        ),
        VEC3D_ONE * 2
    ));
    m_checkered->set_emitter(l_rect_light);

    const double heigh = 100;
    const double width = 100;
    const double depth = 100;

    const double light_size_coef = 0.32;
    const double box_coef = 0.15;

    const double l_h = heigh - VEC3_EPS;
    const double l_w = width * light_size_coef;
    const double l_d = depth * light_size_coef;

    Hittable *rect_ceil  = new h_RectXY({    0,     0, heigh}, {depth, width, heigh}, m_white);
    Hittable *rect_floor = new h_RectXY({    0,     0,     0}, {depth, width, heigh}, m_white);
    Hittable *rect_fwall = new h_RectYZ({depth,     0,     0}, {depth, width, heigh}, m_white);
    Hittable *rect_rwall = new h_RectXZ({    0,     0,    -1}, {depth,     0, heigh + 1}, m_red  );
    Hittable *rect_lwall = new h_RectXZ({    0, width,     0}, {depth, width, heigh}, m_green);
     
    Hittable *rect_light = new h_RectXY(
        {depth / 2 - l_d / 2, width / 2 - l_w / 2, l_h},
        {depth / 2 + l_d * 2, width / 2 + l_w / 2, l_h},
        m_rect_light
    );

    Hittable *rect_light_floor = new h_RectXY(
        {depth / 2 - l_d / 2, width / 2 - l_w / 2, -10},
        {depth / 2 + l_d / 2, width / 2 + l_w / 2, l_h},
        m_rect_light
    );

    t_Marble *marble = new t_Marble({0, 0, 0}, {255, 255, 255}, VEC3D_ONE / 6, 1);
    // Material *m_marble = new m_Lambertian(marble);
    Material *m_marble = new m_Metal(marble, 0.15);

    t_Marble *t_marble = new t_Marble({0, 0, 0}, {255, 255, 255}, VEC3D_ONE / 4, 1);
    Material *m_white_marble = new m_Lambertian(t_marble);

    Hittable *box_1 = new h_Box({ depth * box_coef,  width * box_coef, 0},
                                {-depth * box_coef, -width * box_coef, heigh * 0.75},
                                m_marble);

    Hittable *box_2 = new h_Box({ depth * box_coef,  width * box_coef, 0},
                                {-depth * box_coef, -width * box_coef, heigh * 0.25},
                                m_white_marble);

    Hittable *box_3 = new h_Box({ depth * 3 * box_coef,  width * 2.5 *  box_coef, 0},
                                {-depth * 3 *  box_coef, -width *  2.5 * box_coef, heigh * 0.03},
                                m_glass2);

    Hittable *rot_box_1 = new inst_Translate(new inst_RotZ(box_1,  Pi/3.5), {60, 70, 0});
    Hittable *rot_box_2 = new inst_Translate(new inst_RotZ(box_2, -Pi/3), {30, 25, 0});
    Hittable *rot_box_3 = new inst_Translate(box_3, {depth / 2 - l_d / 2 + 6, width / 2 - l_w / 2 + 16.5, l_h - 22.5});


    // Material *mat = new m_Lambertian({10, 145, 200});
    // Hittable *box = new h_Box({ 0,  0, 0},
    //                           {7.5, 7.5, 150},
    //                            mat);
    // box = new inst_RotX(new inst_RotZ(box, Pi/8), Pi/4);
    // box = new inst_Translate(box, {70, 0, 0});

    // scene->insert(box);

    // cenzure for the girl pic
    // Material *m_cenzure = new m_Dielectric({255, 255, 255}, 1, -1, 0.2);
    // Hittable *rect_cenze1 = new h_RectYZ({50, 44, 58}, {50, 56, 64}, m_cenzure);
    // scene->insert(rect_cenze1);

    // add big sphere at the middle of the scene
    // scene->insert(new h_Sphere({40, 50, 50}, 26, m_marble));

    scene->insert(rect_ceil );
    scene->insert(rect_floor);
    scene->insert(rect_fwall);
    scene->insert(rect_lwall);
    scene->insert(rect_rwall);

    scene->insert(rect_light);

    // add cow marble texture
    Texture *t_cow = new t_Marble({
        new t_Checkered(
            new t_SolidColor({255, 0, 0}),
            new t_Checkered(
                {0, 255, 0},
                {0, 0, 255},
                VEC3D_ONE / 4
            ),
            VEC3D_ONE * 2
        ),
        new t_Checkered(
            new t_SolidColor({200, 125, 75}),
            new t_Checkered(
                {75, 200, 125},
                {125, 75, 200},
                VEC3D_ONE / 4
            ),
            VEC3D_ONE * 2
        ),
        VEC3D_ONE / 5.5, 1
    });

    Material *m_model = new m_Dielectric({120, 175, 240}, 1.7, 0.05, 0.01);

    Hittable *model = new Model("../models/teapot.obj", {m_model}, {0, 0, 0}, 15, true); // remove ../ if you build tracer NOT in build dir
    model = new inst_RotZ(new inst_RotX(model, -Pi/2), Pi * 0.45);
    model = new inst_Translate(model, {35, 50, 0});

    scene->insert(model);

    // image texture with boyar file
    // Texture *t_boyar = new t_Image("../models/boyar.png");
    // Material *m_helmet = new m_Lambertian(t_boyar);

    // add triangle to scene
    // scene->insert(new Triangle({50, 0, 0}, {50, 100, 000}, {50, 0, 100}, m_helmet));

    // add spehere to the center right of the scene
    // Hittable *helmet = new h_Box({ depth * box_coef / 1.3,  width * box_coef / 1.3, 0},
    //                              {-depth * box_coef / 1.3, -width * box_coef / 1.3, heigh * 0.25},
    //                              m_helmet);

    // helmet = new inst_RotX(helmet, -Pi/2);
    // helmet = new inst_Translate(helmet, {depth / 4, width / 4, 30});

    // scene->insert(helmet);


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
        // scene->insert(rot_box_1);
        // scene->insert(rot_box_2);
        // scene->insert(rot_box_3);
        // scene->insert(rect_light_floor);
    }

    return scene;
}

Scene *cornell_box_scene(const zephyr::tracer::config::FullT &conf) {
    Camera *camera = new Camera(CAMERA_POS, CAMERA_HORIZONTAL_ROT, CAMERA_VERTICAL_ROT, 
                                CAMERA_DIST,
                                REAL_SCREEN_WIDTH, REAL_SCREEN_HEIGHT,
                                (double) conf.render.screen.width / (double)  REAL_SCREEN_WIDTH);
    HittableList *objects = cornell_box_objects();

    auto tree = objects->get_bvh_tree();
    
    Scene *scene = new Scene(camera, tree);
    return scene;
}

#pragma clang diagnostic pop
