#ifndef RAYCASTER_INTERSECTION_HPP
#define RAYCASTER_INTERSECTION_HPP

#include "line.hpp"
#include "ray.hpp"

class Intersection {
public:
	static float raycast(const Ray& ray, const Line& line);
};

#endif