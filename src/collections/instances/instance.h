#ifndef INSTANCE_H
#define INSTANCE_H

#include "hits/hittable.h"


class Instance : public Hittable {
public:
    Hittable *obj;

    Instance(Hittable *obj);

    virtual void dump_bvh(int depth) override;
};


#endif // INSTANCE_H