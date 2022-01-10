/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/
#include <core/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>

#include "core/config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include <map>
#include <vector>


#include FT_FREETYPE_H
#include "graphic/texture.h"


namespace emp {
    /// Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };
	
    struct ColorRGB
    {
        float r;
        float g;
        float b;
        float a;

        ColorRGB()
        {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float a = 1.0f;
        }

        ColorRGB(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 1.0f;
        }

        ColorRGB(float r, float g, float b, float a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };
	
    struct Screen
    {
    public:
        int _horizontal = 0;
        int _vertical = 0;
        struct ColorRGB _backgroundColor;

        Screen()
        {
            //Get Info Screen Resolution
            GetDesktopWindowResolution(_horizontal, _vertical);
        }

        Screen(int& horizontal, int& vertical)
        {
            this->_horizontal = horizontal;
            this->_vertical = vertical;
        }

        void SetScreenResolution(int& horizontal, int& vertical)
        {
            this->_horizontal = horizontal;
            this->_vertical = vertical;
        }

        // Get the horizontal and vertical screen sizes in pixel
        void GetDesktopWindowResolution(int& horizontal, int& vertical)
        {
            RECT desktop;
            // Get a handle to the desktop window
            const HWND hDesktop = GetDesktopWindow();
            // Get the size of screen to the variable desktop
            GetWindowRect(hDesktop, &desktop);
            // The top left corner will have coordinates (0,0)
            // and the bottom right corner will have coordinates
            // (horizontal, vertical)
            horizontal = desktop.right;
            vertical = desktop.bottom;
        }
    };

 
 //   class Entity;
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

class SpriteRenderer
{
public:
    SpriteRenderer(int entity)
    {
        this->entity = entity;

    	this->vs = "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec3 aColor;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
    		"layout(location = 3) in vec2 aresolution;\n"

            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "uniform mat4 transform;"

            "void main()\n"
            "{\n"

            "gl_Position = transform * vec4(aPos, 1.0);\n"
            "ourColor = aColor;\n"
            "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
            "}\0";

        this->frag = "#version 330 core\n"
            "out vec4 FragColor;\n"

            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"

            // texture sampler
            "uniform sampler2D texture1;\n"

            "void main()\n"
            "{\n"
            
            "FragColor = texture2D(texture1, TexCoord);\n"
            "}\0";

        // set up vertex data (and buffer(s)) and configure vertex attributes
       // ------------------------------------------------------------------
        float vertices[] = {
            // positions            // colors           // texture coords
            1.0f,  1.0f, 0.0f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0, 0.0f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
        };

        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
    }
    	
    SpriteRenderer(int entity, string path)
    {
        this->entity = entity;
        this->path = path;

        this->vs = "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec3 aColor;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
            "layout(location = 3) in vec2 aresolution;\n"

            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "uniform mat4 transform;"

            "void main()\n"
            "{\n"

            "gl_Position = transform * vec4(aPos, 1.0);\n"
            "ourColor = aColor;\n"
            "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
            "}\0";

        this->frag = "#version 330 core\n"
            "out vec4 FragColor;\n"

            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"

            // texture sampler
            "uniform sampler2D texture1;\n"

            "void main()\n"
            "{\n"

            "FragColor = texture2D(texture1, TexCoord);\n"
            "}\0";

    }

    void Init();

    int entity;
    string path;
	
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

    const char* vs;
    const char* frag;
};

class SpriteManager : public System
{
	SpriteManager() : System()
	{
		
	}
	
	void Init() override;
	void Update(float) override;
    void Draw(Engine& engine);
	void Destroy() override;
};

    //class TextGraphic : public GraphicComponent
    //{
    //public:
    //    map<GLchar, Character> Characters;
    //    std::string text = "";
    //	
    //    TextGraphic(Entity& entity, string name = "TextGraphic");
    //    TextGraphic(Entity& entity, string text, string name = "TextGraphic");

    //    void Init() override;
    //    // render line of text
    //    void RenderText(unsigned int& shader, std::string text, float x, float y, float scale, glm::vec3 color);
    //    void Draw() override;
    //};
	
	class GraphicManager : public System
	{
	public:
        GraphicManager() : System()
        {
	        
        }
		
        void Init() override;
		void Init(Engine&, string, ConfigGraphic&);
		void Update(float) override;
		void Draw();

        void Swap();
		void Destroy() override;

		GLFWwindow* GetWindow();

	protected:
		GLFWwindow* window = nullptr;
		ConfigGraphic* config = nullptr;
        //std::vector<GraphicComponent*> components;
		Screen screen;
        SpriteManager* m_sprite;

		int width = 64;
		int height = 64;
	};
}
