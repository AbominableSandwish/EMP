#include "core/system.h"
#include "glm/gtx/transform.hpp"

namespace emp {
    class Engine;
    class ConfigGraphic;
    class Shader;
  
    class PointLight {
    public:
        PointLight(int entity)
        {
            this->entity = entity;
        }

        PointLight(int entity, float r, float g, float b, glm::vec3 ambiant, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        void Init();

        int entity;

        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;
        
        //light
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;


        void SetAmbient(glm::vec3 ambient) {
            this->ambient = ambient;
        }
        void SetDiffuse(glm::vec3 diffuse) {
            this->diffuse = diffuse;
        }
        void SetSpecular(glm::vec3 specular) {
            this->specular = specular;
        }

        void SetConstant(float constant) {
            this->constant = constant;
        }

        void SetLinear(float linear) {
            this->linear = linear;
        }

        void SetQuadratic(float quadratic) {
            this->quadratic = quadratic;
        }
    };

    class SpotLight {
    public:
        SpotLight(int entity)
        {
            this->entity = entity;
        }

        SpotLight(int entity, float r, float g, float b, glm::vec3 direction, glm::vec3 ambiant, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
        void Init();

        int entity;

        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;

        //light
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;


        void SetAmbient(glm::vec3 ambient) {
            this->ambient = ambient;
        }
        void SetDiffuse(glm::vec3 diffuse) {
            this->diffuse = diffuse;
        }
        void SetSpecular(glm::vec3 specular) {
            this->specular = specular;
        }

        void SetConstant(float constant) {
            this->constant = constant;
        }

        void SetLinear(float linear) {
            this->linear = linear;
        }

        void SetQuadratic(float quadratic) {
            this->quadratic = quadratic;
        }

        void SetDirection(glm::vec3 direction) {
            this->direction = direction;
        }

        void SetCutOff(float cutOff) {
            this->cutOff = cutOff;
        }

        void SetOuterCutOff(float outerCutOff) {
            this->outerCutOff = outerCutOff;
        }
    };


    class DirectionalLight {
    public:
        DirectionalLight(int entity)
        {
            this->entity = entity;
        }

        DirectionalLight(int entity, glm::vec3 direction, glm::vec3 ambiant, glm::vec3 diffuse, glm::vec3 specular);
        void Init();

        int entity;

        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;

        glm::vec3 direction;
        //light
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        void SetAmbient(glm::vec3 ambient) {
            this->ambient = ambient;
        }
        void SetDiffuse(glm::vec3 diffuse) {
            this->diffuse = diffuse;
        }
        void SetSpecular(glm::vec3 specular) {
            this->specular = specular;
        }
        void SetDirection(glm::vec3 direction) {
            this->direction = direction;
        }

    };

    class ComponentManager;
    class LightManager : public System
    {
    public:
        LightManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Destroy() override;

        void Update(float) override;

        void Draw();

        Shader* shader = nullptr;

        std::string vertexCode;
        const char* vertexShaderSource;
        std::string fragmentCode;
        const char* fragmentShaderSource;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;
        float project = 45.0f;

        float time = 0.0f;;
    };
}