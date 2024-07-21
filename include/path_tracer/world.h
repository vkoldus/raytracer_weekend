//
// Created by Vaclav Koldus on 21/07/2024.
//

#ifndef WORLD_H
#define WORLD_H

#include "types.h"
#include "path_tracer/materials/lambertian.h"


World make_world()
{
    return {
        std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>()),
        std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>()),
    };
}

#endif //WORLD_H
