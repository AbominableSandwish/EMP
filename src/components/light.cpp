#include <components/light.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/shader.h>
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>


namespace emp {
    Light::Light(int entity, float r, float g, float b)
    {
        this->entity = entity;
        this->color = glm::vec4(r, g, b, 1.0f);
    }
    //CUBE
    void Light::Init()
    {
        {
            vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "uniform mat4 transform;\n"
                "uniform mat4 view;\n"
                "uniform mat4 projection;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection * view * transform * vec4(aPos, 1.0);\n"
                "}\0";
            fragmentShaderSource = "#version 330 core\n"
                "uniform vec3 color;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = vec4(color, 1.0);\n"
                "}\n\0";
            
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
                0, 1, 3, 1, 2, 3,
                4, 5, 7, 5, 6, 7,
                0, 1, 4, 4, 5, 1,
                2, 3, 7, 6, 7, 2,
                1, 2, 6, 5, 6, 1,
                0, 3, 7, 7, 4, 0
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }
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

            transf = glm::rotate(transf, glm::radians(element.axis_x + time / 20), glm::vec3(1.0f, 0.0f, 0.0f));

            // draw our first triangle
            glUseProgram(element.shaderProgram);
            // glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

             // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int viewLoc = glGetUniformLocation(element.shaderProgram, "view");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(element.shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");
            glUniform3f(colorLoc, element.color.r, element.color.g, element.color.b);
            
            // render container
            glBindVertexArray(element.VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }
}