#include <components/light.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/shader.h>
#include <graphic/graphic.h>
#include <components/transform.h>
#include <components/camera.h>
#include <math/matrice.h>
#include <core/file.h>
#include <fstream>s

namespace emp {

    PointLight::PointLight(int entity, float r, float g, float b, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
    {
        this->entity = entity;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }

    void PointLight::Init()
    {
       
    }

    SpotLight::SpotLight(int entity, float r, float g, float b, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    {
        this->entity = entity;
        this->direction = direction;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;

        this->cutOff = cutOff;
        this->outerCutOff = outerCutOff;
    }

    void SpotLight::Init()
    {

    }

    DirectionalLight::DirectionalLight(int entity, glm::vec3 direction , glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    {
        this->entity = entity;
        this->direction = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }

    void DirectionalLight::Init()
    {

    }

    LightManager::LightManager(Engine& engine, ConfigGraphic& config) : System(engine, "LightManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void LightManager::Init()
    {
        m_component = engine->GetComponentManager();
        {

             this->shader = new Shader();

            bool warning = this->shader->Init(FileSystem::ReadShader("./shader/light/light.vs"), FileSystem::ReadShader("./shader/light/light.fs"));
            if (warning) {
                LOG::Warning(name + " Help!");
            }
            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                // positions       
                 0.5f,  0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,

                 0.5f,  0.5f, 0.5f,
                 0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,
                -0.5f,  0.5f, 0.5f
            };

            unsigned int indices[] = {
                0, 1, 2, 1, 2, 4,
                4, 5, 7, 5, 6, 7,
                0, 1, 4, 4, 5, 1,
                2, 3, 7, 6, 7, 2,
                1, 2, 6, 5, 6, 1,
                0, 3, 7, 7, 4, 0
            };

            glGenVertexArrays(1, &this->shader->VAO);
            glGenBuffers(1, &this->shader->VBO);
            glGenBuffers(1, &this->shader->EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(this->shader->VAO);

            glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->shader->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);



           
        }
    }


    void LightManager::Destroy()
    {
    }

    void LightManager::Update(float dt)
    {
        time += dt;
    }

    void LightManager::Draw()
    {

        auto& arrayElement = engine->GetComponentManager()->GetComponents<PointLight>();
        for (auto& element : arrayElement)
        {
            glm::mat4 transf = m_component->GetComponent<Transform>(element.entity).matrice->GetMatrice();
            auto& list_camera = m_component->GetComponents<Camera>();
            auto& MainCamera = list_camera[0];
            // draw our first triangle
            this->shader->UseProgram();

            // get matrix's uniform location and set matrix
            this->shader->SetMat4("transform", transf);
            glm::mat4 view = MainCamera.GetView();
            this->shader->SetMat4("view", view);
            this->shader->SetMat4("projection", list_camera[0].projection);

            this->shader->SetVec3("color", glm::vec3(element.specular.r, element.specular.g, element.specular.b));

            // render container
            this->shader->BindVertexArray(this->shader->VAO);
            this->shader->DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            this->shader->BindVertexArray(0);
        }
    }
}