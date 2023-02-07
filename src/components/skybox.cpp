#include "components/skybox.h"
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include "glm/gtx/transform.hpp"
#include "graphic/shader.h"
#include <core/file.h>
#include <components/camera.h>
#include <components/light.h>
#include <graphic/stb_image.h>

namespace emp {
    unsigned int loadCubemap(vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                LOG::Error("Cubemap tex failed to load at path: " + faces[i]);
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    SkyboxManager::SkyboxManager(Engine& engine, ConfigGraphic& config) : System(engine, "Skybox System")
	{
		this->config = &config;
		m_component = engine.GetComponentManager();
	}

	 void SkyboxManager::Init() {
		 m_component = engine->GetComponentManager();

		 std::string vertexShaderSource = FileSystem::ReadShader("./shader/skybox.vs");
		 std::string fragmentShaderSource = FileSystem::ReadShader("./shader/skybox.fs");  //multiplelight  

         float skyboxVertices[] = {
             // positions          
             -1.0f,  1.0f, -1.0f,
             -1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f,

             -1.0f, -1.0f,  1.0f,
             -1.0f, -1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f,
             -1.0f,  1.0f,  1.0f,
             -1.0f, -1.0f,  1.0f,

              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,

             -1.0f, -1.0f,  1.0f,
             -1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f, -1.0f,  1.0f,
             -1.0f, -1.0f,  1.0f,

             -1.0f,  1.0f, -1.0f,
              1.0f,  1.0f, -1.0f,
              1.0f,  1.0f,  1.0f,
              1.0f,  1.0f,  1.0f,
             -1.0f,  1.0f,  1.0f,
             -1.0f,  1.0f, -1.0f,

             -1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f, -1.0f,
              1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
              1.0f, -1.0f,  1.0f
         };

         this->shader = new Shader();
         bool warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
         if (warning) {
             LOG::Warning(name + " help!");
         }

         // skybox VAO
       
         glGenVertexArrays(1, &skyboxVAO);
         glGenBuffers(1, &skyboxVBO);
         glBindVertexArray(skyboxVAO);
         glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

         // load textures
         // -------------
         vector<std::string> faces
         {
             "./data/image/skybox/right.jpg",
             "./data/image/skybox/left.jpg",
             "./data/image/skybox/top.jpg",
             "./data/image/skybox/bottom.jpg",
             "./data/image/skybox/front.jpg",
             "./data/image/skybox/back.jpg",
         };
         cubemapTexture = loadCubemap(faces);

         shader->UseProgram();
         shader->SetInt("skybox", 0);

	 }

	 void SkyboxManager::Update(float dt) {
         time += dt;
	 }

	 void SkyboxManager::Destroy() {
         glDeleteVertexArrays(1, &skyboxVAO);
         glDeleteBuffers(1, &skyboxVBO);
	 }
	 void SkyboxManager::Draw() {
         glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
         shader->UseProgram();
         //CAMERA
         auto& list_camera = m_component->GetComponents<Camera>();
         Camera& MainCamera = list_camera[0];
         glm::mat4 projection = MainCamera.projection;
         this->shader->SetMat4("projection", projection);
         glm::mat4 view = MainCamera.GetView2();
         this->shader->SetMat4("view", view);
       
         // skybox cube
         glBindVertexArray(skyboxVAO);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
         glDrawArrays(GL_TRIANGLES, 0, 36);
         glBindVertexArray(0);
         //glDepthFunc(GL_LESS); // set depth function back to default
	 }

   

}