#ifndef RAYCASTER_LINE_HPP
#define RAYCASTER_LINE_HPP

#include <glm/vec3.hpp>

struct Line {
public:
	Line(const glm::vec3& start, const glm::vec3& end) :
		start(start),
		end(end) {}
	Line(const Line&) = default;
	Line(Line&&) = default;
	virtual ~Line() = default;

	glm::vec3 start;
	glm::vec3 end;
};

#endif