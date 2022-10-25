#include "graphic/graphic.h"
#include <core/engine.h>
#include <core/config.h>
#include <graphic/sprite.h>
#include "components/renderer2D.h"
#include "components/square.h"
#include "components/triangle.h"
#include "components/circle.h"
#include "components/cube.h"
#include "components/line.h"
#include "glm/gtx/transform.hpp"


namespace emp {

	GraphicManager::GraphicManager(Engine& engine, ConfigGraphic& config) : System(engine, "GraphicManager")
	{
		this->config = &config;
		m_sprite = new SpriteManager(engine, config);
		m_square = new SquareManager(engine, config);
		m_circle = new CircleManager(engine, config);
		m_triangle = new TriangleManager(engine, config);
		m_cube = new CubeManager(engine, config);
	}
	
	Line* lineX;
	glm::mat4 transf = glm::mat4(1.0f);
	Line* lineY;

	std::vector<Line*> grid_line;

	void grid(ConfigGraphic& config)
	{
		grid_line = std::vector<Line*>();
	
		for (int i = 0; i <= 4; i++)
		{
			glm::vec2 position;
			if (i != 0) 
			{
				position = glm::vec3(((float)i)/4, ((float)i) /4, 0);

				glm::mat4 transf_line = glm::mat4(1.0f);

				grid_line.push_back(new Line(config, glm::vec3(0, position.y, 0), glm::vec3(-1, position.y, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(0, position.y, 0), glm::vec3(1, position.y, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				transf_line = glm::mat4(1.0f);
				grid_line.push_back(new Line(config, glm::vec3(position.x, 0, 0), glm::vec3(position.x, -1, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(position.x, 0, 0), glm::vec3(position.x, 1, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));



				//
				grid_line.push_back(new Line(config, glm::vec3(0, -position.y, 0), glm::vec3(-1, -position.y, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(0, -position.y, 0), glm::vec3(1, -position.y, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				transf_line = glm::mat4(1.0f);
				grid_line.push_back(new Line(config, glm::vec3(-position.x, 0, 0), glm::vec3(-position.x, -1, 0)));

				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(-position.x, 0, 0), glm::vec3(-position.x, 1, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));
			}
			else
			{
				position = glm::vec3(0, 0, 0);
				
				glm::mat4 transf_line = glm::mat4(1.0f);

				grid_line.push_back(new Line(config, glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				transf_line = glm::mat4(1.0f);
				grid_line.push_back(new Line(config, glm::vec3(position.x, 0, 0), glm::vec3(position.x, -1, 0)));

				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));

				grid_line.push_back(new Line(config, glm::vec3(position.x, 0, 0), glm::vec3(position.x, 1, 0)));
				grid_line[grid_line.size() - 1]->setColor(glm::vec3(0.25f, 0.25f, 0.25f));
			}
			
			
		}
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


		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		this->window = glfwCreateWindow(1000, 1000, "Engine Mushroom Portuaire", NULL, NULL);
		this->screen._backgroundColor = ColorRGB(0.14f, 0.14f, 0.14f);
		if (this->config->horizontal > this->config->vertical)			
			this->config->offset_scaling = this->config->horizontal / (float)this->config->vertical;
		if (this->config->horizontal < this->config->vertical)
			this->config->offset_scaling = this->config->vertical / (float)this->config->horizontal;

		glfwSetWindowAttrib(window, GLFW_DECORATED, this->config->decorated);
		glfwSetWindowPos(this->window, this->config->x, this->config->y);
		glfwSetWindowSize(this->window, this->config->window_width, this->config->window_height);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, this->config->transparent);

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(this->window);
		GLenum err = glewInit();

		m_sprite->Init();
		m_square->Init();
		m_circle->Init();
		m_triangle->Init();
		m_cube->Init();


		lineX = new Line(*this->config, glm::vec3(0, 0, 0), glm::vec3(0.12f, 0, 0));
		lineY = new Line(*this->config, glm::vec3(0, 0, 0), glm::vec3(0, 0.2f, 0));

		grid(*config);
	}

	float timer = 0.0f;
	void GraphicManager::Update(float dt)
	{
		timer += dt;
		if (glfwWindowShouldClose(this->window))
		{
			engine->Stop();
		}
		m_cube->Update(dt);
		m_sprite->Update(dt);
		glfwPollEvents();
	}

	


	

	void GraphicManager::Draw()
	{
		//glClearColor(screen._backgroundColor.r, screen._backgroundColor.g, screen._backgroundColor.b, screen._backgroundColor.a);
		//glClear(GL_COLOR_BUFFER_BIT);
		if (WireframeView) {
			// Turn on wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {

		// Turn off wireframe 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		m_sprite->Draw();
		m_square->Draw();
		m_circle->Draw();
		m_triangle->Draw();
		m_cube->Draw();
		

		for (auto element : grid_line)
		{
			element->Draw();
		}
		
		//lineX->setMVP(transf);
		//lineX->setColor(glm::vec3(0, 0, 1));
		//lineX->Draw();
		//lineY->setMVP(transf);
		//lineY->setColor(glm::vec3(0, 1, 0));
		//lineY->Draw();
		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
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


