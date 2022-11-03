// Core
#include <core/engine.h>
#include <core/input.h>
#include <graphic/graphic.h>
#include <core/log.h>
#include <editor/editor.h>
#include <core/file.h>
#include <core/entity.h>
#include <core/component.h>
#include <core/config.h>
// Graphic
#include <graphic/sprite.h>
// Physic
#include <components/rigidbody.h>
#include <components/transform.h>
// Component;
#include <components/triangle.h>
#include <components/square.h>
#include <components/circle.h>
#include <components/cube.h>
#include <components/light.h>
#include <components/model.h>
// External
#include <iostream>
#include <ctime>

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

    	m_component = std::make_unique<ComponentManager>();
        m_systems = new SystemManager();

    	//File
        m_file = this->m_systems->RegisterSystem <FileManager>(*this, "File Manager");
        m_file->Init();

        //Log
        m_log = this->m_systems->RegisterSystem<LogManager>(*this, "Log Manager");
        m_log->Init();

    	//Entity
        m_entity = this->m_systems->RegisterSystem<EntityManager>(*this, "Entity Manager");
        m_entity->Init();

        //Entity
        m_rigidbody= this->m_systems->RegisterSystem<RigidBody2DManager>(*this, "Rigid Manager");
        
    	//Graphic
        emp::ConfigGraphic* configGraphic = new emp::ConfigGraphic("Configuration Graphic", config->mode);
        m_graphic = this->m_systems->RegisterSystem<GraphicManager>(*this, *configGraphic);

        this->m_component->RegisterComponent<Transform>();
        this->m_component->RegisterComponent<SpriteRenderer>();
        this->m_component->RegisterComponent<Square>();
        this->m_component->RegisterComponent<Circle>();
        this->m_component->RegisterComponent<Triangle>();

        this->m_component->RegisterComponent<Cube>();
        this->m_component->RegisterComponent<Light>();
        this->m_component->RegisterComponent<Model>();


        this->m_component->RegisterComponent<RigidBody2D>();
        this->m_graphic->Init();

        m_input = this->m_systems->RegisterSystem<InputSystem>(*this, "Input System");
        m_input->Init();
        m_rigidbody->Init();

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
        LOG::Info("Loading Entities:");
        int i;
        float dt = 0.0f;
        this->m_graphic->Update(dt);
        //for (i = 0; i < 1; ++i)
        //{
        //    this->m_entity->CreateEntity("Sprite_"+ std::to_string(i));
        //    this->m_component->AddComponent(i, Transform(10*i, 10* i, 0));
        //    this->m_component->AddComponent(i, SpriteRenderer(i, "./data/NewLogoPixelColoredx192v2.jpg"));
        //    
        //}

        int entity = this->m_entity->CreateEntity("Cube").id;
        this->m_component->AddComponent(entity, Transform(-75, 0, 0, 0, 0, 0, 0.3f, 0.3f, 0.3f));
        this->m_component->AddComponent(entity, Cube(entity, 1.0f, 1.0f, 1.0f));

        entity = this->m_entity->CreateEntity("Cube").id;
        this->m_component->AddComponent(entity, Transform(-25, 0, 0, 0, 0, 0, 0.3f, 0.3f, 0.3f));
        this->m_component->AddComponent(entity, Cube(entity, 0.66f, 0.66f, 0.66f));

        entity = this->m_entity->CreateEntity("Cube").id;      
        this->m_component->AddComponent(entity, Transform(25, 0, 0, 0, 0, 0, 0.3f, 0.3f, 0.3f));
        this->m_component->AddComponent(entity, Cube(entity, 0.33f, 0.33f, 0.33f));
        
        entity = this->m_entity->CreateEntity("Cube").id;
        this->m_component->AddComponent(entity, Transform(75, 0, 0, 0, 0, 0, 0.3f, 0.3f, 0.3f));
        this->m_component->AddComponent(entity, Cube(entity, 0.1f, 0.1f, 0.1f));

        entity = this->m_entity->CreateEntity("Light").id;
        this->m_component->AddComponent(entity, Transform(0, 75, 75, 0, 0, 0, 0.1f, 0.1f, 0.1f));
        this->m_component->AddComponent(entity, Light(entity, 0.5f, 0, 0.5f));
        
        entity = this->m_entity->CreateEntity("Model").id;
        this->m_component->AddComponent(entity, Transform(0, 0, 75, 0, 0, 0, 0.2f, 0.2f, 0.2f));
        this->m_component->AddComponent(entity, Model(entity, "./data/backpack.obj"));


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

            this->m_input->Update(dt);
            this->m_rigidbody->Update(dt);
            this->m_graphic->Update(dt);
        	
            if(this->m_graphic != nullptr)
				this->m_graphic->Draw();

            end = clock();

            LOG::Debug(std::to_string(dt));
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
        this->m_rigidbody->Destroy();
        this->m_rigidbody = nullptr;
        this->m_graphic->Destroy();
        this->m_graphic = nullptr;
        this->m_entity->Destroy();
        this->m_entity = nullptr;
        this->m_file = nullptr;
	}

    GLFWwindow& Engine::GetWindow()
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

	ComponentManager* Engine::GetComponentManager()
	{
        if (this->m_component != nullptr)
            return  this->m_component.get();
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

