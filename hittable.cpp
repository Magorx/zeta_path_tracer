#include "hittable.h"

Hittable::Hittable():
material(nullptr)
{}

Hittable::Hittable(Material *material_):
material(material_)
{}
