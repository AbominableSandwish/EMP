#include "glm/gtx/transform.hpp"
#include <core/system.h>

namespace emp {
    class Shader;
    class Circle {
    public:
        Circle(int entity)
        {
            this->entity = entity;
        }
        void Init();


        unsigned int segment = 64;
        
        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;

        int entity;
        glm::vec4 color;
        float radius;
        int ammountTriangle = 40;

        void SetColor(glm::vec4 color)
        {
            color = color;
        }

        void SetRadius(float radius)
        {
            this->radius = radius;
        }

        void SetSegment(int segnment)
        {
            this->segment = segment;
        }
    };

    class ComponentManager;
    class ConfigGraphic;

    class CircleManager : public System
    {
    public:
        CircleManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Start() {}

        void Destroy();

        void Update(float dt) {

        };

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        unsigned int segment = 16;
        unsigned int diffuse_map;
        unsigned int specular_map;

        Shader* shader = nullptr;

        float time = 0.0f;

      
    };
}