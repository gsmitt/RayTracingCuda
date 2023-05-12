#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <limits>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;



const double infinito = std::numeric_limits<double>::infinity();

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}