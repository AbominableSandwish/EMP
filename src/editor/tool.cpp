#include "editor/tool.h"

namespace emp
{
	void EmptyTool::Init()
	{
	}

	EmptyTool::EmptyTool(Engine& engine, std::string name): Tool(engine, name)
	{
	}

	void EmptyTool::Update(float)
	{
	}

	void EmptyTool::Draw()
	{
		// render your GUI
		ImGui::Begin("Tool");
		ImGui::Button("Tool say Hello!");
		ImGui::End();
	}

	void EmptyTool::Destroy()
	{
	}
}
