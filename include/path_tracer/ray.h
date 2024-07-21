//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef RAY_H
#define RAY_H

#include "types.h"

class Material;

struct Ray {
    Point3 origin;
    Vector3 direction;

    Point3 at(fp_t t) const
    {
        return origin + t * direction;
    }
};

static Ray operator+(const Ray &l, const Vector3 &r)
{
    return {l.origin, l.direction + r};
}


class HitInfo {
public:
    Point3 p;
    Vector3 normal;
    fp_t t;
    bool front_face;
    std::shared_ptr<const Material> material;
};

#endif //RAY_H
