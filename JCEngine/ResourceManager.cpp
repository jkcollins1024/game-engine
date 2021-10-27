#include "ResourceManager.h"

namespace JCEngine {
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(std::string filePath) {
		return _textureCache.getTexture(filePath);
	}
}
