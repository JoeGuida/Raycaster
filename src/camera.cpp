#include "camera.hpp"

void Camera::translate(const glm::vec3& offset) {
	transform.position += offset;
	update_rays();
}

void Camera::rotate(float velocity) {
	direction = glm::rotateZ(direction, velocity);
	update_rays();
}

void Camera::update_rays() {
	glm::vec3 start_dir = glm::rotateZ(direction, fov / 2.0f);
	float rot_amount = fov / num_rays;

	for (int i = 0; i < num_rays; i++) {
		glm::vec3 dir = glm::rotateZ(start_dir, -rot_amount * i);
		(rays + i)->origin = transform.position;
		(rays + i)->direction = glm::normalize(dir);
	}
}
