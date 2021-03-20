#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    
    //ambient + diffuse + specular = phong

    //diffuse = Id = Light intensity * Reflective material * max(n dot l, 0)
   
	//ambient = RL
	ambient = color_ambient * world.ambient_color * world.ambient_intensity;
    
    //for each pixel
    for(unsigned int i = 0; i < world.lights.size(); i++){
		
		vec3 l = world.lights[i]->position - intersection_point;
		
		//find closest object that intersects ray
		Ray light_to_hit(intersection_point, l);
		Hit hit = world.Closest_Intersection(light_to_hit);
		
		vec3 obj_to_hit = light_to_hit.Point(hit.dist) - intersection_point;
		if(!world.enable_shadows || (!hit.object || obj_to_hit.magnitude() > l.magnitude())){
		
			//diffuse = R(surface property) L (Light energy) * (a/h)
			//a/h = max(normal*lightdirection, 0)
			//multiply by color for 07.txt
			//max between 0 because less than 0 is black
			diffuse += color_diffuse * world.lights[i]->Emitted_Light(l)* std::max(dot(normal, l.normalized()), 0.0);
		
			//specular = R(surface property) L(Light energy) * max(v dot r, 0)^alpha
			//r = -l + 2(n dot l * n)
			vec3 r = (-l + 2 * dot(l, normal) * normal).normalized();
			specular += color_specular * world.lights[i]->Emitted_Light(l) * std::pow(std::max(dot(r, -(ray.direction)), 0.0), specular_power);
		}
	}
    
   
    return ambient + specular + diffuse;
}
