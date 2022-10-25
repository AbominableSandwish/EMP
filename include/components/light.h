#include "core/system.h"
#include "glm/gtx/transform.hpp"

namespace emp {
    class Engine;
    class ConfigGraphic;
  
    class Light {
    public:
        Light(int entity)
        {
            this->entity = entity;

            float red = 1.0f; // (std::rand() % 10);
            float green = 1.0f; // (std::rand() % 10);
            float blue = 1.0f; // (std::rand() % 10);

            this->color = glm::vec4(red, green, blue, 1.0f);
        }

        Light(int entity, float r, float g, float b);
        void Init();

        

        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;


        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;

        int entity;
        glm::vec4 color;
        void SetColor(glm::vec4 color)
        {
            this->color = color;
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


    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;
        float project = 45.0f;

        float time = 0.0f;;
    };
}