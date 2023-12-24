#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <array>
#include <memory>

#include "color.hpp"
#include "line.hpp"
#include "material.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "rectangle.hpp"
#include "shader.hpp"

class Renderer {
public:
	static void initialize() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	static void draw(const Line& line, const Color& color, const std::shared_ptr<Shader>& shader);
	static void draw(const Point& point, const std::shared_ptr<Shader>& shader);
	static void draw(const Ray& ray, const Material& material, const std::shared_ptr<Shader>& shader, float distance = 4.0f);
	static void draw(const Rectangle& rectangle, const std::shared_ptr<Shader>& shader);

private:
	static uint32_t VAO;
	static uint32_t VBO;
	static uint32_t EBO;
};

#endif