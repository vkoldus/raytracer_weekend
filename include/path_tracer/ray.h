//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef RAY_H
#define RAY_H

#include "types.h"

struct Ray {
    Point3 origin;
    Vector3 direction;

    Point3 at(fp_t t) const
    {
        return origin + t * direction;
    }
};

class HitInfo {
public:
    Point3 p;
    Vector3 normal;
    fp_t t;
};

#endif //RAY_H
