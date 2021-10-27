#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace JCEngine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void Init(int screenWidth, int screenHeight);

		void move(float speed, glm::vec2 direction) {
			glm::normalize(direction);

			SetPosition(_position + (direction * speed));
		}

		void SetPosition(const glm::vec2& newPosition) {
			_position = newPosition;
			_needsUpdate = true;
		}

		glm::vec2 GetPosition() { return _position; }

		void SetScale(float newScale) {
			_scale = newScale;
			_needsUpdate = true;
		}

		float GetScale() { return _scale; }

		glm::mat4 GetCameraMatrix() { return _cameraMatrix; }

		void Update();

		glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2& size);

	private:
		bool _needsUpdate;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		float _scale;
		int _screenWidth;
		int _screenHeight;
	};
}

