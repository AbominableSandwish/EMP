
namespace mushgine
{
	class GraphicManager;
    class ConfigEngine;

	class Engine
	{
    public:
        Engine();

        virtual ~Engine() = default;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = default; //move constructor
        Engine(const Engine&) = delete; //delete copy constructor
        //explicit System(const System & system);

        /**
        * \brief Called to initialize the Engine
        */
        void Init(ConfigEngine* config);
        /**
		* \brief Starting the Engine after the Init
		*/
        void Start();
        /**
        * \brief Used instead of the destructor to delete all heap created structure and finalize
        */
        virtual void Destroy();
        /**
        * \brief Called before we load a scene
        */

        bool is_running = false;
    protected:
        GraphicManager* m_Graphic = nullptr;
        ConfigEngine* m_Config = nullptr;
    };
}
