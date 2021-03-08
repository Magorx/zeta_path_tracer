#ifndef COLLECTION_LIGHTS
#define COLLECTION_LIGHTS

#include "light.h"
#include "color.h"

class l_Diffuse : public Light {
public:
	Color color;
	l_Diffuse(const Color color_);
	Vec3d emit(double surf_x, double surf_y, const Vec3d point) const override;
};

#endif // COLLECTION_LIGHTS