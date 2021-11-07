#include "editor/editor.h"
#include <imgui_internal.h>
#include <engine/engine.h>

#include "tool/log_tool.h"

namespace emp
{
	Editor::Editor(Engine& engine)
	{
		this->m_engine = &engine;
	}

	float menuBarHeight;

	void Editor::Init()
	{
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
			| ImGuiWindowFlags_NoSavedSettings
			;

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

			if (ImGui::BeginMenu("Tool"))
			{
				if (ImGui::MenuItem("Profiler"))
				{
					Newtool(ToolType::PROFILER);
				}
				if (ImGui::MenuItem("Logger"))
				{
					Newtool(ToolType::LOGGER);
				}
				if (ImGui::MenuItem("Sample"))
				{
					Newtool(ToolType::SAMPLE);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		ImGui::End();

		// render your GUI
		ImGui::Begin("Editor");
		ImGui::Button("Mushroom Editor say Hello!");
		ImGui::End();


		for (Tool* tool : tools)
		{
			tool->Draw();
		}

		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Editor::Destroy()
	{
	}

	void Editor::Newtool(ToolType type)
	{
		Tool* tool = nullptr;
		switch (type)
		{
		case ToolType::SAMPLE:
			tool = new sampleTool(*(this->m_engine), "Sample");
			break;
		case ToolType::LOGGER:
			tool = new Logger(*(this->m_engine), "Logger");
			tool->Init();
			break;
		}
		if (tool != nullptr)
			tools.push_back(tool);
	}
}
