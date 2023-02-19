#include "core/system.h"
#include "glm/gtx/transform.hpp"
namespace emp {
	class Transform;
	class GraphicManager;
	class Engine;
	class Engine;
	class Camera {
	public:
		
		int entity;

		GraphicManager* m_graphic = nullptr;
		Transform& transform;
		glm::mat4 projection;
		float angle_proj;

		void SetProjection(float angle);

		Camera(Engine& engine, int entity, Transform& transform);

		Camera(Engine& engine, int entity, Transform& transform, float projection);

		glm::vec3 GetPosition();

		void Reset();

		void Init();

		glm::mat4 GetView();
		glm::mat4 GetView2();

		void SetRotation(double xposIn, double yposIn) {
			{
				float xpos = static_cast<float>(xposIn);
				float ypos = static_cast<float>(yposIn);

				if (firstMouse)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouse = false;
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
				lastX = xpos;
				lastY = ypos;

				float sensitivity = 0.1f; // change this value to your liking
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				yaw += xoffset;
				pitch += yoffset;

				// make sure that when pitch is out of bounds, screen doesn't get flipped
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 front;
				front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				front.y = sin(glm::radians(pitch));
				front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				cameraFront = glm::normalize(front);
			}

		}
		// camera
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	private :
		

		bool firstMouse = true;
		float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		float pitch = 0.0f;
		float lastX = 800.0f / 2.0;
		float lastY = 600.0 / 2.0;
		float fov = 45.0f;
	};
}