#include "Box.h"

//fixedRotation defaults to false
Box::Box(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, JCEngine::GLTexture texture, JCEngine::ColorRGBA8 color,
		bool fixedRotation, glm::vec4 uvRect) {

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.position.Set(position.x, position.y);
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	m_fixture = m_body->CreateFixture(&fixtureDef);

	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
}

Box::~Box() {

}

void Box::draw(JCEngine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(m_body->GetPosition().x - m_dimensions.x / 2.0f, m_body->GetPosition().y - m_dimensions.y / 2.0f, m_dimensions);
	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());
}