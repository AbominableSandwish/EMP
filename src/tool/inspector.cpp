#include <tool/inspector.h>
#include "core/log.h"
#include <core/engine.h>
#include <core/entity.h>
#include "graphic/graphic.h"
#include <core/component.h>
#include <math/vector.h>
#include <components/transform.h>

namespace emp {
	class Transform;
}

void Inspector::Init()
	{
		this->manager = m_engine->GetEntityManager();	
	}

	Inspector::Inspector(emp::Engine& engine, std::string name) : Tool(engine, name)
	{
		instance = this;
	}

	void Inspector::Update(float)
	{
	}

	void Inspector::Draw()
	{
		if (Target != -1) {
			int bufSize = 32;
			char buffer[32] = {};
			int c = 0;
			emp::Entity* entity = manager->GetEntity(Target);
			for (auto character : entity->GetName())
			{
				buffer[c] = character;
				c++;
			}
			ImGui::Text("Name: ");
			ImGui::SameLine();
			if(ImGui::InputText("##name", buffer, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				emp::LOG::Debug("Setting attributes Spore");
				entity->SetName(buffer);
			}
			emp::Transform& tranform = m_engine->GetComponentManager()->GetComponent<emp::Transform>(Target);
			
			
			emp::Vector2 position = tranform.GetPosition();
			emp::Vector2 scale = tranform.GetScale();
			emp::Vector2 rotation = tranform.GetRotation();

			ImGui::Text("Matrice: ");
			float input_rotation[2] = { rotation.x, rotation.y };
			float input_position[2] = { position.x, position.y };
			float input_scale[2] = { scale.x, scale.y };
			ImGui::Text("   Position: ");
			ImGui::SameLine();
			ImGui::InputFloat2("##Position", input_position, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			tranform.SetPosition(input_position[0], input_position[1]);
			
			ImGui::Text("   Rotation: ");
			ImGui::SameLine();
			ImGui::InputFloat2("##Rotation", input_rotation, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			tranform.SetRotation(input_rotation[0], input_rotation[1]);

			ImGui::Text("   Scale:    ");
			ImGui::SameLine();
			ImGui::InputFloat2("##Scale", input_scale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			tranform.SetScale(input_scale[0], input_scale[1]);

		}
	}

	void Inspector::Destroy()
	{
		
	}
	
	void Inspector::SetTarget(int id)
	{
		Target = id;
	}

	Inspector* Inspector::GetInstance()
	{
		if (instance == nullptr)
		{
			return nullptr;
		}
		return instance;
	}
