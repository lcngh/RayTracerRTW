#include <iostream>
// moved to rtweekend.h
//#include "vec3.h"
// moved to rtweekend.h
//#include "ray.h"
#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    /* complicated linear math section ...
     we are checking if a ray's position is inside the sphere's geometry
    */
    vec3 oc = r.origin() - center;
    /* replaced by a simplified version
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0* dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    */
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    
    
    //return (discriminant > 0);
    if (discriminant < 0) {
        return -1.0;
    } else {
        //return (-b - sqrt(discriminant)) / (2.0*a);
        return (-half_b - sqrt(discriminant)) / a;
    }
}

// function that takes a reference to a ray object and returns it's color
/*
color ray_color(const ray& r) {
    
    // if sphere at point x,y,z, radius r, ray collide, then return appropriate color
    //
    //if (hit_sphere(point3(0,0,-1), 0.5, r)){
    //    return color(1,0,0);
    //}
    //
    
    // if ray hits sphere, calculate normal
    // want to look into significance of unit vector ...
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    
    // get unit direction of unit-vectorized ray direction
    vec3 unit_direction = unit_vector(r.direction());
    
    // get direction of ray, not a unit vector
    //vec3 ray_direction = r.direction();
    
    // calculate t given ray unit-direction y position
    t = 0.5*(unit_direction.y() + 1.0);
    
    // calculate t, but direction is not a unit vector
    //t = 0.5*(ray_direction.y() + 1.0);
    
    // white/blue gradient with respect to t and ray unit y position 
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
    
}
*/

/* new ray_color function
    accepts two parameters:
    a const reference to a ray object 'r', const reference to hittable object 'world'
    returns a color object (vec3)
*/
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    
    /*if the bounce limit is reached, stop gathering light*/
    if ( depth <= 0 ) { return color(0,0,0); }
    
    /* second argument changed to 0.001 for shadow acne */
    if (world.hit(r, 0.001, infinity, rec)) {
        /*replaced by algorithm for diffuse reflection
            must account for stack overflow, added depth parameter to ray_color()
        */
        //return 0.5 * (rec.normal + color(1,1,1));
		
		/* here are different methods for calculating diffuse impacts.
			it is important to experiment with each one.
			which one works best for many diffuse surfaces? few?
			is there a difference with antialias samples?
			gamma?
		*/
		/* updated with random_unit_vector() function */
        //point3 target = rec.p + rec.normal + random_in_unit_sphere();
		/* updated with random_in_hemisphere() function */
        //point3 target = rec.p + rec.normal + random_unit_vector();
		//point3 target = rec.p + rec.normal + //random_in_hemisphere();
		
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r,rec,attenuation,scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0,0,0);
        
        /*the constant before ray_color corresponds to 
        how intense ray reflections appear
        */
        //return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
        //return 1.0 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{

    // Image properties
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height= static_cast<int>(image_width / aspect_ratio);
    // added for camera antialiasing
    const int samples_per_pixel = 100;
    // added for ray bounce depth
    const int max_depth = 50;
    
    // World Properties
    hittable_list world;
    
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<metal>(color(0.7, 0.3, 0.3), 0.0);
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    
    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    //world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    //world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    //world.add(make_shared<sphere>(point3(0, 100.5, -1), 100));
    
    
    // Camera properties
    /* These have moved to camera.h
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    
    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);
    */
    
    /*New camera section*/
    camera cam;
    
    // old image dimensions
    //const int image_width = 256;
    //const int image_height= 256;
    
    
    // Render the image
    
    
    /* PPM format stuff
    // p3 means colors are in ASCII
    // image_width -> columns
    // image_height-> rows
    // 255 for max color
    // loop for RGB triplets
    // 255 0 142 225 220 0 112 112 112 etc ... 
    */
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    // c++ is row major, meaning the rows are contiguous in memory, not columns
    /* for each row, starting at row 255
      for each column, starting at col 0
      loop moves across rows backwards for visual result purpose (?) 
      or for ppm format purpose ... 
    */  
    for (int j = image_height-1; j >= 0; j--) {
        
        // std::cerr output for progress during render
        // std::flush ignores usual buffer and prints immediately
		// stays on one line! doesn't pollute (or cmd optimization?)
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        
        for (int i = 0; i < image_width; i++) {
            
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                //was accidentally dividing by random_double() before, instead of adding ...
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
            
            // these two lines utilize our new vec3 and color classes
            // a color (or, vec3) is created each loop, and the color is written
            //color pixel_color(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
            
            
            //auto u = double(i) / (image_width-1);
            //auto v = double(j) / (image_height-1);
            //ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            //color pixel_color = ray_color(r);
            //color pixel_color = ray_color(r, world);
            //write_color(std::cout, pixel_color);
            
            
            /* Replaced by vec3.h and color.h
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;
            
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
            */
        }
    
    }
    
    std::cerr << "\nDone.\n";

}

/*
What did I learn coding this?

ppm image format
    the ppm image format can produce an image from certain commands as well as 'triplets' of color values
    we can calculate these values then do program.exe > image.ppm to generate our image

row vs col major
    row/col major is a characteristic of programming languages
    basically, determines how memory is stored/accessed on the computer
    row major: the rows of a matrix are stored contiguously
    col major: the cols of a matrix are stored contiguously
    this must be considered whenever traversal of memory is done in nested loops and such
    
*/

/*
What did I learn? 6JUL2023

ray tracer sends rays through pixels and computes the color seen in that direction
very top-level pseudocode:
    1 calculate the ray from the 'eye' to the pixel
    2 determine which objects the ray intersects
    3 compute color from intersection point    

*/

/*WDIL 8JUL2023 
    I wondered why the ground sphere was green, and why the sphere in center view was a pretty gradient of colors
    obviously, the normal map calculation is the reason for the small sphere 
    but why is the big one only green?
    well, that is because its a really big sphere, and the top part of it is green according to the normal map
    so we are just seeing a zoomed in part of the green part of the small sphere essentially
    
*/

/*WDIL 16JUL23
shadow acne


*/
