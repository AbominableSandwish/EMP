#include "editor/editor.h"
#include <imgui_internal.h>
#include <core/engine.h>

#include "tool/console.h"
#include "graphic/graphic.h"
#include "tool/hierarchy.h"
#include "core/entity.h"

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
		ImGui_ImplGlfw_InitForOpenGL(m_engine->GetWindow(), true);
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
					Entity& entity = this->m_engine->GetEntityManager()->CreateEntity();
					LOG::Info("New Spore { name: " + entity.name + " }");
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
				if (ImGui::MenuItem("Empty"))
				{
					Newtool(ToolType::EMPTY);
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
		int average = 0;
		for (int element : list_fps)
		{
			average += element;
		}
		average / 100;
		// render your GUI
		ImGui::Begin("Editor");
		string text = "FPS : " + to_string(average/60);
		ImGui::Text(text.c_str());
		
		if(ImGui::Button("Mushroom Editor say Hello!"))
		{
			ImGui::Text("Hello...");
		}
		ImGui::End();

		for (Tool* tool : tools)
		{
			tool->Draw();
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
		}
		if (tool != nullptr)
			tools.push_back(tool);
	}
}