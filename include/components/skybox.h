#include "glm/gtx/transform.hpp"
#include <core/system.h>

namespace emp {
    class Shader;
    class Skybox {
    public:
        Skybox(int entity)
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

        void SetColor(glm::vec4 color)
        {
            color = color;
        }
    };

    class ComponentManager;
    class ConfigGraphic;

    class SkyboxManager : public System
    {
    public:
        SkyboxManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Destroy();

        void Update(float dt);

        void Draw();
    private:
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        unsigned int diffuse_map;
        unsigned int specular_map;

        Shader* shader = nullptr;

        float time = 0.0f;
        
        unsigned int skyboxVAO, skyboxVBO;
        unsigned int cubemapTexture;

    };
}