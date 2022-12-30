#include "glm/gtx/transform.hpp"
#include <core/system.h>

namespace emp {
    class Shader;
    class Sphere {
    public:
        Sphere(int entity)
        {
            this->entity = entity;
        }
        void Init();
       
        
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
    };

    class ComponentManager;
    class ConfigGraphic;

    class SphereManager : public System
    {
    public:
        SphereManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Destroy();

        void Update(float dt) {
            this->time += dt;
        };

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;


        unsigned int diffuse_map;
        unsigned int specular_map;

        Shader* shader = nullptr;

        float time = 0.0f;

        const unsigned int segment = 4;
    };
}