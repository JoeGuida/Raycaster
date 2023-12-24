#ifndef RAYCASTER_CAMERA_HPP
#define RAYCASTER_CAMERA_HPP

#include <glm/gtx/rotate_vector.hpp>

#include "ray.hpp"
#include "transform.hpp"

class Camera {
public:
	Camera(float fov, const Transform& transform, const glm::vec3& direction, int num_rays) : 
		fov(fov), 
		transform(transform), 
		direction(direction),
		num_rays(num_rays) 
	{ 
		rays = new Ray[num_rays]; 
	}
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	
	~Camera() {
		delete rays;
	};

	glm::vec3 get_direction() const { return direction; }
	Ray* get_rays() const { return rays; };
	void update_rays();

	Transform transform;

private:
	float fov;
	glm::vec3 direction;
	int num_rays;
	Ray* rays;
};

#endif