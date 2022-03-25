#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"



vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const 
    {
    vec3 color;
    vec3 curr_color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    vec3 view_dir = ray.direction - 2 * dot(ray.direction, normal) * normal;
    Ray reflected_ray(intersection_point, view_dir);

    if (recursion_depth >= world.recursion_depth_limit) {
        color = (1 - reflectivity) * curr_color; 
        return color;
    }
    else{
        color = (1 - reflectivity) * curr_color + reflectivity * world.Cast_Ray(reflected_ray, ++recursion_depth);
        return color;
    }
    
    
}
 