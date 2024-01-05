#ifndef RAYCASTER_RAYCASTHIT_HPP
#define RAYCASTER_RAYCASTHIT_HPP

#include "material.hpp"

struct RaycastHit {
	float t;
	Material material;

	RaycastHit() : t(0.0f), material(Material(glm::vec3(0.0f))) {}
	RaycastHit(const RaycastHit&) = default;
	RaycastHit(RaycastHit&&) = default;
	virtual ~RaycastHit() = default;

	RaycastHit& operator=(const RaycastHit hit) {
		t = hit.t;
		material = hit.material;
		return *this;
	}
};

#endif