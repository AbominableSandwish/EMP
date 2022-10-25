#include <core/system.h>
#include "glm/gtx/transform.hpp"

namespace emp {

    class Square {
    public:
        Square(int entity)
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
    class SquareManager : public System
    {
    public:
        SquareManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Update(float dt) {};

        void Destroy();

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;
    };
}