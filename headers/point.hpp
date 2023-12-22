#ifndef RAYCASTER_POINT_HPP
#define RAYCASTER_POINT_HPP

#include "material.hpp"
#include "transform.hpp"

struct Point {
	Point(float size, const Material& material, const Transform& transform) : size(size), material(material), transform(transform) {}
	Point(const Point&) = default;
	Point(Point&&) = default;
	virtual ~Point() = default;

	float size;
	Material material;
	Transform transform;
};

#endif