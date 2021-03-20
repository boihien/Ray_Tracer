#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
	//R(t) = P + tr
    vec3 color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    
    double k = reflectivity;
    //if(ray hits an an object)
    //return shade_surface
    //color = (1 - k) * color + k * (cast_ray(reflected_ray));
    
    //if k = 0 (not reflective)
    //if k = 1(purely reflective)
    if(recursion_depth >= world.recursion_depth_limit){
		return (1-k) * color;
	}
	
	vec3 v = ray.direction;
	//r = u - 2nnu
	Ray r(intersection_point, v - 2 * dot(v, normal) * normal);
	//increment recursion dpeth everytime call cast ray
	return (1-k) * color + k * world.Cast_Ray(r, ++recursion_depth);
}
