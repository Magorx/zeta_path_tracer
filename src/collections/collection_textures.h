#ifndef COLLECTION_TEXTURES
#define COLLECTION_TEXTURES

#include "color/texture.h"

class t_SolidColor : public Texture {
private:
	Color color;

public:
	t_SolidColor();
	t_SolidColor(const Color &color_);

	virtual Color value(double sx, double sy, const Vec3d &p) const override;
};

class t_Checkered : public Texture {
private:
	Texture *first;
	Texture *second;
	Vec3d freq;

public:
	t_Checkered();
	t_Checkered(const Color &first_, const Color &second_ = VEC3D_ZERO, const Vec3d &freq_ = VEC3D_ONE);
	t_Checkered(Texture *first_, Texture *second_, const Vec3d &freq_);

	virtual Color value(double sx, double sy, const Vec3d &p) const override;
};

#endif // COLLECTION_TEXTURES