#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

// using <namespace>
// this is to just save typing
using std::sqrt;

// beginning of vec3 class
class vec3 {
    
    // public member functions area
    // access specifier
    public:
    
        // default constructor
        // initializes vec3::e to {0,0,0}
        vec3(): e{0,0,0} {}
        
        // constructor that accepts three doubles
        // initializes vec3::e to passed doubles
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
        
        // functions that return x,y,z values of e
        // cannot change member attributes
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }
        
        // operator overload unary -
        // returns a vector with each original value negated 
        // new vector? a constructor is used
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        
        // operator overload subscipt/array access
        // this allows us to access our e like an array
        // this part is necessary for adherence to lvalue and rvalue rules
        // if e is constant, this is used, references a value and not a reference
        double operator[](int i) const { return e[i]; }
        
        // if e is mutable, this is used, references a reference
        double& operator[](int i) {return e[i]; }
        
        // operator overload on +=
        // returns a vec3 reference (vec3&)
        vec3& operator+=(const vec3 &v) {
            
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            
            // the function return value is vec3&, which requires us to return *this
            // return *this is the object
            // return this would be a pointer to the object
            return *this;
            
        }
        
        // operator overload for *=
        // returns a reference
        vec3& operator*=(const double t) {
            
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            
            // requried to return *this if return value is reference
            return *this;
        }
        
        // operator overload for /=
        // returns a reference
        // I believe what is happening here is a shortcut with the *= operator overload
        // division is equivalent to multiplication of reciprocal
        // thereore, we can just use the *= we defined with the reciprocal
        vec3& operator/=(const double t) {
            
            // return *this is required for functions which return value is a reference
            return *this *= 1/t;
        }
        
        // length() member function 
        // returns the length of our vector
        // invokes length_squared()
        double length() const {
            return sqrt(length_squared());
        }
        
        // length_squared() member function
        // returns the 'length squared' of the vector
        // squares the x,y,z values and sums
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        
        /*function that returns true if a vector's dimensions are close to zero*/
        bool near_zero() const {
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
            
        }
        

        
		/* member function that returns a new random vec3
			inline static
			constructs a new vec3 using random_double() function as all 3 arguments
		*/
		inline static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}
		
		/* member function that returns a new random vec3
			inline static
			constructs a new vec3 using the provided min/max doubles
			constructs a new vec3 using the random_double() function based on provided range
		*/
		inline static vec3 random(double min, double max) {
			return vec3(random_double(min,max), random_double(min,max), random_double(min, max));
		}
        
		
    // public member attributes
    // access specifier
    public:
        // initializes array of doubles, e,  of length 3
        double e[3];
};

// type aliases for vec3
// these are just synonyms for our vec3 class
using point3 = vec3;
using color  = vec3;

// vec3 utility function area

// operator overload for insertion operator <<
// basically, how we want to print our vec to ostream
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    
}

// operator overload for addition operator (+)
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// operator overload for subtraction operator (-)
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// operator overload for multiplication operator (*)
// multiplication between two vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// operator overload for multiplication operator (*)
// multiplication between double and vector
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// operator overload for multiplication operator(*)
// multiplication between vector and double (order matters!!!)
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// operator overload for division operator(/)
// uses multiplication overload for shortcut
// we don't make a second one, because dividing a double by a vector doesn't make sense
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// inline member function dot()
// returns a double value
// calculates the dot product of two vectors
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// inline member function cross()
// returns a vec3
// calculates the cross product of two vectors
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1]*v.e[2] - u.e[2]*v.e[1],
                u.e[2]*v.e[0] - u.e[0]*v.e[2],
                u.e[0]*v.e[1] - u.e[1]*v.e[0]);
}

// inline member function unit_vector()
// calculates and returns the unit vector of the passed vector
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

/* function that returns a random point within a unit-sphere
	the purpose of this function is to determine where a ray bounces after impact
	this will most likely be used for diffuse materials only, but we shall see
	this algorithm is a simple rejection method. if we generate a point where the x,y,z are outside
	 a unit circle, we reject it and make a new one. if its inside, accept and move on!
*/
vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1,1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

/* function that generates a random unit-length vector 
	here we can just combine two functions we already wrote!
	first, we generate a random point in a unit-sphere, then we unit-vectorize it
*/
vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

/* function that generates a random unit vector and returns one 
   in the same hemisphere as the normal
	common diffuse method used before adoption of lambertian diffusion
*/
vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	/* if dot product of in_unit_sphere and normal is > 0.0 then in same hemisphere as normal */
	if (dot(in_unit_sphere, normal) > 0.0) {
		return in_unit_sphere;
	} else {
		return -in_unit_sphere;
	}
}

/*function that returns a vec3 representing a reflection off a reflective surface, like metal
   reflected ray direction is calculated like so:
    v + 2b
    while the normal n is a unit vector, v isn't always
    and length of b should be dot(v,n)
*/
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

#endif

/*
What did I learn coding this?

preprocessor header guards
    #ifndef
    #define
    #endif
    This checks if the header file has already been processed
    
using <namespace::function>
    This saves on typing, allowing programmer to just use function name

access specifiers
    public:
    private:
    These specify how the included member attributes or functions can be accessed outside the class
    
constructor stuff
        vec3() <- default constructor, has no parameters/takes no arguments
		
constructor initialization list
    vec3(): e{0,0,0} {}
    After the colon (:), you can specify things to be initialized when the function is called
    e{0,0,0} is initialized

'const' in function body
    throws compiler error if function attempts to alter member attributes

operator overloading
    vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }
    allows you to define how operators effect user-defined classes
    compile-time polymorphism
    
operator[] overloading
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    allows us to use the e member attribute as an array
    keeps lvalue and rvalue consistency
    
ref-qualifiers
    ref-qualified functions can only be called on lvalues. thought I was using one here, but I am not :)
    
function return value&
    vec3& operator+=(){...}
    this specifies that the function returns a reference
    
return *this
    this is required for a function that returns a reference
    return this will return a pointer
    the dereferencing operator(*) will act on the pointer and find what it points to in memory, returning it
    
lvalues and rvalues
    simply, lvalues belong on the left side of assignment operator (=) and rvalues belong on the right
    lvalues are memory-holding values, like references, objects
    rvalues do not hold memory, like primitives
    
type aliases
    using point3 = vec3;
    allows us to use point3 the way we would vec3
    same stuff, different name
    
inline functions
    this one I need to research more. Internet tell of optimizations and such; speed etc...
    as far as I can tell, these are identical to regular class member functions, but they can be defined outside the class definition
    I'm sure there is a lot im missing with this exact implementation. Will try to remember to update if I become enlightened
	
*/

/*WDIL 13JUL23
	diffuse lighting
	when a diffuse object is in a room, what it looks like is determined by the surrounding light as well as its intrinsic color
	for example, light rays are sent to the object, and depending on what wavelengths get absorbed/reflected and their direction distribution determines what it looks like
	a method for determining how rays reflect off a sphere, for example, is by looking at the point of impact
		there is a unit-length radius sphere on each side of the impact point, one outside the sphere object and one inside
		the unit tangent sphere on the same side of the ray's origin is chosen
		a random point in the chosen sphere is chosen
		the ray moves towards this point
		-> diffuse reflection on a sphere (or any diffuse surface?)
		unit sphere inside: (P - n) (n is surface normal)
		unit sphere outside: (P + n)
		send ray to random point S in chosen unit sphere
			this vector is (S - P)
			
*/

/*WDIL 17JUL23
	lambertian reflection
		the reflection method we have been using for diffuse reflections is a simple rejection method
			basically, generate ray in cube where x,y,z all range -1,1, if ray in sphere, then accept!
		this results in a ray distribution scale of cos^3(theta), where theta=angle from the normal
		this results in high probability of new ray being close to normal, low probability of steep angles
		this is realistic, as low-angle light scatters wide and contributes less to final color
		however, we want lambertian distribution, which has ray distribution of cos(theta)
		this is a more uniform distribution of our current distribution method
		
		two important visual differences, according to shirley
		1 shadows are less pronounced
		2 both spheres are lighter in appearence
		
		due to: uniform scattering of light rays, fewer towards the normal !
			diffuse objects appear LIGHTER because more light bounces toward the camera
			for shadows, less light is bouncing straight up, so light direcly under sphere is LIGHTER
*/

/*WDIL 19JUL23
	deciding between our diffuse calculation methods
		shirley describes the initial diffuse methods we learned as 'hacks' that lasted a long
		time before being proved incorrect. incorrect meaning not exactly representative of how it works
		in the real-world. the methods lasted so long, apparently, because
			1 it was mathematically difficult to prove that the distribution was incorrect
			2 it was difficult to explain why a cos(theta) distribution is desirable and what it looks like
		
		random vector random length
			cos^3(theta) distribution
		random vector unit length (lambertian (?))
			cos(theta) distribution
		hemisphere distribution, independent of angle from normal
			uniform scatter all angles away from hit point

*/