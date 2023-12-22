#ifndef RAYCASTER_TRANSFORM_HPP
#define RAYCASTER_TRANSFORM_HPP

#include <glm/vec3.hpp>

struct Transform {
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
		position(position),
		rotation(rotation),
		scale(scale) {}
	Transform(const glm::vec3& position, const glm::vec3& rotation) :
		position(position),
		rotation(rotation),
		scale(glm::vec3(1.0f)) {}
	Transform(const glm::vec3& position) :
		position(position),
		rotation(glm::vec3(0.0f)),
		scale(glm::vec3(1.0f)) {}

	Transform(const Transform&) = default;
	Transform(Transform&&) = default;
	virtual ~Transform() = default;

	Transform& operator=(const Transform& other) {
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		return *this;
	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

#endif