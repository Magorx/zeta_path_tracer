#include "triangle.h"

Triangle::Triangle():
Hittable(),
p0(0, 0, 0),
e1(1, 0, 0),
e2(0, 1, 0)
{}

Triangle::Triangle(const Vec3d &point_0, const Vec3d &point_1, const Vec3d &point_2, Material *material_):
Hittable(material_),
p0(point_0),
e1(point_1 - point_0),
e2(point_2 - point_0),
n1(),
n2(),
n3(),
to_phong_normals(false)
{}

bool Triangle::hit(Ray &ray, HitRecord* hit_record) const {
    Vec3d edge1, edge2, h, s, q;
    double a, u, v;

    edge1 = e1;
    edge2 = e2;
    h = ray.dir.cross(edge2);
    a = edge1.dot(h);

    if (a > -VEC3_EPS && a < VEC3_EPS)
        return false;    // This ray is parallel to this triangle.

    s = ray.orig;
    s -= p0;
    s /= a;

    u = s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = ray.dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = edge2.dot(q);
    if (t > VEC3_EPS && t < hit_record->dist) // ray intersection
    {
        hit_record->point = ray.cast(t);
        hit_record->dist = t;
        hit_record->mat = material;
        hit_record->normal = to_phong_normals ? calc_normal(u, v) : edge1.cross(edge2).normal();
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
    Vec3d P1 = p1();
    Vec3d P2 = p2();

    box =  AABB({std::min(p0[0], std::min(P1[0], P2[0])),
                 std::min(p0[1], std::min(P1[1], P2[1])),
                 std::min(p0[2], std::min(P1[2], P2[2]))},
                {
                 std::max(p0[0], std::max(P1[0], P2[0])),
                 std::max(p0[1], std::max(P1[1], P2[1])),
                 std::max(p0[2], std::max(P1[2], P2[2]))
                });
    return true;
}

Vec3d Triangle::calc_normal(double u, double v) const {
    return (n1 * (1 - u - v) + n2 * u + n3 * v).normal();
}