#ifndef TEXTURE
#define TEXTURE

#include "color/color.h"

class Texture {
public:
	virtual Color value(double sx, double sy, const Vec3d &p) const = 0;
};

#endif // TEXTURE