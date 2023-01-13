#ifndef COLOR_H
#define COLOR_H

struct Color {
	int r;
	int g;
	int b;

	Color() {
		r = 0;
		g = 0;
		b = 0;
	}

	Color(int red, int green, int blue) {
		r = red;
		g = green;
		b = blue;
	}

};

#endif