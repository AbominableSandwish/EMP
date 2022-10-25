#include <components/triangle.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>

namespace emp {
    void Triangle::Init()
    {
        {
            float red = (std::rand() % 10);
            float green = (std::rand() % 10);
            float blue = (std::rand() % 10);

            color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);
            vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "uniform mat4 transform;\n"
                "uniform mat4 view;\n"
                "uniform mat4 projection;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";
            fragmentShaderSource = "#version 330 core\n"
                "uniform vec4 color;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = color;\n"
                "}\n\0";
            // build and compile our shader program
            // ------------------------------------
            // vertex shader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            emp::Shader::CheckVertexCompile(vertexShader);
            // fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            emp::Shader::CheckFragmentCompile(fragmentShader);
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
            emp::Shader::ChechShaderCompile(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                 0.0f, 0.5f, 0.0f,  // bottom right
                 cos(pi / 6) * 0.5f, -sin(pi / 6) * 0.5f, 0.0f,  // middle top
                -cos(pi / 6) * 0.5f, -sin(pi / 6) * 0.5f, 0.0f,  // bottom left
            };
            unsigned int indices[] = {  // note that we start from 0!
                0, 1, 2,  // first Triangle
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

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);
        }
    }
    //
    TriangleManager::TriangleManager(Engine& engine, ConfigGraphic& config) : System(engine, "TriangleManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void TriangleManager::Init()
    {
        m_component = engine->GetComponentManager();
    }


    void TriangleManager::Destroy()
    {
    }


    void TriangleManager::Draw()
    {

        auto arrayElement = engine->GetComponentManager()->GetComponents<Triangle>();
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
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(config->projetcion), (float)1000 / (float)1000, 0.1f, 100.0f);

            // draw our first triangle
            glUseProgram(element.shaderProgram);
            glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int viewLoc = glGetUniformLocation(element.shaderProgram, "view");
            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");


            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(element.shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
        }
    }

}