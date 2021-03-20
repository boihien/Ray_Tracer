#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
	//r(t) = endpoint + u(direction)t
	//t^2 + 2(e-c)ut + (e-c)*(e-c)-r^2 = 0
	//find smallest positive t
	
	Hit sphereInter;
	sphereInter.object = NULL;
	sphereInter.part = -1;
	
	vec3 u = ray.direction;
	vec3 ec = ray.endpoint - center;
	
	double a = u.magnitude_squared(); //t^2 
	double b = 2*dot(u, ec);//2(e-c)*u t
	double c = ec.magnitude_squared() - (radius*radius); //(e-c)*(e-c)-r^2 
	double descriminate = (b*b) - (4*a*c); //(b^2-4ac)
	
	double t1 = (-b + sqrt(descriminate)) / (2 * a); //-b/2 + d
	double t2 = (-b - sqrt(descriminate)) / (2 * a); //-b/2 - d
	
	//if determinant < 0 (no intersection, imaginary number)
	if(descriminate < 0){
		return sphereInter;
	}
	
	//Find closest intersection (smallest t value), if closer set as distance
	if(t1 < t2 && t1 >= small_t){//intersection t1
		sphereInter.dist = t1;
		sphereInter.object = this;
	}
	else if(t1 >= t2 && t2 >= small_t){//intersection t2
		sphereInter.dist = t2;
		sphereInter.object = this; //set t2 object to current
	}
	
	
    return sphereInter;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = point - center; //vector from center to the point
    return normal.normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
