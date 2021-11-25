#include <imgui_internal.h>
#include <tool/log_tool.h>
#include <core/engine.h>
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
		ImGui::Begin("Logger", &is_open, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (collapse == nullptr)
				collapse = new bool;
			if (ImGui::Checkbox("Collapse: ", collapse))
			{
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				this->m_engine->GetLogManager()->ClearLogs();
			}
			ImGui::SameLine();
			string text = "Logs: " + to_string(this->m_engine->GetLogManager()->GetLogs().size());
			ImGui::Text(text.c_str());

			ImGui::EndMenuBar();
		}

		std::vector<Log>* logs = &(this->m_engine->GetLogManager()->GetLogs());
		ImVec2 canvas = ImGui::GetWindowSize();
		canvas.y -= HEADER_HEIGHT;
		int max_line = canvas.y / 16;
		
		ImGui::SetCursorPos(ImVec2(10, HEADER_HEIGHT + CHARACTER_SIZE * logs->size()));
		ImGui::Text("");
		int scrollbar_y = ImGui::GetScrollY();
		

		int id = scrollbar_y / 16;
		
		for (int line = 0; line < max_line; line++)
		{
			if (id >= scrollbar_y / CHARACTER_SIZE && id <= scrollbar_y / CHARACTER_SIZE + max_line + 1) {
				string type;
				ImVec4 color;
				if (id + line < logs->size()) {
					logs;
					switch ((*logs)[id + line].type)
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
					string msg = " " + (*logs)[id + line].msg + "\n";

					ImGui::SetCursorPos(ImVec2(10, HEADER_HEIGHT + (16 * line) + scrollbar_y - 1));
					ImGui::Text("[ LOG ]");
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::Text(type.c_str());
					ImGui::PopStyleColor();
					ImGui::SameLine();
					ImGui::Text(msg.c_str());
				}
			}
			
			if (*collapse)
			{
				ImGui::SetScrollHereY(0.5);
			}
		}

		//Get Resolution Window
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiID active_id = ImGui::GetActiveID();

		if (!active_id && (active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_X) || active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y)))
		{
			LOG::Critical("CLICK Scrollbar");
		}
		else
		{
			
		}

		ImGui::End();
	}

	void Logger::Destroy()
	{
	}
}
