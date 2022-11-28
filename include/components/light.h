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

        float shininess;
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