#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace JCEngine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void keyPress(unsigned int keyID);
		void keyRelease(unsigned int keyID);
		bool isKeyDown(unsigned int keyID);

		//returns whether key was just pressed
		bool isKeyPressed(unsigned int keyID);
		void setMouseCoords(float x, float y);

		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;

		glm::vec2 _mouseCoords;
	};
}


