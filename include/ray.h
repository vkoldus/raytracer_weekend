//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

using Eigen::Vector3d;

struct Ray {
    Vector3d origin;
    Vector3d direction;

    Vector3d at(double t) const
    {
        return orig + t * dir;
    }
};

#endif //RAY_H
