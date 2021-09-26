#include "model.h"
#include "triangle.h"

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