#include "model.h"

Model::Model(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale, bool to_smooth) {
    load(filename, matrs, offset, scale, to_smooth);
}

int read_vertex(char **buffer, size_t &v, size_t &vn, size_t &vt) {
    char *buf = *buffer;
    while (*buf == ' ') ++buf;

    int chars_read = 0;
    char c = 0;

    if (!sscanf(buf, "%lu%n", &v, &chars_read)) {
        *buffer = buf;
        return 0;
    } else {
        buf += chars_read;
    }

    c = *buf;
    ++buf;
    if (c != '/') {
        *buffer = buf;
        return 1;
    }

    if (!sscanf(buf, "%lu%n", &vn, &chars_read)) {
        if (*buf != '/') {
            *buffer = buf;
            return 1;
        }
    } else {
        buf += chars_read;
    }

    c = *buf;
    ++buf;
    if (c != '/') {
        *buffer = buf;
        return 2;
    }

    if (!sscanf(buf, "%lu%n", &vt, &chars_read)) {
        *buffer = buf;
        return 2;
    } else {
        buf += chars_read;
    }

    if (*buf == ' ') {
        ++buf;
    }

    *buffer = buf;
    return 3;
}

bool Model::load(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale, bool to_smooth) {
    if (!filename) {
        logger.error("model", "trying to load filename[nullptr]");
        return false;
    }
    
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        logger.error("model", "failed to load (%s)", filename);
        return false;
    }

    logger.info("model", "loading (%s)", filename);

    std::vector<Vec3d> points;
    char line_type[5];
    char *buffer_alloc = (char*) calloc(100, sizeof(char));
    char *buffer = buffer_alloc;
    while (fscanf(fin, "%4s", line_type) == 1) {
        if (isspace(*line_type)) continue;

        if (strcmp(line_type, "v") == 0) {
            double x, y, z;
            fscanf(fin, "%lg %lg %lg", &x, &y, &z);
            points.push_back(offset + Vec3d(x, y ,z) * scale);
        } else if (strcmp(line_type, "vn") == 0) {
            double x, y, z;
            fscanf(fin, "%lg %lg %lg", &x, &y, &z);
            normals.push_back(Vec3d(x, y ,z).normal());
        } else if (strcmp(line_type, "f") == 0) {
            size_t x, y, z;
            size_t xt, yt, zt;
            size_t xn, yn, zn;

            fgets(buffer, 99, fin);
            if (!read_vertex(&buffer, x, xt, xn)) logger.error("model", "failed to load face\n");
            if (!read_vertex(&buffer, y, yt, yn)) logger.error("model", "failed to load face\n");
            if (!read_vertex(&buffer, z, zt, zn)) logger.error("model", "failed to load face\n");
            buffer = buffer_alloc;

            // fscanf(fin, "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &x, &xt, &xn, &y, &yt, &yn, &z, &zt, &zn);
            --x, --y, --z;
            if (x >= points.size() || y >= points.size() || z >= points.size()) {
                logger.error("model", "load point index overflow");
                return false;
            }

            Triangle *triag = new Triangle(points[x], points[y], points[z], matrs[0]);
            if (to_smooth && xn && yn && zn) {
                --xn, --yn; --zn;
                triag->set_normals(normals[xn], normals[yn], normals[zn]);
            }
            hittables.push_back(triag);
        } else {
            int c;
            while ((c = fgetc(fin)) != '\n') {
                if (c == EOF) {
                    break;
                }
            }
        }

    }
    free(buffer_alloc);
    
    logger.info("model", "(%zu) verticies, (%zu) triangles", points.size(), hittables.size());

    fclose(fin);
    return true;
}

