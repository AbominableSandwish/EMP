#include "glm/gtx/transform.hpp"
#include <core/system.h>

namespace emp {
    class Engine;
    class TemplateScript {
    public:
        TemplateScript(Engine& engine, int entity);
        
        void virtual Init() = 0;
        void virtual Update(float dt) = 0;
        void virtual Destroy() = 0;
    protected:
        int entity;
        Engine* engine = nullptr;
    };

    class Script : public TemplateScript {
    public:
        Script(Engine& engine, int entity);

        void Init() override;
        void Update(float dt) override;
        void Destroy() override;
    };

    class Camera;
    class PlayerScript : public TemplateScript {
    public:
        PlayerScript(Engine& engine, int entity);

        void Init() override;
        void Update(float dt) override;
        void Destroy() override;
        Camera* main_camera = nullptr;
    };

    class ComponentManager;
    class ScriptSystem : public System
    {
    public:
        ScriptSystem(Engine& engine, std::string name);

        void Init();

        void Destroy();

        void Update(float dt);
    private:
        ComponentManager* m_component = nullptr;

        float time = 0.0f;


    };
}