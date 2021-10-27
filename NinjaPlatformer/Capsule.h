#pragma once
#include <box2d\box2d.h>
#include <glm\glm.hpp>
#include <JCEngine\DebugRenderer.h>

class Capsule
{
public:
	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, float density, float friction, bool fixedRotation = false);

	void drawDebug(JCEngine::DebugRenderer& renderer);

	b2Body* getBody() { return m_body; }
	glm::vec2 getDimensions() { return m_dimensions; }
	b2Fixture* getFixture(int index) const { return m_fixtures[index]; }

private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dimensions = glm::vec2(0.0f, 0.0f);
};

