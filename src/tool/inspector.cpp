#include <tool/inspector.h>
#include "core/log.h"
#include <core/engine.h>
#include <core/entity.h>
#include "graphic/graphic.h"
#include <core/component.h>

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
			
			ImGui::Text("   Position:");
			ImGui::Text("\tX : ");
			ImGui::SameLine();
			float f_x = tranform.x;
			ImGui::InputFloat("##X", &f_x, 0.5f, 1.0f, "%.3f");
			tranform.SetPosition(f_x, tranform.y);

			ImGui::Text("\tY : ");
			ImGui::SameLine();
			float f_y = tranform.y;
			ImGui::InputFloat("##Y", &f_y, 0.5f, 1.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			{
				emp::LOG::Debug("Setting attributes Spore");
				tranform.SetPosition(tranform.x, f_y);
			}

			ImGui::Text("   Scale:");
			ImGui::Text("\tX : ");
			ImGui::SameLine();
			float s_x = tranform.scale_x;
			ImGui::InputFloat("##sX", &s_x, 0.5f, 1.0f, "%.3f");
			tranform.SetScale(s_x, tranform.scale_y);

			ImGui::Text("\tY : ");
			ImGui::SameLine();
			float s_y = tranform.scale_y;
			ImGui::InputFloat("##sY", &s_y, 0.5f, 1.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
			{
				emp::LOG::Debug("Setting attributes Spore");
				tranform.SetScale(tranform.scale_x, s_y);
			}

			//emp::TextGraphic& textGraphic = static_cast<emp::TextGraphic&>(*component);
			//int bufSize_text = 256;
			//char buffer_text[256] = {};
			//int c = 0;
			//std::string text = textGraphic.text;
			//for (auto character : text)
			//{
			//	buffer_text[c] = character;
			//	c++;
			//}
			//ImGui::Text("\tText: ");
			//ImGui::SameLine();
			//if (ImGui::InputText("##text", buffer_text, bufSize_text, ImGuiInputTextFlags_EnterReturnsTrue))
			//{
			//	emp::LOG::Debug("Setting attributes Spore");
			//	textGraphic.text = buffer_text;
			//}
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
