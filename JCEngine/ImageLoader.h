#pragma once

#include "GLTexture.h"

#include <string>

namespace JCEngine {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}


