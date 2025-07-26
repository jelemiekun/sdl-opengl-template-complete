#include "ProgramValues.h"

namespace ProgramValues {
	namespace KeyEvents {
		bool isLockedIn = true;
		bool isLockedInPressed = false;
		bool moveForwardPressed = false;
		bool moveBackwardPressed = false;
		bool moveLeftPressed = false;
		bool moveRightPressed = false;
		bool moveUpPressed = false;
		bool moveDownPressed = false;
		bool sprinting = false;
		bool fastZoom = false;
	}

	namespace LightSource {
		glm::vec3 ambient = glm::vec3(0.2f);
		glm::vec3 diffuse = glm::vec3(0.5f);
		glm::vec3 specular = glm::vec3(1.0f);
		glm::vec3 color = glm::vec3(1.0f);
		glm::vec3 position = { 1.0f, 0.3f, 0.5f };
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 rotate = glm::vec3(1.0f);
		int rotateDegrees = 0.0f;
	}

	namespace Object {
		int shininess = 1.0f;
	}
}