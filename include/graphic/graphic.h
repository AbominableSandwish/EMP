/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/


#include <core/system.h>
#include <graphic/screen.h>
#include <glm/gtc/type_ptr.hpp>


	class GLFWwindow;
namespace emp {
	
	class SpriteManager;
	class ComponentManager;
	class SquareManager;
	class CircleManager;
	class TriangleManager;
	class CubeManager;
	class LightManager;
	class ModelManager;


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

		//GLFWwindow& GetWindow();
		GLFWwindow* window = nullptr;
	protected:
		
		ConfigGraphic* config = nullptr;
        //std::vector<GraphicComponent*> components;
		Screen screen;

        SpriteManager* m_sprite;
		SquareManager* m_square;
		CircleManager* m_circle;
		TriangleManager* m_triangle;
		CubeManager* m_cube;
		LightManager* m_light;
		ModelManager* m_model;

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