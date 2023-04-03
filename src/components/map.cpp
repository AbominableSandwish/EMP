#include <components/map.h>
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
    int numX = 512,
        numY = 512,
        numOctaves = 1;
    double persistence = 0.3;

 const int heiht_map = 4;

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

    float velocity = 0.01f;
    glm::vec3 offset;
    int seed;
    Map::Map(int entity, float r, float g, float b)
    {
        this->entity = entity;
        this->color = glm::vec4(r, g, b, 1.0f);
    }
    //CUBE
    void Map::Init()
    {

    }

    //CUBEMANAGER
    MapManager::MapManager(Engine& engine, ConfigGraphic& config) : System(engine, "CubeManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void MapManager::Init()
    {
        bool warning = false;
        m_component = engine->GetComponentManager();

        {
            std::string vertexShaderSource = FileSystem::ReadShader("./shader/model2.vs");
            std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/multiplelight2-5.fs");  //multiplelight  

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

    }

    void MapManager::Start() {
        this->uniformBlockIndex = glGetUniformBlockIndex(shader->shaderProgram, "Matrices");
        // then we link each shader's uniform block to this uniform binding point
        glUniformBlockBinding(shader->shaderProgram, uniformBlockIndex, 0);
        // Now actually create the buffer
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // define the range of the buffer that links to a uniform binding point
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
        //CAMERA       
        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
        glm::mat4& view = MainCamera.GetView();
        //this->shader->SetMat4("view", view);
        //this->shader->SetMat4("projection", MainCamera.projection);

        // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
        //glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // store the projection matrix
        //glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),  &MainCamera.projection);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);


        seed = std::rand() / 500;
        int size = heiht_map;
        int level[heiht_map][heiht_map] = { 0 };
        int turn = 8;
        for (int t = 0; t < turn; t++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int up = seed;
                    level[i][j] += up;
                    double noise = ValueNoise_2D(i, j);
                    level[i][j] = noise * 10000;

                }
            }
        }

        this->array = new  std::vector<Transform>();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                this->array->push_back(Transform(i * 300 - (size * 300 / 2), -1100 + level[i][j], j * 300 - (size * 300 / 2), 0, 0, 0, 3.0f, 3.0f, 3.0f));
            }
        }
    }

    int move_x = 0;
    void MapManager::Refresh() {
        int size = heiht_map;
        int level[heiht_map][heiht_map] = { 0 };
        int turn = 8;
        for (int t = 0; t < turn; t++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int up = seed;
                    level[i][j] += up;
                    double noise = ValueNoise_2D(i + offset.x, j);
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

    void MapManager::Destroy()
    {
    }


    void MapManager::Update(float dt)
    {
        time += dt;
        offset.x = time * 15;
    }

    void MapManager::Draw()
    {
        this->shader->UseProgram();

      
        // render boxes

        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
        glm::vec3 pos = MainCamera.GetPosition();
       // this->shader->SetVec3("viewPos", pos);

        this->shader->BindVertexArray(this->shader->VAO);
        
        glm::mat4& view = MainCamera.GetView();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for (std::vector<Transform>::iterator it = array->begin(); it < array->end(); it++)
        {  
            //Matrice Transform
            glm::mat4& transf = it->matrice->GetMatrice();
            // transf = glm::rotate(transf, glm::radians(time * 50), glm::vec3(0.0f, 1.0f, 0.0f));

            //transf = glm::rotate(transf, glm::radians(sin(time)*180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            this->shader->SetMat4("transform", transf);
            if (update) {
                this->shader->SetVec3("objectColor", glm::vec3(1, 1, 1));

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
                    this->shader->SetVec3("pointLights["+id+"].position", engine->GetComponentManager()->GetComponent<Transform>(arrayLight[i].entity).Position() / 100.0f);
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
                //glActiveTexture(GL_TEXTURE1);
               // glBindTexture(GL_TEXTURE_2D, specular_map);
                
            }
            this->shader->DrawArrays(GL_TRIANGLES, 0, 36);
        }
        update = false;
        Refresh();
    }
}   