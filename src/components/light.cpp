#include <components/light.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/shader.h>
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include <core/file.h>
#include <fstream>

namespace emp {
    Light::Light(int entity, float r, float g, float b)
    {
        this->entity = entity;
        this->color = glm::vec4(r, g, b, 1.0f);
    }
    //CUBE
    void Light::Init()
    {
       
    }

    //CUBEMANAGER
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

        auto arrayElement = engine->GetComponentManager()->GetComponents<Light>();
        for (auto element : arrayElement)
        {
            int PixelPerSize = config->PixelSize;
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);
            //transf = glm::rotate(transf, glm::radians(axis), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(project), (float)1000 / (float)1000, 0.1f, 100.0f);

            transf = glm::rotate(transf, glm::radians(element.axis_x + time / 5), glm::vec3(1.0f, 0.0f, 0.0f));

            // draw our first triangle
            this->shader->UseProgram();

            // get matrix's uniform location and set matrix
            this->shader->SetMat4("transform", transf);
            this->shader->SetMat4("view", view);
            this->shader->SetMat4("projection", projection);

            this->shader->SetVec3("color", glm::vec3(element.color.r, element.color.g, element.color.b));

            // render container
            this->shader->BindVertexArray(this->shader->VAO);
            this->shader->DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            this->shader->BindVertexArray(0);
        }
    }
}