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

    

	void ModelManager::Draw() {

        int PixelPerSize = config->PixelSize;
        auto arrayElement = engine->GetComponentManager()->GetComponents<Model>();

        for (auto element : arrayElement)
        {

            // draw our first triangle
            this->shader->UseProgram();

            auto list_camera = m_component->GetComponents<Camera>();
            Camera MainCamera = list_camera[0];
          
            glm::mat4 view = glm::mat4(1.0f);
            auto transf_camera = m_component->GetComponent<Transform>(MainCamera.entity);
            view = glm::translate(view, glm::vec3(transf_camera.position->r, transf_camera.position->g, transf_camera.position->b));
           
            glm::mat4 projection = MainCamera.projection;
           
            this->shader->SetMat4("view", view);
            this->shader->SetMat4("projection", projection);

            glBindVertexArray(this->shader->VAO);
            //CAMERA
            this->shader->SetVec3("viewPos", MainCamera.GetPosition());
            //Get Transform Data
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;

            //OBJECT
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);

            transf = glm::rotate(transf, glm::radians(time*50), glm::vec3(0.0f, 1.0f, 0.0f));
            //transf = glm::rotate(transf, glm::radians(element.axis_x + time / 10), glm::vec3(0.0f, 1.0f, 0.0f));

             

             

            this->shader->SetMat4("transform", transf);
            this->shader->SetVec3("objectColor", glm::vec3(element.color.r, element.color.g, element.color.b));
                
            // Material properties
            this->shader->SetFloat("material.shininess", element.shininess);
            this->shader->SetVec3("material.color", glm::vec3(element.color.r, element.color.g, element.color.b));
            //LIGHT
            auto arrayLight = engine->GetComponentManager()->GetComponents<PointLight>();
            for each (auto& light in arrayLight) {

            }
            PointLight light = arrayLight[0];
            Vector3 lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            
            // Dirlight properties
            this->shader->SetVec3("dirLight.direction", 0.2f, 1.0f, 0.3f);
            this->shader->SetVec3("dirLight.ambient", 0.0f, 0.0f, 1.0f);
            this->shader->SetVec3("dirLight.diffuse", 0.0f, 1.0f, 0.0f);
            this->shader->SetVec3("dirLight.specular", 1.0f, 0.0f, 0.0f);

            // PointLight properties
            this->shader->SetVec3("pointLights[0].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z));
            this->shader->SetVec3( "pointLights[0].ambient", light.ambient);
            this->shader->SetVec3( "pointLights[0].diffuse", light.diffuse);
            this->shader->SetVec3( "pointLights[0].specular", light.specular);
            this->shader->SetFloat("pointLights[0].constant", light.constant);
            this->shader->SetFloat("pointLights[0].linear", light.linear);
            this->shader->SetFloat("pointLights[0].quadratic", light.quadratic);
             light = arrayLight[2];
             lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            this->shader->SetVec3("pointLights[1].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z));
            this->shader->SetVec3("pointLights[1].ambient", light.ambient);
            this->shader->SetVec3("pointLights[1].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[1].specular", light.specular);
            this->shader->SetFloat("pointLights[1].constant", light.constant);
            this->shader->SetFloat("pointLights[1].linear", light.linear);
            this->shader->SetFloat("pointLights[1].quadratic", light.quadratic);

            this->shader->SetVec3("pointLights[2].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z));
            this->shader->SetVec3("pointLights[2].ambient", light.ambient);
            this->shader->SetVec3("pointLights[2].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[2].specular", light.specular);
            this->shader->SetFloat("pointLights[2].constant", light.constant);
            this->shader->SetFloat("pointLights[2].linear", light.linear);
            this->shader->SetFloat("pointLights[2].quadratic", light.quadratic);
            light = arrayLight[3];
            lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            this->shader->SetVec3("pointLights[3].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z));
            this->shader->SetVec3("pointLights[3].ambient", light.ambient);
            this->shader->SetVec3("pointLights[3].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[3].specular", light.specular);
            this->shader->SetFloat("pointLights[3].constant", light.constant);
            this->shader->SetFloat("pointLights[3].linear", light.linear);
            this->shader->SetFloat("pointLights[3].quadratic", light.quadratic);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuse_map);
            // at init time.
            //glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
            glGenTextures(1, &specular_map);
            glBindTexture(GL_TEXTURE_2D, specular_map);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular_map);

            ///FragPos = vec3(transform * vec4(aPos, 1.0));
            glm::vec3 fragPos = transf * glm::vec4(1.0f, 0.0f, 1.0f, 0.1);
            glm::vec3 normal = glm::mat3(glm::transpose(glm::inverse(transf))) * glm::vec3(1.0f, 0.0f, 0.0f);

            //vec3 lightDir = normalize(light.position - fragPos);

            
                glm::vec3 lightDir = normalize(glm::vec3(lightpos.x, lightpos.y, lightpos.z) - fragPos);
                std::cout << "lightDir = " << lightDir.x << ", " << lightDir.y << ", " << lightDir.z << "\n";
                //// diffuse shading
                float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
                std::cout << "diff = " << diff << "\n";
            //// specular shading
            //vec3 reflectDir = reflect(-lightDir, normal);
                glm::vec3 reflectDir = reflect(-lightDir, normal);
                std::cout << "lightDir = " << reflectDir.x << ", " << reflectDir.y << ", " << lightDir.z << "\n";
              
            //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
               glm::vec3 viewDir = glm::normalize(glm::vec3(0.0f, 0.0f, -3.0f) - glm::vec3(1.0f, 0.0f, 1.0f));
               float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), this->shader->shininess);
               std::cout << "spec = " << spec << "\n";
            //// attenuation
            //float distance = length(light.position - fragPos);
            float distance = glm::length(glm::vec3(lightpos.x, lightpos.y, lightpos.z) - fragPos);
            distance = distance;
            std::cout << "distance = " << distance << "\n";
            //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            std::cout << "attenuation = " << attenuation << "\n";
            //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            //// combine results
            glm::vec3 ambient = light.ambient * glm::vec3(element.color);
            std::cout << "ambient = " << ambient.x << ", " << ambient.y << ", " << ambient.z << "\n";
            //vec3 diffuse = light.diffuse * diff;
            //vec3 specular = light.specular * spec;
            ambient *= attenuation;
            std::cout << "ambient with attenuation= " << ambient.x << ", " << ambient.y << ", " << ambient.z << "\n";
            //diffuse *= attenuation;
            //specular *= attenuation;  

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
