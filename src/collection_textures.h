#ifndef COLLECTION_TEXTURES
#define COLLECTION_TEXTURES

#include "texture.h"

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
	Color color;
	Vec3d freq;

public:
	t_Checkered();
	t_Checkered(const Color &color_, const Vec3d &mod_);

	virtual Color value(double sx, double sy, const Vec3d &p) const override;
};

#endif // COLLECTION_TEXTURES