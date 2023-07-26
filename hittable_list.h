#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        // dont need semicolons after these ...
        // default constructor
        hittable_list() {}
        // constructor with one parameter, a shared pointer 
        // adds passed object to list of objects
        hittable_list(shared_ptr<hittable> object) {add(object);}
        
        // member function clear()
        // no parameters, no return value
        // clears the hittable_list list
        void clear() {objects.clear();}
        /*  member cuntion add()
            one parameter: shared pointer of hittable object
            takes passed object and adds it to the list
        */
        void add(shared_ptr<hittable> object) {objects.push_back(object);}
        
        /* abstract function derived from hittable*/
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        
    public:
        std::vector<shared_ptr<hittable>> objects;
        
    
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    
    return hit_anything;
}


#endif

/*What did I learn coding this?

vector
    included with <vector> header
    generic, array-like collection of an arbitrary type
    automatically grows as values are added
    std::vector<shared_ptr<hittable>> objects;
        a vector 'objects', of type shared_ptr of type hittable
        objects.push_back(object)
            adds value to the end of objects

shared_ptr<type>
    included with the <memory> header
    pointer to allocated <type> with reference counting semantics
    everytime its value is assigned to another shared pointer, reference count increments
    shared pointer goes out of scope (end of bloc or function), reference is decremented
    Once this reference count goes to 0, object gets deleted
    kind of makes some memory management automatic
    
    shared pointers in this case, will allow geometries to share a common instance
        like a bunch of spheres sharing  the same texure map material
    
shared_ptr<type> ptr_name = make_shared<type>(type_constructor_params)
  can be shortened to
auto ptr_name = make_shared<type>(type_constructor_params)
    
    using the 'auto' type, the compiler will determine the type with the evaluated rvalue(? guessing here)
    therefore, dont need to specify shared_ptr<type> because make_shared<type>(...) returns a shared_ptr<type>

for (const auto& object : objects) {...}
    this is a range based for-loop
    notice the ampersand (&) on the type
    without it, we will create a COPY of the objects we are looping through
    with the & we are looping through references, which are cheaper than copies typically

if (object->hit(...)) {...}
    arrow operator (->)
    lets say we want to use object's hit() function
    c++ amateur here wants to do object.hit() (thanks javascript)
    however, the dot operator (.) accesses member attributes
    in c++, 'object' would be a pointer... and a pointer is an address in memory (with its own address)
        ie it doesnt have a hit() function
    okay, so i dereference it with the dereference operator (*)
    *object.hit() the c++ amateur says!
    nope ... precedence makes this not work!
    evaluates to *(object.hit()) ... same issue
    therefore we have to do
        (*object).hit()
    but screw that ... so there is a shorthand !
    object->hit()


std::make_shared

*/