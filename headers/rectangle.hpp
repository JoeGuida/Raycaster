#ifndef RAYCASTER_RECTANGLE_HPP
#define RAYCASTER_RECTANGLE_HPP

#include "material.hpp"
#include "transform.hpp"

struct Rectangle {
	Rectangle(float width, float height, const Material& material, const Transform& transform) :
		width(width),
		height(height),
		material(material),
		transform(transform) {}
	Rectangle(float width, const Material& material, const Transform& transform) :
		width(width),
		height(width),
		material(material),
		transform(transform) {}
	Rectangle(const Rectangle&) = default;
	Rectangle(Rectangle&&) = default;
	virtual ~Rectangle() = default;

	float width;
	float height;
	Material material;
	Transform transform;
};

#endif