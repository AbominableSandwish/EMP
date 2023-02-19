#include "core/system.h"
#include "glm/gtx/transform.hpp"

namespace emp {
    class Engine;
    class ConfigGraphic;
    class Shader;
    class Transform;

    class Map {
    public:
        Map(int entity)
        {
            this->entity = entity;

            float red = 1.0f; // (std::rand() % 10);
            float green = 1.0f; // (std::rand() % 10);
            float blue = 1.0f; // (std::rand() % 10);

            this->color = glm::vec4(red, green, blue, 1.0f);
        }

        Map(int entity, float r, float g, float b);

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
    class MapManager : public System
    {
    public:
        MapManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Start();

        void Refresh();

        void Destroy() override;

        void Update(float) override;

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        Shader* shader = nullptr;
        float project = 45.0f;

        // first. We get the relevant block indices
        unsigned int uniformBlockIndex;
        // Now actually create the buffer
        unsigned int uboMatrices;
        unsigned int diffuse_map;
        unsigned int specular_map;

        float time = 0.0f;
        bool update = true;
        std::vector<Transform> *array = nullptr;
    };
}