#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray& ray) const
{
    double tmin[3] = {0,0,0};
    double tmax[3] = {0,0,0};
    for (int i = 0; i<3; i++){
        tmin[i] = (lo[i]-ray.endpoint[i])/ray.direction[i];
        tmax[i] = (hi[i]-ray.endpoint[i])/ray.direction[i];
    }
    
    
    return true;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
    TODO;
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    //vec3 max_comp = componentwise_max(pt,hi);
    if(dot(componentwise_max(pt,hi),pt)==dot(pt,pt)){
        hi=pt;
    }
    else if(dot(componentwise_min(pt,lo),pt)==dot(pt,pt)){
        lo=pt;
    }
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}
