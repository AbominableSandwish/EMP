/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/

#include <engine/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


namespace mushgine {
	class GraphicManager : public mushgine::System<GraphicManager>
	{
	public:
		void Init() override;
		void Update(float) override;
		void Destroy() override;

	protected:
		GLFWwindow* window;
	};
}
