#include "graphic/sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include "graphic/stb_image.h"

#include "glm/gtx/transform.hpp"
#include "graphic/graphic.h"
#include <core/component.h>
#include <core/engine.h>
#include <core/config.h>
#include <components/transform.h>
#include <math/vector.h>
#include <core/file.h>
#include <graphic/shader.h>




namespace emp
{
	SpriteRenderer::SpriteRenderer(int entity, string path)
	{
		this->entity = entity;
		this->path = path;
	}


	void SpriteRenderer::Init()
	{
		

		//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

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
		m_component = engine.GetComponentManager();
	}

	void SpriteManager::Init() {
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
		std::string vertex = FileSystem::ReadFile("./shader/2d_sprite.vs");
		std::string fragment = FileSystem::ReadFile("./shader/2d_sprite.fs");

		this->shader = new Shader();
		this->shader->Init(vertex, fragment, vertices, indices);
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
			Vector3 position = transform.GetPosition();
			Vector3 scale = transform.GetScale();
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
