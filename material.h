#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
	public:
	/* shirley says we use 'hit_record' here to 'avoid a bunch of arguments so we can stuff w.e. info we want here' ... 
		not entirely sure what that means ...
		apparently, hit_record is a way for us to 'stuff a bunch of arguments into a struct'
			slightly better explanation... but still ... kind of an odd way to describe a struct in my amateur opinion
		when a ray hits a surface, the material pointer in hit_record will be set to the surface's material pointer that
		was given to it during initialization from main()
	*/
		virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

/* lambertian class inheriting material class 
    for diffuse materials
    apparently, lambertian scattering can be handled in a few ways:
        scatter attenuates by its reflectance R
        scatter does not attenuate, but absorbs 1-R rays
        a mix of the above two
        scatter with some probability p and have attenuation be albedo/p
*/
class lambertian : public material {
    
    public:
        lambertian(const color& a) : albedo(a) {}
        
        virtual bool scatter( 
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            /*issue here: random vector can cancel out scatter direction*/
            auto scatter_direction = rec.normal + random_unit_vector();
            
            /*step to catch near-zero reflection vectors*/
            if(scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
        
    public:
        color albedo;
    
};

/*class for relfective metal surface*/
class metal : public material {
    
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
        
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        
        
    public:
        color albedo;
        double fuzz;
    
};


#endif


/* WDIL 19JUL23 
	we make this abstract material class in order to render multiple materials
	generally, there are two options:
		we can have a universal material and when a ray impacts a material, that specific material has
		properties that zero-out some of the universal properties, giving us our desired material
			shirley says this is not a bad approach, but there is another ...
		the second approach involves writing a class that 'encapsulates' this behavior
			this is the approach we are gonna use, which means the class must achieve two things:
				1 produce a scattered ray (or absorb the incident ray)
				2 if scattered, say how much the ray should be attenuated (reduced in effect/force)
*/

/* WDIL 22JUL23 
    fuzzy shiny reflection
        if we want a reflection to look fuzzy, the destination of the ray after reflection must be randomized. we can achieve this by adding some calculation to our scattered ray's construction arguments. if we take the 'reflected' variable, and add the product of a new 'fuzz' parameter and a 'random_in_unit_sphere()' ray, then the destination will be random within a sphere of radius 'fuzz'. fuzz of 0 will be normal shiny.
*/