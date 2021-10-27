#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace JCEngine {
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
	public:
		Glyph() { _texture = 0; _depth = 0; };
		Glyph(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color);
		Glyph(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, float angle);

		GLuint _texture;
		float _depth;

		Vertex _topLeft;
		Vertex _bottomLeft;
		Vertex _topRight;
		Vertex _bottomRight;
	private:
		glm::vec2 rotate(glm::vec2 position, float angle);
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture) {  }

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		void draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color);
		void draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, float angle); //in radians
		void draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2 direction);
		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;
		GlyphSortType _sortType;
		
		std::vector<Glyph> _glyphs;
		std::vector<Glyph*> _glyphPointers;
		std::vector<RenderBatch> _batches;
	};
}


