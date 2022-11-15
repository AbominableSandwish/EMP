#include <components/cube.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <components/light.h>
#include <math/matrice.h>
#include <graphic/shader.h>


namespace emp {
    Cube::Cube(int entity, float r, float g, float b)
    {
        this->entity = entity;
        this->color = glm::vec4(r, g, b, 1.0f);
    }
    //CUBE
    void Cube::Init()
    {
 
    }

    //CUBEMANAGER
    CubeManager::CubeManager(Engine& engine, ConfigGraphic& config) : System(engine, "CubeManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void CubeManager::Init()
    {
        bool warning = false;
        m_component = engine->GetComponentManager();

        {
            const char* vertexShaderSource = "#version 330 core\n"

                "layout (location = 0) in vec3 aPos;\n"
                "layout(location = 1) in vec3 aNormal;\n"

                "out vec3 FragPos;\n"
                "out vec3 Normal;\n"

                "uniform mat4 transform;\n"
                "uniform mat4 view;\n"
                "uniform mat4 projection;\n"
                "void main()\n"
                "{\n"
                "   FragPos = vec3(transform * vec4(aPos, 1.0));\n"
                "   Normal = mat3(transpose(inverse(transform))) * aNormal;\n"

                "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
                "}\0";
                const char* fragmentShader2Source = "#version 330 core\n"
                "in vec3 Normal;\n"
                "in vec3 FragPos;\n"

                "uniform vec3 objectColor;\n"
                "uniform vec3 viewPos;\n"
                "uniform vec3 lightPos;\n"
                "uniform vec3 lightColor;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = vec4(objectColor * lightColor, 1.0);\n"
                "}\n\0";

                const char* fragmentShaderSource = "#version 330 core\n"
                "in vec3 FragPos;\n"
                "in vec3 Normal;\n"

                "uniform vec3 objectColor;\n"
                "uniform vec3 viewPos;\n"
                "uniform vec3 lightPos;\n"
                "uniform vec3 lightColor;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                // ambient
                "float ambientStrength = 0.1;\n"
                "vec3 ambient = ambientStrength * lightColor;\n"

                // diffuse 
                "vec3 norm = normalize(Normal);"
                "vec3 lightDir = normalize(lightPos - FragPos);\n"
                " float diff = max(dot(norm, lightDir), 0.0);\n"
                "vec3 diffuse = diff * lightColor;\n"

                // specular
                "float specularStrength = 0.5;\n"
                "vec3 viewDir = normalize(viewPos - FragPos);\n"
                "vec3 reflectDir = reflect(lightDir, norm);\n"
                "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
                "vec3 specular = specularStrength * spec * lightColor;\n"

                "vec3 result = (ambient + diffuse + specular) * objectColor;\n"
                "FragColor = vec4(result, 1.0);\n"
                "}\n\0";

           

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,



                 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
            };

            this->shader = new Shader();
            warning = this->shader->Init(vertexShaderSource, fragmentShaderSource, vertices);
            if (warning) {
                LOG::Warning( name + " help!");
            }
           
        }
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
        this->shader->UseProgram();
        int PixelPerSize = config->PixelSize;

        auto arrayElement = engine->GetComponentManager()->GetComponents<Cube>();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(project), (float)1000 / (float)1000, 0.1f, 100.0f);

      

        //// get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(this->shader->shaderProgram, "transform");
        unsigned int viewLoc = glGetUniformLocation(this->shader->shaderProgram, "view");
      
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(this->shader->shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        //glBindVertexArray(this->shader->VAO);

        for (auto element : arrayElement)
        {
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
            transf = glm::rotate(transf, glm::radians(element.axis_x + time / 10), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));

            unsigned int objectColorLoc = glGetUniformLocation(this->shader->shaderProgram, "objectColor");
            glUniform3f(objectColorLoc, element.color.r, element.color.g, element.color.b);
            //CAMERA
            unsigned int viewPosLoc = glGetUniformLocation(this->shader->shaderProgram, "viewPos");
            glUniform3f(viewPosLoc, 0.0f, 0.0f, -3.0f);

        
            //LIGHT
            auto arrayLight = engine->GetComponentManager()->GetComponents<Light>();
            Light mainLight = arrayLight[0];
            Vector3 lightpos = engine->GetComponentManager()->GetComponent<Transform>(mainLight.entity).GetPosition();

            unsigned int lightPosLoc = glGetUniformLocation(this->shader->shaderProgram, "lightPos");
            glUniform3f(lightPosLoc, lightpos.x, lightpos.y, lightpos.z);
            unsigned int lightColorLoc = glGetUniformLocation(this->shader->shaderProgram, "lightColor");
            glUniform3f(lightColorLoc, mainLight.color.r, mainLight.color.g, mainLight.color.b);
          
            // render container
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}