#include <graphic/graphic.h>
#include <core/config.h>
#include <graphic/sprite.h>
#include "core/engine.h"



namespace emp {

	GraphicManager::GraphicManager(Engine& engine, ConfigGraphic& config) : System(engine, "GraphicManager")
	{
		this->config = &config;
		m_sprite = new SpriteManager(engine, config);
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

		m_sprite->Init();
	}

	float timer = 0.0f;
	void GraphicManager::Update(float dt)
	{
		timer += dt;
		if (glfwWindowShouldClose(this->window))
		{
			engine->Stop();
		}
		m_sprite->Update(dt);
		glfwPollEvents();
	}

	void GraphicManager::Draw()
	{
		//glClearColor(screen._backgroundColor.r, screen._backgroundColor.g, screen._backgroundColor.b, screen._backgroundColor.a);
		//glClear(GL_COLOR_BUFFER_BIT);
		m_sprite->Draw();
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





}


