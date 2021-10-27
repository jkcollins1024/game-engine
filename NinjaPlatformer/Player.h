#pragma once
#include <NinjaPlatformer\Box.h>

#include <JCEngine/SpriteBatch.h>
#include <JCEngine/GLTexture.h>
#include <JCEngine\InputManager.h>
#include <JCEngine\DebugRenderer.h>

#include "Capsule.h"
#include <JCEngine\TileSheet.h>

enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

class Player
{
public:
	Player() {}
	Player(b2World* world, const glm::vec2& position, const glm::vec2& collisionDimensions, const glm::vec2& drawDimensions, JCEngine::ColorRGBA8 color);
	~Player();

	void draw(JCEngine::SpriteBatch& spriteBatch);
	void drawDebug(JCEngine::DebugRenderer& renderer);
	void update(JCEngine::InputManager& inputManager);

	const Capsule& getCapsule() const { return m_collisionCapsule; }
private:
	Capsule m_collisionCapsule;
	JCEngine::TileSheet m_textureSheet;
	JCEngine::ColorRGBA8 m_color;
	glm::vec2 m_drawDimensions;
	bool m_onGround = false;
	bool m_punching = false;
	//1 for right, -1 for left
	int m_direction = 1;
	float m_animTime = 0.0f;
	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
};

