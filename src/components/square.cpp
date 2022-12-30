#include <components/square.h>
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

namespace emp {
    //
    SquareManager::SquareManager(Engine& engine, ConfigGraphic& config) : System(engine, "SquareManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void SquareManager::Init()
    {
        m_component = engine->GetComponentManager();
       
        std::string vertexShaderSource = FileSystem::ReadShader("./shader/model.vs");
        std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2.fs");  //multiplelight  

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
                  -0.5f, -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
                   0.5f, -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
                   0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
                   0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
                  -0.5f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
                  -0.5f, -0.5f, -0.0f,  0.0f,  0.0f, -1.0f,
        };

        this->shader = new Shader();
        bool warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
        if (warning) {
            LOG::Warning( name + " help!");
        }

        glGenVertexArrays(1, &this->shader->VAO);
        glGenBuffers(1, &this->shader->VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s). 

        glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->shader->VAO);
        // position attribute 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normal attribut; 
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //SALUT MON  pote 
        glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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


    void SquareManager::Destroy()
    {
    }

    void SquareManager::Draw()
    {

        int PixelPerSize = config->PixelSize;
        auto& arrayElement = engine->GetComponentManager()->GetComponents<Square>();
        this->shader->UseProgram();


        //CAMERA
        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
        unsigned int viewPosLoc = glGetUniformLocation(this->shader->shaderProgram, "viewPos");
        glm::vec3 pos = MainCamera.GetPosition();
        glUniform3f(viewPosLoc, pos.x, pos.y, pos.z);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, pos);

        glm::mat4& projection = MainCamera.projection;
        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(this->shader->shaderProgram, "transform");
        unsigned int viewLoc = glGetUniformLocation(this->shader->shaderProgram, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(this->shader->shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        // render boxes
        this->shader->BindVertexArray(this->shader->VAO);

        for (auto& element : arrayElement)
        {
            //Get Transform Data
            auto& transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;

            //Matrice Transform
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);
            transf = glm::rotate(transf, glm::radians(time * 50), glm::vec3(0.0f, 1.0f, 0.0f));

            //transf = glm::rotate(transf, glm::radians(sin(time)*180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            this->shader->SetMat4("transform", transf);
            this->shader->SetVec3("objectColor", glm::vec3(element.color.r, element.color.g, element.color.b));
            //LIGHT


            // Material properties
            this->shader->SetFloat("material.shininess", this->shader->shininess);
            this->shader->SetVec3("material.color", glm::vec3(element.color.r, element.color.g, element.color.b));

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

            this->shader->DrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void Square::Init()
    {
        {
            float red = (std::rand() % 10);
            float green = (std::rand() % 10);
            float blue = (std::rand() % 10);

            color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);
          
        }
    }

}