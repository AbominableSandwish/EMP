#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace emp
{
	class GraphicManager;
    class ConfigEngine;
    class LogManager;
    class FileManager;
    class EntityManager;

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
        void Update();
        void Stop();
        virtual void Destroy();
        GLFWwindow* GetWindow();
		
        bool is_running = false;

        LogManager* GetLogManager();
        EntityManager* GetEntityManager();
        GraphicManager* GetGraphicManager();
		

        double GetFPS()
	{
               return this->fps;
	}

    protected:
        ConfigEngine* config_ = nullptr;
        LogManager* logger_ = nullptr;
        FileManager* file_ = nullptr;
        EntityManager* entity_ = nullptr;
        GraphicManager* graphic_ = nullptr;
	private:
        double fps = 0.0;
        clock_t start, end;
    };

}
