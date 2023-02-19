#include <core/system.h>
#include "glm/gtx/transform.hpp"

namespace emp {
    class Shader;
    class Square {
    public:
        Square(int entity)
        {
            this->entity = entity;
        }
        void Init();

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
        void Start() {};

        void Update(float dt) {
            deltaTime = dt;
            time += dt;
        };

        void Destroy();

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        unsigned int diffuse_map;
        unsigned int specular_map;

        Shader* shader;
        float deltaTime;
        float time = 0.0f;
    };
}