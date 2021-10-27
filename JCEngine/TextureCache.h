#pragma once
#include <string>
#include "GLTexture.h"
#include <map>

namespace JCEngine {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string filePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};
}


