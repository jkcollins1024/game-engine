#include "GameplayScreen.h"
#include <iostream>
#include <SDL\SDL_events.h>
#include <JCEngine/IMainGame.h>
#include<JCEngine/ResourceManager.h>
#include <NinjaPlatformer\Box.h>
#include <random>

GameplayScreen::GameplayScreen(JCEngine::Window* window) {
	m_window = window;
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build()
{
}

void GameplayScreen::destroy()
{
}

void GameplayScreen::onEntry()
{
	//create world with normal gravity
	const b2Vec2 gravity(0.0f, -9.8f);
	game_world = new b2World(gravity);

	//make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);
	b2Body* groundBody = game_world->CreateBody(&groundBodyDef);

	//make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &groundBox;
	fixtureDef.density = 0.0f;
	groundBody->CreateFixture(&fixtureDef);

	//load texture
	JCEngine::GLTexture boxTexture = JCEngine::ResourceManager::getTexture("Assets/bricks_top.png");

	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xDist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yDist(-15.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> colorValue(0, 255);

	const int numBoxes = 10;
	for (int i = 0; i < numBoxes; i++) {
		JCEngine::ColorRGBA8 randColor(colorValue(randGenerator), colorValue(randGenerator), colorValue(randGenerator), 255);
		m_boxes.emplace_back(game_world, glm::vec2(xDist(randGenerator), yDist(randGenerator)), glm::vec2(size(randGenerator)), boxTexture, randColor);
	}

	m_spriteBatch.init();

	//shaders init
	m_textureProgram.compileShaders("Shaders/ColorShading.vert.txt", "Shaders/ColorShading.frag.txt");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_debugRenderer.init();

	//init camera
	m_camera.Init(m_window->GetScreenWidth(), m_window->GetScreenHeight());
	m_camera.SetScale(32.0f);

	m_player = Player(game_world, glm::vec2(0.0f, 30.0f), glm::vec2(0.8f, 1.8f), glm::vec2(2.0f), JCEngine::ColorRGBA8(255, 255, 255, 255));
}

void GameplayScreen::onExit()
{
	m_debugRenderer.dispose();
}

void GameplayScreen::update()
{
	m_camera.Update();
	checkInput();
	m_player.update(m_game->inputManager);
	//update physics simulation
	game_world->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_textureProgram.use();

	//texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("spriteTexture");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//camera matrix
	glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();

	for (auto& box : m_boxes) {
		box.draw(m_spriteBatch);
	}

	m_player.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();

	//debug rendering
	if (m_renderDebug) {
		for (auto& box : m_boxes) {
			b2Body* boxBody = box.getBody();
			glm::vec4 destRect(boxBody->GetPosition().x - box.getDimensions().x / 2.0f,
								boxBody->GetPosition().y - box.getDimensions().y / 2.0f,
								box.getDimensions().x, box.getDimensions().y);

			m_debugRenderer.drawBox(destRect, JCEngine::ColorRGBA8(255, 255, 255, 255), boxBody->GetAngle());
		}

		//render player
		m_player.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
}

void GameplayScreen::checkInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		m_game->onSDLEvent(event);
	}
}
