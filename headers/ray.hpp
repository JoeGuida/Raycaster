#ifndef RAYCASTER_RAY_HPP
#define RAYCASTER_RAY_HPP

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

struct Ray {
	Ray() : origin(glm::vec3(0.0f)), direction(glm::vec3(0.0f, -1.0f, 0.0f)) {}
	Ray(const glm::vec3& origin, const glm::vec3& direction) : 
		origin(origin), 
		direction(glm::normalize(direction)) {}
	Ray(const Ray&) = default;
	Ray(Ray&&) = default;
	virtual ~Ray() = default;

	glm::vec3 origin;
	glm::vec3 direction;
};

#endif