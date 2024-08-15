//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef WORLD_H
#define WORLD_H

#include "path_tracer/materials/dielectric.h"
#include "path_tracer/materials/lambertian.h"
#include "path_tracer/materials/metal.h"
#include "path_tracer/objects.h"
#include "types.h"


using World = std::vector<std::shared_ptr<Object>>;

World make_world()
{
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    // Air bubble in water
    // auto material_left = std::make_shared<Dielectric>(1 / 1.33);
    // Air
    auto material_left = std::make_shared<Dielectric>(1.33);
    auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.1);

    return {
            std::make_shared<Sphere>(Point3(0, -100.5, -1.0), 100, material_ground),
            std::make_shared<Sphere>(Point3(0, 0, -1.2), 0.5, material_center),
            std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left),
            std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right),
    };
}

#endif // WORLD_H
