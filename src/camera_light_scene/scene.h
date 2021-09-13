#ifndef SCENE
#define SCENE

#include "camera_light_scene/camera.h"
#include "hits/hittable.h"

struct Scene {
	Camera   *camera;
	Hittable *objects;

	Scene(Camera *camera_, Hittable *objects_):
	camera(camera_),
	objects(objects_)
	{}
};

#endif // SCENE