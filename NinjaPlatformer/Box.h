#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <JCEngine/Vertex.h>
#include <JCEngine\SpriteBatch.h>
#include <JCEngine\GLTexture.h>
class Box
{
public:
	Box() {}
	Box(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, JCEngine::GLTexture texture, JCEngine::ColorRGBA8 color,
		bool fixedRotation = false, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	~Box();

	void draw(JCEngine::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	glm::vec2 getDimensions() const { return m_dimensions; }
	JCEngine::ColorRGBA8 getColor() const { return m_color; }

private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions = glm::vec2(0.0f, 0.0f);
	glm::vec4 m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	JCEngine::ColorRGBA8 m_color;
	JCEngine::GLTexture m_texture;
};

