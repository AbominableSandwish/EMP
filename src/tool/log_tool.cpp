#include <tool/log_tool.h>
#include <engine/engine.h>
namespace emp
{
	
	void Logger::Init()
	{
	}

	Logger::Logger(Engine& engine, string name) : Tool(engine, name)
	{
	}

	void Logger::Update(float)
	{
	}

	void Logger::Draw()
	{
		ImGui::Begin("Logger", &is_open , ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if(ImGui::Button("Clear"))
			{
				
			}
		
			ImGui::EndMenuBar();
		}
		
		for (Log log : this->m_engine->GetLogManager()->GetLogs())
		{
			string type;
			ImVec4 color;
			switch (log.type)
			{
			case LogType::NONE:
				type = "[ NONE ]";
				break;
			case LogType::DEBUG:
				type = "[ DEBUG ] ";
				color = ImVec4(0, 1, 1, 1);
				break;
			case LogType::INFO:
				type = "[ INFO ]  ";
				color = ImVec4(1, 1, 1, 1);
				break;
			case LogType::WARNING:
				type = "[WARNING] ";
				color = ImVec4(1, 1, 0, 1);
				break;
			case LogType::ERROOR:
				type = "[ ERROR ] ";
				color = ImVec4(1, 0, 0, 1);
				break;
			case LogType::CRITICAL:
				type = "[CRITICAL]";
				color = ImVec4(1, 0, 1, 1);
				break;
			}
			string msg = " " + log.msg + "\n";
			
			
			ImGui::Text("[ TIME ]");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Text(type.c_str());
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text(msg.c_str());

		}
		ImGui::Text("Log");
		ImGui::End();
	}

	void Logger::Destroy()
	{
	}
}
