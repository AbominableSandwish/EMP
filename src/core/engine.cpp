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
#include <components/sphere.h>
#include <components/light.h>
#include <components/model.h>
#include <components/camera.h>
#include <components/script.h>
#include <components/skybox.h>
#include <components/map.h>
#include <components/chunck.h>
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

        
        m_script = this->m_systems->RegisterSystem<ScriptSystem>(*this, "Script Manager");
        m_script->Init();
        
    	//Graphic
        emp::ConfigGraphic* configGraphic = new emp::ConfigGraphic("Configuration Graphic", config->mode);
        m_graphic = this->m_systems->RegisterSystem<GraphicManager>(*this, *configGraphic);

        this->m_component->RegisterComponent<Transform>();
        this->m_component->RegisterComponent<Square>();
        this->m_component->RegisterComponent<Circle>();
        this->m_component->RegisterComponent<Sphere>();
        this->m_component->RegisterComponent<Triangle>();

        this->m_component->RegisterComponent<Cube>();
        this->m_component->RegisterComponent<DirectionalLight>();
        this->m_component->RegisterComponent<PointLight>();
        this->m_component->RegisterComponent<SpotLight>();
        this->m_component->RegisterComponent<Model>();
        this->m_component->RegisterComponent<Camera>();

        this->m_component->RegisterComponent<Map>();
        this->m_component->RegisterComponent<RigidBody2D>(); 

        this->m_component->RegisterComponent<Script>();
        this->m_component->RegisterComponent<PlayerScript>();
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
        int entity;
        //for (int i = 0; i < 40; i++) {
        //    for (int j = 0; j < 20; j++) {
        //        entity = this->m_entity->CreateEntity("Triangle_" + std::to_string(this->m_entity->GetEntitesCount())).id;
        //        this->m_component->AddComponent(entity, Transform(i * 200 + 75 -4000, j * 200, -2500, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        //        this->m_component->AddComponent(entity, Triangle(entity));//, 0.66f, 0.66f, 0.66f
        //    }
        //}

        //entity = this->m_entity->CreateEntity("Square").id;
        //this->m_component->AddComponent(entity, Transform(0, 0, -5000, 0, 0, 0, 500.0f, 500.0f, 500.0f));
        //this->m_component->AddComponent(entity, Square(entity));//, 0.66f, 0.66f, 0

        //entity = this->m_entity->CreateEntity("Circle").id;
        //this->m_component->AddComponent(entity, Transform(0, 0, 0, 0, 0, 0, 1.0f, 1.0f, 1.0f));
        //this->m_component->AddComponent(entity, Circle(entity));//, 0.66f, 0.66f, 0

      /*  for (int i = 0; i < 25; i++ ) {
            for (int j = 0; j < 25; j++) {
                entity = this->m_entity->CreateEntity("Sphere_" +  std::to_string(this->m_entity->GetEntitesCount())).id;
                this->m_component->AddComponent(entity, Transform(i * 200 + 75, -125, j * 200 - 2400 , 0, 0, 0, 1.0f, 1.0f, 1.0f));
                this->m_component->AddComponent(entity, Sphere(entity));
            }
        }
        */



       

        //entity = this->m_entity->CreateEntity("Map").id;
        //this->m_component->AddComponent(entity, Transform(0, 0, 40, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        //this->m_component->AddComponent(entity, Map(entity));
   
        entity = this->m_entity->CreateEntity("Directional_Light").id;
        this->m_component->AddComponent(entity, Transform(0, 0, 40, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        this->m_component->AddComponent(entity, DirectionalLight(entity, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f)));

        const int MaxPointLight = 36;
        for (size_t i = 0; i < MaxPointLight; i++)
        {
            int x = i % 6;
            int y = i / 6;

            float r = std::rand()%2;
            float g = std::rand()%2;
            float b = std::rand()%2;

            entity = this->m_entity->CreateEntity("Point_Light_" + std::to_string(i)).id;
            this->m_component->AddComponent(entity, Transform(x * 3500 - (3 * 3500), -250, y * 3500 - (3 * 3500), 0, 0, 0, 2.0f, 2.0f, 2.0f));
            this->m_component->AddComponent(entity, PointLight(entity, r, g, b, glm::vec3(r, g, b), glm::vec3(r, g, b), glm::vec3(r, g, b), 1.0f, 0.09f, 0.032f));

        }
        
        entity = this->m_entity->CreateEntity("SpotLight").id;
        this->m_component->AddComponent(entity, Transform(0, 400, 0, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        this->m_component->AddComponent(entity, SpotLight(entity, 0.0f, 0.0f, 0.8f, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));


        // entity = this->m_entity->CreateEntity("Player").id;
        //this->m_component->AddComponent(entity, Transform(0, 225, -5000, 0, 0, 0, 5.0f, 5.0f, 5.0f));
        //this->m_component->AddComponent(entity, Model(entity, "./data/byke2/untitled.obj"));
        //this->m_component->AddComponent(entity, PlayerScript(*this, entity));

     /*   entity = this->m_entity->CreateEntity("Loic").id;
        this->m_component->AddComponent(entity, Transform(-500, -165, 0, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        this->m_component->AddComponent(entity, Model(entity, "./data/Omnit/OmnitWithLoic.obj"));

        entity = this->m_entity->CreateEntity("Wheel").id;
        this->m_component->AddComponent(entity, Transform(-500, -165, 0, 0, 0, 0, 2.0f, 2.0f, 2.0f));
        this->m_component->AddComponent(entity, Model(entity, "./data/Mosqui/Wheel.obj"));

       entity = this->m_entity->CreateEntity("mars").id;
        this->m_component->AddComponent(entity, Transform(-500, -165, 0, 0, 0, 0, 5.0f, 5.0f, 5.0f));
        this->m_component->AddComponent(entity, Model(entity, "./data/model/planet/planet.obj"));*/


      /*  entity = this->m_entity->CreateEntity("Sphere_" + std::to_string(this->m_entity->GetEntitesCount())).id;
        this->m_component->AddComponent(entity, Transform(-305, 425, 13,0, 0, 0, 0.2f, 0.2f, 0.2f));
        this->m_component->AddComponent(entity, Sphere(entity));

        entity = this->m_entity->CreateEntity("Sphere_" + std::to_string(this->m_entity->GetEntitesCount())).id;
        this->m_component->AddComponent(entity, Transform(-305, 425, -16, 0, 0, 0, 0.2f, 0.2f, 0.2f));
        this->m_component->AddComponent(entity, Sphere(entity));

        entity = this->m_entity->CreateEntity("Cone_" + std::to_string(this->m_entity->GetEntitesCount())).id;
        this->m_component->AddComponent(entity, Transform(-305, 380, -4.0f, 0, 0, 0, 1.2f, 0.033f, 0.033f));
        this->m_component->AddComponent(entity, Sphere(entity));*/

        entity = this->m_entity->CreateEntity("Main_Camera").id;
        this->m_component->AddComponent(entity, Transform(0, -4, -50));
        this->m_component->AddComponent(entity, Camera(*this, entity, this->m_component->GetComponent<Transform>(entity), 45.0f));


       /* entity = this->m_entity->CreateEntity("Model2").id;
        this->m_component->AddComponent(entity, Transform(0, 0, 0, 0, 0, 0, 0.5f, 0.5f, 0.5f));
        this->m_component->AddComponent(entity, Model(entity, "./data/backpack/backpack.obj"));*/
        this->m_graphic->Start();

        this->is_running = true;
	}

    list<Transform*>* map = nullptr;

    void Engine::Update()
    {
        if (this->is_running)
        {
            ///DELTATIME AND FPS COUNTING
            clock_t deltaTime = clock() - oldTime;
            fps = (1.0f / deltaTime) * 1000.0f;
            oldTime = clock();
            start = clock();
            float dt = float(start - end)/1000.0f;
            counter += dt;


            if(this->m_log != nullptr)
				this->m_log->Update(dt); 
            this->m_file->Update(dt);

            this->m_input->Update(dt);
            this->m_script->Update(dt);
            this->m_rigidbody->Update(dt);
            this->m_graphic->Update(dt);
        	
            if(this->m_graphic != nullptr)
				this->m_graphic->Draw();

            end = clock();

            this->deltaTime = dt;
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
        this->m_script->Destroy();
        this->m_script = nullptr;
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

