//
// Created by Vaclav Koldus on 20/07/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "types.hpp"

struct Ray;
class HitInfo;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &, const HitInfo &, Color &, Ray &) const
    {
        return false;
    }

    virtual void enable_fuzz(bool)
    {
    }
};

#endif // MATERIAL_H
