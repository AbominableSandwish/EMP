#include <components/chunck.h>
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
#include <stdio.h>
#include <stdlib.h>

namespace emp {
    const int heiht_map = 32;
   
    Chunck::Chunck(int entity, float r, float g, float b)
    {
        this->entity = entity;
        this->color = glm::vec4(r, g, b, 1.0f);
    }
    //CUBE
    void Chunck::Init()
    {

    }

    //CUBEMANAGER
    ChunckManager::ChunckManager(Engine& engine, ConfigGraphic& config) : System(engine, "Chunck Manager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }
    glm::mat4* two_d_array;    // the type is a pointer to an int (the element type)
    void ChunckManager::Init()
    {
       // new glm::mat4[heiht_map * heiht_map];
       // modelMatrices = new glm::mat4[heiht_map][heiht_map];

       // x , y = 32
       // tableau[x + y * 32] = 1024ieme element  [1 ... 1024]  
       // tableau[x][y] = 1024ieme element [1 ... 32][1 ... 32]

        
        //// allocate in a single malloc of N x M int-sized elements:
        //two_d_array = (glm::mat4*)malloc(sizeof(glm::mat4) * ROWS * COLS);

        //if (two_d_array == NULL) {
        //    printf("ERROR: malloc failed!\n");
        //    exit(1);
        //}

        //int i, j;
        //for (i = 0; i < ROWS; i++)
        //    for (j = 0; j < COLS; j++)
        //        (*(two_d_array + i * COLS + j)) = glm::translate(glm::mat4(10.0f), glm::vec3(i , 0, j));
        
        bool warning = false;


        m_component = engine->GetComponentManager();
        {
            std::string vertexShaderSource = FileSystem::ReadShader("./shader/model2-5.vs");
            std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2-5.fs");  //multiplelight  

            this->shader = new Shader();
            warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
            if (warning) {
                LOG::Warning(name + " help!");
            }

            unsigned int amount = 10;

            transformMatrices = new glm::mat4[amount];
            for (unsigned int i = 0; i < amount; i++) {
                transformMatrices[i] = glm::mat4(1.0f);
                transformMatrices[i] = glm::translate(transformMatrices[i], glm::vec3(i, i , 0));
            }

            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transformMatrices[0], GL_STATIC_DRAW);


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
            glBindVertexArray(0);
            //////SALUT MON  pote 
            glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
            ////// note that we update the lamp's position attribute's stride to reflect the updated buffer data 
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);


            //unsigned int VAO = this->shader->VAO;
            glBindVertexArray(this->shader->VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);



               
            // at init time
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

    }

    void ChunckManager::Start() {
       

    }

    void ChunckManager::Refresh() {
        int size = heiht_map;
        int level[heiht_map][heiht_map] = { 0 };
        int turn = 8;
        for (int t = 0; t < turn; t++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int up = seed;
                    level[i][j] += up;
                    double noise = 0;
                    level[i][j] = noise * 10000;
                }
            }
        }

        int i = 0;
        for (std::vector<Transform>::iterator it = array->begin(); it < array->end(); it++)
        {
            int x = i % heiht_map;
            int y = i / heiht_map;
            glm::vec3 pos = glm::vec3(x * 300 - (size * 300 / 2), -700 + level[x][y], y * 300 - (size * 300 / 2));
            it->SetPosition(pos);
            i++;
        }
    }

    void ChunckManager::Destroy()
    {
    }


    void ChunckManager::Update(float dt)
    {
        time += dt;
        offset.x = time * 15;
    }

    void ChunckManager::Draw()
    {
        int PixelPerSize = config->PixelSize;
        this->shader->UseProgram();

        //CAMERA
        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];


        glm::mat4 projection = MainCamera.projection;
        // get matrix's uniform location and set matrix
        glm::mat4 view = MainCamera.GetView();
        this->shader->SetMat4("view", view);
        this->shader->SetMat4("projection", projection);

        this->shader->UseProgram();
        this->shader->SetInt("material.diffuse", 0);
        //LIGHT
        // Material properties
        this->shader->SetFloat("material.shininess", this->shader->shininess);
        this->shader->SetVec3("material.color", glm::vec3(1, 1, 1));

        auto& arrayDirLight = engine->GetComponentManager()->GetComponents<DirectionalLight>();
        DirectionalLight& Dirlight = arrayDirLight[0];
        // Dirlight properties
        this->shader->SetVec3("dirLight.direction", Dirlight.direction);
        this->shader->SetVec3("dirLight.ambient", Dirlight.ambient);
        this->shader->SetVec3("dirLight.diffuse", Dirlight.diffuse);
        this->shader->SetVec3("dirLight.specular", Dirlight.specular);

        // PointLight properties
        auto& arrayLight = engine->GetComponentManager()->GetComponents<PointLight>();
        for (size_t i = 0; i < 36; i++)
        {
            string id = std::to_string(i);
            this->shader->SetVec3("pointLights[" + id + "].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[i].entity).Position() / 100.0f);
            this->shader->SetVec3("pointLights[" + id + "].ambient", arrayLight[i].ambient);
            this->shader->SetVec3("pointLights[" + id + "].diffuse", arrayLight[i].diffuse);
            this->shader->SetVec3("pointLights[" + id + "].specular", arrayLight[i].specular);
            this->shader->SetFloat("pointLights[" + id + "].constant", arrayLight[i].constant);
            this->shader->SetFloat("pointLights[" + id + "].linear", arrayLight[i].linear);
            this->shader->SetFloat("pointLights[" + id + "].quadratic", arrayLight[i].quadratic);
        }

        auto& arraySpot = engine->GetComponentManager()->GetComponents<SpotLight>();
        this->shader->SetVec3("spotLight.position", engine->GetComponentManager()->GetComponent<Transform>(arraySpot[0].entity).Position() / 100.0f);
        this->shader->SetVec3("spotLight.direction", arraySpot[0].direction);
        this->shader->SetVec3("spotLight.ambient", arraySpot[0].ambient);
        this->shader->SetVec3("spotLight.diffuse", arraySpot[0].diffuse);
        this->shader->SetVec3("spotLight.specular", arraySpot[0].specular);
        this->shader->SetFloat("spotLight.constant", arraySpot[0].constant);
        this->shader->SetFloat("spotLight.linear", arraySpot[0].linear);
        this->shader->SetFloat("spotLight.quadratic", arraySpot[0].quadratic);
        this->shader->SetFloat("spotLight.cutOff", arraySpot[0].cutOff);
        this->shader->SetFloat("spotLight.outerCutOff", arraySpot[0].outerCutOff);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map);
        // at init time.
        //glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
        glGenTextures(1, &specular_map);
        glBindTexture(GL_TEXTURE_2D, specular_map);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specular_map);


        glBindVertexArray(this->shader->VAO);
        this->shader->DrawArraysInstanced(GL_TRIANGLES, 0, 36, 20);
        glBindVertexArray(0);
    }
}   