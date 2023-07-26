#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
//#include <random> for random_double commented out down below

/*Usings section*/
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

/*Constants section*/
const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

/*Utility functions section*/
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/*Returns real random double between [0,1)*/
inline double random_double() {
    // the 1.0 was 1 on accident. caused significant slowdown and black image ...
    return rand() / (RAND_MAX + 1.0);
    
}

/*Returns real random double between [min, max)*/
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

/*Returns a passed double, clamped to provided range*/
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

/*Returns real random double using <random>
look into what range is specifically here, if its same as others (Exactly [min, max) ?
peter shirley says 'experts' have issue with <random>, look into it see if its worth looking into further
*/
/*
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
*/

/*Common headers section*/
#include "ray.h"
#include "vec3.h"

#endif

/*What did I learn coding this?

c++ doesn't have a portable pi constant

c++ infinity (C++11 and so on) infinity is evaluated as HUGE_VAL (for double type)
    indicates overflow

*/

/*WDIL 9JUL2023
interval notation
    ( -> excluded in range
    [ -< inlcuded in range

(0, 100] -> range is 0 (excluded) to 100 (included)
    which means we cant roll a 0, but we can roll a 100
    
<cstdlib> vs <random> for random number

*/