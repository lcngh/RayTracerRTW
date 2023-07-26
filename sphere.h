#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

/*  Sphere derived class, hittable base class
    default constructor initializes nothing?
    constructor accepts center point and radius, initializes center and radius member attributes
    overridden hit() from hittable 
    
*/
class sphere : public hittable {
    public:
        sphere() {}
        
        /* replaced by constructor that accounts for material */
        //sphere(point3 cen, double r) : center(cen), radius(r) {};
        /* constructor that accepts material pointer */
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
		
		
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        
        
    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
    
};

/*  hit function, derived from hittable
    takes ray reference, double tmin, double tmax, hitrecord reference
    first, we calculate the discriminant of the sphere, if <0 then miss
    then find nearest root in our t range
    update hit record and return true
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;  
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);
    
    // find the nearest root that lies in the acceptatble range
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }
    
    rec.t = root;
    rec.p = r.at(rec.t);
    //rec.normal = (rec.p - center) / radius;
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    
    return true;
}

#endif

/*What did I learn coding this?

what goes in a .cpp file vs .h file
    .h files contain shared DECLARATIONS
    .cpp files contain DEFINITIONS and LOCAL DECLARATIONS
    the full contents of an .h file gets 'injected' into it's include statement
    NEVER #include A .cpp FILE
    .cpp files are compiled
    .cpp should contain definitions that must only be compiled once
    
source code to executable pipeline
    rectangle.h
    rectangle.cpp
    main.cpp
    iostream.h
    ----------> files get preprocessed
    rectangle.h     rectangle.h
    iostream.h      iostream.h
    rectangle.cpp   main.cpp
    ----------> preprocessed source code gets compiled
    rectangle.o   application.o
    ----------> generated object files get linked (std library gets linked too when included and such)
    executable.exe
    
override
    c++11 feature!!!
    compiler flag
    tells the compiler to check the base class for a virtual function of the SAME SIGNATURE

class Derived: public Base {
    
    // This is the format for class inheritance in c++
    
};
*/