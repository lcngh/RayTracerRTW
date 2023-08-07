#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in DEGREES
            double aspect_ratio,
            double aperture,
            double focus_dist
        ) {
            // auto aspect_ratio = 16.0/9.0;
            // auto viewport_height = 2.0;
            // auto viewport_width = aspect_ratio * viewport_height;
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            //auto w = unit_vector(lookfrom - lookat);
            w = unit_vector(lookfrom - lookat);
            //auto u = unit_vector(cross(vup, w));
            u = unit_vector(cross(vup, w));
            //auto v = cross(w, u);
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
            lens_radius = aperture/2;
            // horizontal = viewport_width * u;
            // vertical = viewport_height * v;
            // lower_left_corner = origin - horizontal/2 - vertical/2 - w;

            // auto focal_length = 1.0;
            // origin = point3(0,0,0);
            // horizontal = vec3(viewport_width, 0.0, 0.0);
            // vertical = vec3(0.0, viewport_height, 0.0);
            // lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0, focal_length);
            
        }

        ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }

        // ray get_ray(double s, double t) const {
        //     return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        // }

        // ray get_ray(double u, double v) const {
        //     return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        // }
    
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};

#endif

/*What did I learn coding this?

look into all this camera math

*/

/*WDIL 6AUG23
    camera stuff
        to be positionable, we have to define points we care about
        the point we place the camera can be called lookfrom
        the point we want the camera to point at can be called lookat
        we also need to keep track of our rotation in space, an 'up' vector for the camera
            this vec should lie in the plane orthogonal to the view direction

    things shirley says to keep in mind for camera
        vup (our up vector, by convention), v, and w are all in the same plane
        like before, when fixed cam faced -Z, our new arbitrary view faces -w
        we can, but don't have to, use world up (0, 1, 0) to specify vup
            doing so keeps camera horizontally level, allowing us to change and experiment with it for crazy angles

    implementing defocus blur (depth of field)
        cameras need a big hole, not a pinhole, to capture light
            this would defocus everything
        so we put a lens in the camera
            now, there is a certain distance where everything is in focus
        can think of a lens in this way:
            all light rays that come from a specific point at the focus distance AND hit the lens will be sent back to a single point on the image sensor
        we call the distance between the projection point and the plane where eveything is in perfect focus the 'focus distance'
            **not the same as focal length
                focal length is the distance between projection point and the image plane
        in a physical camera, the focus distance is controlled by the distance between the lens and the film/sensor
        the aperture is a hole to control how big the lens is, effectively.
            for a real camera, if you need more light you increase aperture, and get more defocus blur
            for our virtual camera, we can have a perfect sensor and never need more light, and only have an aperture when we want defocus blur

    generating sample rays for defocus blur
        normally, all rays would originate from the lookfrom point.
        but if we want to accomplish defocus blur, we have to simulate aperture in some way
        we do this by having a disk radius around our lookfrom point, larger disk = larger blur
        disk of 0 radius is our normal point camera

*/