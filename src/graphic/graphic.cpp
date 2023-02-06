#include "graphic/graphic.h"
#include <core/engine.h>
#include <core/config.h>
#include <graphic/sprite.h>
#include <graphic/shader.h>
#include "components/renderer2D.h"
#include "components/square.h"
#include "components/triangle.h"
#include "components/circle.h"
#include "components/cube.h"
#include "components/sphere.h"
#include "components/line.h"
#include "components/light.h"
#include "components/model.h"
#include <core/file.h>
#include "components/camera.h"
#include "glm/gtx/transform.hpp"


namespace emp {

	GraphicManager::GraphicManager(Engine& engine, ConfigGraphic& config) : System(engine, "GraphicManager")
	{
		this->config = &config;
		//m_sprite = new SpriteManager(engine, config);
		m_square = new SquareManager(engine, config);
		m_circle = new CircleManager(engine, config);
		m_triangle = new TriangleManager(engine, config);
		m_cube = new CubeManager(engine, config);
		m_sphere = new SphereManager(engine, config);
	
		m_model = new ModelManager(engine, config);
		m_light = new LightManager(engine, config);
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
	//Render flag
	bool gRenderQuad = true;
	//Graphics program
	GLuint gProgramID = 0;
	GLint gVertexPos2DLocation = -1;
	GLuint gVBO = 0;
	GLuint gIBO = 0;
	


	bool initGL()
	{
		//Success flag
		bool success = true;

		//Generate program
		gProgramID = glCreateProgram();

		//Create vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		//Get vertex source
		const GLchar* vertexShaderSource[] =
		{
			"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
		};

		//Set vertex source
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

		//Compile vertex source
		glCompileShader(vertexShader);

		//Check vertex shader for errors
		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			printf("Unable to compile vertex shader %d!\n", vertexShader);
			//printShaderLog(vertexShader);
			success = false;
		}
		else
		{
			//Attach vertex shader to program
			glAttachShader(gProgramID, vertexShader);


			//Create fragment shader
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			//Get fragment source
			const GLchar* fragmentShaderSource[] =
			{
				"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
			};

			//Set fragment source
			glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

			//Compile fragment source
			glCompileShader(fragmentShader);

			//Check fragment shader for errors
			GLint fShaderCompiled = GL_FALSE;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
			if (fShaderCompiled != GL_TRUE)
			{
				printf("Unable to compile fragment shader %d!\n", fragmentShader);
				//printShaderLog(fragmentShader);
				success = false;
			}
			else
			{
				//Attach fragment shader to program
				glAttachShader(gProgramID, fragmentShader);


				//Link program
				glLinkProgram(gProgramID);

				//Check for errors
				GLint programSuccess = GL_TRUE;
				glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
				if (programSuccess != GL_TRUE)
				{
					printf("Error linking program %d!\n", gProgramID);
					//printProgramLog(gProgramID);
					success = false;
				}
				else
				{
					//Get vertex attribute location
					gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
					if (gVertexPos2DLocation == -1)
					{
						printf("LVertexPos2D is not a valid glsl program variable!\n");
						success = false;
					}
					else
					{
						//Initialize clear color
						glClearColor(0.f, 0.f, 0.f, 1.f);

						//VBO data
						GLfloat vertexData[] =
						{
							-0.5f, -0.5f,
							 0.5f, -0.5f,
							 0.5f,  0.5f,
							-0.5f,  0.5f
						};

						//IBO data
						GLuint indexData[] = { 0, 1, 2, 3 };

						//Create VBO
						glGenBuffers(1, &gVBO);
						glBindBuffer(GL_ARRAY_BUFFER, gVBO);
						glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

						//Create IBO
						glGenBuffers(1, &gIBO);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
					}
				}
			}
		}

		return success;
	}

	
	void GraphicManager::Init()
	{
		//Initialization flag
		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Use OpenGL 3.1 core
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

			//Create window
			this->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
			if (this->window == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Create context
				gContext = SDL_GL_CreateContext(this->window);
				if (gContext == NULL)
				{
					printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					//Initialize GLEW
					glewExperimental = GL_TRUE;
					GLenum glewError = glewInit();
					if (glewError != GLEW_OK)
					{
						printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
					}

					//Use Vsync
					if (SDL_GL_SetSwapInterval(1) < 0)
					{
						printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					}

					//Initialize OpenGL
					if (!initGL())
					{
						printf("Unable to initialize OpenGL!\n");
						success = false;
					}
				}
			}
		}

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		//Init others Sub-Systems Graphic
		//m_sprite->Init();
		m_square->Init();
		m_circle->Init();
		m_triangle->Init();
		m_cube->Init();
		m_sphere->Init();
		m_model->Init();
		m_light->Init();



		std::string vertexShaderSource = FileSystem::ReadShader("./shader/framebuffer/framebuffer.vs");
		std::string fragmentShaderSource = FileSystem::ReadShader("./shader/framebuffer/framebuffer.fs");  //multiplelight  

		this->shader = new Shader();
		bool warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
		if (warning) {
			LOG::Warning(name + " help!");
		}

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
			// positions   // texCoords
			-0.3f,  1.0f,  0.0f, 1.0f,
			-0.3f,  0.7f,  0.0f, 0.0f,
			 0.3f,  0.7f,  1.0f, 0.0f,

			-0.3f,  1.0f,  0.0f, 1.0f,
			 0.3f,  0.7f,  1.0f, 0.0f,
			 0.3f,  1.0f,  1.0f, 1.0f
		};

		// screen quad VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		// framebuffer configuration
		// -------------------------
		this->shader->UseProgram();
		this->shader->SetInt("screenTexture", 0);
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); /// MAGIC !!!!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1000, 1000); // use a single renderbuffer object for both a depth AND stencil buffer. MAGIC
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//lineX = new Line(*this->config, glm::vec3(0, 0, 0), glm::vec3(0.12f, 0, 0));
		//lineY = new Line(*this->config, glm::vec3(0, 0, 0), glm::vec3(0, 0.2f, 0));

		//grid(*config);
	}

	bool pause = false;
	float timer = 0.0f;



	void GraphicManager::Update(float dt)
	{
		if (pause) {
			return;
		}
		timer += dt;
		
		/*if (glfwWindowShouldClose(this->window))
		{
			engine->Stop();
		}
		*/
	
		/*if (sdl_set->GetMainEvent()->type == SDL_WINDOWEVENT)
		{
			if (sdl_set->GetMainEvent()->window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ScreenWidth = sdl_set->GetMainEvent()->window.data1;
				ScreenHeight = sdl_set->GetMainEvent()->window.data2;
				cout << "Window Resized!" << endl;
			}
		}*/

		m_triangle->Update(dt);
		m_cube->Update(dt);
		m_sphere->Update(dt);
		m_light->Update(dt);
		////m_sprite->Update(dt);
		m_model->Update(dt);

		SDL_PollEvent(0);
		//glfwPollEve0nts();
		timer = 0.0f;
	}

	void GraphicManager::Draw()
	{
		// enable depth testing (is disabled for rendering screen-space quad)
		// first render pass: mirror texture.
	    // bind to framebuffer and draw to color texture as we normally 
	    // would, but with the view camera reversed.
	    // bind to framebuffer and draw scene as we normally would to color texture 
	    // ------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (WireframeView) {
			// Turn on wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			// Turn off wireframe 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		//m_sprite->Draw();
		m_square->Draw();
		m_circle->Draw();
		m_triangle->Draw();
		m_cube->Draw();
		m_sphere->Draw();
		m_model->Draw();


		m_light->Draw();
		// second render pass: draw
		//  as normal
		// ----------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/ // also clear the depth buffer now!


		//m_sprite->Draw();
		m_square->Draw();
		m_circle->Draw();
		m_triangle->Draw();
		m_cube->Draw();
		m_sphere->Draw();
		m_model->Draw();

		  // --------------------------------------------
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

		this->shader->UseProgram();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*for (auto& element : grid_line)
		{
			element->Draw();
		}*/
		//Update the surface
	}


	void GraphicManager::Swap()
	{
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	 // -------------------------------------------------------------------------------
		//glfwSwapBuffers(window);
		SDL_GL_SwapWindow(window);
		
	}

	void saveImage(char* filepath, SDL_Window* w) {
		//int width, height;
	 //   //glfwGetFramebufferSize(w, &width, &height);
		//SDL_GL_GetDrawableSize(w, &width, &height);
		//GLsizei nrChannels = 3;
		//GLsizei stride = nrChannels * width;
		//stride += (stride % 4) ? (4 - stride % 4) : 0;
		//GLsizei bufferSize = stride * height;
		//std::vector<char> buffer(bufferSize);
		//glPixelStorei(GL_PACK_ALIGNMENT, 4);
		//glReadBuffer(GL_FRONT);
		//glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
		//stbi_flip_vertically_on_write(true);
		//stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
	}

	void GraphicManager::Destroy()
	{
		//glfwTerminate();
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	Camera& GraphicManager::GetMainCamera()
	{
		return *camera_list->begin();
	}

	/*GLFWwindow& GraphicManager::GetWindow()
	{
		return *this->window;
	}*/

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


