#include "Player.h"

#include <JCEngine/ResourceManager.h>
#include <SDL\SDL.h>


Player::Player(b2World* world, const glm::vec2& position, const glm::vec2& collisionDimensions, const glm::vec2& drawDimensions, JCEngine::ColorRGBA8 color)
{
	//load texture
	//JCEngine::GLTexture playerTexture = JCEngine::ResourceManager::getTexture("Assets/blue_ninja.png");
	//m_collisionBox = Box(world, position, dimensions, playerTexture, color, true, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));

	m_collisionCapsule.init(world, position, collisionDimensions, 1.0f, 0.1f, true);
	JCEngine::GLTexture texture = JCEngine::ResourceManager::getTexture("Assets/blue_ninja.png");
	m_color = color;
	m_drawDimensions = drawDimensions;
	m_textureSheet.init(texture, glm::ivec2(10, 2));
}

Player::~Player()
{
}

void Player::draw(JCEngine::SpriteBatch& spriteBatch)
{
	//m_collisionBox.draw(spriteBatch);

	b2Body* playerBody = m_collisionCapsule.getBody();
	glm::vec4 destRect(playerBody->GetPosition().x - m_drawDimensions.x / 2.0f,
						playerBody->GetPosition().y - m_collisionCapsule.getDimensions() .y / 2.0f,
						m_drawDimensions);

	glm::vec2 velocity = glm::vec2(playerBody->GetLinearVelocity().x, playerBody->GetLinearVelocity().y);

	int tileIndex, numTiles;

	float animationSpeed = 0.2f;
	//get animation frame
	if (m_onGround) {
		if (m_punching) {
			tileIndex = 1;
			numTiles = 4;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_animTime = 0.0f;
				m_moveState = PlayerMoveState::PUNCHING;
			}
		}
		else {
			tileIndex = 0;
			numTiles = 1;
			if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && m_direction > 0) || (velocity.x < 0 && m_direction < 0))) {
				tileIndex = 10;
				numTiles = 6;
				animationSpeed = abs(velocity.x) * 0.025f;
				if (m_moveState != PlayerMoveState::RUNNING) {
					m_animTime = 0.0f;
				}
			}
			m_moveState = PlayerMoveState::RUNNING;
		}
	}
	else {
		if (m_punching) {
			tileIndex = 18;
			numTiles = 1;
			animationSpeed *= 0.25f;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_animTime = 0.0f;
				m_moveState = PlayerMoveState::PUNCHING;
			}
		}
		else  if (abs(velocity.x) >= 6.5f) {
			tileIndex = 10;
			numTiles = 1;
			m_moveState = PlayerMoveState::IN_AIR;
		}
		else if (velocity.y >= 0) {
			tileIndex = 16;
			numTiles = 1;
			m_moveState = PlayerMoveState::IN_AIR;
		}
		else {
			tileIndex = 17;
			numTiles = 1;
			m_moveState = PlayerMoveState::IN_AIR;
		}
	}

	m_animTime += animationSpeed;
	tileIndex = tileIndex + (int)m_animTime % numTiles;

	if (m_animTime > numTiles)
		m_punching = false;

	glm::vec4 uvRect;

	//if looking left, flip so the sprite looks left
	if (m_direction == -1)
		uvRect = m_textureSheet.getUVFlipped(tileIndex);
	else
		uvRect = m_textureSheet.getUV(tileIndex);

	

	spriteBatch.draw(destRect, uvRect, m_textureSheet.texture.id, 0.0f, m_color, m_collisionCapsule.getBody()->GetAngle());
}

void Player::drawDebug(JCEngine::DebugRenderer& renderer)
{
	m_collisionCapsule.drawDebug(renderer);
}

void Player::update(JCEngine::InputManager& inputManager)
{
	m_onGround = false;

	b2Body* playerBody = m_collisionCapsule.getBody();
	if (inputManager.isKeyDown(SDLK_a)) {
		playerBody->ApplyForceToCenter(b2Vec2(-20.0f, 0.0f), true);
		m_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		playerBody->ApplyForceToCenter(b2Vec2(20.0f, 0.0f), true);
		m_direction = 1;
	}
	else {
		playerBody->SetLinearVelocity(b2Vec2(playerBody->GetLinearVelocity().x * 0.95f, playerBody->GetLinearVelocity().y));
	}
	
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT))
		m_punching = true;

	float MAX_SPEED = 7.0f;
	if (playerBody->GetLinearVelocity().x < -MAX_SPEED) {
		playerBody->SetLinearVelocity(b2Vec2(-MAX_SPEED, playerBody->GetLinearVelocity().y));
	}
	else if (playerBody->GetLinearVelocity().x > MAX_SPEED) {
		playerBody->SetLinearVelocity(b2Vec2(MAX_SPEED, playerBody->GetLinearVelocity().y));
	}

	//loop through all contact points
	for (b2ContactEdge* ce = playerBody->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* contact = ce->contact;
		if (contact->IsTouching()) {
			b2WorldManifold manifold;
			contact->GetWorldManifold(&manifold);

			bool below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < playerBody->GetPosition().y - m_collisionCapsule.getDimensions().y / 2.0f + m_collisionCapsule.getDimensions().x / 2.0f + 0.01f) {
					below = true;
					m_onGround = true;
					break;
				}
			}

			if (below) {
				if (inputManager.isKeyPressed(SDLK_SPACE)) {
					playerBody->ApplyLinearImpulse(b2Vec2(0.0f, 15.0f), playerBody->GetPosition(), true);
					break;
				}
			}
		}
	}
	
}
