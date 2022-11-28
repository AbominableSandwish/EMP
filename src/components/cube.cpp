#include <components/cube.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <components/light.h>
#include <components/camera.h>
#include <math/matrice.h>
#include <graphic/shader.h>
#include <core/file.h>

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
            std::string vertexShaderSource = FileSystem::ReadShader("./shader/model.vs");
            std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2.fs");  //multiplelight  

            this->shader = new Shader();
            warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
            if (warning) {
                LOG::Warning(name + " help!");
            }

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
        int PixelPerSize = config->PixelSize;
        auto arrayElement = engine->GetComponentManager()->GetComponents<Cube>();
        this->shader->UseProgram();


        //CAMERA
        auto list_camera = m_component->GetComponents<Camera>();
        Camera MainCamera = list_camera[0];
        unsigned int viewPosLoc = glGetUniformLocation(this->shader->shaderProgram, "viewPos");
        glm::vec3 pos = MainCamera.GetPosition();
        glUniform3f(viewPosLoc, pos.x, pos.y, pos.z);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, pos);
      
        glm::mat4 projection = MainCamera.projection;
        // get matrix's uniform location and set matrix 
        unsigned int transformLoc = glGetUniformLocation(this->shader->shaderProgram, "transform");
        unsigned int viewLoc = glGetUniformLocation(this->shader->shaderProgram, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(this->shader->shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        // render boxes
        this->shader->BindVertexArray(this->shader->VAO);
        
        for (auto element : arrayElement)
        {
            //Get Transform Data
            auto transform = m_component->GetComponent<Transform>(element.entity);
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
            auto arrayLight = engine->GetComponentManager()->GetComponents<PointLight>();
            PointLight light = arrayLight[0];
            Vector3 lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();

            this->shader->SetVec3("lightPos", glm::vec3(lightpos.x, lightpos.y, lightpos.z));

            // Material properties
            this->shader->SetFloat("material.shininess", this->shader->shininess);
            this->shader->SetVec3("material.color", glm::vec3(element.color.r, element.color.g, element.color.b));

            // Dirlight properties
            this->shader->SetVec3("dirLight.direction", 0.2f, 1.0f, 0.3f);
            this->shader->SetVec3("dirLight.ambient", 0.0f, 0.0f, 1.0f);
            this->shader->SetVec3("dirLight.diffuse", 0.0f, 1.0f, 0.0f);
            this->shader->SetVec3("dirLight.specular", 1.0f, 0.0f, 0.0f);

            // PointLight properties
            this->shader->SetVec3("pointLight[0].position", glm::vec3(lightpos.x / 10.0f, lightpos.y / 10.0f, lightpos.z / 10.0f));
            this->shader->SetVec3("pointLight[0].ambient", light.ambient);
            this->shader->SetVec3("pointLight[0].diffuse", light.diffuse);
            this->shader->SetVec3("pointLight[0].specular", light.specular);
            this->shader->SetFloat("pointLight[0].constant", light.constant);
            this->shader->SetFloat("pointLight[0].linear", light.linear);
            this->shader->SetFloat("pointLight[0].quadratic", light.quadratic);

             light = arrayLight[1];
            this->shader->SetVec3("pointLight[1].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z));
            this->shader->SetVec3("pointLight[1].ambient", light.ambient);
            this->shader->SetVec3("pointLight[1].diffuse", light.diffuse);
            this->shader->SetVec3("pointLight[1].specular", light.specular);
            this->shader->SetFloat("pointLight[1].constant", light.constant);
            this->shader->SetFloat("pointLight[1].linear", light.linear);
            this->shader->SetFloat("pointLight[1].quadratic", light.quadratic);
          
            this->shader->DrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}   