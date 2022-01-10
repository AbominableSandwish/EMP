#include <graphic/graphic.h>

#define STB_IMAGE_IMPLEMENTATION
#include "graphic/stb_image.h"
#include "wtypes.h"
#include <iostream>
#include "core/log.h"
#include "core/engine.h"
#include <core/entity.h>
#include <core/component.h>

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
                LOG::Error(msg);
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
                LOG::Error(msg);
            }
        }

        void static ChechShaderCompile(unsigned int& shaderProgram)
        {
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
                LOG::Error(msg);
            }
        }
    }
    //SpriteGraphic::SpriteGraphic(Entity& entity) : GraphicComponent(entity)
    //{
    //	
    //}

    //SpriteGraphic::SpriteGraphic(Entity& entity, string path) : GraphicComponent(path, entity)
    //{
    //}

    void SpriteManager::Init() {  
       
    }

    void SpriteManager::Draw(Engine& engine)
    {
        auto components =  engine.GetComponentManager()->GetComponents<SpriteRenderer>();
        for (auto element : components)
        {

            // bind Texture
            glBindTexture(GL_TEXTURE_2D, element.texture);
            auto position = engine.GetComponentManager()->GetComponent<Transform>(element.entity);
            glm::mat4 transf = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            transf = glm::translate(transf, glm::vec3(position.x, position.y, 0.0f));
            // transf = glm::rotate(transf, (float)glfwGetTime() / speedRotate, glm::vec3(0.0f, 0.0f, 1.0));


             //render container
            glUseProgram(element.shaderProgram);

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));

            glBindVertexArray(element.VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        }
    }


    //TextGraphic::TextGraphic(Entity& entity, string name) : GraphicComponent(entity, name)
    //{
    //}

    //TextGraphic::TextGraphic(Entity& entity, string text, string name) : GraphicComponent(entity, name)
    //{
    //    this->text = text;
    //    entity.AddComponent(this);
    //}

    //void TextGraphic::Init() {
    //    // OpenGL state
    //        // ------------
    //        //glEnable(GL_CULL_FACE);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //    const char* vShaderCode = "#version 330 core\n"
    //        "layout(location = 0) in vec4 vertex; \n"
    //        "out vec2 TexCoords;\n"

    //        "uniform mat4 projection;\n"

    //        "void main()\n"
    //        "{\n"
    //        "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    //        "TexCoords = vertex.zw;\n"
    //        "}\0";

    //    const char* fShaderCode = "#version 330 core\n"
    //        "in vec2 TexCoords;\n"
    //        "out vec4 color;\n"

    //        "uniform sampler2D text;\n"
    //        "uniform vec3 textColor;\n"

    //        "void main()\n"
    //        "{\n"
    //        "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    //        "color = vec4(textColor, 1.0) * sampled;\n"
    //        "}\0";

    //    // 2. compile shaders
    //    unsigned int vertex, fragment;
    //    // vertex shader
    //    vertex = glCreateShader(GL_VERTEX_SHADER);
    //    glShaderSource(vertex, 1, &vShaderCode, NULL);
    //    glCompileShader(vertex);
    //    // utility function for checking shader compilation/linking errors.
    //    // ------------------------------------------------------------------------
    //    GLint success;
    //    GLchar infoLog[1024];
    //    if ("VERTEX" != "PROGRAM")
    //    {
    //        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    //        if (!success)
    //        {
    //            glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
    //            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    //        }
    //    }
    //    else
    //    {
    //        glGetProgramiv(vertex, GL_LINK_STATUS, &success);
    //        if (!success)
    //        {
    //            glGetProgramInfoLog(vertex, 1024, NULL, infoLog);
    //            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    //        }
    //    }
    //    // fragment Shader
    //    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    //    glShaderSource(fragment, 1, &fShaderCode, NULL);
    //    glCompileShader(fragment);
    //    // utility function for checking shader compilation/linking errors.
    //    // ------------------------------------------------------------------------
    //    success;
    //    infoLog[1024];
    //    if ("FRAGMENT" != "PROGRAM")
    //    {
    //        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    //        if (!success)
    //        {
    //            glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
    //            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    //        }
    //    }
    //    else
    //    {
    //        glGetProgramiv(fragment, GL_LINK_STATUS, &success);
    //        if (!success)
    //        {
    //            glGetProgramInfoLog(fragment, 1024, NULL, infoLog);
    //            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    //        }
    //    }
    //    // shader Program
    //    this->shaderProgram = glCreateProgram();
    //    glAttachShader(shaderProgram, vertex);
    //    glAttachShader(shaderProgram, fragment);
    //    glLinkProgram(shaderProgram);
    //    success;
    //    infoLog[1024];
    //    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //    if (!success)
    //    {
    //        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
    //        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    //    }
    //    // delete the shaders as they're linked into our program now and no longer necessery
    //    glDeleteShader(vertex);
    //    glDeleteShader(fragment);

    //    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1920), 0.0f, static_cast<float>(1080 - 30 - 40));
    //    glUseProgram(shaderProgram);
    //    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //    // FreeType
    //    // --------
    //    FT_Library ft;
    //    // All functions return a value different than 0 whenever an error occurred
    //    if (FT_Init_FreeType(&ft))
    //    {
    //        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    //        return;
    //    }

    //    // find path to font
    //    std::string font_name = "./data/fonts/Inconsolata.ttf";
    //    if (font_name.empty())
    //    {
    //        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
    //        return;
    //    }

    //    // load font as face
    //    FT_Face face;
    //    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
    //        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    //        return;
    //    }
    //    else {
    //        // set size to load glyphs as
    //        FT_Set_Pixel_Sizes(face, 0, 48);

    //        // disable byte-alignment restriction
    //        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //        // load first 128 characters of ASCII set
    //        for (unsigned char c = 0; c < 128; c++)
    //        {
    //            // Load character glyph 
    //            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    //            {
    //                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
    //                continue;
    //            }
    //            // generate texture
    //            unsigned int texture;
    //            glGenTextures(1, &texture);
    //            glBindTexture(GL_TEXTURE_2D, texture);
    //            glTexImage2D(
    //                GL_TEXTURE_2D,
    //                0,
    //                GL_RED,
    //                face->glyph->bitmap.width,
    //                face->glyph->bitmap.rows,
    //                0,
    //                GL_RED,
    //                GL_UNSIGNED_BYTE,
    //                face->glyph->bitmap.buffer
    //            );
    //            // set texture options
    //            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //            // now store character for later use
    //            Character character = {
    //                texture,
    //                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    //                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    //                static_cast<unsigned int>(face->glyph->advance.x)
    //            };
    //            Characters.insert(std::pair<char, Character>(c, character));
    //        }
    //        glBindTexture(GL_TEXTURE_2D, 0);
    //    }
    //    // destroy FreeType once we're finished
    //    FT_Done_Face(face);
    //    FT_Done_FreeType(ft);


    //    // configure VAO/VBO for texture quads
    //    // -----------------------------------
    //    glGenVertexArrays(1, &VAO);
    //    glGenBuffers(1, &VBO);
    //    glBindVertexArray(VAO);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    //    glEnableVertexAttribArray(0);
    //    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindVertexArray(0);
    //}

    //void TextGraphic::RenderText(unsigned& shader, std::string text, float x, float y, float scale, glm::vec3 color)
    //{
    //    // activate corresponding render state	
    //    glUseProgram(shader);
    //    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    //    glActiveTexture(GL_TEXTURE0);
    //    glBindVertexArray(VAO);

    //    // iterate through all characters
    //    std::string::const_iterator c;
    //    for (c = text.begin(); c != text.end(); c++)
    //    {
    //        Character ch = Characters[*c];

    //        float xpos = x + ch.Bearing.x * scale;
    //        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    //        float w = ch.Size.x * scale;
    //        float h = ch.Size.y * scale;
    //        // update VBO for each character
    //        float vertices[6][4] = {
    //            { xpos,     ypos + h,   0.0f, 0.0f },
    //            { xpos,     ypos,       0.0f, 1.0f },
    //            { xpos + w, ypos,       1.0f, 1.0f },

    //            { xpos,     ypos + h,   0.0f, 0.0f },
    //            { xpos + w, ypos,       1.0f, 1.0f },
    //            { xpos + w, ypos + h,   1.0f, 0.0f }
    //        };
    //        // render glyph texture over quad
    //        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //        // update content of VBO memory
    //        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

    //        glBindBuffer(GL_ARRAY_BUFFER, 0);
    //        // render quad
    //        glDrawArrays(GL_TRIANGLES, 0, 6);
    //        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    //        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    //    }
    //    glBindVertexArray(0);
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //}

    //void TextGraphic::Draw()
    //{
    //    emp::Transform transform;
    //    // create transformations
    //    for (emp::Component* component : entity->components)
    //    {
    //        if (component->name == "Transform") {
    //            transform = static_cast<emp::Transform&>(*component);
    //        }
    //    }
    //	
    //    RenderText(shaderProgram, text, transform.x * 100, transform.y * 100, 1.0f, glm::vec3(0.8, 0.0f, 0.9f));
    //}

    void GraphicManager::Init()
    {
    }

    void GraphicManager::Init(Engine& engine, string name, ConfigGraphic& config)
    {
        this->engine = &engine;
        this->name = name;
        this->config = &config;
        //Get Info Screen Resolution
        int horizontal = 0;
        int vertical = 0;
        
        //Init Window GLFW
        glfwInit();
        int positionX = 0;
        int positionY = 0;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        this->window = glfwCreateWindow(this->config->width, this->config->height, "Engine Mushroom Portuaire", NULL, NULL);

        this->screen._backgroundColor = ColorRGB(0.14f, 0.14f, 0.14f);
    	
        glfwSetWindowAttrib(window, GLFW_DECORATED, this->config->decorated);
        glfwSetWindowPos(this->window, this->config->x, this->config->y);
        glfwSetWindowSize(this->window, this->config->window_width, this->config->window_height);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, this->config->transparent);

    	
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(this->window);
        GLenum err = glewInit();
    }
    
    float timer = 0.0f;
    void GraphicManager::Update(float dt)
    {
    	timer += dt;
    	if (glfwWindowShouldClose(this->window))
    	{
            engine->Stop();
    	}
    	
    	glfwPollEvents();
    }

    void GraphicManager::Draw()
    {
        //glClearColor(screen._backgroundColor.r, screen._backgroundColor.g, screen._backgroundColor.b, screen._backgroundColor.a);
        //glClear(GL_COLOR_BUFFER_BIT);

       // uncomment this call to draw in wireframe polygons.
       //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glfwPollEvents();
    }


       void GraphicManager::Swap()
       {
           // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
       	// -------------------------------------------------------------------------------
           glfwSwapBuffers(window);
       }


       void GraphicManager::Destroy()
       {
       	glfwTerminate();
       }

       GLFWwindow* GraphicManager::GetWindow()
       {
       	return this->window;
       }

    //   enum Type
       //{
       //	JPG,
       //	PNG,
       //	JPEG,
       //	ICO
       //};

    //   struct Sprite
    //   {
    //       int id;
    //       string name;
    //       //Texture texture;

    //       int x, y;
    //       int size_x, size_y;
    //   };
       //
    //   GraphicComponent::GraphicComponent(Entity& entity, string name)
    //   {
    //       this->entity = &entity;
    //   }

    //   GraphicComponent::GraphicComponent(string path, Entity& entity, string name)
    //   {
    //       this->path = path;
    //       this->entity = &entity;
    //   }

       void SpriteRenderer::Init()
       {
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
           Shader::CheckVertexCompile(vertexShader);
           // fragment shader
           unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
           glShaderSource(fragmentShader, 1, &frag, NULL);
           glCompileShader(fragmentShader);
           Shader::CheckFragmentCompile(fragmentShader);

           // link shaders
           shaderProgram = glCreateProgram();
           glAttachShader(shaderProgram, vertexShader);
           glAttachShader(shaderProgram, fragmentShader);
           glLinkProgram(shaderProgram);
           Shader::ChechShaderCompile(shaderProgram);
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

           stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

           // set the resolution
           //uniform2f(resolutionLocation, 128, 128);

           // load image, create texture and generate mipmaps
           int width, height, level;
           unsigned char* file = stbi_load(path.c_str(), &width, &height, &level, 0);
           Texture::loadTexture(file, width, height, texture, path.c_str(), level);
           glGenerateMipmap(GL_TEXTURE_2D);
           stbi_image_free(file);
       }

}
