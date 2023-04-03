#include <components/camera.h>
#include <components/transform.h>
#include <math/matrice.h>
#include <core/engine.h>
#include <graphic/graphic.h>

namespace emp {
	void Camera::SetProjection(float angle) {
		this->angle_proj = angle;
		this->projection = glm::mat4(1.0f);
		float delta = (float)m_graphic->width / (float)m_graphic->height;
		projection = glm::perspective(glm::radians(angle), (float)(m_graphic->width/ delta) / (float)m_graphic->height, 0.1f, 300.0f);
	}

	void Camera::Reset(){
		this->projection = glm::mat4(1.0f);
		float delta = (float)m_graphic->width / (float)m_graphic->height;
		projection = glm::perspective(glm::radians(this->angle_proj), (float)(m_graphic->width / delta) / (float)m_graphic->height, 0.1f, 300.0f);
	}

	Camera::Camera(Engine& engine, int entity, Transform& transform) : transform(transform)
	{
		this->m_graphic = engine.GetGraphicManager();
		this->entity = entity;
	}

	Camera::Camera(Engine& engine, int entity, Transform& transform, float projection) : entity(entity), transform(transform) {
		this->m_graphic = engine.GetGraphicManager();
		SetProjection(projection);
	}

	glm::vec3 Camera::GetPosition() {
		return glm::vec3(this->transform.GetPosition().x, this->transform.GetPosition().y, this->transform.GetPosition().z);
	}
	void Camera::Init()
	{
	}

	bool isometric = false;
	glm::mat4 Camera::GetView()
	{
		glm::mat4 view;
		if (isometric) {
			/* use this length so that camera is 1 unit away from origin */
			double dist = sqrt(1 / 3.0);
			view = glm::lookAt(glm::vec3(dist, dist, dist), cameraFront, cameraUp);
		}
		else {
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
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