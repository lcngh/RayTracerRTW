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

/*class for dielectric materials, like glass and other see-through things
    for now, it always refracts
*/
class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            //vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
            //scattered = ray(rec.p, refracted);
            
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
            /*at this point attenuation is always 1
                therefore, the glass surface absorbs nothing
            */
        }

    public:
        double ir;
    
    private:
        static double reflectance(double cosine, double ref_idx) {
            // Schlick's approximation for reflectance
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0*r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
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

/*WDIL 6AUG23
    when you look at glass (or anything) at an angle, its reflectance varies
    in this implementation, we use schlick's approximation for this effect
    according to shirley, this polynomial equation is surprisingly accurate

*/