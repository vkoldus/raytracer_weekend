//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include "types.hpp"

inline Vector3 random_in_unit_sphere()
{
    while (true)
    {
        auto v = Vector3::Random();
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

fp_t random_number()
{
    return std::rand() / (RAND_MAX + 1.0);
}

#endif // RANDOM_H
