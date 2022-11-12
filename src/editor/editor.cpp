#include "editor/editor.h"
#include <imgui_internal.h>
#include <core/engine.h>

#include "tool/console.h"
#include "graphic/graphic.h"
#include "tool/hierarchy.h"
#include "core/entity.h"
#include "tool/inspector.h"

namespace emp
{
	Editor::Editor(Engine& engine)
	{
		this->m_engine = &engine;
	}

	float menuBarHeight;

	void Editor::Init()
	{
		graphic = m_engine->GetGraphicManager();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL((m_engine->GetGraphicManager()->window), true);
		ImGui_ImplOpenGL3_Init();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}

	void Editor::Update(float)
	{
	}

	int x = 0;
	int list_fps[100];
	
	void Editor::Draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0
			//| ImGuiWindowFlags_NoDocking 
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoSavedSettings;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::Begin("TOOLBAR", NULL, window_flags);
		ImGui::PopStyleVar();
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					//Do something
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Spore"))
			{
				if (ImGui::MenuItem("New Spore"))
				{
					//auto entity = this->m_engine->GetEntityManager()->CreateEntity();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tool"))
			{
				if (ImGui::MenuItem("Hierarchy"))
				{
					Newtool(ToolType::HIERARCHY);
				}
				if (ImGui::MenuItem("Profiler"))
				{
					Newtool(ToolType::PROFILER);
				}
				if (ImGui::MenuItem("Console"))
				{
					Newtool(ToolType::CONSOLE);
				}
				if (ImGui::MenuItem("Inspector"))
				{
					Newtool(ToolType::INSPECTOR);
				}
				if (ImGui::MenuItem("Demo"))
				{
					Newtool(ToolType::EMPTY);
				}
				ImGui::EndMenu();
			}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Classic View"))
					{
						this->m_engine->GetGraphicManager()->WireframeView = false;
					}
					if (ImGui::MenuItem("Wireframe View"))
					{
						this->m_engine->GetGraphicManager()->WireframeView = true;
					}
					ImGui::EndMenu();
				}
			
			ImGui::EndMainMenuBar();
		}
		ImGui::End();

		graphic->Draw();

		list_fps[x] = (int)this->m_engine->GetFPS();
		x++;
		if (x > 100)
			x = 0;
		int total = 0;
		for (int element : list_fps)
		{
			total += element;
		}
		total / 100;
		// render your GUI
		ImGui::Begin("Editor");
		string average = "FPS : " + to_string(total /60);
		ImGui::Text(average.c_str());
		ImGui::End();

		bool to_remove = false;
		int i = 0;
		int it;
		if (this->tools.size() != 0) {
			for (Tool* tool : tools)
			{
				if (tool->is_open) {
					// render your GUI
					ImGui::Begin(tool->name.c_str(), &(tool->is_open), ImGuiWindowFlags_MenuBar);
					tool->Draw();
					ImGui::End();

				}
				else
				{
					to_remove = true;
					tool = nullptr;
					it = i;
				}

				i++;
			}
			if (to_remove)
				tools.erase(tools.begin() + it);
		}

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		graphic->Swap();
	}

	void Editor::Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::Newtool(ToolType type)
	{
		Tool* tool = nullptr;
		switch (type)
		{
		case ToolType::EMPTY:
			tool = new EmptyTool(*(this->m_engine), "Empty");
			break;
		case ToolType::CONSOLE:
			tool = new Console(*(this->m_engine), "Console");
			tool->Init();
			break;
		case ToolType::HIERARCHY:
			tool = new Hierarchy(*(this->m_engine), "Hierarchy");
			tool->Init();
			break;
		case ToolType::INSPECTOR:

			tool = new Inspector(*(this->m_engine), "Inspector");
			tool->Init();
			break;
		}
		if (tool != nullptr) {
			bool is_exist = false;
			for (Tool* element : tools)
			{
				if(element->name == tool->name)
				{
					is_exist = true;
				}
			}
			if(!is_exist)
				tools.push_back(tool);
		}
	}
}
