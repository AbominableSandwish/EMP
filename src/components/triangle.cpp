#include <components/triangle.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include <core/file.h>
#include <graphic/shader.h>

namespace emp {
    void Triangle::Init()
    {
            float red = (std::rand() % 10);
            float green = (std::rand() % 10);
            float blue = (std::rand() % 10);

            color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);
    }

    TriangleManager::TriangleManager(Engine& engine, ConfigGraphic& config) : System(engine, "TriangleManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void TriangleManager::Init()
    {
        m_component = engine->GetComponentManager();

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            0.0f,                0.5f,                0.0f,  // bottom right
            cos(pi / 6) * 0.5f,  -sin(pi / 6) * 0.5f, 0.0f,  // middle top
            -cos(pi / 6) * 0.5f, -sin(pi / 6) * 0.5f, 0.0f,  // bottom left
        };

        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2,  // first Triangle
        };

       /* this->shaderProgram = new Shader();
        bool warning = this->shaderProgram->Init(FileSystem::ReadShader("./shader/light/light.vs"), FileSystem::ReadShader("./shader/color.fs"), vertices, indices);
        if (warning) {
            LOG::Warning( name + " help!");
        }*/
    }


    void TriangleManager::Destroy()
    {
    }

    void TriangleManager::Draw()
    {
        //// draw our first triangle
        //this->shaderProgram->UseProgram();
        //auto arrayElement = engine->GetComponentManager()->GetComponents<Triangle>();
        //for (auto element : arrayElement)
        //{
        //    int PixelPerSize = config->PixelSize;
        //    auto transform = m_component->GetComponent<Transform>(element.entity);
        //    Vector3 position = transform.GetPosition();
        //    Vector3 scale = transform.GetScale();
        //    std::vector<Vector4> matrice = transform.matrice->matrice4;
        //    glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
        //        matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
        //        matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
        //        position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a); // make sure to initialize matrix to identity matrix first

        //    transf = glm::rotate(transf, glm::radians(element.axis_x), glm::vec3(1.0f, 0.0f, 0.0f));
        //    glm::mat4 view = glm::mat4(1.0f);
        //    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //    glm::mat4 projection = glm::mat4(1.0f);
        //    projection = glm::perspective(glm::radians(config->projetcion), (float)1000 / (float)1000, 0.1f, 100.0f);

  
        //    this->shaderProgram->SetMat4("transform", transf);
        //    this->shaderProgram->SetMat4("view", view);
        //    this->shaderProgram->SetVec4("color", element.color);


        //    this->shaderProgram->DrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //}
    }
}