/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/
#include <engine/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>



#include "stb_image.h"
#include "engine/config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H  


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

	class Component
	{
		
	};

	struct Transform
	{
	public:
		Transform(int x, int y)
		{
            this->x = x;
            this->y = y;
		}
        int x, y;
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
		
		void Init() override
		{
            this->vs = "#version 330 core\n"
                "layout(location = 0) in vec3 aPos;\n"
                "layout(location = 1) in vec3 aColor;\n"
                "layout(location = 2) in vec2 aTexCoord;\n"

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
                "FragColor = texture(texture1, TexCoord);\n"
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
            // build and compile our shader program
          // ------------------------------------
          // vertex shader
            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vs, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
            // fragment shader
            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &frag, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);




            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // load and create a texture 
            // -------------------------
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

            // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
            unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }

            stbi_image_free(data);

            srand(time(NULL));

            speedRotate += rand() % 20;

            speedRotate *= 2.0f;
		}

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

    	void Init() override
        {
            // OpenGL state
			// ------------
            //glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            const char* vShaderCode = "#version 330 core\n"
            "layout(location = 0) in vec4 vertex; \n"
            "out vec2 TexCoords;\n"

            "uniform mat4 projection;\n"

            "void main()\n"
           "{\n"
                "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
                "TexCoords = vertex.zw;\n"
            "}\0";
        	
            const char* fShaderCode = "#version 330 core\n"
                "in vec2 TexCoords;\n"
            "out vec4 color;\n"

            "uniform sampler2D text;\n"
            "uniform vec3 textColor;\n"

            "void main()\n"
            "{\n"
                "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
                "color = vec4(textColor, 1.0) * sampled;\n"
            "}\0";

            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            // utility function for checking shader compilation/linking errors.
			// ------------------------------------------------------------------------
                GLint success;
                GLchar infoLog[1024];
                if ("VERTEX" != "PROGRAM")
                {
                    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
                    if (!success)
                    {
                        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
                        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                    }
                }
                else
                {
                    glGetProgramiv(vertex, GL_LINK_STATUS, &success);
                    if (!success)
                    {
                        glGetProgramInfoLog(vertex, 1024, NULL, infoLog);
                        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                    }
                }
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            // utility function for checking shader compilation/linking errors.
			// ------------------------------------------------------------------------
            success;
            infoLog[1024];
            if ("FRAGMENT" != "PROGRAM")
            {
                glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(fragment, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(fragment, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            // shader Program
            this->shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertex);
            glAttachShader(shaderProgram, fragment);
            glLinkProgram(shaderProgram);
            success;
            infoLog[1024];
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            // delete the shaders as they're linked into our program now and no longer necessery
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        	
            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1920), 0.0f, static_cast<float>(1080-30-40));
            glUseProgram(shaderProgram);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // FreeType
            // --------
            FT_Library ft;
            // All functions return a value different than 0 whenever an error occurred
            if (FT_Init_FreeType(&ft))
            {
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                return;
            }

            // find path to font
            std::string font_name = "./data/fonts/Inconsolata.ttf";
            if (font_name.empty())
            {
                std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
                return ;
            }

            // load font as face
            FT_Face face;
            if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                return;
            }
            else {
                // set size to load glyphs as
                FT_Set_Pixel_Sizes(face, 0, 48);

                // disable byte-alignment restriction
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                // load first 128 characters of ASCII set
                for (unsigned char c = 0; c < 128; c++)
                {
                    // Load character glyph 
                    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    {
                        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                        continue;
                    }
                    // generate texture
                    unsigned int texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                    );
                    // set texture options
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    // now store character for later use
                    Character character = {
                        texture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x)
                    };
                    Characters.insert(std::pair<char, Character>(c, character));
                }
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            // destroy FreeType once we're finished
            FT_Done_Face(face);
            FT_Done_FreeType(ft);


            // configure VAO/VBO for texture quads
            // -----------------------------------
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        // render line of text
		// -------------------
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
