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

Vector3 reflect(const Vector3 &v, const Vector3 &n)
{
    return v - 2 * v.dot(n) * n;
}

Vector3 refract(const Vector3 &unit_v, const Vector3 &normal, double etai_over_etat)
{
    auto cos_theta = fmin((-unit_v).dot(normal), 1.0);
    Vector3 r_out_perp = etai_over_etat * (unit_v + cos_theta * normal);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredNorm())) * normal;
    return r_out_perp + r_out_parallel;
}

#endif // RAY_H
