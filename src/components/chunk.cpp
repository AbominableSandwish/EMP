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

    Chunck::Chunck(int entity, int x, int y, int seed)
    {
        this->entity = entity;
        this->position = glm::vec2(x, y);
        this->seed = seed;
    }

    Chunck::~Chunck() {
        this->Destroy();
    }
    //CUBE

    std::vector<glm::vec3> Chunck::LoadChunck(int x, int y) {
        int size = heiht_map;
        std::vector<glm::vec3> chunck = std::vector<glm::vec3>();
        int level[heiht_map][heiht_map] = { 0 };
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double noise = ValueNoise_2D(i + x + seed, j + y + seed);
                level[i][j] = noise * 100;
            }
        }

        std::vector<glm::vec3> vector = std::vector<glm::vec3>();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int height = level[i][j];
                while (height >= 0 ) {
                    vector.push_back(glm::vec3(i + x, height, j + y));
                    height--;
                }

            }
        }

        return vector;
    }

    //std::queue<glm::vec3> Chunck::LoadChunck(glm::vec3 pos, glm::vec2 parent) {

    //    position = pos;
    //    distance = pos.z;
    //    //LoadMeshRenderer

    //    //ChildChunk
    //    std::queue<glm::vec3> vector = std::queue<glm::vec3>();
    //    for (size_t x = -1; x <= 1; x++)
    //    {
    //        if (x != 0) {
    //            for (size_t y = -1; y <= 1; y++)
    //            {
    //                if (y != 0) {
    //                    glm::vec2 child(pos.x + x, pos.y + y);            
    //                    if (child != parent) {
    //                        if (distance > field_of_view)
    //                            vector.push(glm::vec3(child.x, child.y, distance + 1));
    //                    }
    //                }
    //            }
    //        }
    //    }
    //    return vector;
    //}

    void Chunck::Destroy() {
        this->shader->Free();
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
            std::vector<float> vertices = {
                //back                                    
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
                

                //front
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v1
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v3
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v2
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v4
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v6
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //v5

                //left
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                //right
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //v1
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //v3
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //v2
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, //v4
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //v6
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //v5
                 //bottom
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                //up
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //v1
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //v3
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, //v2
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //v5
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //v4
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f  //v6
            };
            
            shader->GenVertexArrays();
            shader->GenBuffers();
            shader->SetVertexArrays(vertices);

            transformMatrices = new glm::mat4[amount];

            std::vector<glm::vec3> first_chunck = this->LoadChunck(position.x, position.y);
            const int size = std::sqrt(amount);

            int i = 0;
            for each (auto cell in first_chunck)
            {
                const int x = cell.x;
                const int y = cell.z;
                transformMatrices[i] = glm::mat4(1.0f);
                transformMatrices[i] = glm::translate(transformMatrices[i], cell);
                i++;
            }
            number = i;

            this->shader->SetBuffetDataMat4("aTransform", *transformMatrices);
            this->shader->BindVertexArray(0);
            this->shader->GenerateTexture();
        }

        loaded = true;
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
        this->seed = std::rand() % 500;
        this->chunks =  std::vector<Chunck*>();
        for (int i = 0; i < 4; i++)
        {
                this->chunks.push_back(std::move(new Chunck(-1, i * 16 - 32, 1 * 16 - 32, seed)));
                this->chunks[this->chunks.size() - 1]->Init();
        }
    }

    void ChunckManager::Start() {}

    void ChunckManager::Refresh() {
    }

    
    void ChunckManager::Destroy(){
        for each (auto & var in  this->chunks)
        {
              delete(var);
        }
          this->chunks.clear();
    }

    void ChunckManager::Update(float dt)
    {
        time += dt;
        offset.x = time * 15;
        if (time >= 5.5f) {
            time = 0.0f;
            refresh = true;
        }
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

        this->shader->BindTexture();
        this->shader->BindVertexArray();
        this->shader->DrawArraysInstanced(GL_TRIANGLES, 0, 36, number);
        this->shader->BindVertexArray(0);       
    }

    void ChunckManager::Draw()
    {
        for each (auto& chunck in chunks)
        {
            chunck->Draw(m_component);
        }
    }
}