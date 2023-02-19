#include "tool/scene.h"
#include <iostream>
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;

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

void Scene::SaveScene() {
	// read file
	auto json = json::parse("{\"Transform\": \"string\"}");
	// mutate the json
	json["Transform"] = "new string";

	// write to a stream, or the same file
	std::cout << json; // print the json
}
	