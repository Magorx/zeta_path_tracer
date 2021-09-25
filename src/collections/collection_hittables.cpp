#include "collection_hittables.h"

h_Sphere::h_Sphere():
Hittable(),
center(0, 0, 0),
radius(0)
{}

h_Sphere::h_Sphere(const Vec3d center_, double radius_, Material *material_):
Hittable(material_),
center(center_),
radius(radius_)
{}

Vec3d h_Sphere::normal(const Vec3d &point) const {
    Vec3d result = point;
    result -= center;
    result.normalize();
	return result;
}

bool h_Sphere::hit(Ray &ray, HitRecord* hit_record) const {

    Vec3d c_o = ray.orig - center;
    double disc = radius * radius - (c_o.dot(c_o) - ray.dir.dot(c_o) * ray.dir.dot(c_o));
    if (disc < 0) {
        return false;
    }

    double b = -ray.dir.dot(c_o);
    double d1 = b - sqrt(disc);
    double d2 = b + sqrt(disc);
    double d = -1;
    if (d1 > 0 && (d2 > d1 || d2 < 0)) {
        d = d1;
    } else if (d2 > 0 && (d1 > d2 || d1 < 0)) {
        d = d2;
    } else {
        return false;
    }

    if(d > hit_record->dist) return false;

    Vec3d point = ray.cast(d);
    hit_record->mat = material;
    hit_record->set_normal_orientation(ray.dir);
    hit_record->point = point;
    hit_record->dist = d;
    hit_record->normal = this->normal(point);
    get_surface_coords(point, hit_record->surf_x, hit_record->surf_y);
    return true;
}

bool h_Sphere::bounding_box(AABB &box) const {
    Vec3d radvec = Vec3d(radius, radius, radius);
    box.mn = center - radvec;
    box.mx = center + radvec;
    return true;
}

bool h_Sphere::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    Vec3d norm = normal(point);
    double theta = acos(-norm[1]);
    double phi = atan2(-norm[2], norm[0]) + VEC3D_PI;

    sx = phi / (2 * VEC3D_PI);
    sy = theta / VEC3D_PI;
    return true;
}

//=============================================================================

h_RectXY::h_RectXY():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXY::h_RectXY(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), std::min(p0_[1], p1_[1]), p0_[2])),
p1(Vec3d(std::max(p0_[0], p1_[0]), std::max(p0_[1], p1_[1]), p1_[2])) // we can force p1[2] = p0[2], but what for
{}

bool h_RectXY::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[2] - ray.orig[2]) / ray.dir[2];
    if(t > hit_record->dist) return false;

    double x = ray.orig[0] + t * ray.dir[0];
    double y = ray.orig[1] + t * ray.dir[1];
    // fprintf(stderr, "t = %lg x = %lg y = %lg\normal", t, x, y);
    if (t < 0 || x < p0[0] || x > p1[0] || y < p0[1] || y > p1[1] || t > hit_record->dist) {
        // fprintf(stderr, "no\normal");
        return false;
    }

    get_surface_coords({x, y, p0[2]}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->point = ray.cast(hit_record->dist);
    hit_record->normal = Vec3d(0, 0, 1);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;

    return true;
}

bool h_RectXY::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, 0, VEC3_EPS), p1 + Vec3d(0, 0, VEC3_EPS));
    return true;
}

bool h_RectXY::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[0]-p0[0]) / (p1[0]-p0[0]);
    sy = (point[1]-p0[1]) / (p1[1]-p1[1]);
    return true;
}

//=============================================================================

h_RectXZ::h_RectXZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectXZ::h_RectXZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), p0_[1], std::min(p0_[2], p1_[2]))),
p1(Vec3d(std::max(p0_[0], p1_[0]), p1_[1], std::max(p0_[2], p1_[2]))) // we can force p1[2] = p0[2], but what for
{}

bool h_RectXZ::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[1] - ray.orig[1]) / ray.dir[1];
    if(t > hit_record->dist) return false;

    double x = ray.orig[0] + t * ray.dir[0];
    double z = ray.orig[2] + t * ray.dir[2];
    if (t < 0 || x < p0[0] || x > p1[0] || z < p0[2] || z > p1[2] || t > hit_record->dist) {
        return false;
    }

    get_surface_coords({x, p0[1], z}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->point = {x, p0[1], z};
    hit_record->normal = Vec3d(0, 1, 0);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;
    return true;
}

bool h_RectXZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(0, VEC3_EPS, 0), p1 + Vec3d(0, VEC3_EPS, 0));
    return true;
}

bool h_RectXZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[0]-p0[0]) / (p1[0]-p0[0]);
    sy = (point[2]-p0[2]) / (p1[2]-p1[2]);
    return true;
}

//=============================================================================

h_RectYZ::h_RectYZ():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_RectYZ::h_RectYZ(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(p0_[0], std::min(p0_[1], p1_[1]), std::min(p0_[2], p1_[2]))),
p1(Vec3d(p1_[0], std::max(p0_[1], p1_[1]), std::max(p0_[2], p1_[2]))) // we can force p1[2] = p0[2], but what for
{}

bool h_RectYZ::hit(Ray &ray, HitRecord* hit_record) const {

    double t = (p0[0] - ray.orig[0]) / ray.dir[0];
    if(t > hit_record->dist) return false;

    double y = ray.orig[1] + t * ray.dir[1];
    double z = ray.orig[2] + t * ray.dir[2];
    if (t < 0 || y < p0[1] || y > p1[1] || z < p0[2] || z > p1[2] || t > hit_record->dist) {
        return false;
    }

    get_surface_coords({p0[0], y, z}, hit_record->surf_x, hit_record->surf_y);
    hit_record->dist = t;
    hit_record->normal = Vec3d(1, 0, 0);
    hit_record->set_normal_orientation(ray.dir);
    hit_record->mat = material;
    hit_record->point = ray.cast(hit_record->dist);
    return true;
}

bool h_RectYZ::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, 0, 0), p1 + Vec3d(VEC3_EPS, 0, 0));
    return true;
}

bool h_RectYZ::get_surface_coords(const Vec3d &point, double &sx, double &sy) const {
    sx = (point[1]-p0[1]) / (p1[1]-p0[1]);
    sy = (point[2]-p0[2]) / (p1[2]-p1[2]);
    return true;
}

//=============================================================================

h_Box::h_Box():
Hittable(),
p0(VEC3D_ZERO),
p1(VEC3D_ONE)
{}

h_Box::h_Box(const Vec3d &p0_, const Vec3d &p1_, Material *material_):
Hittable(material_),
p0(Vec3d(std::min(p0_[0], p1_[0]), std::min(p0_[1], p1_[1]), std::min(p0_[2], p1_[2]))),
p1(Vec3d(std::max(p0_[0], p1_[0]), std::max(p0_[1], p1_[1]), std::max(p0_[2], p1_[2])))
{
    sides.insert(new h_RectYZ({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectYZ({p1[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));

    sides.insert(new h_RectXZ({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectXZ({p0[0], p1[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));

    sides.insert(new h_RectXY({p0[0], p0[1], p0[2]}, {p1[0], p1[1], p1[2]}, material_));
    sides.insert(new h_RectXY({p0[0], p0[1], p1[2]}, {p1[0], p1[1], p1[2]}, material_));
}

bool h_Box::hit(Ray &ray, HitRecord* hit_record) const {
    return sides.hit(ray, hit_record);
}

bool h_Box::bounding_box(AABB &box) const {
    box = AABB(p0 - Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS), p1 + Vec3d(VEC3_EPS, VEC3_EPS, VEC3_EPS));
    return true;
}

bool h_Box::get_surface_coords(const Vec3d &, double &, double &) const {
    return false;
}

//=============================================================================

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

//=============================================================================

Model::Model(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale) {
    load(filename, matrs, offset, scale);
}

bool Model::load(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale) {
    if (!filename) {
        fprintf(stderr, "[ERR] model::load filename[nullptr]\n");
        return false;
    }
    
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        fprintf(stderr, "[ERR] model::load can't open [%s]\n", filename);
        return false;
    }

    std::vector<Vec3d> points;
    char line_type[5];
    while (fscanf(fin, "%4s", line_type) == 1) {
        if (isspace(*line_type)) continue;

        if (strcmp(line_type, "v") == 0) {
            double x, y, z;
            fscanf(fin, "%lg %lg %lg", &x, &y, &z);
            points.push_back(offset + Vec3d(x, y ,z) * scale);
            printf("vert: %g %g %g\n", x, y, z);
        } else if (strcmp(line_type, "f") == 0) {
            size_t x, y, z, tr;
            fscanf(fin, "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &x, &tr, &tr, &y, &tr, &tr, &z, &tr, &tr);
            --x;
            --y;
            --z;
            printf("f with vecs: %lu %lu %lu\n", x, y, z);
            if (x >= points.size() || y >= points.size() || z >= points.size()) {
                fprintf(stderr, "[ERR] model::load point index overflow\n");
                return false;
            }

            hittables.push_back(new Triangle(points[x], points[y], points[z], matrs[0]));
        } else {
            printf("skipped: {");
            int c;
            while ((c = fgetc(fin)) != '\n') {
                if (c == EOF) {
                    break;
                }
                printf("%c", c);
            }
            printf("}\n");
        }

    }
    printf("read %lu verts\n", points.size());

    fclose(fin);
    return true;
}
