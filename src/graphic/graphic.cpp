#include <graphic/graphic.h>
#define STB_IMAGE_IMPLEMENTATION
#include <graphic/stb_image.h>
#include "wtypes.h"
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace emp {
    unsigned int shaderProgram;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;

    //struct Vector2
    //{
    //    float x;
    //    float y;

    //    Vector2();
    //	Vector2(float x, float y)
    //	{
    //        this->x = x;
    //        this->y = y;
    //	}
    //};
    //struct iVector2
    //{
    //    int x;
    //    int y;
    //};


    GraphicManager::GraphicManager(Engine& engine, string name, ConfigGraphic& config) : System(engine, name)
    {
        this->config = &config;
    }

    GraphicManager::GraphicManager(Engine& engine, string name) : System(engine, name)
    {
    }

	void GenerateSquare()
    {
	    
    }

	void GenerateCircle(int positionX, int positionY)
    {
        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        const char* vertexShaderTexture = "#version 330 core\n"
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

        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "FragColor = vec4(1.0f, 0.1f, 1.0f, 1.0f);\n"
            "};\0";

        const char* fragmentShaderTexture = "#version 330 core\n"
            "out vec4 FragColor;\n"

            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"

            // texture sampler
            "uniform sampler2D texture1;\n"

            "void main()\n"
            "{\n"
            "FragColor = texture(texture1, TexCoord);\n"
            "}\0";

        // build and compile our shader program
          // ------------------------------------
          // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderTexture, NULL);
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
        glShaderSource(fragmentShader, 1, &fragmentShaderTexture, NULL);
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
        string path = "./data/NewLogoPixelColoredx192v2.jpg";
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
    }
	
	
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
        
        // glClearColor(screen._backgroundColor.r, screen._backgroundColor.g, screen._backgroundColor.b, screen._backgroundColor.a);
		// glClear(GL_COLOR_BUFFER_BIT);

        GenerateCircle(0, 0);
       
        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

      


        //render container
        glUseProgram(shaderProgram);

		// get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
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
