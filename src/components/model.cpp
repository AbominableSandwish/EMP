#include <graphic/graphic.h>
#include <components/model.h>
#include <core/engine.h>
#include <core/component.h>
#include <core/config.h>
#include <math/matrice.h>
#include <components/transform.h>
#include <components/camera.h>
#include <components/light.h>
#include <graphic/shader.h>
#include <core/file.h>

namespace emp {
	ModelManager::ModelManager(Engine& engine, ConfigGraphic& config) : System(engine, "Model Manager")
	{
        this->config = &config;
        m_component = engine.GetComponentManager();
	}

	void ModelManager::Init() {
        std::string vertexShaderSource = FileSystem::ReadShader("./shader/model.vs");
        std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2.fs");

        this->shader = new Shader();
        bool warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
        if (warning) {
            LOG::Warning(name + " help!");
        }


        // at init time.
        glm::vec4 color = glm::vec4(1, 1, 1, 1);

        glGenTextures(0, &diffuse_map);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);

        // set texture wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filter parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // set image data
        int size = 156;
        unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
        for (unsigned int i = 0; i < size * size; i++)
        {
            data[i * 3] = (unsigned char)(color.x * 255.0f);
            data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
            data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] data;

        glGenTextures(1, &specular_map);
        glBindTexture(GL_TEXTURE_2D, specular_map);

        // set texture wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filter parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        data = new unsigned char[3 * size * size * sizeof(unsigned char)];
        for (unsigned int i = 0; i < size * size; i++)
        {
            data[i * 3] = (unsigned char)(color.x * 255.0f);
            data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
            data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] data;

        this->shader->UseProgram();

        this->shader->SetInt("material.diffuse", 0);
        this->shader->SetInt("material.specular", 1);
	}

	void ModelManager::Update(float dt) {
        this->time += dt;
	}

    bool updat = true;

	void ModelManager::Draw() {

        int PixelPerSize = config->PixelSize;
        auto& arrayElement = engine->GetComponentManager()->GetComponents<Model>();

        for (auto& element : arrayElement)
        {
            // draw our first triangle
            this->shader->UseProgram();

            auto& list_camera = m_component->GetComponents<Camera>();
            Camera& MainCamera = list_camera[0];
          
            
            glm::mat4 view = MainCamera.GetView();
            this->shader->SetMat4("view", view);
            
           
            glm::mat4 projection = MainCamera.projection;

            this->shader->SetMat4("projection", projection);

            glBindVertexArray(this->shader->VAO);
            //CAMERA
            this->shader->SetVec3("viewPos", MainCamera.GetPosition());

            //OBJECT
            glm::mat4 transf = m_component->GetComponent<Transform>(element.entity).matrice->GetMatrice();
            transf = glm::rotate(transf, glm::radians(time*50), glm::vec3(0.0f, 1.0f, 0.0f));
            //transf = glm::rotate(transf, glm::radians(element.axis_x + time / 10), glm::vec3(0.0f, 1.0f, 0.0f));

            this->shader->SetMat4("transform", transf);

            if (updat) {
                this->shader->SetVec3("objectColor", glm::vec3(element.color.r, element.color.g, element.color.b));

                // Material properties
                this->shader->SetFloat("material.shininess", element.shininess);
                this->shader->SetVec3("material.color", glm::vec3(element.color.r, element.color.g, element.color.b));

                //LIGHT
                // Dirlight properties
                auto& arrayDirLight = engine->GetComponentManager()->GetComponents<DirectionalLight>();
                this->shader->SetVec3("dirLight.direction", arrayDirLight[0].direction);
                this->shader->SetVec3("dirLight.ambient", arrayDirLight[0].ambient);
                this->shader->SetVec3("dirLight.diffuse", arrayDirLight[0].diffuse);
                this->shader->SetVec3("dirLight.specular", arrayDirLight[0].specular);

                // PointLight properties
                auto& arrayLight = engine->GetComponentManager()->GetComponents<PointLight>();
                this->shader->SetVec3("pointLights[0].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[0].entity).Position() / 100.0f);
                this->shader->SetVec3("pointLights[0].ambient", arrayLight[0].ambient);
                this->shader->SetVec3("pointLights[0].diffuse", arrayLight[0].diffuse);
                this->shader->SetVec3("pointLights[0].specular", arrayLight[0].specular);
                this->shader->SetFloat("pointLights[0].constant", arrayLight[0].constant);
                this->shader->SetFloat("pointLights[0].linear", arrayLight[0].linear);
                this->shader->SetFloat("pointLights[0].quadratic", arrayLight[0].quadratic);
                this->shader->SetVec3("pointLights[1].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[1].entity).Position() / 100.0f);
                this->shader->SetVec3("pointLights[1].ambient", arrayLight[1].ambient);
                this->shader->SetVec3("pointLights[1].diffuse", arrayLight[1].diffuse);
                this->shader->SetVec3("pointLights[1].specular", arrayLight[1].specular);
                this->shader->SetFloat("pointLights[1].constant", arrayLight[1].constant);
                this->shader->SetFloat("pointLights[1].linear", arrayLight[1].linear);
                this->shader->SetFloat("pointLights[1].quadratic", arrayLight[1].quadratic);
                this->shader->SetVec3("pointLights[2].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[2].entity).Position() / 100.0f);
                this->shader->SetVec3("pointLights[2].ambient", arrayLight[2].ambient);
                this->shader->SetVec3("pointLights[2].diffuse", arrayLight[2].diffuse);
                this->shader->SetVec3("pointLights[2].specular", arrayLight[2].specular);
                this->shader->SetFloat("pointLights[2].constant", arrayLight[2].constant);
                this->shader->SetFloat("pointLights[2].linear", arrayLight[2].linear);
                this->shader->SetFloat("pointLights[2].quadratic", arrayLight[2].quadratic);
                this->shader->SetVec3("pointLights[3].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[3].entity).Position() / 100.0f);
                this->shader->SetVec3("pointLights[3].ambient", arrayLight[3].ambient);
                this->shader->SetVec3("pointLights[3].diffuse", arrayLight[3].diffuse);
                this->shader->SetVec3("pointLights[3].specular", arrayLight[3].specular);
                this->shader->SetFloat("pointLights[3].constant", arrayLight[3].constant);
                this->shader->SetFloat("pointLights[3].linear", arrayLight[3].linear);
                this->shader->SetFloat("pointLights[3].quadratic", arrayLight[3].quadratic);
                // spotLight
                auto& arraySpot = engine->GetComponentManager()->GetComponents<SpotLight>();
                this->shader->SetVec3("spotLight.position", engine->GetComponentManager()->GetComponent<Transform>(arraySpot[0].entity).Position() / 100.0f);
                this->shader->SetVec3("spotLight.direction", arraySpot[0].direction);
                this->shader->SetVec3("spotLight.ambient", arraySpot[0].ambient);
                this->shader->SetVec3("spotLight.diffuse", arraySpot[0].diffuse);
                this->shader->SetVec3("spotLight.specular", arraySpot[0].specular);
                this->shader->SetFloat("spotLight.constant", arraySpot[0].constant);
                this->shader->SetFloat("spotLight.linear", arraySpot[0].linear);
                this->shader->SetFloat("spotLight.quadratic", arraySpot[0].quadratic);
                this->shader->SetFloat("spotLight.cutOff", arraySpot[0].cutOff);
                this->shader->SetFloat("spotLight.outerCutOff", arraySpot[0].outerCutOff);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuse_map);
                // at init time.
                //glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
                glGenTextures(1, &specular_map);
                glBindTexture(GL_TEXTURE_2D, specular_map);
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specular_map);

                updat = false;
            }

            element.Draw(this->shader->shaderProgram);
        }
	}

	void ModelManager::Destroy() {

	}

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
    {
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}
