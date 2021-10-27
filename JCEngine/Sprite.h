#pragma once
#include <GL/glew.h>
#include <string>
#include "GLTexture.h"

namespace JCEngine {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void draw();
		void init(float x, float y, float width, float height, std::string texturePath);

	private:
		int _x;
		int _y;
		int _width;
		int _height;
		GLuint _vboID;
		GLTexture _texture;
	};

}

