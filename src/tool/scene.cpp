#include "tool/scene.h"

namespace emp
{
}
void Scene::Init()
{
}

Scene::Scene(emp::Engine& engine, std::string name) : Tool(engine, name)
{
}

void Scene::Update(float)
{
}

void Scene::Draw()
{
	// render your GUI
	ImGui::Begin("Scene");
	ImGui::Button("Tool say Hello!");
	ImGui::End();
}

void Scene::Destroy()
{
}
