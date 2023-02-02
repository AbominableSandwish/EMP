#include <core/system.h>
#include "glm/gtx/transform.hpp"


namespace emp {
    class Shader;

    class Triangle {
    public:
        Triangle(int entity)
        {
            this->entity = entity;
        }
        void Init();

        float axis_x = 0.0f;
        float axis_y = 0.0f;
        float axis_z = 0.0f;
        float pi = 3.1415926f;
        

        int entity;
        glm::vec4 color;
        void SetColor(glm::vec4 color)
        {
            color = color;
        }
    };

    class ConfigGraphic;
    class ComponentManager;
    class TriangleManager : public System
    {
    public:
        TriangleManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Update(float dt);

        void Destroy();

        void Draw();
     
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        unsigned int diffuse_map;
        unsigned int specular_map;

        Shader* shader;

        float time = 0.0f;
        float pi = 3.1415926f;
    };
}