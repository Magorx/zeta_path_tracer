#include "instance.h"


Instance::Instance(Hittable *obj):
obj(obj)
{}


void Instance::dump_bvh(int depth) {
    dump_spaces(depth);
    printf("inst {\n");
    
    if (obj) obj->dump_bvh(depth + 1);

    dump_spaces(depth);
    printf("}\n");
}
