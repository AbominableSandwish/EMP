#include "graphic/sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include "graphic/stb_image.h"

#include "glm/gtx/transform.hpp"
#include "graphic/graphic.h"
#include <core/component.h>
#include <core/engine.h>
#include <core/config.h>


namespace emp
{
	SpriteRenderer::SpriteRenderer(int entity, string path)
	{
		this->entity = entity;
		this->path = path;
	}


	void SpriteRenderer::Init()
	{
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
		unsigned int texturetmp = 0;
		// load image, create texture and generate mipmaps
		int width, height, level;
		unsigned char* file = stbi_load(path.c_str(), &width, &height, &level, 0);
		Texture::loadTexture(file, width, height, texture, path.c_str(), level);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(file);

	}
	
	SpriteManager::SpriteManager(Engine& engine, ConfigGraphic& config) : System(engine, "Sprite Manager")
	{
		this->config = &config;
	}

	void SpriteManager::Init() {
		m_component = engine->GetComponentManager();
	}

	void SpriteManager::Update(float)
	{
	}

	void SpriteManager::Draw()
	{
		auto components = m_component->GetComponents<SpriteRenderer>();
		for (SpriteRenderer& element : components)
		{
			// bind Texture
			glBindTexture(GL_TEXTURE_2D, element.texture);
			auto transform = m_component->GetComponent<Transform>(element.entity);
			Vector2 position = transform.GetPosition();
			Vector2 scale = transform.GetScale();
			glm::mat4 transf = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transf = glm::translate(transf, glm::vec3(position.x / config->PixelSize, position.y / config->PixelSize, 0.0f));
			transf = glm::scale(transf, glm::vec3(scale.x, scale.y, 0.0f));
			transf = glm::rotate(transf, (float)transform.GetRotation().x / 57.28f, glm::vec3(0.0f, 0.0f, 1.0));

			//render container
			glUseProgram(element.shaderProgram);

			// get matrix's uniform location and set matrix
			unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));

			glBindVertexArray(element.VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	void SpriteManager::Destroy()
	{
	}
}
