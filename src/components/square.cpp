#include <components/square.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include "glm/gtx/transform.hpp"
#include "graphic/shader.h"


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

        std::string vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 transform;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        std::string fragmentShaderSource = "#version 330 core\n"
            "uniform vec4 color;\n"

            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = color;\n"
            "}\n\0";

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2,  // first Triangle
            1, 2, 3   // second Triangle
        };

        this->shader = new Shader();
        this->shader->Init(vertexShaderSource, fragmentShaderSource, vertices, indices);
    }


    void SquareManager::Destroy()
    {
    }

    void SquareManager::Draw()
    {

        auto arrayElement = engine->GetComponentManager()->GetComponents<Square>();
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
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a); // make sure to initialize matrix to identity matrix first
            transf = glm::rotate(transf, glm::radians(element.axis_x), glm::vec3(1.0f, 0.0f, 0.0f));
            //transf = glm::translate(transf, glm::vec3(position.x / 100, position.y / 100, 0.0f));
            //transf = glm::scale(transf, glm::vec3(scale.x, scale.y, 0.0f));
            //transf = glm::rotate(transf, transform.angle_z, glm::vec3(0.0f, 0.0f, 1.0));
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(config->projetcion), (float)1000 / (float)1000, 0.1f, 100.0f);

            // draw our first triangle
            glUseProgram(shader->shaderProgram);
            glBindVertexArray(shader->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(shader->shaderProgram, "transform");
            unsigned int viewLoc = glGetUniformLocation(shader->shaderProgram, "view");
            unsigned int colorLoc = glGetUniformLocation(shader->shaderProgram, "color");


            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shader->shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLE_FAN, 40, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
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