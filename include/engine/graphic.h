/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/

#include <engine/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace emp {
	class GraphicManager : public System<GraphicManager>
	{
	public:
		void Init() override;
		void Update(float) override;
		void Draw();
		void Destroy() override;

		GLFWwindow* GetWindow();

	protected:
		GLFWwindow* window = nullptr;
	};

	template<typename T>
	class Element
	{
	public:
		virtual void Init() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;
	protected:
		virtual ~Element() {};
		string name;
	};
}
