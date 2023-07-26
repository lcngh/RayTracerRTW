#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>


// write_color() function
// returns void
// parameters: reference to ostream, color (vec3) object
/*
void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
*/

/*New write_color function, with multi-sampling for antialiasing*/
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    
    // Divide color by number of samples
    auto scale = 1.0 / samples_per_pixel;
    /* here we accounted for gamma correction
        our spheres absorb 50% of the ray's light that hit, meaning the image is dark
        so we can do gamma correction here, using 'gamma 2'-according to shirley
        gamma is apparently equivalent to color ^ 1/gamma
        so if we have 'gamma 2' that means gamma = 2; and color^1/2 == sqrt(color)
    */
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    /* changed to account for gamma correction
    r *= scale;
    g *= scale;
    b *= scale;
    */
    // Write translated [0,255] value of each color component
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif

/*
What did I learn coding this?

static_cast<destination type>(source type)
    unary operator that casts one type to anyther
    compile-time casts
    implicit and explicit conversions
    
255.999 explanation
    this was lost on me earlier
    our range of colors, while doing math and stuff, is 0-255
    but our color range in the image output should be 1-256
    therefore, we do this fancy stuff with 255.999 to adjust our domain

*/

/*WDIL 16JUL23

gamma correction
    apparently at this point our spheres absorb 50% of the light that hits it
    I believe this is because of the constant used in main::ray_color()'s return statement
    this causes a dark image, which we can sorta fix with gamma correction
    shirley describes gamma correction as 
        expected by viewers and
        meaning the 0-1 values 'have some transform' before being stored as a byte
    basically, the transformation referred is the 'correction' we do to the luminance curve of our pixels
    our eyes and cameras percieve light differently.
        when light increases, a camera percieves it as a linear increase. twice light, twice bright
        when light increases for an eye, the percieved light increases on a curve. twice light, fraction more bright
    so we adjust the luminance curve using gamma equation to closely resemble our eye's curve
    our eyes do this so we can see a wider range of luminance !
*/