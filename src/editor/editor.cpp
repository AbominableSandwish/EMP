#include "editor/editor.h"
#include <imgui_internal.h>
#include <core/engine.h>

#include "tool/console.h"
#include "graphic/graphic.h"
#include "tool/hierarchy.h"
#include "core/entity.h"
#include "tool/inspector.h"
#include "tool/scene.h"
#include "tool/view.h"
#include "GL/glew.h"
#include "SDL.h"

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

		const char* glsl_version = "#version 330";
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		// 
		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL((m_engine->GetGraphicManager()->window), m_engine->GetGraphicManager()->window);
		//ImGui_ImplGlfw_InitForOpenGL((m_engine->GetGraphicManager()->window), true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Newtool(ToolType::HIERARCHY);
		Newtool(ToolType::INSPECTOR);
	}
	float deltaTime;
	float counterTime;
	bool refresh = true;
	int fps;
	void Editor::Update(float dt)
	{
		counterTime += dt;
		if(counterTime >= 0.25f){
			counterTime = 0.0f;
			refresh = true;
		}
	}

	int x = 0;
	int list_fps[100];
	
	void Editor::Draw()
	{

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_engine->GetGraphicManager()->window);
		ImGui::NewFrame();

		
		ImGuiWindowFlags window_flags = 0
			//| ImGuiWindowFlags_NoDocking 
			| ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoBackground;

		ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);
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
				if (ImGui::MenuItem("Scene"))
				{
					Newtool(ToolType::SCENE);
				}
				if (ImGui::MenuItem("View"))
				{
					Newtool(ToolType::VIEW);
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

		if (refresh) {

			fps = total / 60;
			refresh = false;
		}

		// render your GUI
		ImGui::Begin("Editor");
		string average = "FPS : " + to_string(fps);
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
		ImGuiIO& io = ImGui::GetIO();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		m_engine->GetGraphicManager()->Swap();
	}

	void Editor::Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		ImGui_ImplSDL2_Shutdown();
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
		case ToolType::SCENE:

			tool = new Scene(*(this->m_engine), "Scene");
			tool->Init();
			break;
		case ToolType::VIEW:

			tool = new View(*(this->m_engine), "View");
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
