#include "model.h"

Model::Model(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale) {
    load(filename, matrs, offset, scale);
}

bool Model::load(const char *filename, std::vector<Material*> matrs, const Vec3d &offset, const Vec3d &scale) {
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
    while (fscanf(fin, "%4s", line_type) == 1) {
        if (isspace(*line_type)) continue;

        if (strcmp(line_type, "v") == 0) {
            double x, y, z;
            fscanf(fin, "%lg %lg %lg", &x, &y, &z);
            points.push_back(offset + Vec3d(x, y ,z) * scale);
        } else if (strcmp(line_type, "f") == 0) {
            size_t x, y, z;
            size_t xt, yt, zt;
            size_t xn, yn, zn;

            fscanf(fin, "%lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu", &x, &xt, &xn, &y, &yt, &yn, &z, &zt, &zn);
            --x;
            --y;
            --z;
            if (x >= points.size() || y >= points.size() || z >= points.size()) {
                logger.error("model", "load point index overflow");
                return false;
            }

            hittables.push_back(new Triangle(points[x], points[y], points[z], matrs[0]));
        } else {
            // printf("skipped: {");
            int c;
            while ((c = fgetc(fin)) != '\n') {
                if (c == EOF) {
                    break;
                }
                // printf("%c", c);
            }
            // printf("}\n");
        }

    }
    
    logger.info("model", "(%zu) verticies, (%zu) triangles", points.size(), hittables.size());

    fclose(fin);
    return true;
}

