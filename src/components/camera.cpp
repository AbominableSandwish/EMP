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
}