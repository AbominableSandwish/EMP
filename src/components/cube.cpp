#include <components/cube.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>

namespace emp {
    //CUBE
    void Cube::Init()
    {
        {
            float red = 1.0f; // (std::rand() % 10);
            float green = 1.0f; // (std::rand() % 10);
            float blue = 1.0f; // (std::rand() % 10);

            this->color = glm::vec4(red, green, blue, 1.0f);
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
                "uniform vec3 lightColor;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = vec4(color * lightColor, 1.0);\n"
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
    CubeManager::CubeManager(Engine& engine, ConfigGraphic& config) : System(engine, "CubeManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void CubeManager::Init()
    {
        m_component = engine->GetComponentManager();
    }


    void CubeManager::Destroy()
    {
    }

    void CubeManager::Update(float dt)
    {
        time += dt;
    }

    void CubeManager::Draw()
    {

        auto arrayElement = engine->GetComponentManager()->GetComponents<Cube>();
        for (auto element : arrayElement)
        {
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
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
            unsigned int lightLoc = glGetUniformLocation(element.shaderProgram, "lightColor");
            glUniform3f(colorLoc, element.color.r, element.color.g, element.color.b);
            glUniform3f(lightLoc, 1.0f, 0.0f, 0.0f);
            
            // render container
            glBindVertexArray(element.VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }
}