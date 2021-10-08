#include "editor/tool.h"

#include <imgui_internal.h>
#include <engine/engine.h>
#include <GLFW/glfw3.h>

namespace emp
{
	ToolManager::ToolManager()
	{
		
	}
	float menuBarHeight;
	
	void ToolManager::Init(Engine& engine)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(engine.GetWindow(), true);
		ImGui_ImplOpenGL3_Init();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		tools.push_back(new sampleTool());
	
	}

	void ToolManager::Update(float)
	{
	}
	const float toolbarSize = 25;
	
	void ToolManager::Draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// TODO
		// Save off menu bar height for later.
		menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
		// TODO
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
		//ImGui::SetNextTreeNodeOpen(viewport->ID);

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
  
		ImGui::Button("TODO", ImVec2(0, 37));
  
		ImGui::End();

		ImGui::ShowDemoWindow();

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

	void ToolManager::Destroy()
	{
	}


	
	void sampleTool::Init()
	{
	}

	void sampleTool::Update(float)
	{
	}

	void sampleTool::Draw()
	{
		// render your GUI
		ImGui::Begin("Tool");
		ImGui::Button("Tool say Hello!");
		ImGui::End();

	}

	void sampleTool::Destroy()
	{
	}
}
