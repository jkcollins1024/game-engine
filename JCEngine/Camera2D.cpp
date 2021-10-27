#include "Camera2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace JCEngine {
	Camera2D::Camera2D() {
		_position = glm::vec2(0.0f, 0.0f);
		_cameraMatrix = glm::mat4(1.0f);
		_orthoMatrix = glm::mat4(1.0f);
		_scale = 1.0f;
		_needsUpdate = true;
		_screenWidth = 500;
		_screenHeight = 500;
	}

	Camera2D::~Camera2D() {

	}

	void Camera2D::Init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::Update() {
		if (_needsUpdate) {
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsUpdate = false;
		}
	}

	glm::vec2 Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords) {
		//invert y
		screenCoords.y = _screenHeight - screenCoords.y;

		//convert to 0,0 in the middle of the window
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

		//transform using scale
		screenCoords /= _scale;

		//transform using camera position
		screenCoords += _position;

		return screenCoords;
	}

	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& size) {
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;
		const float MIN_DISTANCEX = (size.x / 2.0f) + (scaledScreenDimensions.x / 2.0f);
		const float MIN_DISTANCEY = (size.y / 2.0f) + (scaledScreenDimensions.y / 2.0f);

		glm::vec2 centerPosition = position + size / 2.0f;
		glm::vec2 distVec = _position - centerPosition;

		float xdepth = MIN_DISTANCEX - abs(distVec.x);
		float ydepth = MIN_DISTANCEY - abs(distVec.y);

		if (xdepth > 0 && ydepth > 0) {
			return true;
		}

		return false;
	}
}
