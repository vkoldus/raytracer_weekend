//
// Created by Vaclav Koldus on 13/07/2024.
//

#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Dense>
#include "consts.h"

using Eigen::Vector3d;
using Eigen::Vector3f;
using Eigen::Vector3i;
using Eigen::Vector2d;

using Point3 = Vector3d;
using Vector3 = Vector3d;
using Vector2 = Vector2d;
using Color = Vector3d;

using fp_t = double;

struct Interval {
    fp_t min, max;

    fp_t size()
    {
        return max - min;
    }

    bool contains(fp_t x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(fp_t x) const
    {
        return min < x && x < max;
    }

    static const Interval empty;
    static const Interval universe;
};

const Interval Interval::empty = {+Infinity, -Infinity};
const Interval Interval::universe = {-Infinity, +Infinity};

#endif //TYPES_H
