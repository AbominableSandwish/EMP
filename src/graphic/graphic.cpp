#include <graphic/graphic.h>
#define STB_IMAGE_IMPLEMENTATION
#include <graphic/stb_image.h>
#include "wtypes.h"
#include <iostream>


namespace emp {
   

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
