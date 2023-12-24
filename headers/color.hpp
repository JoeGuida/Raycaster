#ifndef RAYCASTER_COLOR_HPP
#define RAYCASTER_COLOR_HPP

struct Color {
	float r;
	float g;
	float b;

	Color(float r, float b, float g) : r(r), g(g), b(b) {}
	Color(const Color& color) = default;
	Color(Color&&) = default;
	virtual ~Color() = default;

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color YELLOW;
	static const Color PURPLE;
	static const Color CYAN;
	static const Color WHITE;
	static const Color BLACK;
};

#endif