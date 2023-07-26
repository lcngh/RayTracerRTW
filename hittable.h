#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

/* will have to look into this... as we are not 'including' a file that defines 'material' ...
	shirley says that this is to alert the c++ compiler that the pointer points to a class, for shared_ptr I believe?
*/
class material;

struct hit_record {
    point3 p;
    vec3 normal;
	/* material will help tell us how rays interact with surfaces */
	shared_ptr<material> mat_ptr;
    double t;
    bool front_face;
    
    // function that sets the face normal of the record
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
    
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    
};

#endif


/*What did I learn coding this?

structs vs classes

    class members are private by default
    structs members are public by default

    class instance = object
    struct instance = structure variable

    member class/struct of class are private by default. this is mostly a c++ thing
    member class/struct of struct are public by default

    classes are normally used for data abstraction/inheritance
    structs are normally used for data grouping
    
    class null value possible
    struct null value not possible
    
    BOTH NEED A SEMICOLON (;)
    
virtual function
    basically, when a calss is derived from hittable, it will override the hit() function with its own implementation
    so its inheritance stuff I believe

const after a member function
    putting const after a member functon indicates that it will not modify the class object it is contained it
        except for member attributes labeled 'mutable'

= 0 after a virtual function
    this indicates that the virtual function is a pure virtual function
    this means that it cannot be directly instantiated
    which means a derived class can only instantiate it
    a derived class MUST override it
    pure virtual functions are akin to Java's abstract functions, or abstract functions in general


normals
    when a ray intersects a surface, a normal can go two ways
        towards the intersection point, from the center
            outside normal, outside surface
        towards the center, from the intersection point
            local normal, inside surface
    we can also choose for the normal to always point against the ray
        outside hit -> outside normal
        inside hit -> local normal
    we must decide how normals behave
    
    if we choose normals always face out (center to intersection)
        then we have to figure out which side the ray is on when we color it
        this is done by comparing the ray to the normal
            if ray and normal face same direction, ray is inside object
            if ray and normal face opposite direction, ray is outside object
                we can calculate this by taking the dot product of these two vectors
                if its positive, ray is inside the sphere
    
    if we choose normals always against ray, we cant use dot product to determine which side of the surface the ray is on
    we need to store that info in a setup like this
    
    bool front_face;
    if(dot(ray_direction, outward_normal) > 0.0) {
        // ray inside sphere
        normal = -outward_normal;
        front_face = false;
        
    } else {
        // ray outside sphere
        normal = outward_normal;
        front_face = true;
    }
    
    
tertiary operator

true ? if true then here : otherwise false
    
    
*/