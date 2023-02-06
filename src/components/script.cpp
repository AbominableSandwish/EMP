#include <components/script.h>

#include <core/engine.h>
#include <core/component.h>
#include <core/log.h>
#include <core/input.h>
#include <graphic/graphic.h>

#include <components/camera.h>
#include <components/transform.h>

namespace emp {
	TemplateScript::TemplateScript(Engine& engine, int entity)
	{
		this->engine = &engine;
		this->entity = entity;
	};

	ScriptSystem::ScriptSystem(Engine& engine, std::string name) : System(engine, name) {
		m_component = this->engine->GetComponentManager();
	}

	void ScriptSystem::Init() {
		m_component = this->engine->GetComponentManager();
	}

	void ScriptSystem::Update(float dt) {
		auto& arrayElementPlayer = this->engine->GetComponentManager()->GetComponents<PlayerScript>();
		auto& elementPlayer = arrayElementPlayer[0];
		if (&elementPlayer != nullptr) {
			elementPlayer.Update(dt);
		}
	}

	void ScriptSystem::Destroy() {
		m_component = nullptr;
	}

	PlayerScript::PlayerScript(Engine& engine, int entity) : TemplateScript(engine, entity)
	{
	}

	void PlayerScript::Init()
	{
	}

	void PlayerScript::Update(float dt)
	{
		float cameraSpeedTranslation = static_cast<float>(25.0 * dt);
		float cameraSpeedRotation = static_cast<float>(250.0 * dt);
		if (main_camera == nullptr)
			main_camera = &(engine->GetComponentManager()->GetComponents<Camera>()[0]);

		if (Input::IsKeyDown(MushKey::UP)) {
			glm::vec3 position = main_camera->transform.Position();
			main_camera->transform.SetPosition(position - cameraSpeedTranslation * main_camera->cameraFront);
		}
		if (Input::IsKeyDown(MushKey::DOWN)) {
			glm::vec3 position = main_camera->transform.Position();
			main_camera->transform.SetPosition(position + cameraSpeedTranslation * main_camera->cameraFront);
		}
		if (Input::IsKeyDown(MushKey::LEFT)) {
			glm::vec3 position = main_camera->transform.Position();
			main_camera->transform.SetPosition(position + glm::normalize(glm::cross(main_camera->cameraFront, main_camera->cameraUp)) * cameraSpeedTranslation);
			//Rotation Left
		}
		if (Input::IsKeyDown(MushKey::RIGHT)) {
			glm::vec3 position = main_camera->transform.Position();
			main_camera->transform.SetPosition(position - glm::normalize(glm::cross(main_camera->cameraFront, main_camera->cameraUp)) * cameraSpeedTranslation);
			//Rotation Right
		}
		if (Input::IsKeyDown(MushKey::SPACE)) {
			main_camera->transform.angle_x = main_camera->transform.angle_x + cameraSpeedRotation;
			main_camera->SetRotation(main_camera->transform.angle_x, 0);
			//Up;
		}
		if (Input::IsKeyDown(MushKey::CTRL_L)) {
			main_camera->transform.angle_x = main_camera->transform.angle_x - cameraSpeedRotation;
			main_camera->SetRotation(main_camera->transform.angle_x, 0);
			//Down;
		}
	
	}

	void PlayerScript::Destroy()
	{
	}

	Script::Script(Engine& engine, int entity) : TemplateScript (engine, entity)
	{
	}
	void Script::Init()
	{
	}
	void Script::Update(float dt)
	{
	}
	void Script::Destroy()
	{
	}
}