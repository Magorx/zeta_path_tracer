#ifndef HITTABLE_MODEL_H
#define HITTABLE_MODEL_H

#include "utils_header.h"
#include "triangle.h"

#include "utils/logger.h"

struct Model : public HittableList {
    Model(const char *filename, std::vector<Material*> matrs, const Vec3d &offset = {0, 0, 0}, const Vec3d &scale = {1, 1, 1});
    bool load(const char *filename, std::vector<Material*> matrs, const Vec3d &offset = {0, 0, 0}, const Vec3d &scale = {1, 1, 1});

    virtual void dump_bvh(int depth) override;
};

#endif // HITTABLE_MODEL_H
