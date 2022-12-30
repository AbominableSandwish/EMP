#include <ctime>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace emp
{
	class GraphicManager;
    class ConfigEngine;
    class LogManager;
    class FileManager;
    class EntityManager;
    class SystemManager;
    class ComponentManager;
    class InputSystem;
    class RigidBody2DManager;

	class Engine
	{
    public:
        Engine();
        virtual ~Engine() = default;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = default;
        Engine(const Engine&) = delete;

        void Init(ConfigEngine* m_config);
        void Start();
        void Update();
        void Stop();
        virtual void Destroy();
        bool is_running = false;

        LogManager* GetLogManager();
        EntityManager* GetEntityManager();
        GraphicManager* GetGraphicManager();
        ComponentManager* GetComponentManager();
    double GetFPS()
	{
        return this->fps;
	}
        float deltaTime = 0.0f;
    protected:
        ConfigEngine* m_config = nullptr;
        std::shared_ptr<LogManager> m_log;
        std::shared_ptr<FileManager> m_file;
        std::shared_ptr<EntityManager> m_entity;
        std::shared_ptr<RigidBody2DManager> m_rigidbody;
        std::shared_ptr<GraphicManager> m_graphic;
        std::unique_ptr<ComponentManager> m_component;
        std::shared_ptr<InputSystem> m_input;
        SystemManager* m_systems = nullptr;
	private:
        double fps = 0.0;
        
        clock_t start, end;
    };

}
