#include "core/system.h"
#include "glm/gtx/transform.hpp"

namespace emp {
    class Engine;
    class ConfigGraphic;
    class Shader;

    class Cube {
    public:
        Cube(int entity)
        {
            this->entity = entity;

            float red = 1.0f; // (std::rand() % 10);
            float green = 1.0f; // (std::rand() % 10);
            float blue = 1.0f; // (std::rand() % 10);

            this->color = glm::vec4(red, green, blue, 1.0f);
        }

        Cube(int entity, float r, float g, float b);

        void Init();

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

        Shader* shader = nullptr;
        float project = 45.0f;

        float time = 0.0f;;
    };
}