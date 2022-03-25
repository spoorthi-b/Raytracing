#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"


using namespace std;

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
	vec3 ambient, diffuse, specular;
	ambient = world.ambient_color * world.ambient_intensity * color_ambient;

	for (unsigned i = 0; i < world.lights.size(); i++) {
		Light* current_light = world.lights[i];
        vec3 light_color = current_light->Emitted_Light(ray.direction);
        vec3 intersection_to_light = current_light->position - intersection_point;
		if (world.enable_shadows) {
			vec3 shadowDir = (current_light->position - intersection_point).normalized();
			Ray shadow(intersection_point, shadowDir);

			Hit closehit = world.Closest_Intersection(shadow);

			if (closehit.object == NULL || closehit.dist > (current_light->position - intersection_point).magnitude()) {
                vec3 Rd = color_diffuse;
                vec3 Ld = light_color / intersection_to_light.magnitude_squared();
                intersection_to_light = intersection_to_light.normalized();
                diffuse += Rd * Ld * (max(0.0, dot(normal, intersection_to_light)));

                vec3 Rs = color_specular;
                vec3 v = (2 * dot(normal, intersection_to_light) * normal) - intersection_to_light;
                v = v.normalized();
                specular += Rs * Ld * (pow(max(0.0, dot(v, -ray.direction.normalized())), specular_power));
			}
		}
		else {
            vec3 Rd = color_diffuse;
            vec3 Ld = light_color / intersection_to_light.magnitude_squared();
            intersection_to_light = intersection_to_light.normalized();
            diffuse += Rd * Ld * (max(0.0, dot(normal, intersection_to_light)));

            vec3 Rs = color_specular;
            vec3 v = (2 * dot(normal, intersection_to_light) * normal) - intersection_to_light;
            v = v.normalized();
            specular += Rs * Ld * (pow(max(0.0, dot(v, -ray.direction.normalized())), specular_power));			
		}
	}

	color = ambient + diffuse + specular;
    return color;
}