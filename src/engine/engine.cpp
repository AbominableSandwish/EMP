#include "engine/engine.h"
#include <iostream>
#include <ctime>
#include <engine/graphic.h>
#include <engine/log.h>
#include <editor/editor.h>
#include <engine/file.h>


namespace emp
{
    constexpr int WIDTH = 1000000;

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
        this->m_File = new FileManager(*this, "File Manager");
        this->m_File->Init();
    	
        this->m_Logger = new LogManager(*this ,"Log Manager");
        this->m_Logger->Init();


        std::cout << "[ ... ] Loading configuration\n";
        this->m_Config = config;

        this->m_Graphic = new GraphicManager(*this, "Graphic Manager");
        this->m_Graphic->Init();

        this->m_Editor = new Editor(*this);
        this->m_Editor->Init();

        this->is_running = true;
	}

	void Engine::Start()
	{
        clock_t start, end;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }

        while (this->is_running)
        {
            start = clock();
            end = clock();
            float dt = float(start - end);

            this->m_Logger->Update(dt);
            this->m_Graphic->Update(dt);
            this->m_Editor->Draw();
            this->m_Graphic->Draw();
            this->m_File->Update(dt);
            

            start = clock();
            max = max_value(arr, WIDTH);
            end = clock();
            //printf("max_value: %0.3f sec, max = %d, Timer = %d\n",
                //((float)end - start) / CLOCKS_PER_SEC, max, clock() / 1000);
        }
	}


	void Engine::Destroy()
	{
		
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

