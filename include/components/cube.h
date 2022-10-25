#include "core/system.h"
#include "glm/gtx/transform.hpp"

namespace emp {
    class Engine;
    class ConfigGraphic;
  
    class Cube {
    public:
        Cube(int entity)
        {
            this->entity = entity;
        }
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
            color = color;
        }
    };


    class ComponentManager;
    class CubeManager : public System
    {
    public:
        CubeManager(Engine& engine, ConfigGraphic& config);

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