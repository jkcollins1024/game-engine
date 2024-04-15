#include "DebugRenderer.h"

const float PI = 3.14159265358f;
const char* VERTEX_SOURCE = R"(#version 130

in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main(){
	gl_Position.xy = (P * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
})";

const char* FRAGMENT_SOURCE = R"(#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

void main(){
	color = fragmentColor;
})";

namespace JCEngine {
	DebugRenderer::DebugRenderer()
	{
	}

	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init()
	{
		//set up shaders
		m_program.compileShadersFromSource(VERTEX_SOURCE, FRAGMENT_SOURCE);
		m_program.addAttribute("vertexPosition");
		m_program.addAttribute("vertexColor");
		m_program.linkShaders();

		//set up buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		//position attribute and color attribute
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}

	void DebugRenderer::end()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(DebugVertex), m_vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = m_indices.size();
		m_indices.clear();
		m_vertices.clear();
	}

	glm::vec2 rotate(glm::vec2 position, float angle) {
		glm::vec2 newVector(position.x * cos(angle) - position.y * sin(angle),
			position.x * sin(angle) + position.y * cos(angle));

		return newVector;

	}

	void DebugRenderer::drawBox(const glm::vec4 destRect, const ColorRGBA8& color, float angle)
	{
		int i = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 4);

		glm::vec2 halfDimensions(destRect.z / 2.0f, destRect.w / 2.0f);

		//get centered position points
		glm::vec2 topLeft(-halfDimensions.x, halfDimensions.y);
		glm::vec2 botLeft(-halfDimensions.x, -halfDimensions.y);
		glm::vec2 topRight(halfDimensions.x, halfDimensions.y);
		glm::vec2 botRight(halfDimensions.x, -halfDimensions.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);

		m_vertices[i].position = rotate(topLeft, angle) + halfDimensions + positionOffset;
		m_vertices[i + 1].position = rotate(botLeft, angle) + halfDimensions + positionOffset;
		m_vertices[i + 2].position = rotate(botRight, angle) + halfDimensions + positionOffset;
		m_vertices[i + 3].position = rotate(topRight, angle) + halfDimensions + positionOffset;

		for (int j = i; j < i + 4; j++) {
			m_vertices[j].color = color;
		}

		m_indices.reserve(m_indices.size() + 8);

		//push indices to make line segments
		m_indices.push_back(i);
		m_indices.push_back(i + 1);
		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);
		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);
		m_indices.push_back(i + 3);
		m_indices.push_back(i);
	}

	void DebugRenderer::drawTriangle(const glm::vec2 position, const glm::vec2 vertex1, const glm::vec2 vertex2, const glm::vec2 vertex3, const ColorRGBA8& color, float angle)
	{
		int i = m_vertices.size();
		m_vertices.resize(m_vertices.size() + 3);

		//glm::vec2 halfDimensions(dimensions.x / 2.0f, dimensions.y / 2.0f);

		//get centered position points
		/*glm::vec2 topLeft(-halfDimensions.x, halfDimensions.y);
		glm::vec2 botLeft(-halfDimensions.x, -halfDimensions.y);
		glm::vec2 topRight(halfDimensions.x, halfDimensions.y);
		glm::vec2 botRight(halfDimensions.x, -halfDimensions.y);*/

		//glm::vec2 positionOffset(destRect.x, destRect.y);

		m_vertices[i].position = rotate(vertex1, angle) + position;// +halfDimensions + positionOffset;
		m_vertices[i + 1].position = rotate(vertex2, angle) + position;// +halfDimensions + positionOffset;
		m_vertices[i + 2].position = rotate(vertex3, angle) + position;// +halfDimensions + positionOffset;
		//m_vertices[i + 3].position = rotate(topRight, angle);// +halfDimensions + positionOffset;

		for (int j = i; j < i + 3; j++) {
			m_vertices[j].color = color;
		}

		m_indices.reserve(m_indices.size() + 6);

		//push indices to make line segments
		m_indices.push_back(i);
		m_indices.push_back(i + 1);
		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);
		m_indices.push_back(i + 2);
		m_indices.push_back(i);
	}


	void DebugRenderer::drawCircle(const glm::vec2 center, const ColorRGBA8& color, float radius)
	{
		static const int NUM_VERTICES = 100;

		//set up vertexes for circle
		int start = m_vertices.size();
		m_vertices.resize(m_vertices.size() + NUM_VERTICES);

		for (int i = 0; i < NUM_VERTICES; i++) {
			float angle = PI * 2.0f * (float)i / NUM_VERTICES;
			m_vertices[start + i].position.x = center.x + cos(angle) * radius;
			m_vertices[start + i].position.y = center.y + sin(angle) * radius;
			m_vertices[start + i].color = color;
		}

		//set up vertex indices for line segment drawing
		m_indices.reserve(m_indices.size() + NUM_VERTICES * 2);
		for (int i = 0; i < NUM_VERTICES - 1; i++) {
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}

		m_indices.push_back(start + NUM_VERTICES - 1);
		m_indices.push_back(start);
	}

	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
	{
		m_program.use();

		//camera matrix
		GLint pUniform = m_program.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glBindVertexArray(m_vao);

		glLineWidth(lineWidth);
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		m_program.unuse();
	}

	void DebugRenderer::dispose()
	{
		if (m_vao)
			glDeleteVertexArrays(1, &m_vao);

		if (m_vbo)
			glDeleteBuffers(1, &m_vbo);

		if (m_ibo)
			glDeleteBuffers(1, &m_ibo);

		m_program.dispose();
	}

}
