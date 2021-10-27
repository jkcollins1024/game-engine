#include "InputManager.h"

#include <glm/glm.hpp>

namespace JCEngine {
	InputManager::InputManager() {
		_mouseCoords = glm::vec2(0.0f, 0.0f);
	}

	InputManager::~InputManager() {

	}

	void InputManager::update() {
		for (auto& it : _keyMap) {
			_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::keyPress(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	void InputManager::keyRelease(unsigned int keyID) {
		_keyMap[keyID] = false;
	}
	bool InputManager::isKeyDown(unsigned int keyID) {
		auto it = _keyMap.find(keyID);

		if (it != _keyMap.end())
			return it->second;

		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		auto prevIt = _previousKeyMap.find(keyID);

		//short-circuit if it doesn't exist, but still return current map even if it doesn't exist in previous map
		return (prevIt == _previousKeyMap.end() && it != _keyMap.end() && it->second) || (prevIt != _previousKeyMap.end() && !prevIt->second && it != _keyMap.end() && it->second);
	}

	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}