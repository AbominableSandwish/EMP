#include <components/circle.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include "glm/gtx/transform.hpp"
#include "graphic/shader.h";
#include <core/file.h>
#include <components/camera.h>
#include <components/light.h>

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
        std::string vertexShaderSource = FileSystem::ReadShader("./shader/model.vs");
        std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2.fs");  //multiplelight  

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float pi = 3.141569265f;
        float r = 0.25f;
        float theta = (2 * pi) / segment;
        float vertices[64 * 3 * 6] = { 0.0f };
        for (int ii = 0; ii < segment; ii++)
        {
                //Point O
                vertices[ii * 18 + 0] = 0;
                vertices[ii * 18 + 1] = 0;
                vertices[ii * 18 + 2] = 0;
                              
                vertices[ii * 18 + 3] = 1;
                vertices[ii * 18 + 4] = 1;
                vertices[ii * 18 + 5] = 1;
                              
                //Point A     
                vertices[ii * 18 + 6] = r * cosf(theta * (ii + 1));
                vertices[ii * 18 + 7] = r * sinf(theta * (ii + 1));
                vertices[ii * 18 + 8] = 0;
                              
                vertices[ii * 18 + 9] = 1;
                vertices[ii * 18 + 10] = 1;
                vertices[ii * 18 + 11] = 1;
                //Point B  
                vertices[ii * 18 + 12] = r * cosf(theta * (ii + 2));
                vertices[ii * 18 + 13] = r * sinf(theta * (ii + 2));
                vertices[ii * 18 + 14] = 0;
                              
                vertices[ii * 18 + 15] = 1;
                vertices[ii * 18 + 16] = 1;
                vertices[ii * 18 + 17] = 1;
        }

        this->shader = new Shader();
        bool warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
        if (warning) {
            LOG::Warning(name + " help!");
        }

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

        // at init time.
        glm::vec4 color = glm::vec4(1, 1, 1, 1);

        glGenTextures(0, &diffuse_map);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);

        // set texture wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filter parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // set image data
        int size = 156;
        unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
        for (unsigned int i = 0; i < size * size; i++)
        {
            data[i * 3] = (unsigned char)(color.x * 255.0f);
            data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
            data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] data;

        glGenTextures(1, &specular_map);
        glBindTexture(GL_TEXTURE_2D, specular_map);

        // set texture wrap parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filter parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        data = new unsigned char[3 * size * size * sizeof(unsigned char)];
        for (unsigned int i = 0; i < size * size; i++)
        {
            data[i * 3] = (unsigned char)(color.x * 255.0f);
            data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
            data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] data;

        this->shader->UseProgram();

        this->shader->SetInt("material.diffuse", 0);
        this->shader->SetInt("material.specular", 1);
    }

    void CircleManager::Destroy()
    {
    }

    void CircleManager::Draw()
    {

        int PixelPerSize = config->PixelSize;
        auto& arrayElement = engine->GetComponentManager()->GetComponents<Circle>();
        this->shader->UseProgram();


        //CAMERA
        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
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

        for (auto& element : arrayElement)
        {
            //Get Transform Data
            auto& transform = m_component->GetComponent<Transform>(element.entity);
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


            // Material properties
            this->shader->SetFloat("material.shininess", this->shader->shininess);
            this->shader->SetVec3("material.color", glm::vec3(element.color.r, element.color.g, element.color.b));

            auto arrayDirLight = engine->GetComponentManager()->GetComponents<DirectionalLight>();
            DirectionalLight Dirlight = arrayDirLight[0];
            // Dirlight properties
            this->shader->SetVec3("dirLight.direction", Dirlight.direction);
            this->shader->SetVec3("dirLight.ambient", Dirlight.ambient);
            this->shader->SetVec3("dirLight.diffuse", Dirlight.diffuse);
            this->shader->SetVec3("dirLight.specular", Dirlight.specular);

            // PointLight properties
            auto& arrayLight = engine->GetComponentManager()->GetComponents<PointLight>();
            PointLight& light = arrayLight[0];
            Vector3 lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();

            this->shader->SetVec3("pointLights[0].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z) / 100.0f);
            this->shader->SetVec3("pointLights[0].ambient", light.ambient);
            this->shader->SetVec3("pointLights[0].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[0].specular", light.specular);
            this->shader->SetFloat("pointLights[0].constant", light.constant);
            this->shader->SetFloat("pointLights[0].linear", light.linear);
            this->shader->SetFloat("pointLights[0].quadratic", light.quadratic);
            light = arrayLight[1];
            lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            this->shader->SetVec3("pointLights[1].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z) / 100.0f);
            this->shader->SetVec3("pointLights[1].ambient", light.ambient);
            this->shader->SetVec3("pointLights[1].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[1].specular", light.specular);
            this->shader->SetFloat("pointLights[1].constant", light.constant);
            this->shader->SetFloat("pointLights[1].linear", light.linear);
            this->shader->SetFloat("pointLights[1].quadratic", light.quadratic);
            light = arrayLight[2];
            lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            this->shader->SetVec3("pointLights[2].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z) / 100.0f);
            this->shader->SetVec3("pointLights[2].ambient", light.ambient);
            this->shader->SetVec3("pointLights[2].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[2].specular", light.specular);
            this->shader->SetFloat("pointLights[2].constant", light.constant);
            this->shader->SetFloat("pointLights[2].linear", light.linear);
            this->shader->SetFloat("pointLights[2].quadratic", light.quadratic);
            light = arrayLight[3];
            lightpos = engine->GetComponentManager()->GetComponent<Transform>(light.entity).GetPosition();
            this->shader->SetVec3("pointLights[3].position", glm::vec3(lightpos.x, lightpos.y, lightpos.z) / 100.0f);
            this->shader->SetVec3("pointLights[3].ambient", light.ambient);
            this->shader->SetVec3("pointLights[3].diffuse", light.diffuse);
            this->shader->SetVec3("pointLights[3].specular", light.specular);
            this->shader->SetFloat("pointLights[3].constant", light.constant);
            this->shader->SetFloat("pointLights[3].linear", light.linear);
            this->shader->SetFloat("pointLights[3].quadratic", light.quadratic);
            // spotLight
            auto& arraySpot = engine->GetComponentManager()->GetComponents<SpotLight>();
            SpotLight& spot = arraySpot[0];
            Vector3 pos = engine->GetComponentManager()->GetComponent<Transform>(spot.entity).GetPosition();
            this->shader->SetVec3("spotLight.position", glm::vec3(pos.x, pos.y, pos.z) / 100.0f);
            this->shader->SetVec3("spotLight.direction", spot.direction);
            this->shader->SetVec3("spotLight.ambient", spot.ambient);
            this->shader->SetVec3("spotLight.diffuse", spot.diffuse);
            this->shader->SetVec3("spotLight.specular", spot.specular);
            this->shader->SetFloat("spotLight.constant", spot.constant);
            this->shader->SetFloat("spotLight.linear", spot.linear);
            this->shader->SetFloat("spotLight.quadratic", spot.quadratic);
            this->shader->SetFloat("spotLight.cutOff", spot.cutOff);
            this->shader->SetFloat("spotLight.outerCutOff", spot.outerCutOff);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuse_map);
            // at init time.
            //glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
            glGenTextures(1, &specular_map);
            glBindTexture(GL_TEXTURE_2D, specular_map);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular_map);

            this->shader->DrawArrays(GL_TRIANGLES, 0, element.segment * 3);
            // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}