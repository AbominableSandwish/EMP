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
        LOG::Info("Loading configuration");
    	
        this->config_ = config;
        emp::ConfigGraphic* configGraphic = new emp::ConfigGraphic("Configuration Graphic", config->mode);
    	
        this->file_ = new FileManager(*this, "File Manager");
        this->logger_ = LogManager::GetInstance();
        this->m_entity = new EntityManager(*this, "Entity Manager");
        this->graphic_ = new GraphicManager(*this, "Graphic Manager", *configGraphic);
    	
        this->file_->Init();
        this->logger_->Init();
        this->m_entity->Init();

        for (int i = 10 - 1; i >= 0; --i)
        {
            this->m_entity->CreateEntity();
        }
        this->m_entity->MoveEntity(this->m_entity->GetEntities().size() - 1, 0);
        this->m_entity->MoveEntity(0, 5);
    	//Warning : Object
    	//todo component_manager missing
        this->m_entity->GetEntity(1)->AddComponent(new Transform(-1, 0));
        this->m_entity->GetEntity(2)->AddComponent(new Transform(1, 0));
        this->m_entity->GetEntity(3)->AddComponent(new Transform(0, 0));
        this->m_entity->GetEntity(4)->AddComponent(new Transform(64, 64));

    	this->graphic_->AddComponent(new SpriteGraphic(*(m_entity->GetEntity(1)), "./data/NewLogoPixelColoredx192v2.jpg"));
        this->graphic_->AddComponent(new SpriteGraphic(*(m_entity->GetEntity(2)),"./data/NewLogoPixelColoredx192v2.jpg"));
        this->graphic_->AddComponent(new TextGraphic(*(m_entity->GetEntity(3)), "Abominable Science", "TextGraphic"));
        //todo system_manager missing
        this->graphic_->Init();

        end = 0;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }

        LOG::Info(this->config_->GetName() + " is ready");
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

            this->logger_->Update(dt); 
            this->file_->Update(dt);
            this->graphic_->Update(dt);

             this->graphic_->Draw();

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
        this->logger_ = nullptr;
        this->graphic_->Destroy();
        this->graphic_ = nullptr;
        this->m_entity->Destroy();
        this->m_entity = nullptr;
        this->file_ = nullptr;
	}

    GLFWwindow* Engine::GetWindow()
    {
        return this->graphic_->GetWindow();
    }

    LogManager* Engine::GetLogManager()
    {
    	if(this->logger_ != nullptr)
	    return this->logger_;
        return nullptr;
	
    }

    EntityManager* Engine::GetEntityManager()
    {
        if (this->m_entity != nullptr)
            return this->m_entity;
        return nullptr;

    }

    GraphicManager* Engine::GetGraphicManager()
    {
        if (this->graphic_ != nullptr)
            return  this->graphic_;
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

