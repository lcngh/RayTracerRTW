#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// beginning of our ray class
class ray {
    
    // public member functions
    public:
    
        // empty/default constructor for ray class
        ray() {}
        
        // constructor with origin point and direction parameters
        /*
          params:
            const point3& origin - reference to origin point of ray
            const vec3& direction- reference to direction vector of ray
          initializes:
            So far, I believe it initializes the orig and dir member attributes
            But I am hung up on the fact that vec3 class doesnt have a constructor that accepts an object/reference
            
            ** I believe I have figured it out
            The initialization list is special in that it does not do a function call with the parentheticals
            so
            orig(origin) is equivalent to orig = origin
            NOT
            vec3(origin)
            which is what I thought was going on...
        */
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}
        
        // member function that returns origin point of ray
        point3 origin() const { return orig; }
        
        // member function that returns direction of ray
        vec3 direction() const { return dir; }
        
        // member function that calculates where the ray is
        // using linear interpolation (LERP)
        point3 at(double t) const {
            return orig + t*dir;
        }
        
    // public member attributes
    public:
        point3 orig;
        vec3   dir;
    
    
};

#endif

/*
What did I learn coding this?

linear interpolation (LERP)
    Since rays move in a straight line in 3d space, we
    use a linear equation to represent its movement in 3d space
    orig + t*dir <- basically, equation of a line ...
    but we are using vectors!
    P(t) = A + tb
    P is a 3d position along a line in 3d
    A is the ray origin (point)
    b is the ray direction (vector)
    as t changes, ray moves along the line
    +t is in front of origin A
    -t is behind origin A

Stuck on concept:
    for our vec3 class there are 2 constructors:
        one has no params, the default constructor
        the second has 3 params: 3 double values
        so, why can we do something like point3 orig(origin), where origin is an object?
        
        We most likely cant! I just didn't understand what the initializer list was doing
        I thought it was instantiating classes, but it is assigning values
        see above at the ray constructor that accepts an origin reference and a direction reference

*/