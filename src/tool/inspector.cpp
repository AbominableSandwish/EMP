#include <tool/inspector.h>
#include "core/log.h"
#include <core/engine.h>
#include <core/entity.h>
#include "graphic/graphic.h"
#include <core/component.h>
#include <math/matrice.h>
#include <components/transform.h>
#include <components/light.h>

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
			
			float angle_x = tranform.angle_x;
			float angle_y = tranform.angle_y;
			float angle_z = tranform.angle_z;

			emp::Vector3 position = tranform.GetPosition();
			emp::Vector3 scale = tranform.GetScale();

			ImGui::Text("Matrice: ");
			float input_rotation_x = angle_x;
			float input_rotation_y = angle_y;
			float input_rotation_z = angle_z;
			float input_position[2] = { position.x, position.y };
			float input_scale[2] = { scale.x, scale.y };
			ImGui::Text("   Position: ");
			ImGui::SameLine();

			bool ismodif = false;

			if (ImGui::InputFloat2("##Position", input_position, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)) {
				//tranform.TranslationMatrix(emp::Vector3(input_position[0], input_position[1], 1));
				ismodif = true;
				printf("AAAA");
			}
			
			ImGui::Text("   Rotation: ");
			if(ImGui::InputFloat("##RotationAxeX", &input_rotation_x, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;
			if(ImGui::InputFloat("##RotationAxeY", &input_rotation_y, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;
			if (ImGui::InputFloat("##RotationAxeZ", &input_rotation_z, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;

			ImGui::Text("   Scale:   farf ");
			ImGui::SameLine();
			if(ImGui::InputFloat2("##Scale", input_scale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;

			if (ismodif) {
				tranform.Reset();
				tranform.SetPosition(emp::Vector3(input_position[0], input_position[1], 1));
				tranform.SetRotation(input_rotation_x, emp::Vector3(1, 0, 0));
				tranform.SetRotation(input_rotation_y, emp::Vector3(0, 1, 0));
				tranform.SetRotation(input_rotation_z, emp::Vector3(0, 0, 1));
				tranform.SetScale(input_scale[0], input_scale[1]);
			}

			emp::Light& light = m_engine->GetComponentManager()->GetComponent<emp::Light>(Target);
			ismodif = false;

			float red = light.color.r;
			float green = light.color.g;
			float blue = light.color.b;

			float input_red = red;
			float input_green = green;
			float input_blue = blue;

			ImGui::Text("   Color: ");
			if (ImGui::InputFloat("##RED", &input_red, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;
			if (ImGui::InputFloat("##GREEN", &input_green, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;
			if (ImGui::InputFloat("##BLUE", &input_blue, 0.0f, ImGuiInputTextFlags_EnterReturnsTrue))
				ismodif = true;

			if (ismodif) {
				light.SetColor(glm::vec4(input_red, input_green, input_blue, 1.0f));
			}
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
