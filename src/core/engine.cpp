#include "core/engine.h"
#include <iostream>
#include <ctime>
#include <graphic/graphic.h>
#include <core/log.h>
#include <editor/editor.h>
#include <core/file.h>
#include <core/entity.h>


namespace emp
{
    constexpr int WIDTH = 1000000;

    int random(int min, int max) //range : [min, max]
    {
        static bool first = true;
        if (first)
        {
            srand(time(NULL)); //seeding for the first time only!
            first = false;
        }
        return min + rand() % ((max + 1) - min);
    }

    int max_index(int arr[], int size) {
        size_t max = 0;

        for (int j = 0; j < size; ++j) {
            if (arr[j] > arr[max]) {
                max = j;
            }
        }
        return arr[max];
    }

    int max_value(int arr[], int size) {
        int max = arr[0];

        for (int j = 0; j < size; ++j) {
            if (arr[j] > max) {
                max = arr[j];
            }
        }
        return max;
    }
	
	Engine::Engine() = default;

    void Engine::Init(ConfigEngine* config)
	{	
        m_config = config;
        emp::ConfigGraphic* configGraphic = new emp::ConfigGraphic("Configuration Graphic", config->mode);
    	  
        m_systems = new SystemManager();

    	//File
        m_file = this->m_systems->RegisterSystem <FileManager>(*this, "File Manager");
        m_file->Init();

        //Log
        m_log = this->m_systems->RegisterSystem<LogManager>();
        m_log->Init();

    	//Entity
        m_entity = this->m_systems->RegisterSystem<EntityManager>(*this, "Entity Manager");
        m_entity->Init();

    	for (int i = 10 - 1; i >= 0; --i)
        {
            this->m_entity->CreateEntity();
        }
        this->m_entity->GetEntity(1)->AddComponent(new Transform(-1, 0));
        this->m_entity->GetEntity(2)->AddComponent(new Transform(1, 0));
        this->m_entity->GetEntity(3)->AddComponent(new Transform(0, 0));
        this->m_entity->GetEntity(4)->AddComponent(new Transform(64, 64));

        this->m_entity->MoveEntity(this->m_entity->GetEntities().size() - 1, 0);
        this->m_entity->MoveEntity(0, 5);
    	//Warning : Object
    	//todo component_manager missing

    	//Graphic
        m_graphic = this->m_systems->RegisterSystem<GraphicManager>();
    	this->m_graphic->AddComponent(new SpriteGraphic(*(m_entity->GetEntity(1)), "./data/NewLogoPixelColoredx192v2.jpg"));
        this->m_graphic->AddComponent(new SpriteGraphic(*(m_entity->GetEntity(2)),"./data/NewLogoPixelColoredx192v2.jpg"));
        this->m_graphic->AddComponent(new TextGraphic(*(m_entity->GetEntity(3)), "Abominable Science", "TextGraphic"));

        this->m_graphic->Init(*this, "Graphic Manager", *configGraphic);

        end = 0;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }

        LOG::Info(this->m_config->GetName() + " is ready");
	}

    float counter = 0.0f;
    clock_t oldTime;

    void Engine::Start()
	{
        this->is_running = true;
	}

    void Engine::Update()
    {
        if (this->is_running)
        {
            ///DELTATIME AND FPS COUNTING
            clock_t deltaTime = clock() - oldTime;
            fps = (1.0 / deltaTime) * 1000;
            oldTime = clock();
            start = clock();
            float dt = float(start - end);
            counter += dt;
            if(this->m_log != nullptr)
				this->m_log->Update(dt); 
            this->m_file->Update(dt);
        	
            //this->m_graphic->Update(dt);
        	
            if(this->m_graphic != nullptr)
				this->m_graphic->Draw();

            end = clock();
        }
    }

    void Engine::Stop()
    {
        this->Destroy();
    }

    void Engine::Destroy()
	{
        this->is_running = false;
        this->m_log = nullptr;
        this->m_graphic->Destroy();
        this->m_graphic = nullptr;
        this->m_entity->Destroy();
        this->m_entity = nullptr;
        this->m_file = nullptr;
	}

    GLFWwindow* Engine::GetWindow()
    {
        return this->m_graphic->GetWindow();
    }

    LogManager* Engine::GetLogManager()
    {
    	if(this->m_log != nullptr)
	    return this->m_log.get();
        return nullptr;
	
    }

    EntityManager* Engine::GetEntityManager()
    {
        if (this->m_entity != nullptr)
            return this->m_entity.get();
        return nullptr;

    }

    GraphicManager* Engine::GetGraphicManager()
    {
        if (this->m_graphic != nullptr)
            return  this->m_graphic.get();
        return nullptr;
    }
}

/*include  datetime import datetime
from Scripts.Color import bcolors
from Scripts.SystemManager import SystemManager


class Engine :

    _now = datetime.now()
    _current_time = _now.strftime("%H:%M:%S")
    _start_time = str(_now.today())

    def __init__(self, config) :
  
    return

    def main(self) :
    if self._run :
        self._Manager.__update__()
        return

        def shutdown(self) :
        print("[ ... ] Shutdown Engine")
        self._run = False
        self._Manager.__shutDown__()

        def GetSystem(self, number) :
        return self._Manager.GetSystem(number)

        #order = input("[ Enter for exit ] : ")
#if order == "":
        # Quit
        #self.__shutDown__()
        #self._engine._run = False
        #return
        */

