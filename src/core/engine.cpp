#include "core/engine.h"
#include <iostream>
#include <ctime>
#include <graphic/graphic.h>
#include <core/log.h>
#include <editor/editor.h>
#include <core/file.h>


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
    	
        this->m_Config = config;
        emp::ConfigGraphic* configGraphic = new emp::ConfigGraphic("Configuration Graphic", config->mode);
    	
        this->m_File = new FileManager(*this, "File Manager");
        this->m_Logger = LogManager::GetInstance();
        this->m_Graphic = new GraphicManager(*this, "Graphic Manager", *configGraphic);
    	
        this->m_File->Init();
        this->m_Logger->Init();
        this->m_Graphic->Init();

    	if(config->editor)
    	{
            this->m_Editor = new class Editor(*this);
			this->m_Editor->Init();
    	}

        if (random(0, 10) > 5)
            std::cout << "[ ... ] Welcome Master";
        else
            std::cout << "[ ... ] Hey Master";

        this->is_running = true;
	}

    float counter = 0.0f;
    clock_t oldTime;
	void Engine::Start()
	{
        clock_t start, end;
        end = 0;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }
        LOG::Info(this->m_Config->GetName() + "is ready");
      
        while (this->is_running)
        {
            ///DELTATIME AND FPS COUNTING
            clock_t deltaTime = clock() - oldTime;
            fps = (1.0 / deltaTime) * 1000;
            oldTime = clock();
        	for(int turn = 0; turn < 1; turn++)
				LOG::Debug("clock per sec: "+ to_string(deltaTime));
            start = clock();
            float dt = float(start - end);
            counter += dt;

            this->m_Logger->Update(dt);
            this->m_Graphic->Update(dt);
            this->m_File->Update(dt);
            //LOG::Debug(to_string(counter));
            if (this->m_Editor != nullptr) {
                this->m_Editor->Draw(this->m_Graphic);
            }
        	else
            {
                this->m_Graphic->Draw();
            }

            end = clock();
        }
	}

    void Engine::Stop()
    {
        this->Destroy();
    }


    void Engine::Destroy()
	{
        this->m_Logger = nullptr;
        this->m_Graphic = nullptr;
        this->m_Editor = nullptr;
        this->m_Graphic = nullptr;
        this->m_File = nullptr;
        this->is_running = false;
	}

    GLFWwindow* Engine::GetWindow()
    {
        return this->m_Graphic->GetWindow();
    }

    LogManager* Engine::GetLogManager()
    {
    	if(this->m_Logger != nullptr)
			return this->m_Logger;
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

