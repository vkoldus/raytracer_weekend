//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"

inline Vector3 random_in_unit_sphere()
{
    while (true)
    {
        auto v = (Vector3::Random() - Vector3(1, 1, 1)) / 2;
        if (v.norm() < 1.0)
        {
            return v;
        }
    }
}

inline Vector3 random_on_unit_sphere()
{
    return random_in_unit_sphere().normalized();
}

inline Vector3 random_on_hemisphere(const Vector3 &normal)
{
    auto v = random_on_unit_sphere();
    if (v.dot(normal) > 0)
    {
        return v;
    } else
    {
        return -v;
    }
}

#endif //RANDOM_H
