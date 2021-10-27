#include "SpriteBatch.h"

#include <algorithm>

namespace JCEngine {

	Glyph::Glyph(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color) {
		_texture = texture;
		_depth = depth;

		_topLeft.color = color;
		_topLeft.setPosition(positionRectangle.x, positionRectangle.y + positionRectangle.w);
		_topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

		_bottomLeft.color = color;
		_bottomLeft.setPosition(positionRectangle.x, positionRectangle.y);
		_bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

		_topRight.color = color;
		_topRight.setPosition(positionRectangle.x + positionRectangle.z, positionRectangle.y + positionRectangle.w);
		_topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);

		_bottomRight.color = color;
		_bottomRight.setPosition(positionRectangle.x + positionRectangle.z, positionRectangle.y);
		_bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
	}

	Glyph::Glyph(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		_texture = texture;
		_depth = depth;

		glm::vec2 halfDimensions(positionRectangle.z / 2.0f, positionRectangle.w / 2.0f);

		//get centered position points
		glm::vec2 topLeft(-halfDimensions.x, halfDimensions.y);
		glm::vec2 botLeft(-halfDimensions.x, -halfDimensions.y);
		glm::vec2 topRight(halfDimensions.x, halfDimensions.y);
		glm::vec2 botRight(halfDimensions.x, -halfDimensions.y);

		topLeft = rotate(topLeft, angle) + halfDimensions;
		botLeft = rotate(botLeft, angle) + halfDimensions;
		topRight = rotate(topRight, angle) + halfDimensions;
		botRight = rotate(botRight, angle) + halfDimensions;

		_topLeft.color = color;
		_topLeft.setPosition(positionRectangle.x + topLeft.x, positionRectangle.y + topLeft.y);
		_topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

		_bottomLeft.color = color;
		_bottomLeft.setPosition(positionRectangle.x + botLeft.x, positionRectangle.y + botLeft.y);
		_bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

		_topRight.color = color;
		_topRight.setPosition(positionRectangle.x + topRight.x, positionRectangle.y + topRight.y);
		_topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);

		_bottomRight.color = color;
		_bottomRight.setPosition(positionRectangle.x + botRight.x, positionRectangle.y + botRight.y);
		_bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);
	}

	glm::vec2 Glyph::rotate(glm::vec2 position, float angle) {
		glm::vec2 newVector(position.x * cos(angle) - position.y * sin(angle),
			position.x * sin(angle) + position.y * cos(angle));

		return newVector;

	}

	SpriteBatch::SpriteBatch() {
		_vbo = 0;
		_vao = 0;
	}

	SpriteBatch::~SpriteBatch() {

	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType) {
		_sortType = sortType;
		_batches.clear();

		_glyphs.clear();
	}

	void SpriteBatch::end() {
		_glyphPointers.resize(_glyphs.size());

		for (int i = 0; i < _glyphs.size(); i++) {
			_glyphPointers[i] = &_glyphs[i];
		}

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color) {
		_glyphs.emplace_back(positionRectangle, uvRectangle, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		_glyphs.emplace_back(positionRectangle, uvRectangle, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& positionRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2 direction) {
		const glm::vec2 rightVector(1.0f, 0.0f);
		float angle = acos(glm::dot(rightVector, direction));

		if (direction.y < 0)
			angle = -angle;

		_glyphs.emplace_back(positionRectangle, uvRectangle, texture, depth, color, angle);
	}

	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);

		for (int i = 0; i < _batches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _batches[i].texture);

			glDrawArrays(GL_TRIANGLES, _batches[i].offset, _batches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);

		if (_glyphPointers.empty())
			return;

		int currVertex = 0, currGlyph = 0;
		/*_batches.emplace_back(0, 6, _glyphPointers[0]->texture);

		vertices[currVertex++] = _glyphPointers[currGlyph]->topLeft;
		vertices[currVertex++] = _glyphPointers[currGlyph]->bottomLeft;
		vertices[currVertex++] = _glyphPointers[currGlyph]->bottomRight;
		vertices[currVertex++] = _glyphPointers[currGlyph]->bottomRight;
		vertices[currVertex++] = _glyphPointers[currGlyph]->topRight;
		vertices[currVertex++] = _glyphPointers[currGlyph]->topLeft;*/

		for (currGlyph; currGlyph < _glyphPointers.size(); currGlyph++) {
			if (currGlyph == 0 || _glyphPointers[currGlyph]->_texture != _glyphPointers[currGlyph - 1]->_texture)
				_batches.emplace_back(currVertex, 6, _glyphPointers[currGlyph]->_texture);
			else
				_batches.back().numVertices += 6;

			vertices[currVertex++] = _glyphPointers[currGlyph]->_topLeft;
			vertices[currVertex++] = _glyphPointers[currGlyph]->_bottomLeft;
			vertices[currVertex++] = _glyphPointers[currGlyph]->_bottomRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->_bottomRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->_topRight;
			vertices[currVertex++] = _glyphPointers[currGlyph]->_topLeft;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray() {
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		if (_vbo == 0)
			glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//uv attribute ponter
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
				break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return a->_depth < b->_depth;
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return a->_depth > b->_depth;
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return a->_texture < b->_texture;
	}

}
