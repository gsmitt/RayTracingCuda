#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <ctime>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

using namespace std;


const double INFINITO = std::numeric_limits<double>::infinity();
const double RATIO_16_9 = 16.0/9.0;

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}