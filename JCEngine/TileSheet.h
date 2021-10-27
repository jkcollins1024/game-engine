#pragma once
#include "GLTexture.h"

#include <glm/glm.hpp>

namespace JCEngine {
	class TileSheet {
	public:
		void init(const GLTexture& texture, const glm::ivec2& tileDims) {
			this->texture = texture;
			this->dims = tileDims;
		}
		glm::vec4 getUV(int index) {
			//get the x and y positions on tile sheet
			int xpos = index % dims.x;
			int ypos = index / dims.x;

			//get the size of each tile as a fraction of the sheet size
			float xstep = 1.0f / (float)dims.x;
			float ystep = 1.0f / (float)dims.y;

			//x and y coordinates are position * tile size
			//x and y sizes are the sizes calculated above
			return glm::vec4 (xpos * xstep, ypos * ystep, xstep, ystep);
		}

		glm::vec4 getUVFlipped(int index) {
			//get the x and y positions on tile sheet
			int xpos = (index + 1) % dims.x;
			int ypos = (index + 1) / dims.x;

			//get the size of each tile as a fraction of the sheet size
			float xstep = 1.0f / (float)dims.x;
			float ystep = 1.0f / (float)dims.y;

			//x and y coordinates are position * tile size
			//x and y sizes are the sizes calculated above
			return glm::vec4(xpos * xstep, ypos * ystep, -xstep, ystep);
		}

		GLTexture texture;
		glm::ivec2 dims;
	};
}