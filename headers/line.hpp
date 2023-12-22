#ifndef RAYCASTER_LINE_HPP
#define RAYCASTER_LINE_HPP

#include <glm/vec3.hpp>

#include "material.hpp"
#include "transform.hpp"

struct Line {
public:
	Line(const glm::vec3& start, const glm::vec3& end, const Material& material, const Transform& transform) :
		start(start),
		end(end),
		material(material),
		transform(transform) {}
	Line(const Line&) = default;
	Line(Line&&) = default;
	virtual ~Line() = default;

	glm::vec3 start;
	glm::vec3 end;
	Material material;
	Transform transform;
};

#endif