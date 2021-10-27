#include "Capsule.h"

void Capsule::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, float density, float friction, bool fixedRotation)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.position.Set(position.x, position.y);
	m_body = world->CreateBody(&bodyDef);

	//box for capsule
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x * 0.5f, (dimensions.y - dimensions.x) * 0.5f);

	b2FixtureDef fixture1Def;
	fixture1Def.shape = &boxShape;
	fixture1Def.density = density;
	fixture1Def.friction = friction;
	m_fixtures[0] = m_body->CreateFixture(&fixture1Def);

	//top circle
	b2CircleShape circle1Shape;
	circle1Shape.m_radius = dimensions.x * 0.5f;
	circle1Shape.m_p.Set(0.0f, (dimensions.y - dimensions.x) * 0.5f);

	b2FixtureDef fixture2Def;
	fixture2Def.shape = &circle1Shape;
	fixture2Def.density = density;
	fixture2Def.friction = friction;
	m_fixtures[1] = m_body->CreateFixture(&fixture2Def);

	//bottom circle
	b2CircleShape circle2Shape;
	circle2Shape.m_radius = dimensions.x * 0.5f;
	circle2Shape.m_p.Set(0.0f, -(dimensions.y - dimensions.x) * 0.5f);

	b2FixtureDef fixture3Def;
	fixture3Def.shape = &circle2Shape;
	fixture3Def.density = density;
	fixture3Def.friction = friction;
	m_fixtures[2] = m_body->CreateFixture(&fixture3Def);

	m_dimensions = dimensions;
}

void Capsule::drawDebug(JCEngine::DebugRenderer& renderer)
{
	glm::vec4 destRect(m_body->GetPosition().x - m_dimensions.x * 0.5f,
		m_body->GetPosition().y - (m_dimensions.y - m_dimensions.x) * 0.5f,
		m_dimensions.x, m_dimensions.y - m_dimensions.x);

	renderer.drawBox(destRect, JCEngine::ColorRGBA8(255, 255, 255, 255), m_body->GetAngle());
	renderer.drawCircle(glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y + (m_dimensions.y - m_dimensions.x) * 0.5f),
						JCEngine::ColorRGBA8(255, 255, 255, 255), m_dimensions.x * 0.5f);
	renderer.drawCircle(glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y - (m_dimensions.y - m_dimensions.x) * 0.5f),
						JCEngine::ColorRGBA8(255, 255, 255, 255), m_dimensions.x * 0.5f);
}
