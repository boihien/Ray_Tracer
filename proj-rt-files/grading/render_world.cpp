#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <limits>

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
	//set min_t to a large value
    double min_t = std::numeric_limits<double>::max();
    Hit closest_inter = {NULL, 0, 0};
    //for all objects
    for(unsigned int i = 0; i < objects.size(); i++){
		//use o-> Intersect to get closest hit w/ object
		Hit h = objects.at(i)->Intersection(ray, -1);
		
		//if Hit is the closest so far and larger than small_t
		if(h.dist < min_t && (h.object && h.dist >= small_t)){
			//update smallest dist
			min_t = h.dist;
			//Store the hit as closest
			closest_inter = h;
		}
	}
	//return closest hit
    return closest_inter;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
	// set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position;
    //normalize to keep direction but change distance 
    //world position - camera position to get ray direction
    ray.direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    // determine the color here
    Hit closest = Closest_Intersection(ray);
    const Object* obj = closest.object;
    vec3 intersection = ray.Point(closest.dist);
    
    //if there is intersection
    if(obj){
		//set color using material_shader->shade_surface()
		color = obj->material_shader->Shade_Surface(ray, intersection, obj->Normal(intersection, closest.part), recursion_depth);
	}
	else{
		//flat_shader doesnt matter which 3d vector
		color = background_shader->Shade_Surface(ray, intersection, intersection, 0);
	}
    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
