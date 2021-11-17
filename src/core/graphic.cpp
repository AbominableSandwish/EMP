#include <graphic/graphic.h>
#define STB_IMAGE_IMPLEMENTATION
#include <graphic/stb_image.h>
#include "wtypes.h"
#include <iostream>


namespace emp {


    void SpriteGraphic::Init() {
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


    void TextGraphic::Init() {
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

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1920), 0.0f, static_cast<float>(1080 - 30 - 40));
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
            return;
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

    GraphicManager::GraphicManager(Engine& engine, string name, ConfigGraphic& config) : System(engine, name)
    {
        this->config = &config;
    }

    GraphicManager::GraphicManager(Engine& engine, string name) : System(engine, name)
    {
    }

    Transform obj_1 = Transform(1, 0);
    Transform obj_2 = Transform(-1, 0);
    GraphicComponent* component2 = new SpriteGraphic("./data/wall.jpg", obj_1);
    //Old GraphicComponent component = GraphicComponent("./data/NewLogoPixelColoredx192v2.jpg", obj_2);
    GraphicComponent* component = new SpriteGraphic("./data/NewLogoPixelColoredx192v2.jpg", obj_2);

    GraphicComponent* component3 = new TextGraphic();
	
    void GraphicManager::Init()
    {
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
        this->window = glfwCreateWindow(config->width, config->height, "Engine Mushroom Portuaire", NULL, NULL);

        this->screen._backgroundColor = ColorRGB(0.14f, 0.14f, 0.14f);
    	
        glfwSetWindowAttrib(window, GLFW_DECORATED, config->decorated);
        glfwSetWindowPos(this->window, config->x, config->y);
        glfwSetWindowSize(this->window, config->window_width, config->window_height);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config->transparent);

    	
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(this->window);
        GLenum err = glewInit();

        component->Init();
        component2->Init();

        component3->Init();

    }
	
	float timer = 0.0f;
	void GraphicManager::Update(float dt)
	{
		timer += dt;
		//while (!glfwWindowShouldClose(this->window))
		glfwPollEvents();
	}

	void GraphicManager::Draw()
	{
        
         //glClearColor(screen._backgroundColor.r, screen._backgroundColor.g, screen._backgroundColor.b, screen._backgroundColor.a);
		 //glClear(GL_COLOR_BUFFER_BIT);

        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        component2->Draw();
        component->Draw();
        component3->Draw();
       

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

    enum Type
	{
		JPG,
		PNG,
		JPEG,
		ICO
	};

    struct Texture
	{
        int id;
        string name;
        string path;
        Type type;

    	Texture(string& name, string& path)
    	{
            this->id = 0;
            this->name = name;
            this->path = path;
    	}
	};

    struct Sprite
    {
        int id;
        string name;
        Texture texture;

        int x, y;
        int size_x, size_y;
    };
	
}
