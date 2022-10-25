/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <core/system.h>
#include <graphic/screen.h>
#include <glm/gtc/type_ptr.hpp>

namespace emp {
	namespace Shader
	{
        void static CheckVertexCompile(unsigned int& vertexShader) {
			// check for shader compile errors
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				string msg = "[SHADER] VERTEX::COMPILATION_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}

        void static CheckFragmentCompile(unsigned int& fragmentShader) {
			// check for shader compile errorsg
			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				string msg = "[SHADER] FRAGMENT::COMPILATION_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}

        void static ChechShaderCompile(unsigned int& shaderProgram) {
			// check for shader compile errorsg
			int success;
			char infoLog[512];
			// check for linking errors
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				string msg = "[SHADER] PROGRAM::LINKING_FAILED\n";
				for (char element : msg)
				{
					msg += element;
				}

				msg += "\n";
				//LOG::Error(msg);
			}
		}
	}
	
	class SpriteManager;
	class ComponentManager;
	class SquareManager;
	class CircleManager;
	class TriangleManager;
	class CubeManager;
	class LightManager;

	/// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

	class GraphicManager : public System
	{
	public:
		bool WireframeView = false;
        GraphicManager(Engine& engine, ConfigGraphic& config);
		
        void Init() override;
		void Update(float) override;
		void Draw();

        void Swap();
		void Destroy() override;

		template<typename T>
		void AddGraphic()
		{
			const char* typeName = typeid(T).name();

			if (typeName == "Renderer2D")
			{

			}
			if (typeName == "Square")
			{

			}
			if (typeName == "Circle")
			{

			}
			if (typeName == "Line")
			{

			}
			if (typeName == "Sprite")
			{

			}
		}

		void AddGraphic(int entity);
		void RemoveGraphic();

		GLFWwindow* GetWindow();

	protected:
		GLFWwindow* window = nullptr;
		ConfigGraphic* config = nullptr;
        //std::vector<GraphicComponent*> components;
		Screen screen;

        SpriteManager* m_sprite;
		SquareManager* m_square;
		CircleManager* m_circle;
		TriangleManager* m_triangle;
		CubeManager* m_cube;
		LightManager* m_light;

		int width = 64;
		int height = 64;
	};
}

   //class Entity;
   //class GraphicComponent
   //{
   //public :
//       Entity* entity;
//       
//       unsigned int shaderProgram;
//       unsigned int VBO, VAO, EBO;
//       unsigned int texture;
//       const char* vs;
//       const char* frag;

//       string path;
   //	
//       GraphicComponent(Entity& entity, string name = "");
//       GraphicComponent(string path, Entity& entity, string name = "");

//       virtual void Init() = 0;
//       virtual void Draw() = 0;
   //};