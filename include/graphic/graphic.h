/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/
#include <core/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>



#include "stb_image.h"
#include "core/config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H  
#include "core/component.h"
#include "common/transform.h"


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

	class GraphicComponent : public Component
	{
	public :
        Transform* transform;
        
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;
        unsigned int texture;

        const char* vs;
        const char* frag;

        string path;
		
        GraphicComponent()
		{
		
		}
		
        GraphicComponent(string path, Transform& transform)
        {
            this->path = path;
            this->transform = &transform;
        }

        virtual void Init() = 0;
        virtual void Draw() = 0;

	};

    class SpriteGraphic : public GraphicComponent
    {
    public:
        float speedRotate = 1.0f;

        SpriteGraphic() : GraphicComponent()
        {

        }

        SpriteGraphic(string path, Transform& transform) : GraphicComponent(path, transform)
        {

        }

        void Init() override;


        void Draw() override
        {
            // bind Texture
            glBindTexture(GL_TEXTURE_2D, texture);

            // create transformations
            glm::mat4 transf = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            transf = glm::translate(transf, glm::vec3(this->transform->x, this->transform->y, 0.0f));
            transf = glm::rotate(transf, (float)glfwGetTime() / speedRotate, glm::vec3(0.0f, 0.0f, 1.0));




            //render container
            glUseProgram(shaderProgram);

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

    };

    class TextGraphic : public GraphicComponent
    {
    public:
        map<GLchar, Character> Characters;

    	
        TextGraphic() : GraphicComponent()
        {

        }

        TextGraphic(string path, Transform& transform) : GraphicComponent(path, transform)
        {

        }

        void Init() override;

        // render line of text
        void RenderText(unsigned int& shader, std::string text, float x, float y, float scale, glm::vec3 color)
        {
            // activate corresponding render state	
            glUseProgram(shader);
            glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);

            // iterate through all characters
            std::string::const_iterator c;
            for (c = text.begin(); c != text.end(); c++)
            {
                Character ch = Characters[*c];

                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;
                // update VBO for each character
                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    	void Draw() override
        {

            RenderText(shaderProgram, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
            RenderText(shaderProgram, "(C) AbominableScience.com", 1600.0f, 25.0f, 0.5f, glm::vec3(0.8, 0.0f, 0.9f));
        }

    };
	
	class GraphicManager : public System
	{
	public:
		GraphicManager(Engine&, string, ConfigGraphic&);
		GraphicManager(Engine&, string);
		void Init() override;
		void Update(float) override;
		void Draw();

        void Swap();
		void Destroy() override;

		GLFWwindow* GetWindow();

	protected:
		GLFWwindow* window = nullptr;
		ConfigGraphic* config = nullptr;

		Screen screen;

		int width = 64;
		int height = 64;

	};
}
