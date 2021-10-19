#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace emp
{
	class GraphicManager;
    class ConfigEngine;
    class Editor;
    class LogManager;

	class Engine
	{
    public:
        Engine();
        virtual ~Engine() = default;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = default;
        Engine(const Engine&) = delete;

        void Init(ConfigEngine* config);
        void Start();
        virtual void Destroy();
        GLFWwindow* GetWindow();
		
        bool is_running = false;

        LogManager* GetLogManager();
        
		
    protected:
        LogManager* m_Logger = nullptr;
        GraphicManager* m_Graphic = nullptr;
        ConfigEngine* m_Config = nullptr;
        Editor* m_Editor = nullptr;
    };
}
