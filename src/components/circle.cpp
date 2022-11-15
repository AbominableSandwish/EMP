#include <components/circle.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include "glm/gtx/transform.hpp"
#include "graphic/shader.h";

namespace emp {

    void Circle::Init()
    {
        float red = (std::rand() % 10);
        float green = (std::rand() % 10);
        float blue = (std::rand() % 10);

        color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);


        
    }

    CircleManager::CircleManager(Engine& engine, ConfigGraphic& config) : System(engine, "CircleManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void CircleManager::Init()
    {
        std::string vertexShaderSource = ("#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 transform;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0");
        std::string fragmentShaderSource = "#version 330 core\n"
            "uniform vec4 color;\n"

            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = color;\n"
            "}\n\0";





        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float pi = 3.141569265f;
        float r = 0.25f;
        float theta = (2 * pi) / 359;
        float vertices[361 * 3] = { 0.0f };
        for (int ii = 0; ii < 361; ii++)

        {
            if (ii == 0) {
                vertices[ii * 3] = 0;
                vertices[ii * 3 + 1] = 0;
                vertices[ii * 3 + 2] = 0;
            }
            else {
                vertices[ii * 3] = (r * cosf(theta * (ii + 1)));///1.5f
                vertices[ii * 3 + 1] = r * sinf(theta * (ii + 1));
                vertices[ii * 3 + 2] = 0;
            }


        }

        unsigned int indices[360 * 3 * 3 + 1] = { 0 };
        for (size_t i = 0; i < 360 * 3; i++)
        {
            if (i == 0) {
                indices[0] = 0;
                indices[1] = 0;
                indices[2] = 0;
            }
            else {
                indices[i * 3] = 0;
                indices[i * 3 + 1] = i;
                indices[i * 3 + 2] = i + 1;
            }
        }

        this->Shader = new emp::Shader();
        bool warning = this->Shader->Init(vertexShaderSource, fragmentShaderSource, vertices, indices);
        if (warning) {
            LOG::Warning(name + " help!");
        }
    }

    void CircleManager::Destroy()
    {
    }

    void CircleManager::Draw()
    {
        auto arrayElement = engine->GetComponentManager()->GetComponents<Circle>();
        for (auto element : arrayElement)
        {
            int PixelPerSize = config->PixelSize;
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);

            transf = glm::rotate(transf, glm::radians(element.axis_x), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(config->projetcion), (float)1000 / (float)1000, 0.1f, 100.0f);
            this->Shader->UseProgram();
            this->Shader->BindVertexArray();
         
            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(Shader->shaderProgram, "transform");
            unsigned int viewLoc = glGetUniformLocation(Shader->shaderProgram, "view");
            unsigned int colorLoc = glGetUniformLocation(Shader->shaderProgram, "color");

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(Shader->shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 360 * 3, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
        }
    }
}