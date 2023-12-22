#ifndef RAYCASTER_MATERIAL_HPP
#define RAYCASTER_MATERIAL_HPP

#include <glm/vec3.hpp>

struct Material {
	Material(const glm::vec3& color) : color(color) {}
	Material(const Material&) = default;
	Material(Material&&) = default;
	virtual ~Material() = default;

	Material& operator=(const Material& other) {
		color = other.color;
		return *this;
	}

	glm::vec3 color;
};

#endif