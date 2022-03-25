#include "mesh.h"
#include "plane.h"
#include <fstream>
#include <string>
#include <limits>

#define max_num std::numeric_limits<double>::max()
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
    Hit inter;
    double t;

    if (part >= 0) {
        if (Intersect_Triangle(ray, part, inter.dist)) {
            inter.object = this;
            inter.part = part;
        }
        
    } else {
        inter.dist = max_num;
        for (unsigned i = 0; i < triangles.size(); i++) {
            if (Intersect_Triangle(ray, i, t)) {
                if (t < inter.dist) {
                    inter.object = this;
                    inter.dist = t;
                    inter.part = i;
                }
            }
        }        
    }

    return inter;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    ivec3 triangle_arr = triangles[part];
    vec3 u = vertices[triangle_arr[1]] - vertices[triangle_arr[0]];
    vec3 v = vertices[triangle_arr[2]] - vertices[triangle_arr[0]];
    vec3 norm = cross(u,v).normalized();

    return norm;
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
    // Triangle vertices

    vec3 a_ver = vertices[triangles[tri][0]];
    vec3 b_ver = vertices[triangles[tri][1]];
    vec3 c_ver = vertices[triangles[tri][2]];
    vec3 intersect;
    
    Hit hit = Plane(a_ver, Normal(a_ver, tri)).Intersection(ray, tri);
    intersect = ray.Point(dist);
    double den = dot(cross(ray.direction, b_ver - a_ver), c_ver - a_ver);
    
    if (!hit.object) {
        return false;
    }
    else if (!den) {
        return false;
    }

    double beta = dot(cross(c_ver - a_ver, ray.direction), intersect - a_ver) / den;
    double gamma = dot(cross(ray.direction, b_ver - a_ver), intersect - a_ver) / den;
    double alpha = 1 - (gamma + beta);

    if (alpha > -weight_tol && beta > -weight_tol && gamma > -weight_tol) {
        dist = hit.dist;
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
