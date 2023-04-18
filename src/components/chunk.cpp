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
    int numX = 512,
        numY = 512,
        numOctaves = 1;
    double persistence = 0.3;
    const int heiht_map = 16;

#define maxPrimeIndex 10
    int primeIndex = 0;

    int primes[maxPrimeIndex][3] = {
      { 995615039, 600173719, 701464987 },
      { 831731269, 162318869, 136250887 },
      { 174329291, 946737083, 245679977 },
      { 362489573, 795918041, 350777237 },
      { 457025711, 880830799, 909678923 },
      { 787070341, 177340217, 593320781 },
      { 405493717, 291031019, 391950901 },
      { 458904767, 676625681, 424452397 },
      { 531736441, 939683957, 810651871 },
      { 997169939, 842027887, 423882827 }
    };

    double Noise(int i, int x, int y) {
        int n = x + y * 57;
        n = (n << 13) ^ n;
        int a = primes[i][0], b = primes[i][1], c = primes[i][2];
        int t = (n * (n * n * a + b) + c) & 0x7fffffff;
        return 1.0 - (double)(t) / 1073741824.0;
    }

    double SmoothedNoise(int i, int x, int y) {
        double corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) +
            Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16,
            sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) +
                Noise(i, x, y + 1)) / 8,
            center = Noise(i, x, y) / 4;
        return corners + sides + center;
    }

    double Interpolate(double a, double b, double x) {  // cosine interpolation
        double ft = x * 3.1415927,
            f = (1 - cos(ft)) * 0.5;
        return  a * (1 - f) + b * f;
    }

    double InterpolatedNoise(int i, double x, double y) {
        int integer_X = x;
        double fractional_X = x - integer_X;
        int integer_Y = y;
        double fractional_Y = y - integer_Y;

        double v1 = SmoothedNoise(i, integer_X, integer_Y),
            v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
            v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
            v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
            i1 = Interpolate(v1, v2, fractional_X),
            i2 = Interpolate(v3, v4, fractional_X);
        return Interpolate(i1, i2, fractional_Y);
    }

    double ValueNoise_2D(double x, double y) {
        double total = 0,
            frequency = pow(2, numOctaves),
            amplitude = 1;
        for (int i = 0; i < numOctaves; ++i) {
            frequency /= 0.7f;
            amplitude *= persistence;
            total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
                x / frequency, y / frequency) * amplitude;
        }
        return total / frequency;
    }



   
    Chunck::Chunck(int entity, int x, int y)
    {

        this->entity = entity;

        this->position = glm::vec2(x, y);
    }
    //CUBE

    std::vector<glm::vec3> Chunck::LoadChunck(int x, int y) {
        // seed = std::rand() / 500;
        int size = heiht_map;

        std::vector<glm::vec3> chunck = std::vector<glm::vec3>();
        int level[heiht_map][heiht_map] = { 0 };
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double noise = ValueNoise_2D(i + x, j + y);
                level[i][j] = noise * 100;
            }
        }

        std::vector<glm::vec3> array = std::vector<glm::vec3>();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int height = level[i][j];
                while (height >= -1)
                {
                    array.push_back(glm::vec3(i + x, height, j + y));
                    height--;
                }

            }
        }

        return array;
    }

    void Chunck::Init()
    {
        bool warning = false;
        {
            std::string vertexShaderSource = FileSystem::ReadShader("./shader/model2-5.vs");
            std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2-5.fs");  //multiplelight  

            this->shader = new Shader();
            warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
            if (warning) {
                LOG::Warning("Chunck  help!");
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
            glBindVertexArray(0);
            //buffer transform array
            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);

            transformMatrices = new glm::mat4[amount];

            std::vector<glm::vec3> first_chunck = this->LoadChunck(position.x, position.y);
            const int size = std::sqrt(amount);
            /* const float offset = size / 1.5f;
               for (unsigned int x = 0; x < size; x++) {
               for (unsigned int y = 0; y < size; y++) {
                   transformMatrices[x* size +y] = glm::mat4(3.0f);
                   transformMatrices[x* size +y] = glm::translate(transformMatrices[x * size + y], glm::vec3((- offset) + 1 * x, -2,(- offset) + 1 * y));
               }
           }*/

            int i = 0;
            for each (auto cell in first_chunck)
            {
                const int x = cell.x;
                const int y = cell.z;
                transformMatrices[i] = glm::mat4(3.0f);
                transformMatrices[i] = glm::translate(transformMatrices[i], cell);
                i++;
            }

            glBufferData(GL_ARRAY_BUFFER, amount * 16 * sizeof(float), transformMatrices, GL_DYNAMIC_DRAW);
            int transform_location = glGetAttribLocation(this->shader->shaderProgram, "aTransform");
            glBindVertexArray(this->shader->VAO);
            for (unsigned int i = 0; i < 4; i++) {
                glEnableVertexAttribArray(transform_location + i);
                glVertexAttribPointer(transform_location + i, 4, GL_FLOAT, GL_FALSE,
                    16 * sizeof(float),
                    (const GLvoid*)(sizeof(GLfloat) * i * 4));
                glVertexAttribDivisor(transform_location + i, 1);
            }

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
            const int pixel_size = 156;
            unsigned char* data = new unsigned char[3 * pixel_size * pixel_size * sizeof(unsigned char)];
            for (unsigned int i = 0; i < pixel_size * pixel_size; i++)
            {
                data[i * 3] = (unsigned char)(color.x * 255.0f);
                data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
                data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixel_size, pixel_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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


            data = new unsigned char[3 * pixel_size * pixel_size * sizeof(unsigned char)];
            for (unsigned int i = 0; i < pixel_size * pixel_size; i++)
            {
                data[i * 3] = (unsigned char)(color.x * 255.0f);
                data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
                data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pixel_size, pixel_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            delete[] data;

            this->shader->UseProgram();

            this->shader->SetInt("material.diffuse", 0);
            this->shader->SetInt("material.specular", 1);
        }
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
        m_component = engine->GetComponentManager();
        this->chuncks =  std::vector<Chunck>();

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                this->chuncks.push_back(Chunck(-1, i*16-32, j*16-32));
                this->chuncks[this->chuncks.size() - 1].Init();
            }
        }
    }

    void ChunckManager::Start() {
       

    }

    void ChunckManager::Refresh() {
    }

    void ChunckManager::Destroy()
    {
    }


    void ChunckManager::Update(float dt)
    {
        time += dt;
        offset.x = time * 15;
    }

    void Chunck::Draw(ComponentManager* m_component) {
        int PixelPerSize = 100;
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

        auto& arrayDirLight = m_component->GetComponents<DirectionalLight>();
        DirectionalLight& Dirlight = arrayDirLight[0];
        // Dirlight properties
        this->shader->SetVec3("dirLight.direction", Dirlight.direction);
        this->shader->SetVec3("dirLight.ambient", Dirlight.ambient);
        this->shader->SetVec3("dirLight.diffuse", Dirlight.diffuse);
        this->shader->SetVec3("dirLight.specular", Dirlight.specular);

        // PointLight properties
        auto& arrayLight = m_component->GetComponents<PointLight>();
        for (size_t i = 0; i < 36; i++)
        {
            string id = std::to_string(i);
            this->shader->SetVec3("pointLights[" + id + "].position", m_component->GetComponent<Transform>(arrayLight[i].entity).Position() / 100.0f);
            this->shader->SetVec3("pointLights[" + id + "].ambient", arrayLight[i].ambient);
            this->shader->SetVec3("pointLights[" + id + "].diffuse", arrayLight[i].diffuse);
            this->shader->SetVec3("pointLights[" + id + "].specular", arrayLight[i].specular);
            this->shader->SetFloat("pointLights[" + id + "].constant", arrayLight[i].constant);
            this->shader->SetFloat("pointLights[" + id + "].linear", arrayLight[i].linear);
            this->shader->SetFloat("pointLights[" + id + "].quadratic", arrayLight[i].quadratic);
        }

        auto& arraySpot = m_component->GetComponents<SpotLight>();
        this->shader->SetVec3("spotLight.position", m_component->GetComponent<Transform>(arraySpot[0].entity).Position() / 100.0f);
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
        this->shader->DrawArraysInstanced(GL_TRIANGLES, 0, 36, amount);
        glBindVertexArray(0);
    }

    void ChunckManager::Draw()
    {
        for each (auto chunck in chuncks)
        {
            chunck.Draw(m_component);
        }
       
    }
}   