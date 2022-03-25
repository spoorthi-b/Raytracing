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
    double min_t = std::numeric_limits<double>::max();
    Hit hitobj;
    hitobj.object = NULL;
    Hit normal;
    for(unsigned i=0;i<objects.size();i++){
    	normal = objects[i]->Intersection(ray,-1);
    	if((normal.dist<min_t) && (normal.object && normal.dist>=small_t)){ //normal.obj MISSING
    		hitobj = normal;
    		min_t = normal.dist;
		}
	}
    return hitobj;
}

// set up the initial view ray and call

void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // set up the initial view ray here
    vec3 wp = camera.World_Position(pixel_index);
    vec3 ep = camera.position;
    vec3 dir = wp-ep;
    dir = dir.normalized();
    Ray ray;
    ray.endpoint = ep;
    ray.direction = dir;
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
    Hit closehit = Closest_Intersection(ray);
    if(closehit.object == NULL){
    	color = background_shader->Shade_Surface(ray,{0,0,0},{0,0,0},0);
	}
	else{
		double t = closehit.dist;
		vec3 intersection_point = ray.Point(t);
		vec3 normal = closehit.object->Normal(intersection_point,closehit.part);
        normal = normal.normalized();
		color = closehit.object->material_shader->Shade_Surface(ray,intersection_point,normal,recursion_depth);
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
