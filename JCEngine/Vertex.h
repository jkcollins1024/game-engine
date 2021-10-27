#pragma once

#include <GL/glew.h>

namespace JCEngine {
	//holds 2D position of a vertex
	struct Position {
		float x;
		float y;
	};

	//four bytes for color
	struct ColorRGBA8 {
		ColorRGBA8() { r = 0; g = 0; b = 0; a = 0; };
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) {
			r = R;
			g = G;
			b = B;
			a = A;
		};
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};


	//UV for vertex for textures
	struct UV {
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		ColorRGBA8 color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}

