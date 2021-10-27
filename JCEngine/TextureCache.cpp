#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace JCEngine {
	TextureCache::TextureCache() {

	}

	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::getTexture(std::string filePath) {
		//std::map<std::string, GLTexture>::iterator mit = _textureMap.find(filePath);

		auto mit = _textureMap.find(filePath);

		if (mit == _textureMap.end()) {
			//std::cout << "Loaded texture from file" << '\n';
			//load texture from filepath and return
			GLTexture newTexture = ImageLoader::loadPNG(filePath);

			_textureMap.insert(make_pair(filePath, newTexture));

			return newTexture;
		}
		//std::cout << "Loaded cache texture" << '\n';
		return mit->second;
	}
}

