#include "MainGame.h"
#include <JCEngine\JCEngine.h>

#include "Actor.h"
#include "Gun.h"

#include <JCEngine\ResourceManager.h>
#include <math.h>
#include <iostream>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <JCEngine\GameErrors.h>


MainGame::MainGame(int screenWidth, int screenHeight) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_zombieCount = 0;
	_state = GameState::PLAY;
	_currentLevel = 0;

	_numHumansKilled = 0;
	_numZombiesKilled = 0;
	_numHumansAlive = 0;

	//init game engine
	JCEngine::init();

	//create window, set color to gray
	_window.create("Zombies", screenWidth, screenHeight, 0);
	_window.setColor(0.8f, 0.8f, 0.8f, 1.0f);

	//construct shader program and init
	initShaders();

	//init audio
	_audioEngine.init();

	//init camera
	_camera.Init(screenWidth, screenHeight);
	_hudCamera.Init(screenWidth, screenHeight);
	_hudCamera.SetPosition(glm::vec2(screenWidth / 2, screenHeight / 2));

	//init sprite batchers
	_spriteBatch.init();
	_hudSpriteBatch.init();

	//init particle stuff
	_bloodParticleBatch = new JCEngine::ParticleBatch2D(1000, 0.05f, JCEngine::ResourceManager::getTexture("Textures/particle.png"),
		//particle update function
		[](JCEngine::Particle2D& particle, float deltaTime) { 
			particle.position = particle.position + particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.lifetime * 255.0f); 
		});
	_particleEngine.addParticleBatch(_bloodParticleBatch);

	//init sprite font
	_spriteFont = new JCEngine::SpriteFont("Fonts/chintzy.ttf", 32);

	//init fps limiter
	_fpsLimiter.init(60.0f);
}

MainGame::~MainGame() {
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}

	for (int i = 0; i < _actors.size(); i++) {
		delete _actors[i];
	}

	delete _spriteFont;
}

void MainGame::run() {
	initSystems();

	initLevel();

	gameLoop();
}

void MainGame::initSystems() {
	//initializing most things in the constructor for now
}

void MainGame::initLevel() {
	//get level 1
	_levels.push_back(new Level("Levels/level2.txt"));

	//load music
	JCEngine::Music music = _audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);

	//create the player character 
	glm::vec2 spriteSize(64.0f, 64.0f);
	/*Player playerCharacter(_texture);
	_actors.push_back(playerCharacter);*/
	//_player = Player(_texture, _levels[_currentLevel]->getStartPlayerPosition(), &_camera);
	float levelWidth = (float)_levels[_currentLevel]->getWidth() * 64.0f;
	float levelHeight = (float)_levels[_currentLevel]->getHeight() * 64.0f;
	glm::vec2 startPlayerPosition = _levels[_currentLevel]->getStartPlayerPosition();
	/*_player = Player(glm::vec2(levelWidth / 2.0f, levelHeight / 2.0f), &_camera);*/
	_player = Player(glm::vec2(32.0f, 32.0f), &_camera);

	_camera.SetPosition(_player.getPosition());

	//create humans with random positions
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randomx(5, _levels[_currentLevel]->getWidth() - 5);
	std::uniform_int_distribution<int> randomy(5, _levels[_currentLevel]->getHeight() - 5);


	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		glm::vec2 humanPosition(randomx(randomEngine) * 64.0f, randomy(randomEngine) * 64.0f);
		_actors.push_back(new Human(humanPosition, spriteSize));
		_numHumansAlive++;
	}

	const std::vector<glm::vec2> startPositions = _levels[_currentLevel]->getZombieStartPositions();
	for (int i = 0; i < startPositions.size(); i++) {
		_actors.push_back(new Zombie(startPositions[i], spriteSize));
		_zombieCount++;
	}

	//set up guns
	_player.addGun(new Gun("Magnum", 30, 1, 0.0f, 20.0f, 30, _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player.addGun(new Gun("Shotgun", 60, 12, 0.5f, 20.0f, 4, _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player.addGun(new Gun("MP5", 5, 1, 0.2f, 20.0f, 20, _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
	
}

void MainGame::initShaders() {
	_shaderProgram.compileShaders("Shaders/ColorShading.vert.txt", "Shaders/ColorShading.frag.txt");
	_shaderProgram.addAttribute("vertexPosition");
	_shaderProgram.addAttribute("vertexColor");
	_shaderProgram.addAttribute("vertexUV");
	_shaderProgram.linkShaders();
}

void MainGame::gameLoop() {
	const int MAX_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MILLISECONDS_PER_SECOND = 1000;
	const float DESIRED_FRAME_TIME = MILLISECONDS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float currentTicks = SDL_GetTicks();

	while (_state == GameState::PLAY) {
		_fpsLimiter.start();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - currentTicks;
		currentTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

		checkVictory();

		_inputManager.update();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_STEPS) {
			float deltaTime = std::min(MAX_DELTA_TIME, totalDeltaTime);

			processInput(deltaTime);

			_camera.SetPosition(_player.getPosition());

			updateAgents(deltaTime);

			_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}
		
		

		//update camera
		_camera.Update();
		_hudCamera.Update();

		draw();

		_fpsLimiter.stop();
	}
}

void MainGame::checkVictory() {
	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

	// If all zombies are dead we win!
	if (_zombieCount == 0) {
		// Print victory message
		std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
			_numHumansKilled, _numZombiesKilled, _actors.size() - 1, _levels[_currentLevel]->getNumHumans());

		// Easy way to end the game :P
		JCEngine::fatalError("");
	}
}

void MainGame::updateAgents(float deltaTime) {
	glm::vec2 spriteSize(64.0f, 64.0f);

	for (int i = 0; i < _actors.size(); i++) {
		_actors[i]->move(_levels[_currentLevel]->getLevelData(), _actors, &_player, deltaTime);
	}

	for (int i = 0; i < _actors.size() - 1; i++) {
		for (int j = i + 1; j < _actors.size(); j++) {
			if (_actors[i]->collideWithActor(_actors[j])) {
				if (_actors[i]->isZombie() && _actors[j]->isHuman()) {
					addBlood(_actors[j]->getPosition(), 5);
					_actors.push_back(new Zombie(_actors[j]->getPosition(), spriteSize));
					_zombieCount++;
					_actors[j] = _actors.back();
					_actors.pop_back();
					_numHumansAlive--;
				}
				else if (_actors[j]->isZombie() && _actors[i]->isHuman()) {
					addBlood(_actors[i]->getPosition(), 5);
					_actors.push_back(new Zombie(_actors[i]->getPosition(), spriteSize));
					_zombieCount++;
					_actors[i] = _actors.back();
					_actors.pop_back();
					_numHumansAlive--;
				}
			}
			if (_actors[i]->collideWithActor(&_player)) {
				if (_actors[i]->isZombie()) {
					addBlood(_player.getPosition(), 5);
					std::printf("*** You lose! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
						_numHumansKilled, _numZombiesKilled, _actors.size() - 1, _levels[_currentLevel]->getNumHumans());

					JCEngine::fatalError("YOU LOST");
				}
					
			}
		}
	}

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i]->update(deltaTime);

		if (_bullets[i]->isDead() || _bullets[i]->collideWithLevel(_levels[_currentLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
			i--;
			continue;
		}

		for (int j = 0; j < _actors.size();) {
			if (_bullets[i]->collideWithActor(_actors[j])) {
				addBlood(_bullets[i]->getPosition(), 10);
				//change this to apply damage
				if (_actors[j]->applyDamage(_bullets[i]->getDamage())) {
					if (_actors[j]->isZombie()) {
						_numZombiesKilled++;
						_zombieCount--;
					}
					else if (_actors[j]->isHuman()) {
						_numHumansKilled++;
						_numHumansAlive--;
					}
						

					delete _actors[j];
					_actors[j] = _actors.back();
					_actors.pop_back();
					
				}

				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				i--;

				//restart loop for bullet
				break;
			}
			else {
				j++;
			}
		}
	}
}

void MainGame::processInput(float deltaTime) {
	SDL_Event ev;

	const float CAMERA_SPEED = 5.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type)
		{
		case SDL_QUIT:
			_state = GameState::EXIT;
			return;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(ev.motion.x, ev.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.keyPress(ev.button.button);

			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.keyRelease(ev.button.button);
			break;
		case SDL_KEYDOWN:
			_inputManager.keyPress(ev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.keyRelease(ev.key.keysym.sym);
			break;
		default:
			break;
		}
	}

	glm::vec2 movementDirection = glm::vec2(0.0f, 0.0f);

	if (_inputManager.isKeyDown(SDLK_w))
		movementDirection.y += 1;
	if (_inputManager.isKeyDown(SDLK_s))
		movementDirection.y -= 1;
	if (_inputManager.isKeyDown(SDLK_a))
		movementDirection.x -= 1;
	if (_inputManager.isKeyDown(SDLK_d))
		movementDirection.x += 1;
	if (_inputManager.isKeyDown(SDLK_q))
		_camera.SetScale(_camera.GetScale() - SCALE_SPEED);
	if (_inputManager.isKeyDown(SDLK_e))
		_camera.SetScale(_camera.GetScale() + SCALE_SPEED);

	if (_inputManager.isKeyDown(SDLK_1))
		_player.trySwitchGun(0);
	if (_inputManager.isKeyDown(SDLK_2))
		_player.trySwitchGun(1);
	if (_inputManager.isKeyDown(SDLK_3))
		_player.trySwitchGun(2);


	//only move camera and player if it is necessary
	glm::normalize(movementDirection);
	if (movementDirection.x != 0 || movementDirection.y != 0) {
		_player.setDirection(movementDirection);
		_player.move(_levels[_currentLevel]->getLevelData(), _actors, &_player, deltaTime);
		//_player.collideWithLevel(_levels[_currentLevel]->getLevelData());
	}

	_player.setDirectionFacing(&_inputManager);

	if (_player.getCurrentGunIndex() != -1) {
		_player.updateGun(&_inputManager, _bullets, deltaTime);
	}
}

void MainGame::draw() {
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);

	//make sure shader uses texture 0
	GLint textureLocation = _shaderProgram.getUniformLocation("spriteTexture");
	glUniform1i(textureLocation, 0);

	//set camera matrix
	GLuint pLocation = _shaderProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.GetCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_levels[_currentLevel]->draw();

	_spriteBatch.begin();

	const glm::vec2 actorDimensions = glm::vec2(64.0f, 64.0f);

	_player.draw(_spriteBatch);

	for (int i = 0; i < _actors.size(); i++) {
		if (_camera.isBoxInView(_actors[i]->getPosition(), actorDimensions)) {
			_actors[i]->draw(_spriteBatch);
		}
	}

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i]->draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	//draw particles
	_particleEngine.draw(&_spriteBatch);

	drawHUD();

	glBindTexture(GL_TEXTURE_2D, 0);
	_shaderProgram.unuse();

	_window.swapBuffer();
}

void MainGame::drawHUD() {
	char buffer[256];
	_hudSpriteBatch.begin();

	GLuint pLocation = _shaderProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _hudCamera.GetCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	sprintf_s(buffer, "Number of Humans: %d", _numHumansAlive);

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(1.0f), 0, JCEngine::ColorRGBA8(255, 255, 255, 255));

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(glm::vec2 position, int numParticles) {
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 2 * M_PI);
	glm::vec2 velocity(1.0f, 0.0f);

	JCEngine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++)
		_bloodParticleBatch->addParticle(position, color, glm::rotate(velocity, randAngle(randEngine)), 15.0f);
}
