#include "editor/tool.h"

namespace emp
{
	void sampleTool::Init()
	{
	}

	sampleTool::sampleTool(Engine& engine, std::string name): Tool(engine, name)
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
