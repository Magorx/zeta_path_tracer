#ifndef COLLECTION_LIGHTS
#define COLLECTION_LIGHTS

#include "camera_light_scene/light.h"
#include "color/color.h"

class l_Diffuse : public Light {
public:
	Color color;
	l_Diffuse(const Color color_);
	Vec3d emit(double, double, const Vec3d&) const override;
};

#endif // COLLECTION_LIGHTS