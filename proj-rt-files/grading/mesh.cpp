#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>
#include "plane.h"

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    
    return {};
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0); //will always succeed, part is index of triangle
    // triangle norm = (B-A) x (C-A) / (B-A) x (C-A)
    
    ivec3 t = triangles[part];
    
    int A = 0;
    int B = 1;
    int C = 2;
    
    vec3 BA = vertices[t[B]] - vertices[t[A]];
    
    vec3 CA = vertices[t[C]] - vertices[t[A]];
    
    return cross(BA, CA).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
	//triangle index
	int A = 0;
    int B = 1;
    int C = 2;
    
    vec3 a = vertices[triangles[tri][A]];
    vec3 b = vertices[triangles[tri][B]];
    vec3 c = vertices[triangles[tri][C]];
    vec3 intersectPoint;
    vec3 u = ray.direction;
    vec3 BA = b - a;
    vec3 CA = c - a;
    vec3 y = intersectPoint - a;
    
    Hit intersectHit = Plane(a, Normal(a,tri)).Intersection(ray, tri);
    
    //intersection point is 0 then didnt hit
    if(intersectHit.object == 0){
		return false;
	}
    
    intersectPoint = ray.Point(dist);
    
    //if area of ABC is 0 then didnt intersect
    //denom is area of ABC
	double denom = dot(cross(u, BA),CA);
	if(denom == 0){
		return false;
	}
	
	//sarea(PBC)/area(ABC)
	double gamma = dot(cross(u, BA), y);
	//sarea(APC)/area(APC)
	double beta =  dot(cross(CA, u), y);
	//1 = alpha + beta + gamma
	double alpha = 1 - (gamma+beta);
	
	//check if p in area
    //if(alpha > 0, beta > 0, gamma > 0)
	if(alpha > 0 && beta > 0 && gamma > 0){
		dist = intersectHit.dist;
		return true;
	}
    
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
