#include <components/camera.h>
#include <components/transform.h>
#include <math/matrice.h>$

namespace emp {
	Camera::Camera(int entity, Transform& transform) : transform(transform)
	{
		this->entity = entity;
	}

	glm::vec3 Camera::GetPosition() {
		return glm::vec3(this->transform.GetPosition().x, this->transform.GetPosition().y, this->transform.GetPosition().z);
	}
	glm::mat4 Camera::GetView()
	{
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		view = glm::translate(view, transform.Position());
		return view;
	}

	glm::mat4 Camera::GetView2()
	{
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//view = glm::translate(view, transform.Position());
		return view;
	}

}