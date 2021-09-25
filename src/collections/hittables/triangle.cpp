#include "triangle.h"

Triangle::Triangle():
Hittable(),
p0(0, 0, 0),
p1(1, 0, 0),
p2(0, 1, 0)
{}

Triangle::Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_):
Hittable(material_),
p0(point_0),
p1(point_1),
p2(point_2)
{}

bool Triangle::hit(Ray &ray, HitRecord* hit_record) const {
    Vec3d edge1, edge2, h, s, q;
    double a, f, u, v;

    edge1 = p1 - p0;
    edge2 = p2 - p0;
    h = ray.dir.cross(edge2);
    a = edge1.dot(h);
    if (a > -VEC3_EPS && a < VEC3_EPS)
        return false;    // This ray is parallel to this triangle.
    f = 1.0 / a;
    s = ray.orig - p0;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * ray.dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * edge2.dot(q);
    if (t > VEC3_EPS && t < hit_record->dist) // ray intersection
    {
        hit_record->point = ray.cast(t);
        hit_record->dist = t;
        hit_record->mat = material;
        hit_record->normal = edge1.cross(edge2);
        hit_record->normal.normalize();
        hit_record->set_normal_orientation(ray.dir);
        hit_record->front_hit = true;
        hit_record->surf_x = 0;
        hit_record->surf_y = 0;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool Triangle::bounding_box(AABB &box) const {
    box =  AABB({std::min(p0[0], std::min(p1[0], p2[0])),
                 std::min(p0[1], std::min(p1[1], p2[1])),
                 std::min(p0[2], std::min(p1[2], p2[2]))},
                {
                 std::max(p0[0], std::max(p1[0], p2[0])),
                 std::max(p0[1], std::max(p1[1], p2[1])),
                 std::max(p0[2], std::max(p1[2], p2[2]))
                });
    return true;
}
